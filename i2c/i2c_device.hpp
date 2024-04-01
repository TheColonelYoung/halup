/**
 * @file i2c_device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "i2c/i2c_master.hpp"

/**
 * @brief Generic class for all devices, which are connected to I2C bus
 *
 */
class I2C_device{
private:
    /**
     * @brief Object of master bus on which is device connected
     */
    I2C_master master;

    /**
     * @brief   Address of device on I2C bus
     *          7 highest bits of byte, bit 0 is not used for addressing but for mode (read/write)
     *          0bxxxxxxx0, 7 bit address must be shifted to left
     */
    uint8_t address = 0;

public:
    I2C_device() = default;
    /**
     * @brief Construct a new I2C device object
     *
     * @param master Object of master I2C bus on which is device connected
     * @param address Address of device on I2C bus
     */
    I2C_device(I2C_master &master, unsigned char address);

public:
    /**
     * @brief           Transmit data to device
     *
     * @param data      Data to be send by bus
     * @return bool     True if packet was successfully transmitted (ACKed)
     */
    bool Transmit(const std::vector<uint8_t> &data) const;

    /**
     * @brief   Receive data from device on bus
     *
     * @param length    Number of bytes to be received
     * @return optional<vector<uint8_t>> Return read value if transmission was successful, otherwise empty optional
     */
    std::optional<std::vector<uint8_t>> Receive(uint length) ;

    /**
     * @brief   Write data to device using standart I2C method
     *          Sending address of device register and then data to write
     *          Address is not autoincremented
     *
     * @param mem_address   Address in device memory to write data
     * @param data          Data to be written into device
     * @return bool         True if packet was successfully transmitted (ACKed)
     */
    template<typename T = uint8_t>
    bool Write(T mem_address, std::vector<uint8_t> data){
        std::vector<uint8_t> address_bytes(sizeof(T));
        std::reverse_copy(reinterpret_cast<const uint8_t*>(&mem_address),
                          reinterpret_cast<const uint8_t*>(&mem_address) + sizeof(T),
                          address_bytes.begin());
        data.insert(data.begin(), address_bytes.begin(), address_bytes.end());
        return master.Transmit_poll(address, data);
    }

    /**
     * @brief   Read data from device memory
     *          Address is transmitted to device and then is received number of bytes from device
     *
     * @param mem_address       Address in device memory to read data
     * @param length            Number of bytes to be received
     * @return vector<uint8_t>  Received data
     */
    template<typename T = uint8_t>
    std::optional<std::vector<uint8_t>> Read(T mem_address, uint length){
        std::vector<uint8_t> address_bytes(sizeof(T));
        // Switch endianity of address
        std::reverse_copy(reinterpret_cast<const uint8_t*>(&mem_address),
                          reinterpret_cast<const uint8_t*>(&mem_address) + sizeof(T),
                          address_bytes.begin());
        if(master.Transmit_poll(address, address_bytes)){
            return master.Receive_poll(address, length);
        } else {
            return {};
        }
    }
};
