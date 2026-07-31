#include "UART_Hand_PID.h"

/* main.c includes this implementation until CCS regenerates its legacy source manifest. */
#ifdef UART_HAND_PID_EMBEDDED

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ti_msp_dl_config.h"

#include "control.h"

#define UART_HAND_PID_RX_MAX          (96U)
#define UART_HAND_PID_TX_MAX          (48U)
#define UART_HAND_PID_RUN_LIMIT_MS    (20000U)

typedef enum {
    PARSE_OK = 0,
    PARSE_FORMAT,
    PARSE_RANGE
} Parse_Result_t;

static char g_rx_buf[UART_HAND_PID_RX_MAX];
static uint8_t g_rx_len;
static bool g_rx_overflow;
static char g_tx_buf[UART_HAND_PID_TX_MAX];
static uint8_t g_tx_len;
static uint8_t g_tx_pos;
static bool g_has_written_params;
static uint32_t g_run_start_ms;

static uint8_t xor_checksum(const char *text)
{
    uint8_t value = 0U;

    while (*text != '\0') {
        value ^= (uint8_t) *text++;
    }
    return value;
}

static int hex_value(char value)
{
    if ((value >= '0') && (value <= '9')) {
        return value - '0';
    }
    value = (char) toupper((unsigned char) value);
    if ((value >= 'A') && (value <= 'F')) {
        return value - 'A' + 10;
    }
    return -1;
}

static bool parse_sequence_token(const char *token, uint16_t *sequence)
{
    char *end;
    unsigned long value;

    if ((strncmp(token, "SEQ:", 4U) != 0) || !isdigit((unsigned char) token[4])) {
        return false;
    }
    errno = 0;
    value = strtoul(&token[4], &end, 10);
    if ((errno != 0) || (*end != '\0') || (value > 65535UL)) {
        return false;
    }
    *sequence = (uint16_t) value;
    return true;
}

static uint16_t find_sequence(const char *body)
{
    const char *text = strstr(body, "SEQ:");
    uint32_t value = 0U;

    if ((text == 0) || !isdigit((unsigned char) text[4])) {
        return 0U;
    }
    text += 4;
    while (isdigit((unsigned char) *text)) {
        value = (value * 10U) + (uint32_t) (*text++ - '0');
        if (value > 65535U) {
            return 0U;
        }
    }
    return (uint16_t) value;
}

static Parse_Result_t parse_gain_token(const char *token, const char *prefix, float *value)
{
    char *end;
    float parsed;
    size_t prefix_len = strlen(prefix);

    if ((strncmp(token, prefix, prefix_len) != 0) || (token[prefix_len] == '\0')) {
        return PARSE_FORMAT;
    }
    errno = 0;
    parsed = strtof(&token[prefix_len], &end);
    if ((errno != 0) || (end == &token[prefix_len]) || (*end != '\0')) {
        return PARSE_FORMAT;
    }
    if (!isfinite(parsed) || (parsed < 0.0f)) {
        return PARSE_RANGE;
    }
    *value = parsed;
    return PARSE_OK;
}

static void queue_reply(const char *format, uint16_t sequence, const char *detail)
{
    int written;
    uint8_t checksum;

    if (g_tx_pos != g_tx_len) {
        return;
    }
    if (detail == 0) {
        written = snprintf(g_tx_buf, sizeof(g_tx_buf), format, (unsigned int) sequence);
    } else {
        written = snprintf(g_tx_buf, sizeof(g_tx_buf), format, (unsigned int) sequence, detail);
    }
    if ((written < 0) || ((size_t) written >= (sizeof(g_tx_buf) - 4U))) {
        return;
    }
    checksum = xor_checksum(g_tx_buf);
    (void) snprintf(&g_tx_buf[written], sizeof(g_tx_buf) - (size_t) written,
                    "*%02X\n", (unsigned int) checksum);
    g_tx_len = (uint8_t) strlen(g_tx_buf);
    g_tx_pos = 0U;
}

static void queue_ack(uint16_t sequence)
{
    queue_reply("ACK,%u", sequence, 0);
}

static void queue_error(uint16_t sequence, const char *reason)
{
    queue_reply("ERR,%u,%s", sequence, reason);
}

static void tx_service(void)
{
    while ((g_tx_pos < g_tx_len) && !DL_UART_isTXFIFOFull(UART_HAND_PID_INST)) {
        DL_UART_transmitData(UART_HAND_PID_INST, (uint8_t) g_tx_buf[g_tx_pos]);
        g_tx_pos++;
    }
    if (g_tx_pos == g_tx_len) {
        g_tx_pos = 0U;
        g_tx_len = 0U;
    }
}

static Parse_Result_t parse_setall(char *body, uint16_t *sequence,
                                   float *kp, float *ki, float *kd)
{
    char *tokens[6];
    char *token;
    uint8_t count = 0U;
    Parse_Result_t result;

    token = strtok(body, " ");
    while ((token != 0) && (count < (uint8_t) (sizeof(tokens) / sizeof(tokens[0])))) {
        tokens[count++] = token;
        token = strtok(0, " ");
    }
    if ((token != 0) || (count != 5U) || (strcmp(tokens[0], "SETALL") != 0) ||
        !parse_sequence_token(tokens[1], sequence)) {
        return PARSE_FORMAT;
    }
    result = parse_gain_token(tokens[2], "KP:", kp);
    if (result != PARSE_OK) {
        return result;
    }
    result = parse_gain_token(tokens[3], "KI:", ki);
    if (result != PARSE_OK) {
        return result;
    }
    return parse_gain_token(tokens[4], "KD:", kd);
}

static bool parse_run(char *body, uint16_t *sequence)
{
    char *command = strtok(body, " ");
    char *seq_token = strtok(0, " ");

    return (command != 0) && (seq_token != 0) && (strtok(0, " ") == 0) &&
           (strcmp(command, "RUN") == 0) && parse_sequence_token(seq_token, sequence);
}

static void process_frame(char *frame, uint32_t now_ms)
{
    char *star = strrchr(frame, '*');
    uint16_t sequence;
    float kp;
    float ki;
    float kd;
    Parse_Result_t result;
    int high;
    int low;

    if ((star == 0) || (star[1] == '\0') || (star[2] == '\0') || (star[3] != '\0')) {
        queue_error(find_sequence(frame), "FORMAT");
        return;
    }
    high = hex_value(star[1]);
    low = hex_value(star[2]);
    if ((high < 0) || (low < 0)) {
        queue_error(find_sequence(frame), "FORMAT");
        return;
    }
    *star = '\0';
    sequence = find_sequence(frame);
    if (xor_checksum(frame) != (uint8_t) ((high << 4) | low)) {
        queue_error(sequence, "CHECKSUM");
        return;
    }

    if (strncmp(frame, "SETALL ", 7U) == 0) {
        result = parse_setall(frame, &sequence, &kp, &ki, &kd);
        if (result == PARSE_FORMAT) {
            queue_error(sequence, "FORMAT");
        } else if (result == PARSE_RANGE) {
            queue_error(sequence, "RANGE");
        } else if (Control_IsEnabled()) {
            queue_error(sequence, "BUSY");
        } else if (!Control_LoadPipeMotorPid(kp, ki, kd)) {
            queue_error(sequence, "STATE");
        } else {
            g_has_written_params = true;
            queue_ack(sequence);
        }
        return;
    }

    if (strncmp(frame, "RUN ", 4U) == 0) {
        if (!parse_run(frame, &sequence)) {
            queue_error(sequence, "FORMAT");
        } else if (!g_has_written_params) {
            queue_error(sequence, "STATE");
        } else if (Control_IsEnabled()) {
            queue_error(sequence, "BUSY");
        } else {
            g_run_start_ms = now_ms;
            Control_Enable(true);
            queue_ack(sequence);
        }
        return;
    }

    queue_error(sequence, "FORMAT");
}

static void rx_service(uint32_t now_ms)
{
    while ((g_tx_len == 0U) && !DL_UART_isRXFIFOEmpty(UART_HAND_PID_INST)) {
        char byte_in = (char) DL_UART_receiveData(UART_HAND_PID_INST);

        if (byte_in == '\r') {
            continue;
        }
        if (byte_in == '\n') {
            if (!g_rx_overflow && (g_rx_len > 0U)) {
                g_rx_buf[g_rx_len] = '\0';
                process_frame(g_rx_buf, now_ms);
            } else if (g_rx_overflow) {
                queue_error(0U, "FORMAT");
            }
            g_rx_len = 0U;
            g_rx_overflow = false;
            continue;
        }
        if (g_rx_len < (UART_HAND_PID_RX_MAX - 1U)) {
            g_rx_buf[g_rx_len++] = byte_in;
        } else {
            g_rx_overflow = true;
        }
    }
}

void UART_Hand_PID_Init(void)
{
    g_rx_len = 0U;
    g_rx_overflow = false;
    g_tx_len = 0U;
    g_tx_pos = 0U;
    g_has_written_params = false;
    g_run_start_ms = 0U;
}

void UART_Hand_PID_Service(uint32_t now_ms)
{
    tx_service();
    rx_service(now_ms);
    if (Control_IsEnabled() &&
        ((uint32_t) (now_ms - g_run_start_ms) >= UART_HAND_PID_RUN_LIMIT_MS)) {
        Control_Enable(false);
    }
    tx_service();
}

#endif /* UART_HAND_PID_EMBEDDED */
