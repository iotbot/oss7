/*! \file leds.h
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
 *  \author glenn.ergeerts@uantwerpen.be
 *
 */

#ifndef __LEDS_H__
#define __LEDS_H__

void led_init();

void led_on(unsigned char led_nr);
void led_off(unsigned char led_nr);
void led_toggle(unsigned char led_nr);


#endif // __LEDS_H__
