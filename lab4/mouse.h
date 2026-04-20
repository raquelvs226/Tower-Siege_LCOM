#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

extern uint8_t mouse_byte;
extern bool mouse_ih_error;

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();
void (mouse_ih)();
int mouse_send_cmd(uint8_t cmd);
int mouse_disable_data_reporting();
bool mouse_sync_packet(uint8_t byte, struct packet *pp);

#endif
