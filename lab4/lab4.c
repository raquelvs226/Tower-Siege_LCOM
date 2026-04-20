// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8042.h" 
#include "i8254.h"
#include "timer.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no;
    uint32_t irq_set;

    if (mouse_subscribe_int(&bit_no) != 0) return 1;
    irq_set = BIT(bit_no);

    // O LCF fornece esta função para mouse_test_packet
    if (mouse_enable_data_reporting() != 0) {
        mouse_unsubscribe_int();
        return 1;
    }

    uint32_t packets_received = 0;
    int ipc_status;
    message msg;
    struct packet pp;

    while (packets_received < cnt) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & irq_set) {
                mouse_ih();
                if (!mouse_ih_error) {
                    if (mouse_sync_packet(mouse_byte, &pp)) {
                        mouse_print_packet(&pp);
                        packets_received++;
                    }
                }
            }
        }
    }

    if (mouse_disable_data_reporting() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    uint8_t mouse_bit_no, timer_bit_no;
    uint32_t mouse_irq_set, timer_irq_set;

    if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    mouse_irq_set = BIT(mouse_bit_no);

    if (timer_subscribe_int(&timer_bit_no) != 0) {
        mouse_unsubscribe_int();
        return 1;
    }
    timer_irq_set = BIT(timer_bit_no);

    // mouse_test_async tem de implementar enable por conta própria
    if (mouse_send_cmd(MOUSE_ENABLE) != 0) {
        timer_unsubscribe_int();
        mouse_unsubscribe_int();
        return 1;
    }

    uint32_t timer_freq = sys_hz();

    int ipc_status;
    message msg;
    struct packet pp;
    uint32_t idle_ticks = 0;
    counter = 0;

    while (true) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {

            if (msg.m_notify.interrupts & timer_irq_set) {
                timer_int_handler();
                idle_ticks++;
                // Termina se passou o tempo de idle
                if (idle_ticks >= (uint32_t)(idle_time * timer_freq)) {
                    break;
                }
            }

            if (msg.m_notify.interrupts & mouse_irq_set) {
                mouse_ih();
                if (!mouse_ih_error) {
                    if (mouse_sync_packet(mouse_byte, &pp)) {
                        mouse_print_packet(&pp);
                        idle_ticks = 0; // reset do idle ao receber packet completo
                    }
                }
            }
        }
    }

    if (mouse_disable_data_reporting() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;

    return 0;
}
