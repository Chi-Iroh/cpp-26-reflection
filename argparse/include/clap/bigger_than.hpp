#pragma once

#include <optional>

#include "./clap.hpp"
#include "./traits.hpp"

namespace clap {
    template<HasBiggerThan T>
    struct BiggerThan : public Constraint<T> {
        const T exclusiveLowerBound;

        explicit constexpr BiggerThan(const T& exclusiveLowerBound) :
            exclusiveLowerBound{ exclusiveLowerBound }
        {}

        bool check(const T& value) const {
            return value > this->exclusiveLowerBound;
        }
    };

    template<HasBiggerOrEqualThan T>
    struct BiggerThanOrEqualTo : public Constraint<T> {
        const T inclusiveLowerBound;

        explicit constexpr BiggerThanOrEqualTo(const T& inclusiveLowerBound) :
            inclusiveLowerBound{ inclusiveLowerBound }
        {}

        bool check(const T& value) const {
            return value >= this->inclusiveLowerBound;
        }
    };
}
