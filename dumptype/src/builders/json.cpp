#include <format>
#include <stack>

#include "include/builders/json.hpp"

void JsonBuilder::addNewlineAndComma() {
    if (this->firstElem.top()) {
        this->firstElem.top() = false;
    } else {
        this->buffer += ",";
    }
    this->buffer += "\n";
}

void JsonBuilder::addField(std::string_view name, std::string_view value) {
    if (this->locked) {
        return;
    }
    this->addNewlineAndComma();
    this->buffer += std::format("{}{}: \"{}\"", this->indentation, name, value);
}

void JsonBuilder::addSubElement(std::string_view name) {
    if (this->locked) {
        return;
    }
    this->addNewlineAndComma();
    this->firstElem.push(true);
    this->buffer += std::format("{}{}: {{", this->indentation, name);
    this->indentation += '\t';
}

void JsonBuilder::endSubElement() {
    if (this->locked) {
        return;
    }
    this->indentation.pop_back();
    this->firstElem.pop();
    this->buffer += "\n" + this->indentation + "}";
}

std::string JsonBuilder::end() {
    if (this->locked) {
        return this->buffer;
    }

    while (!this->indentation.empty()) {
        this->endSubElement();
    }
    this->locked = true;
    return this->buffer;
}
