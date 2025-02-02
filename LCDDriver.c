
#include "stm32f4xx_hal.h"

// 8bit command = 0b 1101 0101
// can only send 4 bits per i2c data window
// (because need to send 4 additional meta bits:
// 		Backlight K, ENABLE, R/W, RS
// need to split 8bit data command into 2x8bit commands with overhead bits
// Need to send command with EN on - and subsequentially, command with EN off !!!
void commLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD){
	// incoming datastream 8bit: P7-P4 K E RW RS
	char high = rawcmd & 0b11110000;			// most significant 4 bits of rawcmd
	char low = (rawcmd << 4) & 0b11110000;		// least significant 4 bits of rawcmd
	uint8_t cmd[4];
	cmd[0] = high | 0b00001100;  // 1st part with EN on, also keep LED Kathode on
	cmd[1] = high | 0b00001000;  // 1st part with EN off
	cmd[2] = low  | 0b00001100;  // 1st part with EN on
	cmd[3] = low  | 0b00001000;  // 1st part with EN off
	// We are now sending all these 8bit bytes one after another
	// [I2C_start ... SLAVE ADDRESS ... ACK? ... cmd[0] (high mask with EN on) ... ACK? ... cmd[1] (high mask with EN off) ... ACK?
	// ... cmd[2] (low mask with EN on) ... ACK? ... cmd[3] (low mask with EN off) ... ACK? ]
	HAL_I2C_Master_Transmit(&handle_i2c, addrLCD, (uint8_t*) cmd, 4, 1000);
}

// This is same as commLCD but RS is now 1 (using data register instead of command register)
void charLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD){
	char high = rawcmd & 0b11110000;
	char low = (rawcmd << 4) & 0b11110000;
	uint8_t cmd[4];
	cmd[0] = high | 0b00001101;
	cmd[1] = high | 0b00001001;
	cmd[2] = low  | 0b00001101;
	cmd[3] = low  | 0b00001001;
	HAL_I2C_Master_Transmit(&handle_i2c, addrLCD, (uint8_t*) cmd, 4, 1000);
}

void turnoffLCD(I2C_HandleTypeDef handle_i2c, int addrLCD){
		uint8_t cmd[4];
		cmd[0] = 0b00000100;
		cmd[1] = 0b00000000;
		cmd[2] = 0b00000100;
		cmd[3] = 0b00000000;
		HAL_I2C_Master_Transmit(&handle_i2c, addrLCD, (uint8_t*) cmd, 4, 1000);
}

void turnonLCD(I2C_HandleTypeDef handle_i2c, int addrLCD){
		uint8_t cmd[4];
		cmd[0] = 0b00001100;
		cmd[1] = 0b00001000;
		cmd[2] = 0b00001100;
		cmd[3] = 0b00001000;
		HAL_I2C_Master_Transmit(&handle_i2c, addrLCD, (uint8_t*) cmd, 4, 1000);
}

void lcdWriteStr(char *str, I2C_HandleTypeDef handle_i2c, int addrLCD){
  while(*str){
	  charLCD(*str++, handle_i2c, addrLCD);
  }
}

void setCursorPos(int y, int x, I2C_HandleTypeDef handle_i2c, int addrLCD){
    switch (y){
        case 0:
            x |= 0b10000000;
            break;
        case 1:
            x |= 0b11000000;
            break;
    }
    commLCD(x, handle_i2c, addrLCD);
}

void clearLCD(I2C_HandleTypeDef handle_i2c, int addrLCD){
	commLCD(0b00000001, handle_i2c, addrLCD);
}

// https://cdn.sparkfun.com/assets/9/5/f/7/b/HD44780.pdf
//this is just a list of instructions to "start" the LCD in 4-bit mode
// (+ some additional settings such entry mode set...)
void startLCD4Bit(I2C_HandleTypeDef handle_i2c, int addrLCD){
	// enable 4bit mode
	commLCD (0b00110000, handle_i2c, addrLCD);
	  HAL_Delay(5);
	  commLCD (0b00110000, handle_i2c, addrLCD);
	  HAL_Delay(1);
	  commLCD (0b00110000, handle_i2c, addrLCD);
	  HAL_Delay(10);
	  commLCD (0b00100000, handle_i2c, addrLCD);
	  HAL_Delay(10);

	  commLCD (0b00101000, handle_i2c, addrLCD); // sets data length DL, number of lines and character font
	  HAL_Delay(1);
	  commLCD (0b00001000, handle_i2c, addrLCD); // display off, cursor off
	  HAL_Delay(30);
	  clearLCD(handle_i2c, addrLCD);
	  HAL_Delay(2);
	  commLCD (0b00000110, handle_i2c, addrLCD); // set cursor move direction
	  HAL_Delay(1);
	  commLCD (0b00001100, handle_i2c, addrLCD);  // display on, cursor off
	  HAL_Delay(50);
	  setCursorPos(0, 0, handle_i2c, addrLCD);

}
