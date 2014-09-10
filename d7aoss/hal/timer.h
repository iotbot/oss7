/*! \file timer.h
 *
 * \copyright (C) Copyright 2013 University of Antwerp (http://www.cosys-lab.be) and others.\n
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.\n
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * \author maarten.weyn@uantwerpen.be
 *
 */

#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

void hal_timer_init();

uint16_t hal_timer_getvalue();

void hal_timer_setvalue(uint16_t next_event);

void hal_timer_enable_interrupt();

void hal_timer_disable_interrupt();


void hal_benchmarking_timer_init();
uint32_t hal_benchmarking_timer_getvalue();
void hal_benchmarking_timer_start();
void hal_benchmarking_timer_stop();

#endif /* HAL_TIMER_H_ */
