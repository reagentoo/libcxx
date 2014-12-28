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
    using Get = _S<_xs...>;

private:
    /* This field is required
     * for zero weight of Value
     */
#ifdef ZERO_VALUE
    _X x[0];
#endif
};

/* Value aliases */
template <bool... _xs>
using Bool = Value<bool, _xs...>;

template <char... _xs>
using Char = Value<char, _xs...>;
template <short... _xs>
using Short = Value<short, _xs...>;
template <int... _xs>
using Int = Value<int, _xs...>;
template <long... _xs>
using Long = Value<long, _xs...>;
template <longlong... _xs>
using LongLong = Value<longlong, _xs...>;

template <uchar... _xs>
using UChar = Value<uchar, _xs...>;
template <ushort... _xs>
using UShort = Value<ushort, _xs...>;
template <uint... _xs>
using UInt = Value<uint, _xs...>;
template <ulong... _xs>
using ULong = Value<ulong, _xs...>;
template <ulonglong... _xs>
using ULongLong = Value<ulonglong, _xs...>;

/* Validator constraint */
template <class _X, Type::E _e, TypeValidator<_X, _e>... _xs>
struct Type::Validator<Value<_X, _xs...>, _e>
{
    using Type = Value<_X, _xs...>;
};

}

#endif // VALUE_TRAITS_H
