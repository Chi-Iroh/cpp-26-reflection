#include <array>
#include <format>
#include <meta>
#include <string>
#include <string_view>
#include <print>

class DescriptorBuilder {
public:
    virtual void addField(std::string_view name, std::string_view value) = 0;
    virtual void addSubElement(std::string_view name) = 0;
    virtual void endSubElement() = 0;
    virtual std::string end() = 0;
};

class JsonBuilder : public DescriptorBuilder {
private:
    std::string buffer{ "{\n" };
    bool locked{ false };
    std::string indentation{ "\t" };

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
        this->buffer += std::format("{}{}: {{\n", this->indentation, name);
        this->indentation += '\t';
    }

    virtual void endSubElement() {
        if (this->locked) {
            return;
        }
        this->indentation.pop_back();
        this->buffer += this->indentation + "}\n";
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

struct Identifier {
    std::string_view name;
    std::string_view type_name;
};

template<typename T>
constexpr std::size_t numberOfIdentifiers = nonstatic_data_members_of(^^T, std::meta::access_context::current()).size();

template<typename T>
constexpr std::string_view prettyPrintType = display_string_of(^^T);

template<typename T, std::derived_from<DescriptorBuilder> Builder>
class Descriptor {
protected:
    const std::string_view current_type_name{ prettyPrintType<T> };
    std::array<Identifier, numberOfIdentifiers<T>> identifiers{};

public:
    constexpr Descriptor() {
        for (std::size_t i{ 0 }; const auto member : nonstatic_data_members_of(^^T, std::meta::access_context::current())) {
            this->identifiers[i++] = {
                .name = identifier_of(member),
                .type_name = display_string_of(type_of(member))
            };
        }
    }

    virtual std::string str() {
        Builder builder{};
        builder.addField("type", this->current_type_name);
        builder.addSubElement("layout");


        for (const auto& [name, type_name] : this->identifiers) {
            builder.addField(name, type_name);
        }
        return builder.end();
    }

    virtual std::string str([[maybe_unused]] const T& instance) {
        return "";
    }
};

template<typename T>
using JsonDescriptor = Descriptor<T, JsonBuilder>;

template<typename T>
using YamlDescriptor = Descriptor<T, YamlBuilder>;

struct C {
    int a;
    float b;
    std::string c;
};

int main() {
    std::println("JSON:\n{}", JsonDescriptor<C>().str());
    std::println("{:-<40}", "");
    std::println("YAML:\n{}", YamlDescriptor<C>().str());
}
