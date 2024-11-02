#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <array>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <limits>

struct ButtonEncoderBase {
    using index_t = uint8_t;
    using mask_t = uint8_t;
    using clock_t = std::chrono::steady_clock;
    using duration_t = std::chrono::milliseconds;

    clock_t::time_point m_prev_update = clock_t::time_point::min();
    clock_t::time_point m_latest_update = clock_t::time_point::min();
    clock_t::time_point m_latest_change = clock_t::time_point::min();
    clock_t::time_point m_latest_candidate_change = clock_t::time_point::min();

    bool just_held_for(duration_t duration) const {
        auto const hold_point = m_latest_change + duration;
        return hold_point > m_prev_update && hold_point <= m_latest_update;
    }
    
    duration_t since_last_update() const {
        return std::chrono::duration_cast<duration_t>(clock_t::now() - m_latest_update);
    }

protected:

    void update() {
        m_prev_update = m_latest_update;
        m_latest_update = clock_t::now();
    }

    mask_t get_bitmask(uint8_t const pins[], index_t size) {
        mask_t bitmask = 0;

        for (index_t i = 0; i < size; i += 1) {
            bool const pressed = digitalRead(pins[i]) == LOW;
            bitmask |= static_cast<mask_t>(pressed) << i;
        }

        return bitmask;
    }
};

template< uint8_t NPin >
struct ButtonEncoder : ButtonEncoderBase {
    std::array<uint8_t, NPin> const m_pins;

    mask_t m_pressed_mask = 0;
    std::optional<index_t> m_just_presed;
    std::optional<index_t> m_just_released;

    mask_t m_pressed_candidate = 0;

    ButtonEncoder(std::array<uint8_t, NPin> pins)
        : m_pins{pins}
    {
        static_assert(NPin < 8);

        for (uint8_t const pin : m_pins) {
            pinMode(pin, INPUT_PULLUP);
            //gpio_pin_wakeup_enable(pin, GPIO_PIN_INTR_LOLEVEL);
        }
    }

    constexpr uint8_t num_buttons() {
        return (1 << NPin) - 1;
    }

    void update() {
        ButtonEncoderBase::update();

        m_just_presed = std::nullopt;
        m_just_released = std::nullopt;

        mask_t const pressed_candidate = get_bitmask(m_pins.data(), NPin);
        if (pressed_candidate != m_pressed_candidate) {
            m_pressed_candidate = pressed_candidate;
            m_latest_candidate_change = m_latest_update;
        }

        if (m_pressed_candidate != m_pressed_mask &&
            m_latest_update - m_latest_candidate_change > std::chrono::milliseconds{20})
        {
            mask_t const prev_pressed = m_pressed_mask;
            m_pressed_mask = m_pressed_candidate;
            m_latest_change = m_latest_update;

            if (m_pressed_mask != 0) {
                m_just_presed = m_pressed_mask;
            }
            if (prev_pressed != 0) {
                m_just_released = prev_pressed;
            }
        }
    }

    bool is_pressed(mask_t btn) const {
        return m_pressed_mask == btn;
    }

    bool is_released() const {
        return m_pressed_mask == 0 && m_pressed_candidate == 0;
    }
    
    bool is_just_pressed(mask_t btn) const {
        return m_just_presed == btn;
    }

    bool is_just_released(mask_t btn) const {
        return m_just_released == btn;
    }
};
