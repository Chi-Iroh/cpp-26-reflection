#include <cstdio>
#include <exception>
#include <optional>
#include <print>
#include <string>

#include "dumptype/include/describe.hpp"
#include "dumptype/include/builders/json.hpp"

#include "argparse/include/args_parser.hpp"
#include "argparse/include/clap.hpp"

struct Args {
    std::string name;

    [[=clap::Between<unsigned int>(1900, 2100)]]
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
