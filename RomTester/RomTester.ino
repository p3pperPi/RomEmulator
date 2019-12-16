#include <Wire.h>
int i = 0;
int j = 1;
const byte ADDR = 0b01010000;
void setup() {
 Serial.begin(38400);
  Wire.begin();
}


byte buff[16];

void loop() {
	for(byte i = 0;i < 0x0F;i++){
		Serial.print('\t');
		Serial.print(i,HEX);
	}
	Serial.println();
	for(byte page = 0;page < 4;page++){
		for(byte upper = 0;upper < 0x0F ;upper++){
		  Wire.beginTransmission(ADDR + page);
		  Wire.write(upper);
		  Wire.endTransmission();
		  Wire.requestFrom(ADDR+page, 16);
			byte pos = 0;
		  while(Wire.available()){
				buff[pos] = Wire.read();
				pos++;
			}
			
			Serial.print("p");
			Serial.print(page);
			Serial.print("/");
			Serial.print(upper,HEX);
			Serial.print("0 : ");
			for(byte i = 0;i < 0x0F;i++){
				Serial.print('\t');
				Serial.print(buff[i],HEX);
			}
			Serial.println();
		}
	}

	delay(3000);
}
