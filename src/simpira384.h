#ifndef simpira384_H
#define simpira384_H

#include <stdint.h>

void simpira384_permute(uint8_t state_u8[48]);

void simpira384_permute_inv(uint8_t state_u8[48]);

#endif