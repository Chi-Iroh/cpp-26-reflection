#pragma once

#include <meta>
#include <string>
#include <string_view>

#include "include/builder.hpp"

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
