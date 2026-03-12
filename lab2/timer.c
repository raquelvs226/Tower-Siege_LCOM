#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) {
    return 1;
  }

  // Build the Read-Back Command
  // TIMER_RB_SEL: Selects the specific timer (0, 1, or 2)
  uint8_t rb_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  // Send the command to the Control Register
  if (sys_outb(TIMER_CTRL, rb_command) != 0) {
    return 1;
  }

  // Read the status from the corresponding timer port
  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    return 1;
  }
  
  return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;

  switch (field) {
    case tsf_all:
      // Shows the full byte (status word) in hexadecimal
      val.byte = st;
      break;

    case tsf_initial:
      // Isolate bits 5 and 4 (Type of Access)
      val.in_mode = (st & (BIT(5) | BIT(4))) >> 4; 
      break;

    case tsf_mode:
      // Isolate bits 3, 2 and 1 (Programmed Mode)
      val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      // 6 and 7 are redundant (equivalent to 2 and 3)
      if (val.count_mode == 6) val.count_mode = 2;
      if (val.count_mode == 7) val.count_mode = 3;
      break;

    case tsf_base:
      // Isolate bit 0
      val.bcd = (st & BIT(0));
      break;

    default:
      return 1;
  }

  if (timer_print_config(timer, field, val) != 0) { 
    return 1;
  }

  return 0;
}
