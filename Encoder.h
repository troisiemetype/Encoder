/*
 * This Arduino library is for polling a rotary encoder
 * Copyright (C) 2016  Pierre-Loup Martin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder{

public:

	enum coderType_t{
		SINGLE_STEP = 0,
		DOUBLE_STEP,
		QUAD_STEP,
	};

	Encoder();

	void begin(uint8_t pin1, uint8_t pin2, coderType_t = SINGLE_STEP);

	void setCoderType(coderType_t type);
	void setDebounceDelay(uint16_t delay);

	bool update();
	
	int8_t getStep();

	void reverse();

	void attach(void *function(int8_t));
	void detach();

	void exec();

protected:

	bool _debounce(uint8_t pin);

	void (*_function)(int8_t);

	bool _invert;
	coderType_t _type;

	uint8_t _pin[2];

	bool _state[2];
	bool _pState[2];
	bool _now[2];
	bool _prev[2];

	uint16_t _time[2];

	bool _direction;
	bool _change;
	bool _quadChange;
	int8_t _step;

	uint16_t _debounceDelay;

};

#endif