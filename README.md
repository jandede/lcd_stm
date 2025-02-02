# lcd_stm
I2C LCD driver for STM32 (HAL)

## Send 8-bit command
void commLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD);

## Send 8-bit data
void charLCD(char rawcmd, I2C_HandleTypeDef handle_i2c, int addrLCD);

## Write str to display
void lcdWriteStr(char *str, I2C_HandleTypeDef handle_i2c, int addrLCD);

void setCursorPos(int row, int col, I2C_HandleTypeDef handle_i2c, int addrLCD);
void startLCD4Bit(I2C_HandleTypeDef handle_i2c, int addrLCD);
