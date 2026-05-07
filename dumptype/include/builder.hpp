#pragma once

#include <string>
#include <string_view>

class DescriptorBuilder {
public:
    virtual void addField(std::string_view name, std::string_view value) = 0;
    virtual void addSubElement(std::string_view name) = 0;
    virtual void endSubElement() = 0;
    virtual std::string end() = 0;
};
