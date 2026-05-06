#include <format>

#include "include/builders/yaml.hpp"

void YamlBuilder::addNewline() {
    if (this->firstElem.top()) {
        this->firstElem.top() = false;
    } else {
        this->buffer += "\n";
    }
}

void YamlBuilder::addField(std::string_view name, std::string_view value) {
    if (this->locked) {
        return;
    }
    this->addNewline();
    this->buffer += std::format("{}{}: \"{}\"", this->indentation, name, value);
}

void YamlBuilder::addSubElement(std::string_view name) {
    if (this->locked) {
        return;
    }
    this->addNewline();
    this->firstElem.push(true);
    this->buffer += std::format("{}{}:\n", this->indentation, name);
    this->indentation += '\t';
}

void YamlBuilder::endSubElement() {
    if (this->locked) {
        return;
    }
    this->firstElem.pop();
    this->indentation.pop_back();
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
