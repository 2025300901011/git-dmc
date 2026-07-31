#ifndef KEY_H
#define KEY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KEY_ID_1 = 0,
    KEY_ID_2,
    KEY_ID_3,
    KEY_ID_4,
    KEY_ID_COUNT
} Key_Id_t;

void Key_Init(void);
void Key_Task(uint32_t now_ms);
bool Key_WasPressed(Key_Id_t key);

#ifdef __cplusplus
}
#endif

#endif /* KEY_H */
