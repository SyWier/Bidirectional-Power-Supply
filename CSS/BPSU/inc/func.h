/*
 * func.h
 *
 *  Created on: Apr 27, 2025
 *      Author: Daniel
 */

#ifndef INC_FUNC_H_
#define INC_FUNC_H_

#include <stdbool.h>

typedef struct {
    bool state;
    bool enable;
    bool disable;
} State_t;

void Blink(uint32_t pin, uint32_t cntMax, uint32_t* cnt);
void LedState();
void GateControl(bool enable);

#endif /* INC_FUNC_H_ */
