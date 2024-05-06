#include "TMP117.hpp"

TMP117::TMP117(I2C_master master, unsigned char address):
    I2C_device(master, address)
{
}

std::optional<float> TMP117::Temperature(){
    auto register_values = Read(static_cast<uint8_t>(Registers::Temp_Result),2);
    if (register_values.has_value() == false) {
        return {};
    }
    int16_t temp_value = (register_values.value()[0] << 8) + register_values.value()[1];
    return temp_value * 0.0078125f;
}

std::optional<uint16_t> TMP117::ID(){
    auto register_values = Read(static_cast<uint8_t>(Registers::Device_ID),2);
    if (register_values.has_value() == false) {
        return {};
    }
    return (register_values.value()[0] << 8) + register_values.value()[1];
}

void TMP117::Configure_mode(TMP117::Mode mode){
    auto register_values = Read(static_cast<uint8_t>(Registers::Configuration),2);
    if (register_values.has_value() == false) {
        return;
    }
    uint16_t config_value = (register_values.value()[0] << 8) + register_values.value()[1];
    config_value &= ~(0b11 << 10);
    config_value |= static_cast<uint8_t>(mode) << 10;
    Write(static_cast<uint8_t>(Registers::Configuration), { static_cast<uint8_t>(config_value >> 8), static_cast<uint8_t>(config_value & 0xFF) });
}

std::optional<bool> TMP117::Data_ready(){
    auto register_values = Read(static_cast<uint8_t>(Registers::Configuration),2);
    if (register_values.has_value() == false) {
        return {};
    }
    uint16_t config_value = (register_values.value()[0] << 8) + register_values.value()[1];
    return (config_value & (1 << 13));
}
