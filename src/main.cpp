/*
 * @Author: MIngya jin2899042558@163.com
 * @Date: 2024-07-18 15:45:53
 * @LastEditors: MIngya jin2899042558@163.com
 * @LastEditTime: 2024-07-19 10:13:29
 * @FilePath: \Codingc:\Users\ASUS\Documents\PlatformIO\Projects\huiguang_timer\src\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <math.h>

RTC_DS3231 rtc;
int datapin = 2;
int latchpin = 3;
int clockpin = 4;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // 将RTC时间设置成编译程序的日期与时间
    
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);

}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    // 串口返回ds3231的时间，测试用

    int hour_one = now.hour() % 10;
    int hour_ten = now.hour() / 10;
    int min_one = now.minute() % 10;
    int min_ten = now.minute() / 10;

    int data_hour = hour_ten * pow(2,4) + hour_one;
    int data_min = min_ten * pow(2,4) + min_one;

    digitalWrite(latchpin, LOW);
    shiftOut(datapin, clockpin, LSBFIRST, data_hour);
    shiftOut(datapin, clockpin, LSBFIRST, data_min);
    digitalWrite(latchpin, HIGH);

    Serial.println();
    delay(1000);
}