#ifndef Rtcc_Hashing_h
#define Rtcc_Hashing_h

#include "Utils/SpUtils/DataTypes.h"
#include "Utils/SpUtils/ErrorHandling.h"
#include "Utils/SpUtils/SwapByteOrder.h"
#include "Utils/SpUtils/type_trait.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <optional>
#include <string>
#include <tuple>
#include <utility>

using namespace std;

namespace retccUtils {
    template <typename T, typename Enable> struct DenseMapInfo;

    class hash_code {
        size_t value;

    public:
        hash_code() = default;

        explicit hash_code(size_t value) : value(value) {}

        explicit operator size_t() const { return value; }

        friend bool operator==(const hash_code &lhs, const hash_code &rhs) {
            return lhs.value == rhs.value;
        }
        friend bool operator!=(const hash_code &lhs, const hash_code &rhs) {
            return lhs.value != rhs.value;
        }

        friend size_t hash_value(const hash_code &code) { return code.value; }
    };

    template <typename T>
    enable_if_t<is_integral_or_enum<T>::value, hash_code> hash_value(T value);

    template <typename T> hash_code hash_value(const T *ptr);

    template <typename T, typename U>
    hash_code hash_value(const pair<T, U> &arg);

    template <typename... Ts>
    hash_code hash_value(const tuple<Ts...> &arg);

    template <typename T>
    hash_code hash_value(const basic_string<T> &arg);

    template <typename T> hash_code hash_value(const optional<T> &arg);

    void set_fixed_execution_hash_seed(uint64_t fixed_value);

    namespace hashing {
        namespace detail {

            inline uint64_t fetch64(const char *p) {
                uint64_t result;
                memcpy(&result, p, sizeof(result));
                if (sys::IsBigEndianHost)
                    sys::swapByteOrder(result);
                return result;
            }

            inline uint32_t fetch32(const char *p) {
                uint32_t result;
                memcpy(&result, p, sizeof(result));
                if (sys::IsBigEndianHost)
                    sys::swapByteOrder(result);
                return result;
            }
            static constexpr uint64_t k0 = 0xc3a5c85c97cb3127ULL;
            static constexpr uint64_t k1 = 0xb492b66fbe98f273ULL;
            static constexpr uint64_t k2 = 0x9ae16a3b2f90404fULL;
            static constexpr uint64_t k3 = 0xc949d7c7509e6557ULL;

            inline uint64_t rotate(uint64_t val, size_t shift) {
                return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
            }

            inline uint64_t shift_mix(uint64_t val) {
                return val ^ (val >> 47);
            }

            inline uint64_t hash_16_bytes(uint64_t low, uint64_t high) {
                const uint64_t kMul = 0x9ddfea08eb382d69ULL;
                uint64_t a = (low ^ high) * kMul;
                a ^= (a >> 47);
                uint64_t b = (high ^ a) * kMul;
                b ^= (b >> 47);
                b *= kMul;
                return b;
            }

            inline uint64_t hash_1to3_bytes(const char *s, size_t len, uint64_t seed) {
                uint8_t a = s[0];
                uint8_t b = s[len >> 1];
                uint8_t c = s[len - 1];
                uint32_t y = static_cast<uint32_t>(a) + (static_cast<uint32_t>(b) << 8);
                uint32_t z = static_cast<uint32_t>(len) + (static_cast<uint32_t>(c) << 2);
                return shift_mix(y * k2 ^ z * k3 ^ seed) * k2;
            }

            inline uint64_t hash_4to8_bytes(const char *s, size_t len, uint64_t seed) {
                uint64_t a = fetch32(s);
                return hash_16_bytes(len + (a << 3), seed ^ fetch32(s + len - 4));
            }

            inline uint64_t hash_9to16_bytes(const char *s, size_t len, uint64_t seed) {
                uint64_t a = fetch64(s);
                uint64_t b = fetch64(s + len - 8);
                return hash_16_bytes(seed ^ a, rotate(b + len, len)) ^ b;
            }

            inline uint64_t hash_17to32_bytes(const char *s, size_t len, uint64_t seed) {
                uint64_t a = fetch64(s) * k1;
                uint64_t b = fetch64(s + 8);
                uint64_t c = fetch64(s + len - 8) * k2;
                uint64_t d = fetch64(s + len - 16) * k0;
                return hash_16_bytes(llvm::rotr<uint64_t>(a - b, 43) +
                                     llvm::rotr<uint64_t>(c ^ seed, 30) + d,
                                     a + llvm::rotr<uint64_t>(b ^ k3, 20) - c + len + seed);
            }

            inline uint64_t hash_33to64_bytes(const char *s, size_t len, uint64_t seed) {
                uint64_t z = fetch64(s + 24);
                uint64_t a = fetch64(s) + (len + fetch64(s + len - 16)) * k0;
                uint64_t b = llvm::rotr<uint64_t>(a + z, 52);
                uint64_t c = llvm::rotr<uint64_t>(a, 37);
                a += fetch64(s + 8);
                c += llvm::rotr<uint64_t>(a, 7);
                a += fetch64(s + 16);
                uint64_t vf = a + z;
                uint64_t vs = b + llvm::rotr<uint64_t>(a, 31) + c;
                a = fetch64(s + 16) + fetch64(s + len - 32);
                z = fetch64(s + len - 8);
                b = llvm::rotr<uint64_t>(a + z, 52);
                c = llvm::rotr<uint64_t>(a, 37);
                a += fetch64(s + len - 24);
                c += llvm::rotr<uint64_t>(a, 7);
                a += fetch64(s + len - 16);
                uint64_t wf = a + z;
                uint64_t ws = b + llvm::rotr<uint64_t>(a, 31) + c;
                uint64_t r = shift_mix((vf + ws) * k2 + (wf + vs) * k0);
                return shift_mix((seed ^ (r * k0)) + vs) * k2;
            }

            inline uint64_t hash_short(const char *s, size_t length, uint64_t seed) {
                if (length >= 4 && length <= 8)
                    return hash_4to8_bytes(s, length, seed);
                if (length > 8 && length <= 16)
                    return hash_9to16_bytes(s, length, seed);
                if (length > 16 && length <= 32)
                    return hash_17to32_bytes(s, length, seed);
                if (length > 32)
                    return hash_33to64_bytes(s, length, seed);
                if (length != 0)
                    return hash_1to3_bytes(s, length, seed);

                return k2 ^ seed;
            }

            struct hash_state {
                uint64_t h0 = 0, h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0, h6 = 0;

                static hash_state create(const char *s, uint64_t seed) {
                    hash_state state = {0,
                                        seed,
                                        hash_16_bytes(seed, k1),
                                        llvm::rotr<uint64_t>(seed ^ k1, 49),
                                        seed * k1,
                                        shift_mix(seed),
                                        0};
                    state.h6 = hash_16_bytes(state.h4, state.h5);
                    state.mix(s);
                    return state;
                }

                static void mix_32_bytes(const char *s, uint64_t &a, uint64_t &b) {
                    a += fetch64(s);
                    uint64_t c = fetch64(s + 24);
                    b = llvm::rotr<uint64_t>(b + a + c, 21);
                    uint64_t d = a;
                    a += fetch64(s + 8) + fetch64(s + 16);
                    b += llvm::rotr<uint64_t>(a, 44) + d;
                    a += c;
                }

                void mix(const char *s) {
                    h0 = llvm::rotr<uint64_t>(h0 + h1 + h3 + fetch64(s + 8), 37) * k1;
                    h1 = llvm::rotr<uint64_t>(h1 + h4 + fetch64(s + 48), 42) * k1;
                    h0 ^= h6;
                    h1 += h3 + fetch64(s + 40);
                    h2 = llvm::rotr<uint64_t>(h2 + h5, 33) * k1;
                    h3 = h4 * k1;
                    h4 = h0 + h5;
                    mix_32_bytes(s, h3, h4);
                    h5 = h2 + h6;
                    h6 = h1 + fetch64(s + 16);
                    mix_32_bytes(s + 32, h5, h6);
                    swap(h2, h0);
                }

                uint64_t finalize(size_t length) {
                    return hash_16_bytes(hash_16_bytes(h3, h5) + shift_mix(h1) * k1 + h2,
                                         hash_16_bytes(h4, h6) + shift_mix(length) * k1 + h0);
                }
            };


            extern uint64_t fixed_seed_override;

            inline uint64_t get_execution_seed() {
                const uint64_t seed_prime = 0xff51afd7ed558ccdULL;
                static uint64_t seed = fixed_seed_override ? fixed_seed_override : seed_prime;
                return seed;
            }

            template <typename T> struct is_hashable_data
                    : integral_constant<bool, ((is_integral_or_enum<T>::value ||
                                                is_pointer<T>::value) &&
                                               64 % sizeof(T) == 0)> {};

            template <typename T, typename U> struct is_hashable_data<pair<T, U> >
            : integral_constant<bool, (is_hashable_data<T>::value &&
                                       is_hashable_data<U>::value &&
                                       (sizeof(T) + sizeof(U)) ==
                                       sizeof(pair<T, U>))> {};

        template <typename T>
        enable_if_t<is_hashable_data<T>::value, T>
        get_hashable_data(const T &value) {
            return value;
        }
        template <typename T>
        enable_if_t<!is_hashable_data<T>::value, size_t>
        get_hashable_data(const T &value) {
            using ::llvm::hash_value;
            return hash_value(value);
        }

        template <typename T>
        bool store_and_advance(char *&buffer_ptr, char *buffer_end, const T& value,
                               size_t offset = 0) {
            size_t store_size = sizeof(value) - offset;
            if (buffer_ptr + store_size > buffer_end)
                return false;
            const char *value_data = reinterpret_cast<const char *>(&value);
            memcpy(buffer_ptr, value_data + offset, store_size);
            buffer_ptr += store_size;
            return true;
        }

        template <typename InputIteratorT>
        hash_code hash_combine_range_impl(InputIteratorT first, InputIteratorT last) {
            const uint64_t seed = get_execution_seed();
            char buffer[64], *buffer_ptr = buffer;
            char *const buffer_end = end(buffer);
            while (first != last && store_and_advance(buffer_ptr, buffer_end,
                                                      get_hashable_data(*first)))
                ++first;
            if (first == last)
                return hash_short(buffer, buffer_ptr - buffer, seed);
            assert(buffer_ptr == buffer_end);

            hash_state state = state.create(buffer, seed);
            size_t length = 64;
            while (first != last) {
                buffer_ptr = buffer;
                while (first != last && store_and_advance(buffer_ptr, buffer_end,
                                                          get_hashable_data(*first)))
                    ++first;

                rotate(buffer, buffer_ptr, buffer_end);

                state.mix(buffer);
                length += buffer_ptr - buffer;
            };

            return state.finalize(length);
        }

        template <typename ValueT>
        enable_if_t<is_hashable_data<ValueT>::value, hash_code>
        hash_combine_range_impl(ValueT *first, ValueT *last) {
            const uint64_t seed = get_execution_seed();
            const char *s_begin = reinterpret_cast<const char *>(first);
            const char *s_end = reinterpret_cast<const char *>(last);
            const size_t length = distance(s_begin, s_end);
            if (length <= 64)
                return hash_short(s_begin, length, seed);

            const char *s_aligned_end = s_begin + (length & ~63);
            hash_state state = state.create(s_begin, seed);
            s_begin += 64;
            while (s_begin != s_aligned_end) {
                state.mix(s_begin);
                s_begin += 64;
            }
            if (length & 63)
                state.mix(s_end - 64);

            return state.finalize(length);
        }

    } // namespace detail
} // namespace hashing


template <typename InputIteratorT>
hash_code hash_combine_range(InputIteratorT first, InputIteratorT last) {
    return ::llvm::hashing::detail::hash_combine_range_impl(first, last);
}


namespace hashing {
    namespace detail {

        struct hash_combine_recursive_helper {
            char buffer[64] = {};
            hash_state state;
            const uint64_t seed;

        public:
            hash_combine_recursive_helper()
                    : seed(get_execution_seed()) {}
            template <typename T>
            char *combine_data(size_t &length, char *buffer_ptr, char *buffer_end, T data) {
                if (!store_and_advance(buffer_ptr, buffer_end, data)) {
                    size_t partial_store_size = buffer_end - buffer_ptr;
                    memcpy(buffer_ptr, &data, partial_store_size);

                    if (length == 0) {
                        state = state.create(buffer, seed);
                        length = 64;
                    } else {
                        state.mix(buffer);
                        length += 64;
                    }
                    buffer_ptr = buffer;

                    if (!store_and_advance(buffer_ptr, buffer_end, data,
                                           partial_store_size))
                        llvm_unreachable("buffer smaller than stored type");
                }
                return buffer_ptr;
            }

            template <typename T, typename ...Ts>
            hash_code combine(size_t length, char *buffer_ptr, char *buffer_end,
                              const T &arg, const Ts &...args) {
                buffer_ptr = combine_data(length, buffer_ptr, buffer_end, get_hashable_data(arg));

                return combine(length, buffer_ptr, buffer_end, args...);
            }

            hash_code combine(size_t length, char *buffer_ptr, char *buffer_end) {
                if (length == 0)
                    return hash_short(buffer, buffer_ptr - buffer, seed);

                rotate(buffer, buffer_ptr, buffer_end);

                state.mix(buffer);
                length += buffer_ptr - buffer;

                return state.finalize(length);
            }
        };

    } // namespace detail
} // namespace hashing

template <typename ...Ts> hash_code hash_combine(const Ts &...args) {
    ::llvm::hashing::detail::hash_combine_recursive_helper helper;
    return helper.combine(0, helper.buffer, helper.buffer + 64, args...);
}

namespace hashing {
    namespace detail {
        inline hash_code hash_integer_value(uint64_t value) {
            const uint64_t seed = get_execution_seed();
            const char *s = reinterpret_cast<const char *>(&value);
            const uint64_t a = fetch32(s);
            return hash_16_bytes(seed + (a << 3), fetch32(s + 4));
        }

    } // namespace detail
} // namespace hashing

template <typename T>
enable_if_t<is_integral_or_enum<T>::value, hash_code> hash_value(T value) {
    return ::llvm::hashing::detail::hash_integer_value(
            static_cast<uint64_t>(value));
}

template <typename T> hash_code hash_value(const T *ptr) {
    return ::llvm::hashing::detail::hash_integer_value(
            reinterpret_cast<uintptr_t>(ptr));
}

template <typename T, typename U>
hash_code hash_value(const pair<T, U> &arg) {
    return hash_combine(arg.first, arg.second);
}

template <typename... Ts> hash_code hash_value(const tuple<Ts...> &arg) {
    return apply([](const auto &...xs) { return hash_combine(xs...); }, arg);
}

template <typename T>
hash_code hash_value(const basic_string<T> &arg) {
    return hash_combine_range(arg.begin(), arg.end());
}

template <typename T> hash_code hash_value(const optional<T> &arg) {
    return arg ? hash_combine(true, *arg) : hash_value(false);
}

template <> struct DenseMapInfo<hash_code, void> {
    static inline hash_code getEmptyKey() { return hash_code(-1); }
    static inline hash_code getTombstoneKey() { return hash_code(-2); }
    static unsigned getHashValue(hash_code val) { return val; }
    static bool isEqual(hash_code lhs, hash_code rhs) { return lhs == rhs; }
};

} // namespace llvm

namespace std {

    template<>
    struct hash<llvm::hash_code> {
        size_t operator()(llvm::hash_code const& Val) const {
            return Val;
        }
    };

} // namespace std;

#endif //Rtcc_Hashing_h