/*
 * demux.cpp
 *
 *  Created on: Jun 10, 2025
 *      Author: Krzysztof
 */
#include "demux.h"

void writeEncoderDemuxAddress(uint8_t channel){
	HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (GPIO_PinState)bitRead(channel, 0));
	HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (GPIO_PinState)bitRead(channel, 1));
	HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (GPIO_PinState)bitRead(channel, 2));
	HAL_GPIO_WritePin(MUX_D_GPIO_Port, MUX_D_Pin, (GPIO_PinState)bitRead(channel, 3));
}



