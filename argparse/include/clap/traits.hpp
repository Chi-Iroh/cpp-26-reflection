#pragma once

#include <concepts>

namespace clap {
    template<typename T>
    concept HasBiggerThan = requires (const T& a, const T& b) {
        { a > b } -> std::same_as<bool>;
    };

    template<typename T>
    concept HasLowerThan = requires (const T& a, const T& b) {
        { a < b } -> std::same_as<bool>;
    };

    template<typename T>
    concept HasBiggerThanOrEqualTo = requires (const T& a, const T& b) {
        { a >= b } -> std::same_as<bool>;
    };

    template<typename T>
    concept HasLowerThanOrEqualTo = requires (const T& a, const T& b) {
        { a <= b } -> std::same_as<bool>;
    };

    template<auto... Vals>
    using CommonValueType = std::common_type_t<decltype(Vals)...>;
}
