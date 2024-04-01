#pragma once

#include "global_includes.hpp"

#define PORT_SIZE         ((uint32_t*) GPIOB - (uint32_t*) GPIOA)
#define ASCII_BASE        (65)
#define PORT_START_ADRESS ((uint32_t*) GPIOA)

/**
 * @brief Represent GPIO of MCU
 * Must be configured in CubeMX
 */
class Pin {
    char port;

protected:
    uint8_t pin_number;

public:

public:
    /**
     * @brief Construct a new Pin object
     *
     * @param port_set Port name, X represents that given pin is not assigned
     *                      to port, for example is connected via expander
     * @param pin_number_set Pin number
     */
    Pin(char port = 0, uint8_t pin_number = -1);

    /**
     * @brief
     *
     * @param rhs Pin for
     * @return true Pins are equal
     * @return false Pins are different
     */
    bool operator==(const Pin& rhs);

    /**
     * @brief Change logic level of pin to opposite from actual
     */
    virtual void Toggle();

    /**
     * @brief Set pin to logic level
     *
     * @param value Logic level to set
     */
    virtual void Set(bool value);

    /**
     * @brief Read logic level from pin
     *
     * @return true  High logic level on input
     * @return false Low logic level on input
     */
    virtual bool Read();
};
