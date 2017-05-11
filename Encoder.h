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

	void begin(int pin1, int pin2, coderType_t = SINGLE_STEP);

	void setCoderType(coderType_t type);
	void setDebounceDelay(int delay);

	bool update();
	
	char getStep();

	void reverse();

	void attach(void *function(char));
	void detach();

	void exec();

protected:

	bool _debounce(int pin);

	void (*_function)(char);

	bool _invert;
	coderType_t _type;

	int _pin[2];

	bool _state[2];
	bool _pState[2];
	bool _now[2];
	bool _prev[2];

	long _time[2];
	long _prevTime[2];

	bool _direction;
	bool _change;
	bool _quadChange;
	char _step;

	int _debounceDelay;

};

#endif