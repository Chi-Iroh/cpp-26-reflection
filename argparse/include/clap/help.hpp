#include <array>

#include "argparse/include/clap/clap.hpp"
#include "argparse/include/clap/traits.hpp"

namespace clap {
    struct _Help {};

    template<auto Msg, auto... Flags>
    requires (sizeof...(Flags) > 0) // at least 1 value
    struct Help : public _Help {
        using T = std::common_type_t<decltype(Flags)...>;

        static constexpr decltype(Msg) msg{ Msg };
        static constexpr std::array<T, sizeof...(Flags)> flags{ Flags... };
    };
}
