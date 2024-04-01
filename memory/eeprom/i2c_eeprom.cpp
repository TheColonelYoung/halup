#include "i2c_eeprom.hpp"

I2C_EEPROM::I2C_EEPROM(I2C_master master, unsigned char address, const uint32_t memory_size) :
    I2C_device(master, address),
    memory_size(memory_size)
{ }
