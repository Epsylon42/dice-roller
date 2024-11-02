#include <chrono>

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "constants.h"
#include "sleep_utils.h"

using namespace std::chrono;

void disable_modem() {
    //wifi_set_sleep_type(LIGHT_SLEEP_T);
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
}

bool const use_light_sleep = false;
void test_delay(milliseconds duration) {
    if (!use_light_sleep) {
        delay(duration.count());
    } else {
        light_sleep(duration);
    }
}

static void wakeup_callback() {
    Serial.println("Wake up from light sleep");
    Serial.flush();
}

void light_sleep(microseconds duration) {
    extern os_timer_t *timer_list;
    timer_list = nullptr;
    wifi_fpm_open();
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
    //gpio_pin_wakeup_enable(PIN_B1, GPIO_PIN_INTR_LOLEVEL);
    //gpio_pin_wakeup_enable(PIN_B2, GPIO_PIN_INTR_LOLEVEL);
    //gpio_pin_wakeup_enable(PIN_B3, GPIO_PIN_INTR_LOLEVEL);
    gpio_pin_wakeup_enable(PIN_MODE_SW, GPIO_PIN_INTR_LOLEVEL);
    //gpio_pin_wakeup_disable();
    wifi_fpm_set_wakeup_cb(wakeup_callback);
    if (duration == microseconds::max()) {
        wifi_fpm_do_sleep(0xFFFFFFF);
        delay(500);
    } else {
        wifi_fpm_do_sleep(duration.count());
        delay(duration_cast<milliseconds>(duration + 1ms).count());
    }
    Serial.println("Woke up"); // the interrupt callback hits before this is executed*/
}

