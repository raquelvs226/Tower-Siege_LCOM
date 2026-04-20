#ifndef _I8042_H_
#define _I8042_H_

// IRQ lines
#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12

// I/O ports
#define KBC_OUT_BUF     0x60
#define KBC_CMD_REG     0x64

// Status register bits
#define KBC_OBF         BIT(0)
#define KBC_IBF         BIT(1)
#define KBC_AUX         BIT(5)
#define KBC_PAR_ERR     BIT(7)
#define KBC_TO_ERR      BIT(6)

// Delay and retry settings
#define DELAY           20000
#define MAX_RETRIES     5

// Scancodes especiais
#define ESC_BREAKCODE   0x81
#define TWO_BYTE_PREFIX 0xE0

// KBC commands
#define READ_CMD_BYTE   0x20
#define WRITE_CMD_BYTE  0x60
#define MOUSE_WRITE_CMD 0xD4

// Command byte bits
#define KBD_INT_BIT     BIT(0)
#define MOUSE_INT_BIT   BIT(1)

// Mouse commands
#define MOUSE_ENABLE    0xF4
#define MOUSE_DISABLE   0xF5

// Mouse ACK bytes
#define MOUSE_ACK       0xFA
#define MOUSE_NACK      0xFE
#define MOUSE_ERROR     0xFC

// Sync bit
#define MOUSE_SYNC_BIT  BIT(3)

#endif
