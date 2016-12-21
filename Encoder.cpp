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

/*
 * This library is intended to be used to read rotary encoders. There are several types of rotary encoder.
 * This library has been written for those which stop on each click with both pins at high or low state,
 * alternatively. It could be used with those which always stop on each click at the same state. In this case
 * it may lead to two steps per click.
 */


#include "Encoder.h"

//Constructor. Init some vars
Encoder::Encoder(){
	_invert = false;

	_debounceDelay = 1;

	_change = false;

	_time[0] = _time[1] = millis();

	_function = NULL;

}

//Define the two pins used to connect the encoder
void Encoder::begin(int pin1, int pin2){
	_pin[0] = pin1;
	_pin[1] = pin2;

	pinMode(_pin[0], INPUT_PULLUP);
	pinMode(_pin[1], INPUT_PULLUP);
}

//Set the debounce delay
void Encoder::setDebounceDelay(int delay){
	_debounceDelay = delay;
}

//Update the reading of digital pins. Has to be called as often as possible.
//Returns true if a step has happened.
bool Encoder::update(){
	_debounce(0);
	_debounce(1);

	//If the states of the two pins are different, we have a change
	if((_state[0] != _state[1]) && !_change){
		_change = true;
		_direction = _invert ^ (_pState[0] == _state[0]);
/*
		Serial.print("Change: ");
		Serial.print(_state[0]);
		Serial.print("   |   ");
		Serial.print(_state[1]);
		Serial.println();
*/
		return false;
	}

	//If the states of the two pins are the same, we may have a click.
	if((_state[0] == _state[1]) && _change){
		_change = false;
		if(_direction){
			_step = 1;			
		} else {
			_step = -1;
		}
/*
		Serial.print("Click:  ");
		Serial.print(_state[0]);
		Serial.print("   |   ");
		Serial.print(_state[1]);
		Serial.println();
*/
		return true;
	}

	return false;

}

//Get a reading after polling. Set back to zero once read.
//exec() uses this function, so be carreful: calling exec() after having calling this
//Will result in the function called with 0 as paramater.
char Encoder::getStep(){
	char state = _step;
	_step = 0;
	return state;
}

//Set the reverse reading. Invert the value returned for a click in one direction.
void Encoder::reverse(){
	_invert = !_invert;
}

//Attach an external function to the encoder.
//The function should have a char as parameter, so the encoder will send it's value.
void Encoder::attach(void *function(char)){
	_function = function;
}

//Remove the function previously attached.
void Encoder::detach(){
	_function = NULL;
}

//Execute the function attached to the encoder, by passing it the last step read (-1, 0 or 1).
//Beware: this function uses getStep(), so don't call getStep() before to call exec(), or it will send 0!
void Encoder::exec(){
	if(_function == NULL) return;
	_function(getStep());
}

//Debounce the reading. Protected function, cannot be called by user.
void Encoder::_debounce(int pin){

	//Store the last read, get a new one.
	_prev[pin] = _now[pin];

	_now[pin] = digitalRead(_pin[pin]);

	//If different, set the debounce time to zero.
	if(_now[pin] != _prev[pin]){
		_time[pin] = millis();
		return;
	}

	//If state has changed AND debounce time is elapsed, update last state of both pin, and update the state of the current one.
	if((_state[pin] != _now[pin]) && ((millis() - _time[pin]) > _debounceDelay)){
		_pState[0] = _state[0];
		_pState[1] = _state[1];
		_state[pin] = _now[pin];
	}
}

