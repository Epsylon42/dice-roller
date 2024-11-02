#pragma once

#include <chrono>

void disable_modem();
void test_delay(std::chrono::milliseconds duration);
void light_sleep(std::chrono::microseconds duration = std::chrono::microseconds::max());
