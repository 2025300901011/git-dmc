from pyb import UART, LED
import sensor, time, struct

# =========================
# Debug
# =========================
DEBUG_PRINT = True
PRINT_EVERY_N_FRAMES = 5

# =========================
# LED
# =========================
led_red = LED(1)    # lost / stop
led_green = LED(2)  # ok / hold
led_blue = LED(3)   # heartbeat

led_red.off()
led_green.off()
led_blue.off()

# Power-on mark
led_red.on()
led_green.on()
led_blue.on()
time.sleep_ms(250)
led_red.off()
led_green.off()
led_blue.off()

# =========================
# UART
# =========================
uart = UART(3, 115200, timeout_char=1000)

# =========================
# Camera
# =========================
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)

sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_auto_exposure(False, exposure_us=12000)

sensor.skip_frames(time=1000)
clock = time.clock()

# =========================
# Pipe calibration
# =========================
# ROI is a normal rectangle used only to limit blob search.
# Make it large enough to cover the whole pipe and the ball movement range.
ROI_X = 12
ROI_Y = 20
ROI_W = 136
ROI_H = 85

# Two points on the pipe centerline. Adjust these before competition.
# P0 and P1 define the pipe axis, so tilted pipes are handled by projection.
PIPE_X0 = 18
PIPE_Y0 = 70
PIPE_X1 = 145
PIPE_Y1 = 55

PIPE_SEND_CENTER_X = 80
PIPE_SEND_CENTER_Y = 60
MAX_PIPE_DIST = 18

_pipe_vx = PIPE_X1 - PIPE_X0
_pipe_vy = PIPE_Y1 - PIPE_Y0
_pipe_len = (_pipe_vx * _pipe_vx + _pipe_vy * _pipe_vy) ** 0.5
_pipe_ux = _pipe_vx / _pipe_len
_pipe_uy = _pipe_vy / _pipe_len


def project_to_pipe(cx, cy):
    dx = cx - PIPE_X0
    dy = cy - PIPE_Y0
    s = dx * _pipe_ux + dy * _pipe_uy
    d = -dx * _pipe_uy + dy * _pipe_ux
    return s, d


def pipe_s_to_send_x(s):
    return int(PIPE_SEND_CENTER_X + (s - _pipe_len / 2))


# =========================
# Detection
# =========================
DARK_THRESHOLD = (0, 120)

MIN_PIXELS_FIND = 2

LOCK_DIST = 50
HOLD_FRAMES = 10

# Big blob reject: hand/shadow/pipe/background
MAX_BLOB_W = 40
MAX_BLOB_H = 35
MAX_BLOB_PIXELS = 1200

# Tracking filter
ALPHA = 0.70

fx = 80
fy = 60
last_x = 80
last_y = 60
last_send_x = PIPE_SEND_CENTER_X
last_send_y = PIPE_SEND_CENTER_Y
has_lock = False
lost_count = 99
frame_count = 0


# Old OpenMV blob order:
# b[0]=x, b[1]=y, b[2]=w, b[3]=h, b[4]=cx, b[5]=cy, b[6]=pixels
def bx(b):
    return b[0]


def by(b):
    return b[1]


def bw(b):
    return b[2]


def bh(b):
    return b[3]


def bcx(b):
    return b[4]


def bcy(b):
    return b[5]


def bpixels(b):
    return b[6]


def send_point(x, y):
    uart.write(struct.pack("<BHHB", 0xAA, int(x), int(y), 0x55))


while True:
    clock.tick()
    frame_count += 1

    img = sensor.snapshot()

    if DEBUG_PRINT:
        img.draw_rectangle((ROI_X, ROI_Y, ROI_W, ROI_H))
        img.draw_line((PIPE_X0, PIPE_Y0, PIPE_X1, PIPE_Y1))

    blobs = img.find_blobs([DARK_THRESHOLD],
                           roi=(ROI_X, ROI_Y, ROI_W, ROI_H),
                           pixels_threshold=MIN_PIXELS_FIND,
                           area_threshold=MIN_PIXELS_FIND,
                           merge=True)

    best = None
    best_score = -99999
    best_s = 0
    best_d = 0

    for b in blobs:
        w = bw(b)
        h = bh(b)
        cx = bcx(b)
        cy = bcy(b)
        px = bpixels(b)

        if w < 2 or h < 2:
            continue

        if w > MAX_BLOB_W:
            continue
        if h > MAX_BLOB_H:
            continue
        if px > MAX_BLOB_PIXELS:
            continue

        s, d = project_to_pipe(cx, cy)
        if s < 0 or s > _pipe_len:
            continue
        if abs(d) > MAX_PIPE_DIST:
            continue

        score = 0

        if px > 20:
            score += 40
        if px > 80:
            score += 40
        if px > 160:
            score += 40
        if px > 450:
            score -= 40

        if w >= 5 and w <= 35:
            score += 30
        if h >= 5 and h <= 30:
            score += 30

        diameter = (w + h) / 2
        score += 50 - abs(diameter - 18) * 3

        aspect = w / h
        if aspect < 0.3 or aspect > 2.8:
            continue
        score += 30 - abs(w - h)

        if not has_lock:
            score += 40 - abs(d)

        if has_lock:
            dist = abs(cx - last_x) + abs(cy - last_y)
            score += 160 - dist * 4

        if score > best_score:
            best_score = score
            best = b
            best_s = s
            best_d = d

    if best is not None and has_lock:
        dist = abs(bcx(best) - last_x) + abs(bcy(best) - last_y)
        if dist > LOCK_DIST:
            best = None

    if best is not None:
        raw_x = bcx(best)
        raw_y = bcy(best)

        fx = int(ALPHA * raw_x + (1.0 - ALPHA) * fx)
        fy = int(ALPHA * raw_y + (1.0 - ALPHA) * fy)

        fs, fd = project_to_pipe(fx, fy)
        send_x = pipe_s_to_send_x(fs)
        send_y = int(PIPE_SEND_CENTER_Y + fd)

        last_x = raw_x
        last_y = raw_y
        last_send_x = send_x
        last_send_y = send_y
        has_lock = True
        lost_count = 0

        send_point(send_x, send_y)

        if DEBUG_PRINT:
            img.draw_rectangle((bx(best), by(best), bw(best), bh(best)))
            img.draw_cross((raw_x, raw_y))
            img.draw_circle((raw_x, raw_y, max(bw(best), bh(best)) // 2))

        led_green.on()
        led_red.off()
        status = "OK"

    else:
        lost_count += 1

        if has_lock and lost_count <= HOLD_FRAMES:
            send_point(last_send_x, last_send_y)
            led_green.on()
            led_red.off()
            status = "HOLD"
        else:
            has_lock = False
            led_green.off()
            led_red.on()
            status = "STOP"

    if frame_count % 15 == 0:
        led_blue.toggle()

    if DEBUG_PRINT and frame_count % PRINT_EVERY_N_FRAMES == 0:
        if best is not None:
            print("STAT:%s CX:%d CY:%d FX:%d FY:%d SX:%d SY:%d D:%d WH:%d,%d PX:%d SCORE:%d N:%d FPS:%.1f" %
                  (status,
                   bcx(best),
                   bcy(best),
                   fx,
                   fy,
                   last_send_x,
                   last_send_y,
                   int(best_d),
                   bw(best),
                   bh(best),
                   bpixels(best),
                   best_score,
                   len(blobs),
                   clock.fps()))
        else:
            print("STAT:%s SX:%d SY:%d LOST:%d N:%d FPS:%.1f" %
                  (status,
                   last_send_x,
                   last_send_y,
                   lost_count,
                   len(blobs),
                   clock.fps()))
