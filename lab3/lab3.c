#include <lcom/lcf.h>
#undef kbc_ih

#include <lcom/lab3.h>


#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "i8042.h"

extern uint8_t scancode;
extern bool ih_error;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (kbd_test_scan)() {
  int ipc_status;
  message msg;
  uint8_t bit_no;

  // Subscribe keyboard interrupts
  if (kbd_subscribe_int(&bit_no) != 0) return 1;

  uint8_t scancodes[2]; // Buffer for 1 or 2 byte scancodes
  uint8_t size = 0;

  // Loop until ESC breakcode is received
  while (scancode != ESC_BREAKCODE) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(bit_no)) {

              // Handle keyboard interrupt
              kbc_ih();

              // If error occurred, ignore this interrupt
              if (ih_error) { size = 0; continue; }

              scancodes[size++] = scancode;

              // If first byte of a 2-byte scancode, wait for next
              if (scancode == TWO_BYTE_PREFIX && size == 1) continue;

              // Print scancode (make = 0, break = 1)
              kbd_print_scancode(!(scancodes[size-1] & BIT(7)), size, scancodes);

              size = 0;
              }
            break;
          default:
            break;
        }
      }
  }
  // Unsubscribe keyboard interrupts
  if (kbd_unsubscribe_int() != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {
  uint8_t scancodes[2];
  uint8_t size = 0;

  // Loop until ESC breakcode is received
  while (scancode != ESC_BREAKCODE) {

    // Read scancode using polling
    if (kbd_poll() != 0) continue;

    scancodes[size++] = scancode;

    // Handle 2-byte scancodes
    if (scancode == TWO_BYTE_PREFIX && size == 1) continue;

    // Print scancode
    kbd_print_scancode(!(scancodes[size-1] & BIT(7)), size, scancodes);

    size = 0;
  }

  // Restore interrupts after polling mode
  return kbd_restore_interrupts();
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  message msg;
  uint8_t kbd_bit_no, timer_bit_no;

  // Subscribe keyboard and timer interrupts
  if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
  if (timer_subscribe_int(&timer_bit_no) != 0) return 1;

  uint8_t scancodes[2];
  uint8_t size = 0;

  int idle_seconds = 0;
  counter = 0;

  // Loop until ESC is pressed or no input for n seconds
  while (scancode != ESC_BREAKCODE && idle_seconds < n) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:

            // Timer interrupt: count time
            if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
              timer_int_handler();

              // 60 ticks ≈ 1 second
              if (counter % 60 == 0)
                idle_seconds++;
            }

            // Keyboard interrupt: process input
            if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {

              kbc_ih();

              if (ih_error) { size = 0; continue; }

                scancodes[size++] = scancode;

                if (scancode == TWO_BYTE_PREFIX && size == 1) continue;

                kbd_print_scancode(!(scancodes[size-1] & BIT(7)), size, scancodes);

                size = 0;

                  // Reset idle timer when a key is pressed
                  counter = 0;
                  idle_seconds = 0;
              }
            break;
        }
      }
  }

  // Unsubscribe interrupts
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;

  return 0;
}
