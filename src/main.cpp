#include <print>

#include "include/builders.hpp"
#include "include/describe.hpp"

struct C {
    int a;
    float b;
    std::string c;
};

int main() {
    C c{
        .a = 7,
        .b = 3.14159,
        .c = "helloworld !"
    };

    std::println("JSON:\n{}", jsonDescribe<C>());
    std::println("JSON:\n{}", jsonDescribe<C>(c));
    std::println("{:-<40}", "");
    std::println("YAML:\n{}", yamlDescribe<C>());
    std::println("YAML:\n{}", yamlDescribe<C>(c));
}
