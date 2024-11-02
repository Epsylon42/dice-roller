#include <array>
#include <cinttypes>
#include <chrono>

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#include "constants.h"
#include "dice.h"
#include "display.h"
#include "sleep_utils.h"
//#include "settings.h"
#include "ButtonEncoder.h"

using namespace std::chrono;

#if 1 == HARDWARE_VERSION
Adafruit_SSD1331 g_display_device = Adafruit_SSD1331(&SPI, PIN_CS, PIN_DC, PIN_RST);
#elif 2 == HARDWARE_VERSION
Adafruit_SSD1331 g_display_device = Adafruit_SSD1331(&SPI, PIN_CS, PIN_DC);
#endif

ButtonEncoder<3> g_dice_buttons{{PIN_B1, PIN_B2, PIN_B3}};
ButtonEncoder<1> g_mode_button{{PIN_MODE_SW}};

//SettingsStorage g_settings_storage;
//Settings g_settings{g_settings_storage.settings};

ESP8266TrueRandomClass g_rng{};

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

uint16_t const COLOR = GREEN;

void setup() {
    Serial.begin(115200);
    Serial.print("Serial ON");

    disable_modem();

    Serial.print("Setup done");
    g_display_device.begin();
    Serial.print("Display initialized");
    g_display_device.fillScreen(BLACK);

    g_display_device.drawRect(0, 0, g_display_device.width(), g_display_device.height(), COLOR);
    g_display_device.setCursor(0,0);
    g_display_device.setTextColor(COLOR);
}


void callback() {
    Serial.print("End LightSleep:");
    Serial.println(millis());
    Serial.flush();
}

void goToSleep() {
    uint32_t sleep_time_in_ms = 2000;
    Serial.print("go to LightSleep:");
    Serial.println(millis());
    Serial.flush();

    wifi_set_opmode(NULL_MODE);
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
    wifi_fpm_set_wakeup_cb(callback);
    gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_MODE_SW), GPIO_PIN_INTR_LOLEVEL);
    gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_B1), GPIO_PIN_INTR_LOLEVEL);
    gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_B2), GPIO_PIN_INTR_LOLEVEL);
    gpio_pin_wakeup_enable(GPIO_ID_PIN(PIN_B3), GPIO_PIN_INTR_LOLEVEL);
    wifi_fpm_open();
    wifi_fpm_do_sleep(0xFFFFFFF);
    delay(100);
    Serial.println("Woke up");
}

enum class CheckRollsResult {
    Processing,
    Idle,
};
CheckRollsResult check_rolls();

void loop() {
    if (check_rolls() == CheckRollsResult::Idle) {
        delay(50);
        goToSleep();
    }
}

bool full_cleared = true;
void clearScreen() {
    g_display_device.fillRect(1, 1, g_display_device.width() - 2, g_display_device.height() - 2, BLACK);
    if (full_cleared) {
        g_display_device.drawRect(0, 0, g_display_device.width(), g_display_device.height(), COLOR);
        full_cleared = false;
    }
}

DiceDescription const* history_dice = nullptr;
std::array<uint16_t, 6> roll_history;
size_t roll_history_size;
bool history_redraw_required = false;

void updHistory(DiceDescription const* dice, uint16_t value)
{
    if (dice != history_dice) {
        history_dice = dice;
        roll_history_size = 0;
    }

    if (roll_history_size >= roll_history.size()) {
        for (size_t i = 0; i < roll_history.size() - 1; i += 1) {
            roll_history[i] = roll_history[i + 1];
        }
        roll_history_size -= 1;
    }

    roll_history[roll_history_size] = value;
    roll_history_size += 1;
    history_redraw_required = true;
}

void clearHistory() {
    history_dice = nullptr;
    roll_history_size = 0;
}

void flushHistory() {
    if (history_redraw_required) {
        history_redraw_required = false;
        g_display_device.setTextSize(1);
        g_display_device.setCursor(g_display_device.width() - 10, 7);

        for (size_t i = 0; i < roll_history_size; i += 1) {
            g_display_device.print(roll_history[i]);
            g_display_device.print(" ");
        }

        // display battery. should be moved elsewhere
        uint16_t battery_reading = analogRead(A0);
        g_display_device.setCursor(g_display_device.width() - 10, g_display_device.height() - 12);
        g_display_device.print("BTR:");
        g_display_device.print(battery_reading);
    }

}

CheckRollsResult check_rolls() {
    auto const held_for = g_dice_buttons.since_last_update();

    g_mode_button.update();
    if (g_mode_button.is_just_pressed(1)) {
        clearScreen();
        clearHistory();
    } else if (g_mode_button.is_pressed(1) && g_mode_button.just_held_for(1s)) {
        g_display_device.fillScreen(BLACK);
        full_cleared = true;
    }

    g_display_device.setTextSize(2);
    g_dice_buttons.update();
    for (auto const& dice : DICE_DESCRIPTION) {
        if (g_dice_buttons.is_just_pressed(dice.button)) {
            clearScreen();
            g_display_device.setCursor(g_display_device.width() - 1 - 12 * 2, g_display_device.height() / 2 - 11);
            g_display_device.print(dice.name.data());
            return CheckRollsResult::Processing;
        } else if (dice.size == 10 && g_dice_buttons.is_pressed(dice.button) && g_dice_buttons.just_held_for(2s)) {
            clearScreen();
            g_display_device.setCursor(g_display_device.width() - 1 - 12 * 2, g_display_device.height() / 2 - 11);
            g_display_device.print(DICE_D100.name.data());
            return CheckRollsResult::Processing;
        } else if (g_dice_buttons.is_just_released(dice.button)) {
            uint32_t value;
            if (dice.size == 10 && held_for >= 2s) {
                value = DICE_D100.roll();
            } else {
                value = dice.roll();
            }
            updHistory(&dice, value);
            std::array<char, 32> value_buf;
            snprintf(value_buf.data(), value_buf.size(), "%" PRIu32, value);

            g_display_device.print(":");
            if (value < 10) {
                g_display_device.print(" ");
            }
            g_display_device.print(value_buf.data());
        }
    }

    flushHistory();

    if (g_dice_buttons.is_released() && g_mode_button.is_released()) {
        return CheckRollsResult::Idle;
    } else {
        return CheckRollsResult::Processing;
    }
}
