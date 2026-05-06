#include <format>

#include "include/builders/yaml.hpp"

void YamlBuilder::addField(std::string_view name, std::string_view value) {
    if (this->locked) {
        return;
    }
    this->buffer += std::format("{}{}: \"{}\"\n", this->indentation, name, value);
}

void YamlBuilder::addSubElement(std::string_view name) {
    if (this->locked) {
        return;
    }
    this->buffer += std::format("{}{}:\n", this->indentation, name);
    this->indentation += '\t';
}

void YamlBuilder::endSubElement() {
    if (this->locked) {
        return;
    }
    this->indentation.pop_back();
    this->buffer += this->indentation + '\n';
}

std::string YamlBuilder::end() {
    if (this->locked) {
        return this->buffer;
    }

    while (!this->indentation.empty()) {
        this->endSubElement();
    }
    return this->buffer;
}
