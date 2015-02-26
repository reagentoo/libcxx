#ifndef SAME_H
#define SAME_H

namespace xx {

/* ==== */
/* Same */
template<class _X, class...>
struct Same
{
    using Type = _X;
};

/* Same::Type alias */
template<class... _Xs>
using SameType = typename Same<_Xs...>::Type;

/* Same constraint */
template<class _X, class... _Xs>
struct Same<_X, _X, _Xs...>
{
    using Type = SameType<_X, _Xs...>;
};

/* Same constraint */
template<class _X, class _Y, class... _Zs>
struct Same<_X, _Y, _Zs...>;

}

#endif // SAME_H
