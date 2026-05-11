#pragma once

#include <array>
#include <concepts>
#include <type_traits>

#include "argparse/include/clap/clap.hpp"
#include "argparse/include/clap/traits.hpp"

namespace clap {
    template<bool CheckIfPresent, auto... Values>
    requires (sizeof...(Values) > 0) // at least 1 value
    struct _In : public Constraint<CommonValueType<Values...>> {
        using T = std::common_type_t<decltype(Values)...>;
        static constexpr std::array<T, sizeof...(Values)> values{ Values... };

        template<typename U>
        requires std::equality_comparable_with<T, U>
        bool check(const U& arg) const {
            bool found{ false };

            for (const T& value : this->values) {
                if (value == arg) {
                    found = true;
                    break;
                }
            }

            if constexpr (!CheckIfPresent) {
                return !found;
            }
            return found;
        }

        constexpr auto to_str() const {
            return ConstraintFormat{ "{}in {}", CheckIfPresent ? "" : "not ", this->values };
        }
    };

    template<auto... Values>
    requires (sizeof...(Values) > 0) // at least 1 value
    struct AllowedValues : public _In<true, Values...> {};

    template<auto... Values>
    requires (sizeof...(Values) > 0) // at least 1 value
    struct ForbiddenValues : public _In<false, Values...> {};
}
