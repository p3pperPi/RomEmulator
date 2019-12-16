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
//	byte n = 0;
//	while(Wire.available()){
//		buff[n] = Wire.read();
//		n++;
//	}
//	Serial.print(i,HEX);
//	Serial.print(" : ");
//	for(int m = 0;m < n;m++){
//		Serial.print(" ");
//		Serial.print(buff[m],HEX);
//	}
//	Serial.println();
//  i+=16;
//  if(i==1024){
//   i = 0;
//   for(int page = 0;page < 4; page++){
//    Wire.beginTransmission(addr + page);
//    Wire.write(addr);
//    for(byte n = 0;n < 0xFF;n++){
//     Wire.write(n - j);
//    }
//    Wire.endTransmission();
//   }
//   j++;
//   delay(2000);
//  }
//  delay(10);
}
