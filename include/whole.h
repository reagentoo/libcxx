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

        Eq,
        Neq,
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

    /* _____________________ */
    /* comparation operators */
    auto operator ==(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Eq>(), t.m_x); }
    auto operator !=(const This& t) const
    { return sfinae<bool>(Try<Sfinae::Neq>(), t.m_x); }


    inline auto& value()
    { return m_x; }
    inline const auto& value() const
    { return m_x; }

private:
    X m_x;
};

}

#endif // WHOLE_H
