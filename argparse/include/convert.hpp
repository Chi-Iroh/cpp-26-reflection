#pragma once

#include <charconv>
#include <concepts>
#include <expected>
#include <optional>
#include <string_view>
#include <type_traits>

#include "./error.hpp"

template<typename To>
struct Convert {
    static std::expected<To, Error> convert(std::string_view) {
        return std::unexpected(Error::UnimplementedConversion);
    }
};

template<typename To>
requires (std::is_same_v<To, std::string_view> || std::is_same_v<To, std::string>)
struct Convert<To> {
    static std::expected<To, Error> convert(std::string_view str) {
        return To{ str };
    }
};

template<>
struct Convert<bool> {
    static std::expected<bool, Error> convert(std::string_view str) {
        if (str == "true" || str == "1" || str == "on") {
            return true;
        } else if (str == "false" || str == "0" || str == "off") {
            return false;
        }
        return std::unexpected(Error::InvalidValue);
    }
};

template<typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template<Arithmetic To>
struct Convert<To> {
    static std::expected<To, Error> convert(std::string_view str) {
        To result{};
        const std::from_chars_result status{ std::from_chars(str.data(), str.end(), result) };
        if (status && status.ptr == str.end()) {
            return result;
        }
        return std::unexpected(Error::InvalidNumber);
    }
};

template<typename T>
struct Convert<std::optional<T>> {
    static std::expected<std::optional<T>, Error> convert(std::string_view str) {
        return Convert<T>::convert(str);
    }
};
