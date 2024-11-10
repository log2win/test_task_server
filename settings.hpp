#pragma once

#include <cstdint>

constexpr uint16_t SERVER_PORT = 8000;

constexpr int32_t NUMBER_OF_THREADS = 4;

constexpr int64_t BUFFER_SIZE = 65536;

// word is sequence of characters no longer than MAX_WORD_LENGTH characters
constexpr int32_t MAX_WORD_LENGTH = 1024;

// possible 's, 't, 'll, and probably others
constexpr int32_t NUM_CHARS_AFTER_APOSTROPHE = 2;
