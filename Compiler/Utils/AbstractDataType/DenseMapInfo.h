#ifndef Rtcc_DenseMapInfo_h
#define Rtcc_DenseMapInfo_h

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

using namespace std;

//USING DENSE-MAP PLZZZZZZZZZZZZZZZZZZZ
//NO MORE HASHTABLE BECAUSE IT IS TOOOOO SLOW
//I DON'T WANT SO SLOW BECAUSE COMPILE TIME IS LIFE

namespace retccUtils {
    namespace detail {
        static inline uint32_t combineHashValue(uint32_t a, uint32_t b) {
            uint64_t key = (uint64_t) a << 32 | (uint64_t) b;
            key += ~(key << 32);
            key ^= key >> 22u;
            key += ~(key << 27);
            key ^= key >> 31u;
            key += ~(key << 13);
            key ^= key >> 8u;
            key += key << 3;
            key ^= key >> 15u;
            return static_cast<uint32_t>(key);
        }
    } //namespace end detail

    template<typename T, typename Enable = void>
    struct DenseMapInfo {
    };

    template<typename T>
    struct DenseMapInfo<T *> {
        static constexpr uintptr_t Log2MaxAlign = 12;

        [[maybe_unused]] static inline T *getEmptyKey() {
            auto Val = static_cast<uintptr_t>(-1);
            Val <<= Log2MaxAlign;
//            return static_cast<T*>(Val);
//            return const_cast<T*>(Val);
            return reinterpret_cast<T *>(Val);
        }

        static inline T *getTombstoneKey() {
            auto Val = static_cast<uintptr_t>(-2);
            Val <<= Log2MaxAlign;
            //            return static_cast<T*>(Val);
//            return const_cast<T*>(Val);
            return reinterpret_cast<T *>(Val);
        }

        [[maybe_unused]] static unsigned getHashValue(const T *PtrVal) {
            return (unsigned((uintptr_t) PtrVal) >> 4) ^
                   (unsigned((uintptr_t) PtrVal) >> 9);
        }

        [[maybe_unused]] static bool isEqual(const T *lhs, const T *rhs) { return lhs == rhs; }
    };

    template<>
    struct DenseMapInfo<char> {
        static inline char getEmptyKey() { return ~0; }

        static inline char getTombstoneKey() { return ~0 - 1; }

        [[maybe_unused]] static unsigned getHashValue(const char &Val) { return Val * 37U; }

        [[maybe_unused]] static bool isEqual(const char &lhs, const char &rhs) { return lhs == rhs; }
    };


    template<>
    struct DenseMapInfo<unsigned> {
        static inline unsigned getEmptyKey() { return ~0U; }

        static inline unsigned getTombstoneKey() { return ~0U - 1; }

        [[maybe_unused]] static unsigned getHashValue(const unsigned &Val) { return Val * 37U; }

        [[maybe_unused]] static bool isEqual(const unsigned &lhs, const unsigned &rhs) { return lhs == rhs; }
    };

    template<>
    struct DenseMapInfo<unsigned short> {
        static inline unsigned short getEmptyKey() { return 0xFFFF; }

        static inline unsigned short getTombstoneKey() { return 0xFFFF - 1; }

        [[maybe_unused]] static unsigned getHashValue(const unsigned short &Val) { return Val * 37U; }

        [[maybe_unused]] static bool isEqual(const unsigned short &lhs, const unsigned short &rhs) {
            return lhs == rhs;
        }
    };


    template<>
    struct DenseMapInfo<unsigned char> {
        static inline unsigned char getEmptyKey() { return ~0; }

        static inline unsigned char getTombstoneKey() { return ~0 - 1; }

        [[maybe_unused]] static unsigned getHashValue(const unsigned char &Val) { return Val * 37U; }

        [[maybe_unused]] static bool isEqual(const unsigned char &lhs, const unsigned char &rhs) { return lhs == rhs; }
    };


    template<>
    struct DenseMapInfo<unsigned long> {
        static inline unsigned long getEmptyKey() { return ~0UL; }

        static inline unsigned long getTombstoneKey() { return ~0UL - 1L; }

        [[maybe_unused]] static unsigned getHashValue(const unsigned long &Val) { return (unsigned) (Val * 37UL); }

        [[maybe_unused]] static bool isEqual(const unsigned long &lhs, const unsigned long &rhs) { return lhs == rhs; }
    };

    template<>
    struct DenseMapInfo<unsigned long long> {
        static inline unsigned long long getEmptyKey() { return ~0ULL; }

        static inline unsigned long long getTombstoneKey() { return ~0ULL - 1ULL; }

        [[maybe_unused]] static unsigned getHashValue(const unsigned long long &Val) {
            return (unsigned) (Val * 37ULL);
        }

        [[maybe_unused]] static bool isEqual(const unsigned long long &lhs, const unsigned long long &rhs) {
            return lhs == rhs;
        }
    };

    template<>
    struct DenseMapInfo<short> {
        static inline short getEmptyKey() { return 0x7FFF; }

        static inline short getTombstoneKey() { return -0x7FFF - 1; }

        [[maybe_unused]] static unsigned getHashValue(const short &Val) { return Val * 37U; }

        [[maybe_unused]] static bool isEqual(const short &lhs, const short &rhs) { return lhs == rhs; }
    };


    template<>
    struct DenseMapInfo<long> {
        static inline long getEmptyKey() { return (1UL << (sizeof(long) * 8 - 1)) - 1UL; }

        static inline long getTombstoneKey() { return getEmptyKey() - 1L; }

        [[maybe_unused]] static unsigned getHashValue(const long &Val) { return (unsigned) (Val * 37UL); }

        [[maybe_unused]] static bool isEqual(const long &lhs, const long &rhs) { return lhs == rhs; }
    };

    template<>
    struct DenseMapInfo<int> {
        static inline int getEmptyKey() { return 0x7fffffff; }

        static inline int getTombstoneKey() { return -0x7fffffff - 1; }

        [[maybe_unused]] static unsigned getHashValue(const int &Val) { return (unsigned) (Val * 37U); }

        [[maybe_unused]] static bool isEqual(const int &lhs, const int &rhs) { return lhs == rhs; }
    };

    template<>
    struct DenseMapInfo<long long> {
        static inline long long getEmptyKey() { return 0x7fffffffffffffffLL; }

        static inline long long getTombstoneKey() { return -0x7fffffffffffffffLL - 1; }

        [[maybe_unused]] static unsigned getHashValue(const long long &Val) { return (unsigned) (Val * 37ULL); }

        [[maybe_unused]]  static bool isEqual(const long long &lhs, const long long &rhs) {
            return lhs == rhs;
        }
    };

    template<typename T, typename U>
    struct DenseMapInfo<pair<T, U>> {
        using Pair = pair<T, U>;
        using FirstInfo = DenseMapInfo<T>;
        using SecondInfo = DenseMapInfo<U>;

        static inline Pair getEmptyKey() { return make_pair(FirstInfo::getEmptyKey(), SecondInfo::getEmptyKey()); }

        static inline Pair getTombstoneKey() {
            return make_pair(FirstInfo::getTombstoneKey(), SecondInfo::getTombstoneKey());
        }

        [[maybe_unused]] static unsigned getHashValue(const Pair &PairVal) {
            return detail::combineHashValue(FirstInfo::getHashValue(PairVal.first),
                                            SecondInfo::getHashValue(PairVal.second));
        }

        [[maybe_unused]] static unsigned getHashValuePiecewise(const T &First, const U &Second) {
            return detail::combineHashValue(FirstInfo::getHashValue(First), SecondInfo::getHashValue(Second));
        }

        [[maybe_unused]]  static bool isEqual(const Pair &lhs, const Pair &rhs) {
            return FirstInfo::isEqual(lhs.first, rhs.first) && SecondInfo::isEqual(lhs.second, rhs.second);
        }
    };

    template<typename... Ts>
    struct DenseMapInfo<tuple<Ts...>> {
        using Tuple = tuple<Ts...>;

        static inline Tuple getEmptyKey() { return Tuple(DenseMapInfo<Ts>::getEmptyKey()...); }

        static inline Tuple getTombstoneKey() { return Tuple(DenseMapInfo<Ts>::getTombstoneKey()...); }

        template<unsigned I>
        [[maybe_unused]] static unsigned getHashValueImpl(const Tuple &values, false_type) {
            using EltType = tuple_element_t<I, Tuple>;
            integral_constant<bool, I + 1 == sizeof...(Ts)> atEnd;
            return detail::combineHashValue(DenseMapInfo<EltType>::getHashValue(get<I>(values)),
                                            getHashValueImpl(values, atEnd));
        }

        static unsigned getHashValueImpl(const Tuple &, true_type) { return 0; }

        [[maybe_unused]] static unsigned getHashValue(const tuple<Ts...> &values) {
            integral_constant<bool, 0 == sizeof...(Ts)> atEnd;
            return getHashValueImpl(values, atEnd);
        }

        template<unsigned I>
        [[maybe_unused]] static bool isEqualImpl(const Tuple &lhs, const Tuple &rhs, false_type) {
            using EltType = tuple_element_t<I, Tuple>;
            integral_constant<bool, I + 1 == sizeof...(Ts)> atEnd;
            return DenseMapInfo<EltType>::isEqual(get<I>(lhs), get<I>(rhs)) &&
                   isEqualImpl(lhs, rhs, atEnd);
        }

        static bool isEqualImpl(const Tuple &, const Tuple &, true_type) { return true; }

        [[maybe_unused]] static bool isEqual(const Tuple &lhs, const Tuple &rhs) {
            integral_constant<bool, 0 == sizeof...(Ts)> atEnd;
            return isEqualImpl(lhs, rhs, atEnd);
        }
    };

} //namespace end retccUtils

#endif