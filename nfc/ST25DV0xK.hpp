/**
 * @file ST25DV0xK.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 */

#pragma once

#include "i2c/i2c_device.hpp"
#include "memory/eeprom/i2c_eeprom.hpp"
#include "gpio/pin.hpp"

#include "emio/emio.hpp"

#include <vector>
#include <optional>
#include <map>

/**
 * @brief   ST25DV0xK: Dynamic NFC/RFID tag IC with 4-64 Kbit EEPROM
 *          Memory is chip is divided into 4 sections
 *          - User memory - For NFC data
 *          - Dynamic registers - Temporary volatile changes to chip configuration
 *          - Mailbox - 256 bytes of fast memory for direct data streaming
 *          - System registers - Configuration of chip, non-volatile, password protected
 *                             - Also contains reads only device information like ID, etc
 */
class ST25DV0xK : public I2C_device{
private:

    I2C_EEPROM *const user_memory;

    /**
     * @brief   Pin which is used to control low power mode of NFC
     */
    Pin * const lpd_gpio;

    /**
     * @brief Size of memory in kb
     */
    const uint8_t memory_size;

    /**
     * @brief Address of first mailbox byte
     */
    static const uint16_t mailbox_start_address = 0x2008;

    static const uint64_t default_password = 0x0;

    /**
     * @brief Mapping of value in register MEM_SIZE to real memory size in kb
     *          Value of MEM_SIZE is expressed as amount of RF blocks (4bytes)
     */
    const std::map<uint16_t, uint8_t> memory_size_map = {
        {0x007F, 4},
        {0x01ff, 16},
        {0x07ff, 64}
    };

public:
    /**
     * @brief  Registers of ST25DV0xK IC, accessed by I2C
     */
    enum class Registers_system: uint16_t {
        GPO         = 0x0000,
        IT_TIME     = 0x0001,
        EH_MODE     = 0x0002,
        RF_MNGT     = 0x0003,
        RFA1SS      = 0x0004,
        ENDA1       = 0x0005,
        RFA2SS      = 0x0006,
        ENDA2       = 0x0007,
        RFA3SS      = 0x0008,
        ENDA3       = 0x0009,
        RFA4SS      = 0x000a,
        I2CSS       = 0x000b,
        LOCK_CCFILE = 0x000c,
        MB_MODE     = 0x000d,
        MB_WDG      = 0x000e,
        LOCK_CFG    = 0x000f,
        LOCK_DSFID  = 0x0010,
        LOCK_AFI    = 0x0011,
        DSFID       = 0x0012,
        AFI         = 0x0013,
        MEM_SIZE    = 0x0014,
        BLK_SIZE    = 0x0016,
        IC_REF      = 0x0017,
        UID         = 0x0018,
        PROD_CODE   = 0x001d,
        MANUF_CODE  = 0x001e,
        IC_REV      = 0x0020,
        I2C_PWD     = 0x0900,
    };

    enum class Registers_dynamic: uint16_t {
        GPO_CTRL    = 0x2000,
        EH_CTRL     = 0x2002,
        RF_MNGT     = 0x2003,
        I2C_SSO     = 0x2004,
        IT_STS      = 0x2005,
        MB_CTRL     = 0x2006,
        MB_LEN      = 0x2007
    };

    enum class State: bool{
        Off  = false,
        On   = true
    };

    /**
     * @brief Construct a new ST25DV0xK object
     *
     * @param master    I2C bus/interface/master which is connected to sensor
     * @param address   Address of sensor on I2C bus, 8-bit address format padded with 0 at the end
     * @param memory_size   Size of memory in kB
     */
    ST25DV0xK(I2C_master master, unsigned char address, Pin * const lpd_gpio, uint8_t memory_size);

    /**
     * @brief   Read value for register
     *
     * @param register_name Name of register
     * @return uint8_t      Value of register
     */
    std::optional<uint8_t> Read_register(Registers_system register_name);

    /**
     * @brief Write value to a register
     *
     * @param register_name Name of register
     * @param value         New value of register
     */
    uint8_t Write_register(Registers_system register_name, uint8_t value);

    /**
     * @brief   Read value for register
     *
     * @param register_name Name of register
     * @return uint8_t      Value of register
     */
    std::optional<uint8_t> Read_register(Registers_dynamic register_name);

    /**
     * @brief Write value to a register
     *
     * @param register_name Name of register
     * @param value         New value of register
     */
    uint8_t Write_register(Registers_dynamic register_name, uint8_t value);

    /**
     * @brief Write data into EEPROM memory of NFC
     *
     * @param address   Address of first byte in memory
     * @param data      Data to write into memory
     * @return uint8_t  Number of bytes written
     */
    uint8_t Write_memory(uint16_t address, std::vector<uint8_t> &data);

    /**
     * @brief Read data from EEPROM memory of NFC
     *
     * @param address   Address of first byte in memory
     * @param length    Number of bytes to read
     * @return std::optional<std::vector<uint8_t>>  Data read from memory if read from device was successful, otherwise empty optional
     */
    std::optional<std::vector<uint8_t>> Read_memory(uint16_t address, uint16_t length);

    /**
     * @brief Read ID of device
     *
     * @return std::optional<uint8_t>  ID of device if read from device was successful, otherwise empty optional
     *                                 Correct ID of device is stored in constant and should be 0x02
     */
    std::optional<uint8_t> ID();

    /**
     * @brief Control low power mode of NFC
     *
     * @param state New state of low power mode, If On then NDC IC is forced into low power mode
     */
    void Low_power(State state);

    /**
     * @brief Read state of lock which protects system registers
     *
     * @return std::optional<bool>  State of lock if read from device was successful, otherwise empty optional
     */
    std::optional<bool> Locked_state();

    /**
     * @brief Present password to IC in order to unlock system registers which are non-volatile
     *
     * @param password  Password to present to IC, 64-bit value, default is 0x0
     * @return true     Password was presented successfully
     * @return false    Password was not presented successfully, incorrect password
     */
    bool Present_password(uint64_t password = default_password);

    /**
     * @brief Write new password into device and lock system registers
     *
     * @param old_password  Old password to unlock system registers
     * @param new_password  New password to write into device
     * @return true     Password was changed successfully
     * @return false    Password was not changed successfully, incorrect old password
     */
    bool Change_password(uint64_t old_password, uint64_t new_password);

    /**
     * @brief       Control state of RF communication. On/Off
     *
     * @param state    New state of RF control
     * @return true    Update of RF control was successful
     * @return false   Update of RF control was not successful
     */
    bool RF_control(State state);

    /**
     * @brief      Reads state of RF communication. On/Off
     *
     * @return std::optional<State> State of RF communication if read from device was successful, otherwise empty optional
     */
    std::optional<State> RF_control();

    /**
     * @brief Format given system register value into binary representation
     *
     * @param register_name Name of register to format
     * @return std::string  Formatted string of register value
     */
    std::string Format_register(Registers_system register_name);

    /**
     * @brief Format given dynamic register value into binary representation
     *
     * @param register_name Name of register to format
     * @return std::string  Formatted string of register value
     */
    std::string Format_register(Registers_dynamic register_name);

    /**
     * @brief   Return size of memory in kb
     *
     * @return uint8_t  Size of memory in kb
     */
    uint8_t Memory_size() const{ return memory_size; };
};
