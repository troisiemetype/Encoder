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

	Encoder();

	void begin(int pin1, int pin2);

	void setDebounceDelay(int delay);

	bool update();
	
	char getStep();

	void reverse();

protected:

	void _debounce(int pin);

	bool _invert;

	int _pin[2];

	bool _state[2];
	bool _pState[2];
	bool _now[2];
	bool _prev[2];

	long _time[2];
	long _prevTime[2];

	bool _direction;
	bool _change;
	char _step;

	int _debounceDelay;






};

#endif