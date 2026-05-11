#pragma once

#include <optional>
#include <type_traits>

template<typename T>
constexpr bool _is_optional{ false };

template<typename T>
constexpr bool _is_optional<std::optional<T>>{ true };

template<typename T>
constexpr bool is_optional{ _is_optional<std::remove_cvref_t<T>> };
