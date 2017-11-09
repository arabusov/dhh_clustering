#pragma once
#include <stdint.h>
struct sor
{
  uint16_t row : 10;
  uint16_t dhp_index : 2;
  uint16_t dedx : 2;
  uint16_t soc_flag : 1;
  uint16_t sor_flag : 1;
};

struct pix
{
  uint16_t adc_val : 8;
  uint16_t col : 6;
  uint16_t inc_row : 1;
  uint16_t pix_flag : 1;
};


