#include "argparse/include/args_parser.hpp"

ArgsParser::ArgsParser(int argc, char* argv[]) {
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
