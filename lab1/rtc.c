#include "rtc.h"
#include <minix/syslib.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

static int bcd_to_bin(uint8_t bcd) { 
  return ((bcd >> 4) * 10) + (bcd & 0x0F); //converter de bcd para decimal
}

int rtc_read_date(rtc_date *date) { 

  uint32_t regA;
  uint32_t regB;
  uint32_t day, month, year;

  // esperar até UIP = 0
  do {
    sys_outb(RTC_ADDR_REG, RTC_REG_A); //escreve em 0x70
    sys_inb(RTC_DATA_REG, &regA); //lê o valor do registo em 0x71
  } while (regA & RTC_UIP_MSK);

  // ler reg B
  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &regB);

  // ler day
  sys_outb(RTC_ADDR_REG, RTC_REG_DAY);
  sys_inb(RTC_DATA_REG, &day);

  // ler month
  sys_outb(RTC_ADDR_REG, RTC_REG_MONTH);
  sys_inb(RTC_DATA_REG, &month);

  // ler year
  sys_outb(RTC_ADDR_REG, RTC_REG_YEAR);
  sys_inb(RTC_DATA_REG, &year);

  // converter se BCD
  if (!(regB & RTC_DM_MSK)) {
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);
  }

  date->day = day;
  date->month = month;
  date->year = year;
  return 0; 
}