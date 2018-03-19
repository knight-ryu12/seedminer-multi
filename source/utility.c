#include "utility.h"

uint8_t *u64_to_u8(uint64_t val)
{
	static uint8_t tmp_u8[8];
	uint8_t *tmpval = (uint8_t *)&val; //pointer to the original val;
	for(int i = 0; i < 8; i++)
	{
		tmp_u8[i] = tmpval;
	}
	return tmp_u8;
}