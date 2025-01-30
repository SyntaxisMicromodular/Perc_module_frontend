/*
 * cpp_main.h
 *
 *  Created on: Dec 5, 2024
 *      Author: Krzysztof
 */

#ifndef INC_CPP_MAIN_H_
#define INC_CPP_MAIN_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void setup();

void EncoderInterrupt();

void messageReceived(char* buf, uint16_t size);

void loop();

#ifdef __cplusplus
}
#endif


#endif /* INC_CPP_MAIN_H_ */
