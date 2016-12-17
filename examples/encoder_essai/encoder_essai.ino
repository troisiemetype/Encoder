#include <Encoder.h>

Encoder encoder = Encoder();

void setup(){
	Serial.begin(115200);
	encoder.begin(2, 3);

}

void loop(){
	encoder.update();
	char step = encoder.getStep();

	if(step){
		Serial.println((int)step);
	}

}