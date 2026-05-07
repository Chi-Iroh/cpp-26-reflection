#include <print>
#include <string>
#include <utility>

#include "include/builders.hpp"
#include "include/describe.hpp"

struct C {
    int a;
    float b;
    std::string c;
    struct D {
        int* ptr;
        std::pair<int, std::string> pair;
    } d;
    char e;
};

int main() {
    int n;
    C c{
        .a = 7,
        .b = 3.14159,
        .c = "helloworld !",
        .d = {
            .ptr = &n,
            .pair = { 485, "goodbye" }
        },
        .e = 'J'
    };

    std::println("JSON type:\n{}", jsonDescribe<C>());
    std::println("{:-<20}", "");
    std::println("JSON value:\n{}", jsonDescribe<C>(c));
    std::println("{:-<40}", "");
    std::println("YAML type:\n{}", yamlDescribe<C>());
    std::println("{:-<20}", "");
    std::println("YAML value:\n{}", yamlDescribe<C>(c));
    std::println("{:-<40}", "");
    std::println("XML type:\n{}", xmlDescribe<C>());
    std::println("{:-<20}", "");
    std::println("XML value:\n{}", xmlDescribe<C>(c));
}
