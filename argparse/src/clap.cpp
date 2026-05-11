#include "argparse/include/clap/clap.hpp"

namespace clap {
    ConstraintViolation::ConstraintViolation(const std::string& msg) :
        msg{ msg }
    {}

    const char* ConstraintViolation::what() const noexcept {
        return this->msg.c_str();
    }
}
