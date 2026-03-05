#include "bitwise.h"
#include <stdarg.h> //mask

#define TODO return 255
#define BIT(n) (1 << n)

uint8_t clear(uint8_t msk, int pos) { 
  return msk & ~BIT(pos);
}

uint8_t set(uint8_t msk, int pos) { 
  return msk | BIT(pos); 
}

bool is_set(uint8_t msk, int pos) { 
  return msk & BIT(pos); 
}

uint8_t lsb(uint16_t wide_msk) { 
  return (wide_msk & 0xFF); 
}

uint8_t msb(uint16_t wide_msk) { 
  return (wide_msk >> 8); 
}

uint8_t mask(int pos, ...) { 
  uint8_t result=0;
  va_list args;
  va_start(args, pos);
  while (pos != MSK_END) {
    result |= BIT(pos);
        pos = va_arg(args, int);
    }
    va_end(args); 
  return result; 
}
