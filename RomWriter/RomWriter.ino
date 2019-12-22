#include <EEPROM.h>
byte data[] = {
	#include "data.h"
};


void setup() {	
	for(int i = 0;i < sizeof(data);i++){
		EEPROM.update(i,data[i]);
	}
	Serial.begin(38400);

	delay(5000);

	Serial.println();
	
	Serial.print("EEPROM Size : ");
	Serial.print(EEPROM.length());
	Serial.println("byte");
	
	Serial.print(sizeof(data));
	Serial.println("byte written.");

	for(int page = 0;page < 4;page++){
		for(int upper = 0;upper <= 0x0F;upper++){
			
			Serial.print("p");
			Serial.print(page);
			Serial.print("/");
			Serial.print(upper,HEX);
			Serial.print("0 : ");
			for(byte lower = 0;lower <= 0x0F;lower++){
				int itr = 
					((page  & 0x03) << 8) |
					((upper & 0x0F) << 4) |
					((lower & 0x0F) << 0) ;
				Serial.print('\t');
				Serial.print(EEPROM.read(itr),HEX);
//				Serial.print(data[itr],HEX);
			}
			Serial.println();
		}
	}
}

void loop() {

}
