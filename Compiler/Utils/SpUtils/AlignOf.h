#ifndef RTCC_ALIGNOF_H
#define RTCC_ALIGNOF_H

#include <type_traits>

namespace rtccUtils {
    // Aligned Char Array Union template
//    template<typename T, typename Ts>
//    template<typenameT, typename Ts...> //WTF IS TS...
    template<typename T, typename... Ts>
    struct AlignedCharArrayUnion {
        using AlignedUnion = std::aligned_union_t<1, T, Ts...>;
        alignas(alignof(AlignedUnion)) char buffer[sizeof(AlignedUnion)];
    };

} // namespace rtccUtils

#endif