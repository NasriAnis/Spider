#include <int.h>

void pic_setup_icw(void);
void pic_set_masks(uint8_t master_mask, uint8_t slave_mask);
void pic_send_eoi(uint8_t irq);