#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"

uint8_t mouse_byte = 0;
bool mouse_ih_error = false;
static int hook_id_mouse = 12;

int mouse_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = hook_id_mouse;
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int mouse_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_ih)() {
    uint8_t status;
    mouse_ih_error = true;
    if (util_sys_inb(KBC_CMD_REG, &status) != 0) return;
    if (status & KBC_OBF) {
        if (util_sys_inb(KBC_OUT_BUF, &mouse_byte) != 0) return;
        if (status & (KBC_PAR_ERR | KBC_TO_ERR)) return;
        mouse_ih_error = false;
    }
}

static int kbc_write(int port, uint8_t cmd) {
    uint8_t status;
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
        if ((status & KBC_IBF) == 0)
            return sys_outb(port, cmd);
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

static int kbc_read(uint8_t *val) {
    uint8_t status;
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
        if (status & KBC_OBF) {
            if (util_sys_inb(KBC_OUT_BUF, val) != 0) return 1;
            if (status & (KBC_PAR_ERR | KBC_TO_ERR)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

int mouse_send_cmd(uint8_t cmd) {
    uint8_t ack;
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        if (kbc_write(KBC_CMD_REG, MOUSE_WRITE_CMD) != 0) return 1;
        if (kbc_write(KBC_OUT_BUF, cmd) != 0) return 1;
        if (kbc_read(&ack) != 0) return 1;
        if (ack == MOUSE_ACK)   return 0;
        if (ack == MOUSE_ERROR) return 1;
    }
    return 1;
}

int mouse_disable_data_reporting() {
    return mouse_send_cmd(MOUSE_DISABLE);
}

bool mouse_sync_packet(uint8_t byte, struct packet *pp) {
    static uint8_t buf[3];
    static int idx = 0;

    if (idx == 0 && !(byte & MOUSE_SYNC_BIT)) return false;

    buf[idx++] = byte;
    if (idx < 3) return false;

    idx = 0;

    pp->lb   = (buf[0] & BIT(0)) != 0;
    pp->rb   = (buf[0] & BIT(1)) != 0;
    pp->mb   = (buf[0] & BIT(2)) != 0;
    pp->x_ov = (buf[0] & BIT(6)) != 0;
    pp->y_ov = (buf[0] & BIT(7)) != 0;

    pp->delta_x = (buf[0] & BIT(4)) ? ((int16_t)buf[1] - 256) : (int16_t)buf[1];
    pp->delta_y = (buf[0] & BIT(5)) ? ((int16_t)buf[2] - 256) : (int16_t)buf[2];

    pp->bytes[0] = buf[0];
    pp->bytes[1] = buf[1];
    pp->bytes[2] = buf[2];

    return true;
}
