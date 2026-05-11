#pragma once

#include <concepts>
#include <optional>

#include "./clap.hpp"

namespace clap {
    template<typename T>
    concept HasLessThan = requires (T a, T b) {
        { a < b } -> std::same_as<bool>;
    };

    template<HasLessThan T>
    struct Lower : public Constraint<T> {
        const T exclusiveLowerBound;

        constexpr Lower(const T& exclusiveLowerBound) :
            exclusiveLowerBound{ exclusiveLowerBound }
        {}

        bool check(const T& value) const {
            return value < this->exclusiveLowerBound;
        }
    };
}
