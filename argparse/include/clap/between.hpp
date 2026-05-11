#pragma once

#include <concepts>
#include <format>
#include <optional>

#include "argparse/include/clap/clap.hpp"
#include "argparse/include/clap/traits.hpp"

namespace clap {
    template<typename T>
    requires (HasLowerThanOrEqualTo<T> && HasBiggerThanOrEqualTo<T>)
    struct Between : public Constraint<T> {
        const T min;
        const T max;

        explicit constexpr Between(const T& from, const T& to) :
            min{ from < to ? from : to },
            max{ to > from ? to : from }
        {}

        bool check(const T& value) const {
            return this->min <= value && value <= this->max;
        }

        constexpr auto to_str() const {
            return ConstraintFormat{ "from {} to {}", this->min, this->max };
        }
    };
}
