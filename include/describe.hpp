#pragma once

#include <meta>
#include <print>
#include <string>
#include <string_view>

#include "include/builder.hpp"

constexpr std::meta::access_context ctx{ std::meta::access_context::current() };

// Only void* and (cv-qualified) char* are supported, not other pointer types like int*
template<typename T>
requires (!std::is_void_v<T> && !std::is_same_v<char, std::remove_cvref_t<T>>) // Avoids clashing with void* and char* default formatters
struct std::formatter<T*> : std::formatter<void*> {
    constexpr formatter() {};

    auto format(T* ptr, std::format_context& ctx) const {
        return std::formatter<void*>::format(static_cast<void*>(ptr), ctx);
    }
};

template <typename T>
consteval bool has_data_members() {
    constexpr std::meta::info info = ^^T;

    try {
        nonstatic_data_members_of(info, ctx); // throws for simple types like int
    } catch (std::meta::exception&) {
        return false;
    }
    return !define_static_array(nonstatic_data_members_of(info, ctx)).empty();
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
Builder _describe(const T* const instance, Builder& builder) {
    builder.addField("type", display_string_of(^^T));
    builder.addSubElement(instance == nullptr ? "layout" : "data");

    template for (constexpr std::meta::info member : define_static_array(nonstatic_data_members_of(^^T, ctx))) {
        constexpr std::string_view identifier{ identifier_of(member) };
        using MemberType = typename [:type_of(member):];
        const MemberType* const memberPtr{ instance == nullptr ? nullptr : &instance->[:member:] };

        if constexpr (has_data_members<typename [:type_of(member):]>()) {
            builder.addSubElement(identifier);
            _describe<MemberType, Builder>(memberPtr, builder);
        } else {
            builder.addField(identifier, instance == nullptr ? display_string_of(type_of(member)) : std::format("{}", *memberPtr));
        }
    }
    return builder;
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
std::string describe(const T& instance) {
    Builder builder{};
    return _describe<T, Builder>(&instance, builder).end();
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
std::string describe() {
    Builder builder{};
    return _describe<T, Builder>(nullptr, builder).end();
}

template<typename T, std::derived_from<DescriptorBuilder> Builder>
constexpr auto makeDescribeFunction = [] <typename... Ts> (const Ts&... ts) {
    return describe<T, Builder>(ts...);
};
