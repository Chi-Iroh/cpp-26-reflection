#include <array>
#include <format>
#include <meta>
#include <string>
#include <string_view>
#include <print>
#include <stack>

class DescriptorBuilder {
public:
    virtual void addField(std::string_view name, std::string_view value) = 0;
    virtual void addSubElement(std::string_view name) = 0;
    virtual void endSubElement() = 0;
    virtual std::string end() = 0;
};

class JsonBuilder : public DescriptorBuilder {
private:
    std::string buffer{ "{" };
    bool locked{ false };
    std::string indentation{ "\t" };
    std::stack<bool> firstElem{ { true } };

    void addComma() {
        if (this->firstElem.top()) {
            this->firstElem.top() = false;
        } else {
            this->buffer += ",";
        }
        this->buffer += "\n";
    }

public:
    virtual void addField(std::string_view name, std::string_view value) {
        if (this->locked) {
            return;
        }
        this->addComma();
        this->buffer += std::format("{}{}: \"{}\"", this->indentation, name, value);
    }

    virtual void addSubElement(std::string_view name) {
        if (this->locked) {
            return;
        }
        this->addComma();
        this->firstElem.push(true);
        this->buffer += std::format("{}{}: {{", this->indentation, name);
        this->indentation += '\t';
    }

    virtual void endSubElement() {
        if (this->locked) {
            return;
        }
        this->indentation.pop_back();
        this->firstElem.pop();
        this->buffer += "\n" + this->indentation + "}";
    }

    virtual std::string end() {
        if (this->locked) {
            return this->buffer;
        }

        while (!this->indentation.empty()) {
            this->endSubElement();
        }
        return this->buffer;
    }
};

class YamlBuilder : public DescriptorBuilder {
private:
    std::string buffer{};
    bool locked{ false };
    std::string indentation{};

public:
    virtual void addField(std::string_view name, std::string_view value) {
        if (this->locked) {
            return;
        }
        this->buffer += std::format("{}{}: \"{}\"\n", this->indentation, name, value);
    }

    virtual void addSubElement(std::string_view name) {
        if (this->locked) {
            return;
        }
        this->buffer += std::format("{}{}:\n", this->indentation, name);
        this->indentation += '\t';
    }

    virtual void endSubElement() {
        if (this->locked) {
            return;
        }
        this->indentation.pop_back();
        this->buffer += this->indentation + '\n';
    }

    virtual std::string end() {
        if (this->locked) {
            return this->buffer;
        }

        while (!this->indentation.empty()) {
            this->endSubElement();
        }
        return this->buffer;
    }
};

template<typename T>
constexpr std::string_view prettyPrintType = display_string_of(^^T);

template<typename T, std::derived_from<DescriptorBuilder> Builder>
std::string _describe(const T* const instance) {
    Builder builder{};
    builder.addField("type", prettyPrintType<T>);
    builder.addSubElement(instance == nullptr ? "layout" : "data");

    template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^T, std::meta::access_context::current()))) {
        builder.addField(identifier_of(member), instance == nullptr ? display_string_of(type_of(member)) : std::format("{}", instance->[:member:]));
    }
    return builder.end();
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
std::string describe(const T& instance) {
    return _describe<T, Builder>(&instance);
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
std::string describe() {
    return _describe<T, Builder>(nullptr);
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
constexpr auto makeDescribeFunction = [] <typename... Ts> (const Ts&... ts) {
    return describe<T, Builder>(ts...);
};

template<typename T>
constexpr auto jsonDescribe{ makeDescribeFunction<T, JsonBuilder> };

template<typename T>
constexpr auto yamlDescribe{ makeDescribeFunction<T, YamlBuilder> };

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
