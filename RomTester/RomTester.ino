#include <Wire.h>
int i = 0;
int j = 1;
const byte addr = 0b01010000;
void setup() {
 Serial.begin(38400);
  Wire.begin();
}

byte data[1024];

void loop() {
  Wire.beginTransmission(addr + (i/0xFF));
  Wire.write(i%0xFF);
  Wire.endTransmission();
  Wire.requestFrom(addr + (i/0xFF), 1, true);

	while(Wire.available()){
	  Serial.print(i);
	  Serial.print(":");
	  Serial.println(Wire.read());
	}
  i++;
  if(i==1024){
   i = 0;
   for(int page = 0;page < 4; page++){
    Wire.beginTransmission(addr + page);
    Wire.write(addr);
    for(byte n = 0;n < 0xFF;n++){
     Wire.write(n - j);
    }
    Wire.endTransmission();
   }
   j++;
  }
  delay(100);
}
