/**
 * @file TMP117.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 */

#pragma once

#include <optional>

#include "i2c/i2c_device.hpp"

/**
 * @brief   High-Accuracy, Low-Power, Digital Temperature Sensor with I2C Interface
 */
class TMP117 : public I2C_device{
public:
    /**
     * @brief  Registers of TMP117 sensor
     */
    enum class Registers: uint8_t {
        Temp_Result    = 0x00,
        Configuration  = 0x01,
        THigh_Limit    = 0x02,
        TLow_Limit     = 0x03,
        EEPROM_UL      = 0x04,
        EEPROM1        = 0x05,
        EEPROM2        = 0x06,
        Temp_Offset    = 0x07,
        EEPROM3        = 0x08,
        Device_ID      = 0x0F
    };

    /**
     * @brief Construct a new TMP117 object
     *
     * @param master    I2C bus/interface/master which is connected to sensor
     * @param address   Address of sensor on I2C bus, 8-bit address padded with 0 at the end
     */
    TMP117(I2C_master master, unsigned char address);

    /**
     * @brief   Read temperature from sensor
     *
     * @return float   Temperature in Celsius
     */
    std::optional<float> Temperature();

    /**
     * @brief Reads device ID from register, correct value for TMP117 is 0x117
     *
     * @return uint16_t Device ID of chip, correct value for TMP117 is 0x117
     */
    std::optional<uint16_t> ID();
};
