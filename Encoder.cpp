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
 * You can choose the appropriate one by using the coderType_t enum values
 * SINGLE_STEP is for encoder with one step per click, or non clicked encoders.
 * DOUBLE_STEP is for encoders with two steps per click.
 * QUAD_STEP is for encoders with four teps per click.
 *
 * Start by calling begin with the two pins of the encoder, and optionnaly the encoder type.
 * If not specified, it will default to SINGLE_STEP.
 *
 * Set the debounce delay, or leave it with the default value of 200 microseconds.
 *
 * By calling reverse(), you will reverse the reading direction for every read.
 * Call it again if you want to come back to previous setting.
 *
 * Call update() as ogten as posible in your program (typically once per loop).
 * update() will return true if the encoder has been moved.
 *
 * Then call getStep(), that will return the value of the step, positive or negative.
 *
 * With attach(function), you can link any function to the encoder.
 * That function must take a char or int8_t as parameter, and return nothing (void).
 * Then instead of calling getStep(), you can call exec(),
 * that will automaticely pass the step to your function.
 *
 * Use detach() to free the encoder from your function.
 */


#include "Encoder.h"

//Constructor. Init some vars
Encoder::Encoder(){
	_invert = false;

	_debounceDelay = 200;

	_change = false;

	_time[0] = _time[1] = micros();

	_function = NULL;

}

//Define the two pins used to connect the encoder
void Encoder::begin(uint8_t pin1, uint8_t pin2, Encoder::coderType_t type){
	_pin[0] = pin1;
	_pin[1] = pin2;

	pinMode(_pin[0], INPUT_PULLUP);
	pinMode(_pin[1], INPUT_PULLUP);

	setCoderType(type);
}

//Set the coder type
void Encoder::setCoderType(Encoder::coderType_t type){
	_type = type;
}

//Set the debounce delay, microseconds
void Encoder::setDebounceDelay(uint16_t delay){
	_debounceDelay = delay;
}

//Update the reading of digital pins. Has to be called as often as possible.
//Returns true if a step has happened.
bool Encoder::update(){
	bool step = 0;

	//Decode steps according to encoder type:
	//SINGLE_STEP is an encoder where each click is a single pin change
	//DOUBLE_STEP is an encoder where each click is on the same state,
	//and a step is a change on both pin, one direction or other
	//QUAD_STEP is an encoder where each click is two DOUBLE_STEP.
	if(_type == SINGLE_STEP){
		if(_debounce(0)){
			if(_state[0] ^_state[1]){
				_step = 1;
			} else {
				_step = -1;
			}
			step = true;

		} else if(_debounce(1)){
			if(_state[0] ^_state[1]){
				_step = -1;
			} else {
				_step = 1;
			}
			step = true;
		}

		step = false;

	} else {
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
	//		step = false;
		}

		//If the states of the two pins are the same, we may have a click.
		if((_state[0] == _state[1]) && _change){
			_quadChange = !_quadChange;
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
			if(_type == QUAD_STEP){
				step = _quadChange;
			} else {
				step = true;
			}
		}		
	}

	return step;

}

//Get a reading after polling. Set back to zero once read.
//exec() uses this function, so be carreful: calling exec() after having calling this
//Will result in the function called with 0 as paramater.
int8_t Encoder::getStep(){
	int8_t state = _step;
	_step = 0;
	return state;
}

//Set the reverse reading. Invert the value returned for a click in one direction.
void Encoder::reverse(){
	_invert = !_invert;
}

//Attach an external function to the encoder.
//The function should have a char as parameter, so the encoder will send it's value.
void Encoder::attach(void *function(int8_t)){
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
bool Encoder::_debounce(uint8_t pin){

	//Store the last read, get a new one.
	_prev[pin] = _now[pin];

	_now[pin] = digitalRead(_pin[pin]);

	//If different, set the debounce time to zero.
	if(_now[pin] != _prev[pin]){
		_time[pin] = micros();
		return false;
	}

	//If state has changed AND debounce time is elapsed, update last state of both pin, and update the state of the current one.
	if((_state[pin] != _now[pin]) && ((micros() - _time[pin]) > _debounceDelay)){
		_pState[0] = _state[0];
		_pState[1] = _state[1];
		_state[pin] = _now[pin];

		return true;
	}

	return false;
}

