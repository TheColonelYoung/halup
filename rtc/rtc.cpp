#include "rtc.hpp"

RTC_internal::RTC_internal() {
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    HAL_RTC_Init(&hrtc);
}

void RTC_internal::Set_time(const Time& time) {
    RTC_TimeTypeDef sTime;
    sTime.Hours = time.hours;
    sTime.Minutes = time.minutes;
    sTime.Seconds = time.seconds;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

RTC_internal::Time RTC_internal::Get_time() {
    RTC_TimeTypeDef sTime;
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    return {sTime.Hours, sTime.Minutes, sTime.Seconds};
}

void RTC_internal::Set_date(const Date& date) {
    RTC_DateTypeDef sDate;
    sDate.Year = date.year;
    sDate.Month = date.month;
    sDate.Date = date.day;
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

RTC_internal::Date RTC_internal::Get_date() {
    RTC_DateTypeDef sDate;
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    return {sDate.Year, sDate.Month, sDate.Date};
}

void RTC_internal::Set_alarm(const Time& time) {
    sAlarm.AlarmTime.Hours = time.hours == 255 ? 0 : time.hours;
    sAlarm.AlarmTime.Minutes = time.minutes == 255 ? 0 : time.minutes;
    sAlarm.AlarmTime.Seconds = time.seconds == 255 ? 0 : time.seconds;
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    if (time.hours == 255) {
        sAlarm.AlarmMask |= RTC_ALARMMASK_HOURS;
    }
    if (time.minutes == 255) {
        sAlarm.AlarmMask |= RTC_ALARMMASK_MINUTES;
    }
    if (time.seconds == 255) {
        sAlarm.AlarmMask |= RTC_ALARMMASK_SECONDS;
    }
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 1;
    sAlarm.Alarm = RTC_ALARM_A;
    HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}

RTC_internal::Time RTC_internal::Get_alarm() {
    HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
    return {sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds};
}
