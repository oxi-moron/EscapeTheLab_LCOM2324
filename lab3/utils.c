#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdint.h>
#include <assert.h>

uint32_t counter;

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t new_val = *value;

  assert(sys_inb(port, &new_val) == 0);
  
  *value = new_val & 0x000000FF;

#ifdef LAB3
  counter++;
#endif

  return 0;
}
