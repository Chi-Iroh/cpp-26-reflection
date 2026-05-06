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

    std::println("JSON type:\n{}", jsonDescribe<C>());
    std::println("JSON value:\n{}", jsonDescribe<C>(c));
    std::println("{:-<40}", "");
    std::println("YAML type:\n{}", yamlDescribe<C>());
    std::println("YAML value:\n{}", yamlDescribe<C>(c));
    std::println("{:-<40}", "");
    std::println("XML type:\n{}", xmlDescribe<C>());
    std::println("XML value:\n{}", xmlDescribe<C>(c));
}
