/*
 * bitop.h
 *
 *  Created on: Jun 10, 2025
 *      Author: Krzysztof
 *      based on Arduino implementation
 */

#ifndef INC_BITOP_H_
#define INC_BITOP_H_

#include <cstdint>

template <typename T>
inline T bitRead(T value, uint8_t bit) { return ((value) >> (bit)) & 0x01; }

template <typename T>
inline T bitSet(T value, uint8_t bit) { value |= (1UL << (bit)); return value; }

template <typename T>
inline T bitClear(T value, uint8_t bit) { value &= ~(1UL << (bit)); return value; }

template <typename T>
inline T bitWrite(T value, uint8_t bit, bool bitvalue) { if (bitvalue) return bitSet(value, bit); else return bitClear(value, bit); }


#endif /* INC_BITOP_H_ */
