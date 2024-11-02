#pragma once

#include "storage.h"

template< template <typename> class SContainer >
struct SettingsBaseV1 {
    SContainer<bool> d100_by_default;
    SContainer<uint16_t> default_color;
};

struct SettingsStorage {
    uint16_t const version = 1;
    SettingsBaseV1<IdContainer> settings;

    void eeprom_store() const {
        File file = LittleFS.open("settings.bin", "wb");
        file.write(reinterpret_cast<uint8_t const*>(this), sizeof(*this));
        file.close();
    }

    void eeprom_load() {
        File file = LittleFS.open("settings.bin", "rb");
        if (!file || file.size() < sizeof(*this)) {
            return;
        }

        file.read(reinterpret_cast<uint8_t*>(this), std::min(sizeof(*this), file.size()));
        file.close();
    }
};

struct Settings : SettingsBaseV1<DiffContainer> {
    Settings(SettingsBaseV1<IdContainer>& store)
        : SettingsBaseV1 {
            store.d100_by_default,
            store.default_color,
        }
    {
    }
};

extern SettingsStorage g_settings_storage;
extern Settings g_settings;
