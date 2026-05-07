#pragma once

#include <stack>

#include "../builder.hpp"

class YamlBuilder : public DescriptorBuilder {
private:
    std::string buffer{};
    bool locked{ false };
    std::string indentation{};
    std::stack<bool> firstElem{ { true } };

    void addNewline();

public:
    virtual void addField(std::string_view name, std::string_view value);
    virtual void addSubElement(std::string_view name);
    virtual void endSubElement();
    virtual std::string end();
};

#include "../describe.hpp"

template<typename T>
constexpr auto yamlDescribe{ makeDescribeFunction<T, YamlBuilder> };
