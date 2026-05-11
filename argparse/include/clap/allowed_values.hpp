#pragma once

#include <array>
#include <concepts>

#include "argparse/include/clap/clap.hpp"
#include "argparse/include/clap/traits.hpp"

namespace clap {
    template<auto... Allowed>
    requires (sizeof...(Allowed) > 0 && (std::equality_comparable<decltype(Allowed)> && ...)) // at least 1 value, and they can be compared against each other
    struct AllowedValues : public Constraint<CommonValueType<Allowed...>> {
        using Elem = std::common_type_t<decltype(Allowed)...>;
        static constexpr std::array<Elem, sizeof...(Allowed)> allowedValues{{ Allowed... }};

        bool check(const Elem& value) const {
            for (const Elem& allowed : this->allowedValues) {
                if (value == allowed) {
                    return true;
                }
            }
            return false;
        }
    };
}
