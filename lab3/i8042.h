#ifndef _I8042_H_
#define _I8042_H_

// IRQ line used by the keyboard
#define IRQ_KEYBOARD    1

// I/O ports of the Keyboard Controller (KBC)
#define KBC_OUT_BUF     0x60   // Output buffer (data from KBC)
#define KBC_CMD_REG     0x64   // Status/Command register

// Status register bits
#define KBC_OBF         BIT(0) // Output buffer full (data available)
#define KBC_IBF         BIT(1) // Input buffer full (cannot write yet)
#define KBC_PAR_ERR     BIT(7) // Parity error
#define KBC_TO_ERR      BIT(6) // Timeout error

// Special scancodes
#define ESC_BREAKCODE   0x81   // ESC key breakcode (used to exit)
#define TWO_BYTE_PREFIX 0xE0   // Prefix for 2-byte scancodes

// KBC commands
#define READ_CMD_BYTE   0x20   // Read command byte
#define WRITE_CMD_BYTE  0x60   // Write command byte

// Command byte bits
#define KBD_INT_BIT     BIT(0) // Enable keyboard interrupts

// Delay and retry settings
#define DELAY           20000  // Delay between retries (in microseconds)
#define MAX_RETRIES     5      // Max number of retries

#endif
