#pragma once

#include <deque>
#include <iostream>
#include <vector>

template <typename Container>
std::ostream& printContainer(std::ostream& output, const Container& items) {
    bool first = true;
    for (const auto& i : items) {
        if (!first) {
            output << " ";
        }
        first = false;
        output << i;
    }
    return output;
}

template <typename T>
std::ostream& operator<<(std::ostream& output, const std::vector<T>& items) {
    return printContainer(output, items);
}

template <typename T>
std::ostream& operator<<(std::ostream& output, const std::deque<T>& items) {
    return printContainer(output, items);
}

inline bool isASCII(unsigned char c) { return (c & 0x80) == 0; }

inline bool isApostrophe(unsigned char c) { return c == '\''; }
