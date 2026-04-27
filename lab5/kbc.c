#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"

uint8_t scancode = 0;
bool ih_error = false;
int hook_id_kbd = 1;

int kbd_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    if (sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != OK)
        return 1;
    *bit_no = BIT(hook_id_kbd);  // BIT() após a chamada
    return 0;
}

int kbd_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_kbd);
}

void (kbc_ih)() {
    uint8_t status;
    ih_error = true;
    if (util_sys_inb(KBC_CMD_REG, &status) != 0) return;
    if (status & KBC_OBF) {
        if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return;
        if ((status & KBC_PAR_ERR) || (status & KBC_TO_ERR)) return;
        ih_error = false;
    }
}

int kbd_poll() {
    uint8_t status;
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
        if (status & KBC_OBF) {
            if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return 1;
            if ((status & KBC_PAR_ERR) || (status & KBC_TO_ERR)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

static int kbc_write_cmd(int port, uint8_t cmd) {
    uint8_t status;
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
        if ((status & KBC_IBF) == 0)
            return sys_outb(port, cmd);
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

static int kbc_read_ret(uint8_t *ret) {
    uint8_t status;
    for (int i = 0; i < MAX_RETRIES; i++) {
        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
        if (status & KBC_OBF) {
            if (util_sys_inb(KBC_OUT_BUF, ret) != 0) return 1;
            if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) != 0) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

int kbd_restore_interrupts() {
    uint8_t cmd_byte;
    if (kbc_write_cmd(KBC_CMD_REG, READ_CMD_BYTE) != 0) return 1;
    if (kbc_read_ret(&cmd_byte) != 0) return 1;
    cmd_byte |= KBD_INT_BIT;
    if (kbc_write_cmd(KBC_CMD_REG, WRITE_CMD_BYTE) != 0) return 1;
    if (kbc_write_cmd(KBC_OUT_BUF, cmd_byte) != 0) return 1;
    return 0;
}
