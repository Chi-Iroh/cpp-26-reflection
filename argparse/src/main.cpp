#include <cstdio>
#include <map>
#include <meta>
#include <optional>
#include <print>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "dumptype/include/describe.hpp"
#include "dumptype/include/builders/json.hpp"

#include "argparse/include/convert.hpp"

constexpr std::meta::access_context ctx{ std::meta::access_context::current() };

template<typename Arg, typename T>
concept Callable = requires (T func, Arg arg) {
    func(arg);
};

template<typename T, typename F>
requires Callable<T, F>
void operator>>(const std::optional<T>& opt, F f) {
    if (opt.has_value()) {
        f(opt.value());
    }
}

template<typename T>
constexpr bool is_optional{ false };

template<typename T>
constexpr bool is_optional<std::optional<T>>{ true };

class ArgsParser {
private:
    using Value = std::optional<std::string_view>;
    std::map<std::string_view, Value> args{};

public:
    ArgsParser(int argc, char* argv[]) {
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

    template<typename T>
    void parseArgs(T& args) {
        template for (constexpr std::meta::info member : define_static_array(nonstatic_data_members_of(^^T, ctx))) {
            constexpr std::string_view argName{ identifier_of(member) };
            using MemberType = typename [:type_of(member):];
            if (this->args.contains(argName)) {
                this->args.at(argName) >> [&args, argName] (const std::string_view& val) {
                    const std::expected arg{ Convert<MemberType>::convert(val) };
                    if (arg.has_value()) {
                        args.[:member:] = arg.value();
                    } else {
                        throw std::invalid_argument(
                            std::format(
                                "Encountered error {} while trying to convert argument '{}' (value '{}') to requested type {} in args type {} !",
                                errorName(arg.error()),
                                argName,
                                val,
                                display_string_of(type_of(member)),
                                display_string_of(^^T)
                            )
                        );
                    }
                };
            } else if (!is_optional<MemberType>) {
                throw std::invalid_argument(std::format("Missing required argument '{}' !", argName));
            }
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

struct Args {
    std::string name;
    std::optional<unsigned int> year;
};

int main(int argc, char* argv[]) {
    try {
        const Args args{ ArgsParser{ argc, argv }.parseArgs<Args>() };
        std::println("{}", jsonDescribe<Args>(args));
    } catch (std::exception& err) {
        std::println(stderr, "ERROR: {}", err.what());
        return 1;
    }
}
