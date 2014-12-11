#ifndef VALUE_TRAITS_H
#define VALUE_TRAITS_H

#include <type_traits.h>

namespace xx {

/* ===== */
/* Value */
template <class _X, _X... _xs>
struct Value
{
    using Type = _X;
    template <template <_X...> class _S>
    using Pass = _S<_xs...>;
};

/* Value aliases */
template <bool... _i>
using Bool = Value<bool, _i...>;

template <char... _i>
using Char = Value<char, _i...>;
template <short... _i>
using Short = Value<short, _i...>;
template <int... _i>
using Int = Value<int, _i...>;
template <long... _i>
using Long = Value<long, _i...>;
template <longlong... _i>
using LongLong = Value<longlong, _i...>;

template <uchar... _i>
using UChar = Value<uchar, _i...>;
template <ushort... _i>
using UShort = Value<ushort, _i...>;
template <uint... _i>
using UInt = Value<uint, _i...>;
template <ulong... _i>
using ULong = Value<ulong, _i...>;
template <ulonglong... _i>
using ULongLong = Value<ulonglong, _i...>;

/* Validator constraint */
template <class _X, Type::E _e, TypeValidator<_X, _e>... _i>
struct Type::Validator<Value<_X, _i...>, _e>
{
    using Type = Value<_X, _i...>;
};

}

#endif // VALUE_TRAITS_H
