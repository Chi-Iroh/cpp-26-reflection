#pragma once

#include <format>
#include <map>
#include <meta>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "argparse/include/convert.hpp"
#include "argparse/include/clap.hpp"
#include "argparse/include/optional_traits.hpp"

constexpr std::meta::access_context ctx{ std::meta::access_context::current() };

template<typename Arg, typename T>
concept Callable = requires (T func, Arg arg) {
    func(arg);
};

class ArgsParser {
private:
    using Value = std::optional<std::string_view>;
    std::map<std::string_view, Value> args{};

public:
    ArgsParser(int argc, char* argv[]);

    template<typename T>
    void parseArgs(T& args) {
        template for (constexpr std::meta::info member : define_static_array(nonstatic_data_members_of(^^T, ctx))) {
            constexpr std::string_view argName{ identifier_of(member) };
            using MemberType = typename [:type_of(member):];

            if (!this->args.contains(argName)) {
                if (!is_optional<MemberType>) {
                    throw std::invalid_argument{ std::format("Missing required argument '{}' !", argName) };
                }
                continue;
            }

            const std::string_view val{ this->args.at(argName).value() };
            const std::expected expectedArg{ Convert<MemberType>::convert(val) };

            if (!expectedArg.has_value()) {
                throw std::invalid_argument{
                    std::format(
                        "Encountered error {} while trying to convert argument '{}' (value '{}') to requested type {} in args type {} !",
                        errorName(expectedArg.error()),
                        argName,
                        val,
                        display_string_of(type_of(member)),
                        display_string_of(^^T)
                    )
                };
            }

            const MemberType arg{ expectedArg.value() };

            template for (constexpr auto annotation : define_static_array(annotations_of(member))) {
                using AnnotationType = typename [:type_of(annotation):];

                if constexpr (!std::is_base_of_v<clap::_Constraint, AnnotationType>) {
                    continue; // ignoring irrelevant annotations
                }

                const auto fail = [] <typename Arg> (Arg arg) {
                    throw clap::ConstraintViolation{
                        std::format(
                            "Value '{}' violates constraint of type '{}' !",
                            arg,
                            identifier_of(^^AnnotationType)
                        )
                    };
                };

                constexpr AnnotationType an{ extract<AnnotationType>(annotation) };
                if constexpr (is_optional<MemberType>) {
                    if (!arg.has_value()) {
                        continue;
                    } else if (!an.check(arg.value())) {
                        fail(arg.value());
                    }
                } else if constexpr (!an.check(arg)) {
                    fail(arg);
                }
            }
            args.[:member:] = arg;
        }
    }

    template<typename T>
    requires std::is_default_constructible_v<T>
    T parseArgs() {
        T args{};
        parseArgs<T>(args);
        return args;
    }
};
