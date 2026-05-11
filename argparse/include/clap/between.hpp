#pragma once

#include <concepts>
#include <optional>

#include "./clap.hpp"
#include "./lower_than.hpp"
#include "./bigger_than.hpp"

namespace clap {
    template<typename T>
    struct Between : public Constraint<T> {
        const LowerThanOrEqualTo<T> lowerThan;
        const BiggerThanOrEqualTo<T> biggerThan;

        explicit constexpr Between(const T& from, const T& to) :
            lowerThan{ to > from ? to : from },
            biggerThan{ from < to ? from : to }
        {}

        bool check(const T& value) const {
            return this->lowerThan.check(value) && this->biggerThan.check(value);
        }
    };
}
