#include "nodes.h"

uint16_t **parse_data_nodes(uint8_t *node_data, size_t node_data_length)
{
	// Each node is composed of 8 bytes. Based off https://github.com/jason0597/Seedminer/blob/master/src/main/java/FileParsing.java#L34
	static uint16_t return_data[2][node_data_length / 8]; 
	
	for(int i = 0, j = 0; i < node_data_length; i+=8)
	{
		
	}
}