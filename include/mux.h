#ifndef MUX_H
#define MUX_H

#include <value_traits.h>

namespace xx {

/* =========== */
/* Multiplexer */
template <uint _index,
          class _X, class... _Xs>
class Mux
{
    using M = Mux<_index - 1, _Xs...>;

public:
    using Type = typename M::Type;

    inline static auto value(_X&, _Xs&... xs)
    { return M::value(xs...); }
};

/* Multiplexer constraint */
template <class _X, class... _Xs>
class Mux<0, _X, _Xs...>
{
public:
    using Type = _X;

    inline static auto value(_X& x, _Xs&...)
    { return x; }
};

/* Mux::Type alias */
template <uint _index, class... _Xs>
using MuxType = typename Mux<_index, _Xs...>::Type;

template <uint _index, class... _Xs>
inline auto mux(const UInt<_index>, _Xs&... xs)
{ return Mux<_index, _Xs...>::value(xs...); }

#ifdef ENABLE_PLAIN_WRAPPERS
template <uint _index, class... _Xs>
inline auto mux(_Xs&... xs)
{ return mux(UInt<_index>(), xs...); }
#endif

}

#endif // MUX_H
