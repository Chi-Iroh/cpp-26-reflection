#pragma once

#include <string_view>

enum class Error {
    InvalidValue,
    InvalidNumber,
    UnimplementedConversion
};

std::string_view errorName(Error err);
