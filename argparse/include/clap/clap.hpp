#pragma once

#include <exception>
#include <meta>
#include <string>

#include "argparse/include/optional_traits.hpp"

namespace clap {
    struct _Constraint {
        bool check(auto) const = delete(
            "Do not use class _Constraint directly, it's the base class. Use one which inherits from Constraint<T> instead !"
        );
    };

    template<typename Checked>
    struct Constraint : public _Constraint {
        bool check(const Checked& value) const = delete(
            "Do not use class Constraint directly, it only exists to show the correct prototype for check() method, "
            "as virtual functions are disallowed here because of std::meta's extract<> requiring structural types !"
        );
    };

    class ConstraintViolation : public std::exception {
        const std::string msg;

    public:
        explicit ConstraintViolation(const std::string& msg);

        virtual const char* what() const noexcept;
    };
}
