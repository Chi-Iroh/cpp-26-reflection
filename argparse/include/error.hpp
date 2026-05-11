#pragma once

#include <string_view>

enum class Error {
    InvalidValue,
    InvalidNumber,
    Overflow,
    UnimplementedConversion
};

std::string_view errorName(Error err);
