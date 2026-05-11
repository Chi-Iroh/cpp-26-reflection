#pragma once

#include <array>
#include <concepts>

#include "argparse/include/clap/clap.hpp"

namespace clap {
    template<std::equality_comparable T, std::size_t N>
    struct AllowedValues : public Constraint<T> {
        const std::array<T, N> allowedValues;

        // template<typename... Ts>
        // requires (std::same_as<Ts, T> && ...)
        explicit constexpr AllowedValues(const std::array<T, N>& allowedValues) :
            allowedValues{ allowedValues }
        {}

        bool check(const T& value) const {
            for (const T& allowed : this->allowedValues) {
                if (value == allowed) {
                    return true;
                }
            }
            return false;
        }
    };
}
