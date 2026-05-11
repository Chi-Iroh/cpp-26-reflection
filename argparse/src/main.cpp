#include <cstdio>
#include <exception>
#include <optional>
#include <print>
#include <string>

#include "dumptype/include/describe.hpp"
#include "dumptype/include/builders/json.hpp"

#include "argparse/include/args_parser.hpp"
#include "argparse/include/clap.hpp"

// https://www.scs.stanford.edu/~dm/blog/va-opt.html, §The FOR_EACH macro
#define EXPAND(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) __VA_ARGS__

#define COMMA ,
#define PARENTHESES ()
#define FOR_EACH(f, ...) __VA_OPT__(EXPAND(_FOR_EACH(f, __VA_ARGS__)))
#define _FOR_EACH(f, a, ...) f(a) __VA_OPT__(COMMA FOR_EACH_AGAIN PARENTHESES (f, __VA_ARGS__))
#define FOR_EACH_AGAIN() _FOR_EACH
#define STATIC_STRINGS(...) FOR_EACH(std::define_static_string, __VA_ARGS__)

struct Args {
    [[=clap::AllowedValues<STATIC_STRINGS("hello", "world", "goodbye")>()]]
    std::string name;

    [[=clap::Between<unsigned int>(1900, 2100)]]
    [[=clap::ForbiddenValues<2000>()]]
    std::optional<unsigned int> year;

    [[=clap::AllowedValues<3.14159f, 2.71828f>()]]
    float constant;
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
