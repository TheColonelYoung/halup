/**
 * @file rtc.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 */

#pragma once

#include "stm32l4xx_hal.h"


extern RTC_HandleTypeDef hrtc;

/**
 * @brief Represent internal peripheral RTC of STM32
 */
class RTC_internal {

    RTC_AlarmTypeDef sAlarm;

public:
    /**
     * @brief   Structure which represent time in hours, minutes and seconds
     */
    struct __attribute__((packed)) Time {
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
    };

    /**
     * @brief   Structure which represent date in year, month and day
     */
    struct __attribute__((packed)) Date {
        uint8_t year;
        uint8_t month;
        uint8_t day;
    };

    /**
     * @brief   Structure which represent timestamp as time and date substructures
     */
    struct __attribute__((packed)) Timestamp {
        Time time;
        Date date;
    };

public:
    RTC_internal();

    void Init();

    void Set_time(const Time& time);

    Time Get_time();

    void Set_date(const Date& date);

    Date Get_date();

    void Set_alarm(const Time& time);

    Time Get_alarm();
};
