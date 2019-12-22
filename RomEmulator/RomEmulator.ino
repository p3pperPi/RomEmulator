#include <avr/pgmspace.h>
#include "SoftIIC.h"
#include <EEPROM.h>

// Note: these can be *any* pins, not just a4/a5.
#define SDA_PIN  A4
#define SCL_PIN  A5

#define SERIAL_PORT_SPEED 38400
#define IIC_SPEED 50

//CONSTANTS for AT24C08A
const uint8_t RET_ACK  = 0x01;
const uint8_t RET_NACK = 0x00;
const bool RW_READ  = 1;
const bool RW_WRITE = 0;

const byte ROM_ADDR = 0x14;//0b00010100


//CONSTANTS and variables for I2C function
int  byte_Counter = 0;
byte current_Page = 0;
bool current_RW   = 0;
byte current_MEM  = 0; //currnet ROM address


// treat address
// 0bnnnnnxxx -> nnnnn
byte get_addr_digit(byte addr){
 return (addr & 0xF8) >> 3;
}

// 0bxxxxxnnx -> nn
byte get_page_digit(byte addr){
 return (addr & 0x06) >> 1;
}

// 0bxxxxxxxn -> n
bool get_rw_digit(byte addr){
 return (bool)(addr & 0x01);
}







SoftIIC  my_SoftIIC = SoftIIC(SCL_PIN, SDA_PIN, IIC_SPEED, true, true, true);

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  
	display_rom();
}

void loop() {

  // Last, act as A 24c04 eeprom (read-only) slave
  uint8_t successful_bytes = 0;
  uint16_t TOTAL_EXPECTED_BYTES = 512;
  while (successful_bytes < TOTAL_EXPECTED_BYTES) {
    successful_bytes = successful_bytes + my_SoftIIC.SlaveHandleTransaction(
      respond_to_address,
      respond_to_command,
      respond_to_data,

      get_current_register_address,
      set_current_register_address,

      read_iic_slave,
      write_iic_slave);
  }

  Serial.print("get data :");
  Serial.println(successful_bytes);

}



void display_rom(){
	for(int page = 0;page < 4;page++){
		Serial.print("page");
		Serial.print(page);
		for(byte i = 0;i <= 0x0F;i++){
			Serial.print('\t');
			Serial.print('n');
			Serial.print(i,HEX);
		}
		Serial.println();
		for(int upper = 0;upper <= 0x0F;upper++){
			Serial.print(upper,HEX);
			Serial.print("n : ");
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

//////////////////////////////////////////////////////////// These functions should be edited to give the iic slave a 'personality'. ////////////////////////////////////////////////////////////////


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// EEPROM Functions
//
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void ROM_set_byte(byte page,byte addr,byte data){
 EEPROM.update( ((page&0x03) << 8) | addr , data);
}

byte ROM_read_byte(byte page,byte addr){
 return  EEPROM.read(((page&0x03) << 8) | addr );
}



// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// I2C Functions
//
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

uint8_t get_current_register_address(uint8_t chipaddr) {
  return 0x00;
}
uint8_t set_current_register_address(uint8_t chipaddr, uint8_t regaddr) {
  return 0x00;
}




uint8_t respond_to_address(uint8_t chipaddr){
  if(get_addr_digit(chipaddr) == ROM_ADDR){
    byte_Counter = 0;
    current_Page = get_page_digit(chipaddr);
    current_RW   = get_rw_digit(chipaddr);

    return RET_ACK;
  }
  return RET_NACK;
}


uint8_t respond_to_command(uint8_t word_Address){
  current_MEM = word_Address;
  return RET_ACK;
}


uint8_t respond_to_data(uint8_t received_Byte){
  return RET_ACK;
}



uint8_t read_iic_slave(uint8_t chipaddr_7bit, uint8_t* value) {
  *value = ROM_read_byte(current_Page,current_MEM+byte_Counter);
  byte_Counter++;
  return 0; // send data to the master until master returns NACK.
}

uint8_t write_iic_slave(uint8_t chipaddr_7bit, uint8_t received_Byte) {
  ROM_set_byte(current_Page,current_MEM+byte_Counter,received_Byte);
  byte_Counter++;
  return 0x00; // NO MEANING
}
