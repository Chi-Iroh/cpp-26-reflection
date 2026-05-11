#pragma once

#include <array>
#include <concepts>

#include "argparse/include/clap/clap.hpp"

namespace clap {
    template<std::equality_comparable T, T... Allowed>
    struct AllowedValues : public Constraint<T> {
        static constexpr std::array<T, sizeof...(Allowed)> allowedValues{{ Allowed... }};

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
