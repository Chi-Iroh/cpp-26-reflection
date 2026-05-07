#pragma once

#include <stack>

#include "../builder.hpp"

class XmlBuilder : public DescriptorBuilder {
private:
    std::string buffer{ "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<root>\n" };
    bool locked{ false };
    std::string indentation{ "\t" };
    std::stack<bool> firstElem{ { true } };
    std::stack<std::string_view> groups{ { "root" } };

    void addNewline();

public:
    virtual void addField(std::string_view name, std::string_view value);
    virtual void addSubElement(std::string_view name);
    virtual void endSubElement();
    virtual std::string end();
};

#include "../describe.hpp"

template<typename T>
constexpr auto xmlDescribe{ makeDescribeFunction<T, XmlBuilder> };
