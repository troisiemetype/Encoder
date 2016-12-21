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


#include <Encoder.h>

//Create an instance of Encoder
Encoder encoder = Encoder();

//Var definition for led
byte led = 13;
bool state = 0;

//This will store readings from millis();
long time = 0;

//This stores the delay after which the function is disabled.
long disable = 5000;

void setup(){

	//led definition
	pinMode(led, OUTPUT);
	digitalWrite(led, state);
	
	//Create an encoder on pin 7 and 8.
	encoder.begin(7, 8);

	//Attach the stepHandler function to the encoder
	encoder.attach(stepHandler);

	//Init disable
	time = millis();

}

void loop(){
	//If there was a step, then execute the function attached to encoder.
	if(encoder.update()){
		encoder.exec();
	}

	//If the disable delay has passed without any move, then the function is removed from encoder.
	if((millis() - time) > disable){
		encoder.detach();
	}

}

void stepHandler(char step){
	//Get the current time
	time = millis();
	//if step is 1, toggle the rapidely during one second.
	if(step == 1){
		for(int i = 0; i < 20; i++){
			digitalWrite(led, (state^=1));
			delay(50);
		}
	//Else light it for one second.
	} else {
		digitalWrite(led, 1);
		delay(1000);
		digitalWrite(led, 0);
	}

}