#undef kbc_ih 

#ifndef _KBC_H_
#define _KBC_H_

#include <stdint.h>
#include <stdbool.h>
 
// Subscribes keyboard interrupts
int kbd_subscribe_int(uint8_t *bit_no);

// Unsubscribes keyboard interrupts
int kbd_unsubscribe_int();

// Keyboard interrupt handler
void kbc_ih();

// Polling method to read scancode (without interrupts)
int kbd_poll();

// Restores keyboard interrupts after polling
int kbd_restore_interrupts();
 
#endif
