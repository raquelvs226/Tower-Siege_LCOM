#ifndef _KBC_H_
#define _KBC_H_

#include <stdint.h>
#include <stdbool.h>

extern uint8_t scancode;
extern bool ih_error;

int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int();
void (kbc_ih)();
int kbd_poll();
int kbd_restore_interrupts();

#endif
