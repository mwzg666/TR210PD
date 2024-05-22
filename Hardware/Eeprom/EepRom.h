#ifndef __EEPROM_H__
#define __EEPROM_H__

//===================================定义=================================




//==================================变量声明=============================



//==================================函数声明=============================

void EEPROM_read(u32 EE_address,u8 *DataAddress,u16 length);
u8 EEPROM_write(u32 EE_address,u8 *DataAddress,u16 length);
void EEPROM_SectorErase(u32 EE_address);


#endif

