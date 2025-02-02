/*
 * LCDDriver.h
 *
 *  Created on: Jan 29, 2025
 *      Author: v
 */

#ifndef INC_LCDDRIVER_H_
#define INC_LCDDRIVER_H_

void commLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD);
void charLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD);
void lcdWriteStr(char *str, I2C_HandleTypeDef handle_i2c, int addrLCD);
void setCursorPos(int row, int col, I2C_HandleTypeDef handle_i2c, int addrLCD);
void startLCD4Bit(I2C_HandleTypeDef handle_i2c, int addrLCD);


#endif /* INC_LCDDRIVER_H_ */


