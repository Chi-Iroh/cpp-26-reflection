#include <format>

#include "include/builders/xml.hpp"

void XmlBuilder::addNewline() {
    if (this->firstElem.top()) {
        this->firstElem.top() = false;
    } else {
        this->buffer += "\n";
    }
}

void XmlBuilder::addField(std::string_view name, std::string_view value) {
    if (this->locked) {
        return;
    }
    this->addNewline();
    this->buffer += std::format("{0}<{1}>{2}</{1}>", this->indentation, name, value);
}

void XmlBuilder::addSubElement(std::string_view name) {
    if (this->locked) {
        return;
    }
    this->addNewline();
    this->buffer += std::format("{}<{}>\n", this->indentation, name);
    this->indentation += '\t';
    this->firstElem.push(true);
    this->groups.push(name);
}

void XmlBuilder::endSubElement() {
    if (this->locked) {
        return;
    }
    this->addNewline();
    this->indentation.pop_back();
    this->buffer += std::format("{}</{}>", this->indentation, this->groups.top());
    this->groups.pop();
}

std::string XmlBuilder::end() {
    if (this->locked) {
        return this->buffer;
    }

    while (!this->indentation.empty()) {
        this->endSubElement();
    }
    return this->buffer;
}
