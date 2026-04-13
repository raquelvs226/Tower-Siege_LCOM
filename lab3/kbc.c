#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"
 
// Global variables shared with other files
uint8_t scancode = 0;   // Stores the last read scancode
bool ih_error = false;  // Indicates if an error occurred in interrupt handler
int hook_id_kbd = 1;    // Hook id for keyboard interrupts
 
int kbd_subscribe_int(uint8_t *bit_no) {
    // Check for NULL pointer
    if (bit_no == NULL) return 1;

    // Return hook id as bit number
    *bit_no = hook_id_kbd;

    // Subscribe interrupts with re-enable and exclusive policy
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
}
 
int kbd_unsubscribe_int() {
    // Remove interrupt policy
    return sys_irqrmpolicy(&hook_id_kbd);
}
 
void kbc_ih() {
    uint8_t status;

    ih_error = true; // Assume error by default

    // Read status register
    if (util_sys_inb(KBC_CMD_REG, &status) != 0) return;

    // Check if output buffer has data
    if (status & KBC_OBF) {

        // Read scancode
        if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return;

        // Check for errors
        if ((status & KBC_PAR_ERR) || (status & KBC_TO_ERR)) return;

        ih_error = false; // No error
    }
}
 
int kbd_poll() {
    uint8_t status;
 
    // Try multiple times to read data
    for (int i = 0; i < MAX_RETRIES; i++) {

        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;

        // If data available
        if (status & KBC_OBF) {

            if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return 1;

            // Check errors
            if ((status & KBC_PAR_ERR) || (status & KBC_TO_ERR)) return 1;

            return 0; // Success
        }

        // Wait before retrying
        tickdelay(micros_to_ticks(DELAY));
    }
 
    return 1; // Failed after retries
}
 
// Writes a command to the KBC (waits until input buffer is empty)
static int kbc_write_cmd(int port, uint8_t cmd) {
    uint8_t status;
 
    for (int i = 0; i < MAX_RETRIES; i++) {

        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;

        // If input buffer is empty, we can write
        if ((status & KBC_IBF) == 0)
            return sys_outb(port, cmd);

        tickdelay(micros_to_ticks(DELAY));
    }
 
    return 1;
}
 
// Reads return value from KBC
static int kbc_read_ret(uint8_t *ret) {
    uint8_t status;
 
    for (int i = 0; i < MAX_RETRIES; i++) {

        if (util_sys_inb(KBC_CMD_REG, &status) != 0) return 1;
 
        if (status & KBC_OBF) {

            if (util_sys_inb(KBC_OUT_BUF, ret) != 0) return 1;

            // Check for errors
            if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) != 0) return 1;

            return 0;
        }
 
        tickdelay(micros_to_ticks(DELAY));
    }
 
    return 1;
}
 
int kbd_restore_interrupts() {
    uint8_t cmd_byte;
 
    // Read current command byte
    if (kbc_write_cmd(KBC_CMD_REG, READ_CMD_BYTE) != 0) return 1;
    if (kbc_read_ret(&cmd_byte) != 0) return 1;
 
    // Enable keyboard interrupts (set bit 0)
    cmd_byte |= KBD_INT_BIT;
 
    // Write updated command byte
    if (kbc_write_cmd(KBC_CMD_REG, WRITE_CMD_BYTE) != 0) return 1;
    if (kbc_write_cmd(KBC_OUT_BUF, cmd_byte) != 0) return 1;
 
    return 0;
}
