#ifndef WHOLE_H
#define WHOLE_H

#include <backend.h>
#include <same.h>

namespace xx {

template <class _X>
_X& declref(...);

/* ===== */
/* Whole */
template <class _X>
union Whole {
private:
    template <class>
    friend union Whole;

    using X = _X;

    template <class _Y>
    using W = Whole<_Y>;

    using This = W<X>;

    enum class Sfinae {
        Ctor,
        Dtor,
        Copy,

        /* *Mv required for gcc only
         * (in order to avoid ambiguous) */
        CtorMv,
        CopyMv,

        Inc,    //TODO
        IncSf,  //TODO
        Dec,    //TODO
        DecSf,  //TODO

        Add,
        Sub,
        Mul,
        Div,
        Mod,

        Eq,
        Neq,
        Greater,
        GreaterEq,
        Less,
        LessEq,

        Not, //TODO
        And,
        Or,

        BitNeg, //TODO
        BitAnd,
        BitOr,
        BitXor,
        BitShift,
        BitRShift,
    };

    template <Sfinae _s>
    using Try = Value<Sfinae, _s>;

    /* ________________ */
    /* sfinae ctor/dtor */
    template<class _Z, class... _Ys>
    inline auto sfinae(const Try<Sfinae::Ctor>)
    -> SameType<_Z, decltype(*new X(declref<_Ys>()...))>
    { return *new (&m_x)X(); }
    template<class _Z, class... _Ys>
    inline auto sfinae(const Try<Sfinae::Dtor>)
    -> SameType<_Z, decltype(delete &declref<X>(declref<_Ys>()...))>
    { m_x.~X(); }

    /* ______________________ */
    /* sfinae copy/move ctors */
    template<class _Z, class... _Ys>
    inline auto sfinae(const Try<Sfinae::Ctor>, const _Ys&... ys)
    -> SameType<_Z, decltype(*new X(ys...))>
    { return *new (&m_x)X(ys...); }
    template<class _Z, class... _Ys>
    inline auto sfinae(const Try<Sfinae::CtorMv>, _Ys&&... ys)
    -> SameType<_Z, decltype(*new X(std::move(ys)...))>
    { return *new (&m_x)X(std::move(ys)...); }

    /* __________________________ */
    /* sfinae copy/move operators */
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Copy>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() = y)>
    { return m_x = y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::CopyMv>, _Y&& y)
    -> SameType<_Z, decltype(declref<X>() = std::move(y))>
    { return m_x = xx::move(y); }

    /* ___________________________ */
    /* sfinae arithmetic operators */
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Add>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() + y)>
    { return m_x + y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Sub>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() - y)>
    { return m_x - y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Mul>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() * y)>
    { return m_x * y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Div>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() / y)>
    { return m_x / y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Mod>, const _Y& y)
    -> SameType<_Z, decltype(declref<X>() % y)>
    { return m_x % y; }

    /* ____________________________ */
    /* sfinae comparation operators */
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Eq>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() == y)>
    { return m_x == y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Neq>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() != y)>
    { return m_x != y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Greater>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() > y)>
    { return m_x > y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::GreaterEq>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() >= y)>
    { return m_x >= y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Less>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() < y)>
    { return m_x < y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::LessEq>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() <= y)>
    { return m_x <= y; }

    /* ________________________ */
    /* sfinae logical operators */
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::And>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() && y)>
    { return m_x && y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::Or>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() || y)>
    { return m_x || y; }

    /* ________________________ */
    /* sfinae bitwise operators */
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::BitAnd>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() & y)>
    { return m_x & y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::BitOr>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() | y)>
    { return m_x | y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::BitXor>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() ^ y)>
    { return m_x ^ y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::BitShift>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() << y)>
    { return m_x << y; }
    template<class _Z, class _Y>
    inline auto sfinae(const Try<Sfinae::BitRShift>, const _Y& y) const
    -> SameType<_Z, decltype(declref<X>() >> y)>
    { return m_x >> y; }

    /* ____________ */
    /* sfinae stubs */
    template <class _Z, Sfinae _s>
    inline _Z sfinae(const Try<_s>) const
    { return sfinae_throw<_Z>(_s, 0); }
    template <class _Z, Sfinae _s, class... _Ys>
    inline _Z sfinae(const Try<_s>, const _Ys&...) const
    { return sfinae_throw<_Z>(_s, sizeof...(_Ys)); }

    /* TODO: try to uncomment
     * this stub function in gcc-5.x.x. */
    /*
    template <class _Z, Sfinae _s, class... _Ys>
    inline _Z sfinae(const Try<_s>, _Ys&&...) const
    { return sfinae_throw<_Z>(_s, sizeof...(_Ys)); }
    */

    template <class _Z>
    inline _Z sfinae_throw(Sfinae, int) const
    { throw -1; }

public:
    /* _________ */
    /* ctor/dtor */
    inline Whole()
    { sfinae<X&>(Try<Sfinae::Ctor>()); }
    inline ~Whole()
    { sfinae<void>(Try<Sfinae::Dtor>()); }

    /* _______________ */
    /* copy/move ctors */
    inline Whole(const This& t)
    { sfinae<X&>(Try<Sfinae::Ctor>(), t.m_x); }
    inline Whole(This&& t)
    { sfinae<X&>(Try<Sfinae::CtorMv>(), std::move(t.m_x)); }

    /* ___________________ */
    /* copy/move operators */
    template <class... _Ys>
    inline Whole(const W<_Ys>&... ys)
    { sfinae<X&>(Try<Sfinae::Ctor>(), ys.m_x...); }
    template <class... _Ys>
    inline Whole(W<_Ys>&&... ys)
    { sfinae<X&>(Try<Sfinae::CtorMv>(), std::move(ys.m_x)...); }

    /* ___________________ */
    /* copy/move operators */
    auto operator =(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Copy>(), t.m_x);
        return *this;
    }
    auto operator =(This&& t)
    {
        sfinae<X&>(Try<Sfinae::CopyMv>(), std::move(t.m_x));
        return *this;
    }

    /* ____________________ */
    /* arithmetic operators */
    auto operator +(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Add>(), t.m_x);
        return *this;
    }
    auto operator -(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Sub>(), t.m_x);
        return *this;
    }
    auto operator *(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Mul>(), t.m_x);
        return *this;
    }
    auto operator /(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Div>(), t.m_x);
        return *this;
    }
    auto operator %(const This& t)
    {
        sfinae<X&>(Try<Sfinae::Mod>(), t.m_x);
        return *this;
    }

    /* _____________________ */
    /* comparation operators */
    auto operator ==(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Eq>(), t.m_x); }
    auto operator !=(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Neq>(), t.m_x); }
    auto operator >(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Greater>(), t.m_x); }
    auto operator >=(const This& t) const
    { return sfinae<bool>(Try<Sfinae::GreaterEq>(), t.m_x); }
    auto operator <(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Less>(), t.m_x); }
    auto operator <=(const This& t) const
    { return sfinae<bool>(Try<Sfinae::LessEq>(), t.m_x); }

    /* _________________ */
    /* logical operators */
    auto operator &&(const This& t) const
    { return sfinae<bool>(Try<Sfinae::And>(), t.m_x); }
    auto operator ||(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Or>(), t.m_x); }

    /* _________________ */
    /* bitwise operators */
    auto operator &(const This& t) const
    {
        sfinae<X&>(Try<Sfinae::BitAnd>(), t.m_x);
        return *this;
    }
    auto operator |(const This& t) const
    {
        sfinae<X&>(Try<Sfinae::BitOr>(), t.m_x);
        return *this;
    }
    auto operator ^(const This& t) const
    {
        sfinae<X&>(Try<Sfinae::BitXor>(), t.m_x);
        return *this;
    }
    auto operator <<(const This& t) const
    {
        sfinae<X&>(Try<Sfinae::BitShift>(), t.m_x);
        return *this;
    }
    auto operator >>(const This& t) const
    {
        sfinae<X&>(Try<Sfinae::BitRShift>(), t.m_x);
        return *this;
    }

    inline auto& value()
    { return m_x; }
    inline const auto& value() const
    { return m_x; }

private:
    X m_x;
};

}

#endif // WHOLE_H
