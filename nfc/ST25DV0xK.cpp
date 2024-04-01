#include "ST25DV0xK.hpp"
#include <cstdint>
#include <vector>

ST25DV0xK::ST25DV0xK(I2C_master master, unsigned char address, Pin * lpd_gpio, uint8_t memory_size):
    I2C_device(master, address),
    user_memory(new I2C_EEPROM(master, 0xa6, 0x2000)),
    lpd_gpio(lpd_gpio),
    memory_size(memory_size)
{
}

std::optional<uint8_t> ST25DV0xK::Read_register(Registers_system register_name){
    auto register_data = Read<uint16_t>(static_cast<uint16_t>(register_name),1);
    if(not register_data.has_value()){
        return {};
    }
    return register_data.value()[0];
}

uint8_t ST25DV0xK::Write_register(Registers_system register_name, uint8_t value){
    auto data = vector<uint8_t>{value};
    return Write<uint16_t>(static_cast<uint16_t>(register_name), data);
}

std::optional<uint8_t> ST25DV0xK::Read_register(Registers_dynamic register_name){
    auto register_data = user_memory->Read<uint16_t>(static_cast<uint16_t>(register_name),1);
    if(not register_data.has_value()){
        return {};
    }
    return register_data.value()[0];
}

uint8_t ST25DV0xK::Write_register(Registers_dynamic register_name, uint8_t value){
    auto data = vector<uint8_t>{value};
    uint16_t address = static_cast<uint16_t>(register_name);
    return user_memory->Write<uint16_t>(address, data);
}

uint8_t ST25DV0xK::Write_memory(uint16_t address, std::vector<uint8_t> &data){
    return user_memory->Write<uint16_t>(address, data);
}

std::optional<std::vector<uint8_t>> ST25DV0xK::Read_memory(uint16_t address, uint16_t length){
    return user_memory->Read<uint16_t>(address, length);
}

std::optional<uint8_t> ST25DV0xK::ID(){
    return Read_register(Registers_system::MANUF_CODE);
}

void ST25DV0xK::Low_power(ST25DV0xK::State state){
    if(static_cast<bool>(state)){
        lpd_gpio->Set(1);
    }else{
        lpd_gpio->Set(0);
    }
}

std::optional<bool> ST25DV0xK::Locked_state(){
    auto register_data = Read_register(Registers_dynamic::I2C_SSO);
    if (register_data) {
        return !(register_data.value());
    } else {
        return {};
    }
}

bool ST25DV0xK::Present_password(uint64_t password){
    std::vector<uint8_t> password_present_command(19);
    password_present_command[0] = (static_cast<uint16_t>(Registers_system::I2C_PWD) & 0xff00) >> 8;
    password_present_command[1] = (static_cast<uint16_t>(Registers_system::I2C_PWD) & 0x00ff);
    for(int i = 0; i < 8; i++){ // Copy password
        password_present_command[i+2] = (password & ((uint64_t)0xff << (i*8))) >> (i*8);
    }

    password_present_command[10] = 0x09; // Validation code

    for(int i = 0; i < 8; i++){
        password_present_command[i+11] = (password & ((uint64_t)0xff << (i*8))) >> (i*8);
    }
    return Transmit(password_present_command);
}

bool ST25DV0xK::Change_password(uint64_t old_password, uint64_t new_password){
    return false;
}

bool ST25DV0xK::RF_control(ST25DV0xK::State state){
    uint8_t reg_value = 0x0;
    if (state == ST25DV0xK::State::On) {
        reg_value = 0x00;
    } else if (state == ST25DV0xK::State::Off) {
        reg_value = 0x03;
    }
    return Write_register(Registers_dynamic::RF_MNGT, reg_value);
}

std::optional<ST25DV0xK::State> ST25DV0xK::RF_control(){
    auto RF_state_register = Read_register(Registers_dynamic::RF_MNGT);
    if (RF_state_register.has_value()) {
        if ((RF_state_register.value() & 0x03) == 0x00) {
            return ST25DV0xK::State::On;
        } else if ((RF_state_register.value() & 0x03) != 0x00){
            return ST25DV0xK::State::Off;
        }
    }
    return {};
}

std::string ST25DV0xK::Format_register(ST25DV0xK::Registers_system register_name){
    auto register_data = Read<uint16_t>(static_cast<uint16_t>(register_name),1);
    if(not register_data.has_value()){
        return "";
    }
    return emio::format("0b{:08b}",register_data.value()[0]);
}

std::string ST25DV0xK::Format_register(ST25DV0xK::Registers_dynamic register_name){
    //auto register_data = Read<uint16_t>(static_cast<uint16_t>(register_name),1);
    auto register_data = user_memory->Read<uint16_t>(static_cast<uint16_t>(register_name),1);
    if(not register_data.has_value()){
        return "";
    }
    return emio::format("0b{:08b}",register_data.value()[0]);
}
