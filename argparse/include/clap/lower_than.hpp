#pragma once

#include <concepts>
#include <optional>

#include "./clap.hpp"
#include "./traits.hpp"

namespace clap {
    template<HasLowerThan T>
    struct LowerThan : public Constraint<T> {
        const T exclusiveUpperBound;

        explicit constexpr LowerThan(const T& exclusiveUpperBound) :
            exclusiveUpperBound{ exclusiveUpperBound }
        {}

        bool check(const T& value) const {
            return value < this->exclusiveUpperBound;
        }
    };

    template<HasLowerThanOrEqualTo T>
    struct LowerThanOrEqualTo : public Constraint<T> {
        const T inclusiveUpperBound;

        explicit constexpr LowerThanOrEqualTo(const T& inclusiveUpperBound) :
            inclusiveUpperBound{ inclusiveUpperBound }
        {}

        bool check(const T& value) const {
            return value <= this->inclusiveUpperBound;
        }
    };
}
