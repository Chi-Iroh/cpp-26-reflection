#include <meta>

#include "argparse/include/error.hpp"

std::string_view errorName(Error err) {
    template for (constexpr std::meta::info enumerator : define_static_array(enumerators_of(^^Error))) {
        if (err == [:enumerator:]) {
            return identifier_of(enumerator);
        }
    }
    return "<unknown error>";
}
