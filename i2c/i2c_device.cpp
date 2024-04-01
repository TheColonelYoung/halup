#include "i2c_device.hpp"

I2C_device::I2C_device(I2C_master &master, unsigned char address):
    master(master), address(address){

}

bool I2C_device::Transmit(const std::vector<uint8_t> &data) const{
    return master.Transmit_poll(address, data);
}

std::optional<std::vector<uint8_t>> I2C_device::Receive(uint length){
    return master.Receive_poll(address, length);
}
