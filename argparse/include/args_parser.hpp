#pragma once

#include <cstdlib>
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

using StaticString = std::decay_t<decltype(std::define_static_string("a"))>;

constexpr std::array<StaticString, 3> default_help_flags{ "-h", "--help", "-?" };

template<typename Args>
class ArgsParser {
private:
    using Value = std::optional<std::string_view>;
    std::map<std::string_view, Value> args;
    const std::string_view program_name;

    static consteval std::pair<std::meta::info, std::size_t> get_help_annotation() {
        template for (constexpr std::meta::info annotation : define_static_array(annotations_of(^^Args))) {
            using AnnotationType = typename [:type_of(annotation):];
            if constexpr (std::is_base_of_v<clap::_Help, AnnotationType>) {
                return { annotation, extract<AnnotationType>(annotation).flags.size() };
            }
        }
        return { std::meta::info{} /* null reflection */, default_help_flags.size() };
    }

    static constexpr std::pair<std::meta::info, std::size_t> help_flags_info{ get_help_annotation() };

    static consteval std::array<StaticString, help_flags_info.second> _help_flags() {
        constexpr std::meta::info help_annotation{ help_flags_info.first };
        if constexpr (help_annotation != std::meta::info{}) {
            using AnnotationType = typename [:type_of(help_annotation):];
            return extract<AnnotationType>(help_annotation).flags;
        } else {
            return default_help_flags;
        }
    }

    static constexpr auto help_flags{ _help_flags() };

    bool has_help_flag() const {
        for (const auto& [flag, _] : this->args) {
            for (const StaticString& help_flag : this->help_flags) {
                if (flag == help_flag) {
                    return true;
                }
            }
        }
        return false;
    }

    void show_help() const {
        std::print("USAGE: {} {}", this->program_name, this->help_flags);

        template for (constexpr std::meta::info arg : define_static_array(nonstatic_data_members_of(^^Args, ctx))) {
            using ArgType = typename [:type_of(arg):];

            std::print(" ");
            if constexpr (is_optional<ArgType>) {
                std::print("[");
            }
            std::print("{}=", identifier_of(arg));

            if constexpr (define_static_array(annotations_of(arg)).size() > 0) {
                std::print("{{ ");
                template for (bool first{ true }; constexpr std::meta::info annotation : define_static_array(annotations_of(arg))) {
                    if (!first) {
                        std::print(", ");
                    }

                    using AnnotationType = typename [:type_of(annotation):];
                    constexpr AnnotationType format{ extract<AnnotationType>(annotation) };
                    std::apply([format] (const auto&... args) { std::print(format.to_str().fmt.get(), args...); }, format.to_str().args);

                    first = false;
                }
                std::print(" }}");
            }

            if constexpr (is_optional<ArgType>) {
                std::print("]");
            }
        }

        std::println();

        template for (constexpr std::meta::info annotation : define_static_array(annotations_of(^^Args))) {
            using AnnotationType = typename [:type_of(annotation):];
            constexpr AnnotationType _annotation{ extract<AnnotationType>(annotation) };
            if constexpr (std::is_base_of_v<clap::_Help, AnnotationType>) {
                std::println("Description:\n\t{}", _annotation.msg);
                break;
            }
        }
        std::exit(0);
    }

public:
    ArgsParser(int argc, char* argv[]) :
        program_name{ argv[0] }
    {
        for (int i = 1 /* skipping program name */; i < argc; i++) {
            const std::string_view arg{ argv[i] };
            const std::size_t equalSignPos{ arg.find('=') };
            if (equalSignPos == std::string_view::npos) {
                this->args.emplace(arg, std::nullopt);
            } else {
                this->args.emplace(arg.substr(0, equalSignPos), arg.substr(equalSignPos + 1));
            }
        }
    }

    void parseArgs(Args& args) {
        if (this->has_help_flag()) {
            this->show_help();
            return;
        }

        template for (constexpr std::meta::info member : define_static_array(nonstatic_data_members_of(^^Args, ctx))) {
            constexpr std::string_view argName{ identifier_of(member) };
            using MemberType = typename [:type_of(member):];

            if (!this->args.contains(argName)) {
                if (!is_optional<MemberType>) {
                    throw std::invalid_argument{ std::format("Missing required argument '{}' !", argName) };
                }
                continue;
            }

            if (!this->args.at(argName).has_value()) {
                throw std::invalid_argument{ std::format("Provided argument '{}' needs an accompanying value !", argName) };
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
                        display_string_of(^^Args)
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
                            display_string_of(remove_cvref(dealias(^^AnnotationType)))
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
                } else if (!an.check(arg)) {
                    fail(arg);
                }
            }
            args.[:member:] = arg;
        }
    }

    Args parseArgs() {
        static_assert(std::is_default_constructible_v<Args>);
        Args args{};
        this->parseArgs(args);
        return args;
    }
};
