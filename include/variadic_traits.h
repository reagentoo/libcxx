#ifndef VARIADIC_TRAITS_H
#define VARIADIC_TRAITS_H

#include <value_traits.h>

namespace xx {

template <class _X, class... _Xs>
auto head(_X x, _Xs...)
{ return x; }

template <class _F, class _X, class... _Xs>
auto tail(_F func, _X, _Xs... xs)
{ return func(xs...); }

template <class _X, class..._Xs>
struct Variadic
{
    using Head = _X;
    template <template <class...> class _S>
    using Tail = _S<_Xs...>;
};

template <class... _Xs>
using Head = typename Variadic<_Xs...>::Head;

template <template <class...> class _Successor,
          class... _Xs>
using Tail = typename Variadic<_Xs...>
::template Tail<_Successor>;

template <class... _Xs>
constexpr uint size(const Variadic<_Xs...>)
{ return sizeof...(_Xs); }

#ifdef ENABLE_PLAIN_WRAPPERS
template <class... _Xs>
constexpr uint size()
{ return size(Variadic<_Xs...>()); }
#endif

template <class... _Xs>
using Size = UInt<size(Variadic<_Xs...>())>;

}

#endif // VARIADIC_TRAITS_H
