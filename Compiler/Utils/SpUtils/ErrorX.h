#ifndef Rtcc_ErrorX_h
#define Rtcc_ErrorX_h

#include "Utils/SpUtils/AlignOf.h"

#include <cassert>
#include <system_error>
#include <type_traits>
#include <utility>

using namespace std;

namespace retccUtils{
    template <class T>
    class ErrorX {
    public:
        template <class AnotherT> friend class ErrorX;
        /// is_reference_v -> judge a type is reference type or not.
        /// is_reference_v returns a bool value, if T is a reference type, return true, else return false;
        static constexpr bool isReference = is_reference_v<T>;
        /// reference_wrapper -> Wrap a object to reference.
        using wrapping = reference_wrapper<remove_reference_t<T>>;
        using storage_type = conditional_t<isReference, wrapping, T>;
    private:
        /// conditional_t -> During compile time, choose one type in 2 types.
        /// remove_reference -> Delete reference modifier "&" or "&&"
        /// \code
        /// remove_reference_t<int&>
        /// remove_reference_t<int&&>
        /// \endcode
        /// First code block returns "int", second code block returns "const int"

        //I don't think typedef is a better idea than using
//        typedef reference = remove_reference_t<T> &;
//        typedef const_reference = const remove_reference_t<T> &;
//        typedef pointer = remove_reference_t<T> *;
//        typedef const_pointer = const remove_reference_t<T> *;
        using reference = remove_reference_t<T> &; //Define alias for type modifier
        using const_reference = const remove_reference_t<T> &;
        using pointer = remove_reference_t<T> *;
        using const_pointer = const remove_reference_t<T> *;
    public:
        /// enable_if_t -> Make Compile Judgement in template function
        /// is_error_condition_enum, is_error_code_enum -> Judge a enumeration is error code / error condition or not
        /// make_error_code -> Convert error enumeration to error code object
        template<class X>
        ErrorX(X ErrorCde, typename std::enable_if<is_error_code_enum<X>::value
                                                   || is_error_condition_enum<X>::value, int>::type * = nullptr)
                : HasError(true)
        {
            new (getErrorStorage()) error_code(make_error_code(ErrorCde));
        }

        ErrorX(error_code EC) : HasError(true)
        {
            new (getErrorStorage()) error_code(EC);
        }
        template <class OtherT>
        ErrorX(OtherT &&Val,
        enable_if_t<is_convertible_v<OtherT, T>> * = nullptr)
        : HasError(false) {
            new (getStorage()) storage_type(forward<OtherT>(Val));
        }
        ErrorX(const ErrorX &Other) {
            copyConstruct(Other);
        }
        template <class OtherT>
        ErrorX(const ErrorX<OtherT> &Other,
                enable_if_t<is_convertible_v<OtherT, T>> * = nullptr) {
            copyConstruct(Other);
        }
        template <class OtherT>
        explicit ErrorX(
                const ErrorX<OtherT> &Other,
                enable_if_t<!is_convertible_v<OtherT, const T &>> * = nullptr) {
            copyConstruct(Other);
        }
        ErrorX(ErrorX &&Other) {
            moveConstruct(move(Other));
        }
        template <class OtherT>
        ErrorX(ErrorX<OtherT> &&Other,
        enable_if_t<is_convertible_v<OtherT, T>> * = nullptr) {
            moveConstruct(move(Other));
        }
        template <class OtherT>
        explicit ErrorX(
                ErrorX<OtherT> &&Other,
                enable_if_t<!is_convertible_v<OtherT, T>, int> * = nullptr) {
            moveConstruct(move(Other));
        }
        ErrorX &operator=(const ErrorX &Other) {
            copyAssign(Other);
            return *this;
        }

        ErrorX &operator=(ErrorX &&Other) {
            moveAssign(move(Other));
            return *this;
        }

        ~ErrorX() {
            if (!HasError)
                getStorage()->~storage_type();
        }

        explicit operator bool() const {
            return !HasError;
        }
        reference get() { return *getStorage(); }
        const_reference get() const { return const_cast<ErrorX<T> *>(this)->get(); }
        error_code getError() const {
            return HasError ? *getErrorStorage() : error_code();
        }
        pointer operator ->() {
            return toPointer(getStorage());
        }
        const_pointer operator->() const { return toPointer(getStorage()); }
        reference operator *() {
            return *getStorage();
        }
        const_reference operator*() const { return *getStorage(); }
    private:
        template <class OtherT>
        void copyConstruct(const ErrorX<OtherT> &Other) {
            if (!Other.HasError) {
                // Get the other value.
                HasError = false;
                new (getStorage()) storage_type(*Other.getStorage());
            } else {
                // Get other's error.
                HasError = true;
                new (getErrorStorage()) error_code(Other.getError());
            }
        }
        template <class T1>
        static bool compareThisIfSameType(const T1 &a, const T1 &b) {
            return &a == &b;
        }
        template <class T1, class T2>
        static bool compareThisIfSameType(const T1 &a, const T2 &b) {
            return false;
        }
        template <class OtherT>
        void copyAssign(const ErrorX<OtherT> &Other) {
            if (compareThisIfSameType(*this, Other))
                return;

            this->~ErrorX();
            new (this) ErrorX(Other);
        }
        template <class OtherT>
        void moveConstruct(ErrorX<OtherT> &&Other) {
            if (!Other.HasError) {
                // Get the other value.
                HasError = false;
                new (getStorage()) storage_type(move(*Other.getStorage()));
            } else {
                // Get other's error.
                HasError = true;
                new (getErrorStorage()) error_code(Other.getError());
            }
        }
        template <class OtherT>
        void moveAssign(ErrorX<OtherT> &&Other) {
            if (compareThisIfSameType(*this, Other))
                return;

            this->~ErrorX();
            new (this) ErrorX(move(Other));
        }
        pointer toPointer(pointer Val) {
            return Val;
        }

        const_pointer toPointer(const_pointer Val) const { return Val; }

        pointer toPointer(wrapping *Val) {
            return &Val->get();
        }

        const_pointer toPointer(const wrapping *Val) const { return &Val->get(); }

        storage_type *getStorage() {
            assert(!HasError && "An Error exists, it is unavailable to get value");
            return reinterpret_cast<storage_type *>(&TStorage);
        }
        const storage_type *getStorage() const {
            assert(!HasError && "An Error exists, it is unavailable to get value");
            return reinterpret_cast<const storage_type *>(&TStorage);
        }

        error_code *getErrorStorage() {
            assert(HasError && "An Error exists, it is unavailable to get value");
            return reinterpret_cast<error_code *>(&ErrorStorage);
        }

        const error_code *getErrorStorage() const {
            return const_cast<ErrorX<T> *>(this)->getErrorStorage();
        }

        union {
            rtccUtils::AlignedCharArrayUnion<storage_type> TStorage;
            rtccUtils::AlignedCharArrayUnion<error_code> ErrorStorage;
        };
//        bool HasError = true;
        bool HasError : 1; //Bit-Zone
    };

    template <class T, class E>
    enable_if_t<is_error_code_enum<E>::value ||
                     is_error_condition_enum<E>::value,
            bool>
    operator==(const ErrorX<T> &Err, E Code) {
        return Err.getError() == Code;
    }
} //namespace end retccUtils

#endif