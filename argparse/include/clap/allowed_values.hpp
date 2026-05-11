#pragma once

#include <array>
#include <concepts>

#include "argparse/include/clap/clap.hpp"
#include "argparse/include/clap/traits.hpp"

namespace clap {
    template<auto... Allowed>
    requires (sizeof...(Allowed) > 0) // at least 1 value
    struct AllowedValues : public Constraint<CommonValueType<Allowed...>> {
        using T = std::common_type_t<decltype(Allowed)...>;
        static constexpr std::array<T, sizeof...(Allowed)> allowedValues{ Allowed... };

        template<typename U>
        requires std::equality_comparable_with<T, U>
        bool check(const U& value) const {
            for (const T& allowed : this->allowedValues) {
                if (value == allowed) {
                    return true;
                }
            }
            return false;
        }
    };
}
