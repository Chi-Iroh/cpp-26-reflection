#pragma once

#include <exception>
#include <format>
#include <meta>
#include <string>
#include <tuple>
#include <utility>

#include "argparse/include/optional_traits.hpp"

#define _CONSTRAINT_DELETED_METHOD_WARNING "Do not use class _Constraint directly, it's the base class. Use one which inherits from Constraint<T> instead !"
#define CONSTRAINT_DELETED_METHOD_WARNING "***** Maybe you forgot to implement check() method ? *****  "      \
    "Do not use class Constraint directly, it only exists to show the correct prototype for check() method, " \
    "as virtual functions are disallowed here because of std::meta's extract<> requiring structural types !"

namespace clap {
    template<typename... Ts>
    struct ConstraintFormat {
        std::format_string<Ts...> fmt;
        std::tuple<Ts...> args;

        constexpr ConstraintFormat(std::string_view fmt, const Ts&... args) :
            fmt{ fmt },
            args{ args... }
        {}
    };

    struct _Constraint {
        bool check(auto) const = delete(_CONSTRAINT_DELETED_METHOD_WARNING);

        constexpr auto /* will be ConstraintFormat<...> */ to_str() const = delete(_CONSTRAINT_DELETED_METHOD_WARNING);
    };

    template<typename Checked>
    struct Constraint : public _Constraint {
        bool check(const Checked& value) const = delete(CONSTRAINT_DELETED_METHOD_WARNING);

        constexpr auto to_str() const = delete(CONSTRAINT_DELETED_METHOD_WARNING);
    };

    class ConstraintViolation : public std::exception {
        const std::string msg;

    public:
        explicit ConstraintViolation(const std::string& msg);

        virtual const char* what() const noexcept;
    };
}
