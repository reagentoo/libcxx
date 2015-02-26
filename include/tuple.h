#ifndef TUPLE_H
#define TUPLE_H

#include <backend.h>
#include <mux.h>
#include <range.h>
#include <variadic_traits.h>

namespace xx {

/* =============== */
/* Tuple container */
template <bool _union, class... _Pack>
class Tuple
{
    template <bool, class...>
    friend class Tuple;

    template <class... _Xs>
    using T = Tuple<_union, _Xs...>;

    using This = T<_Pack...>;

    template <uint _i>
    using At = MuxType<_i, _Pack...>;

    template <uint... _i>
    using Custom = T<At<_i>...>;

    template <uint _i, uint _s, bool _r = false>
    using Expand = RangeExpand<_i, _s, _r>;

    template <uint _i, bool _r = false>
    using Extreme = At<_i ? _r ? (_i - 1) : 0 : 0>;

    using X = Head<_Pack...>;
    using Xs = Tail<T, _Pack...>;

    static constexpr uint m_size = sizeof...(_Pack);

protected:
    /* ________________ */
    /* Subtuple getters */
    template <uint _i>
    inline auto& tail(const UInt<_i>)
    { return m_.xs.tail(UInt<_i - 1>()); }
    inline auto& tail(const UInt<0>)
    { return *this; }

    template <uint _i>
    inline const auto& tail(const UInt<_i>) const
    { return m_.xs.tail(UInt<_i - 1>()); }
    inline const auto& tail(const UInt<0>) const
    { return *this; }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _i>
    inline auto& tail()
    { return tail(UInt<_i>()); }

    template <uint _i>
    inline const auto& tail() const
    { return tail(UInt<_i>()); }
#endif

public:
    /* ____________ */
    /* Constructors */
    inline  Tuple() = default;
    inline ~Tuple() = default;

    inline Tuple(const This& t) :
        m_(t.m_)
    { }
    inline Tuple(This&& t) :
        m_(xx::move(t.m_))
    { }

    template <class... _Xs>
    inline explicit Tuple(const X& x, const _Xs&... xs) :
        m_(x, Xs(xs...))
    { }
    template <class... _Xs>
    inline explicit Tuple(X&& x, _Xs&&... xs) :
        m_(xx::move(x), Xs(xx::move(xs)...))
    { }

    /* _________ */
    /* Operators */
    inline auto operator ==(const This& t) const
    { return eq(t); }
    inline auto operator !=(const This& t) const
    { return neq(t); }

    template <uint _i>
    inline auto& operator [](const UInt<_i> i)
    { return value(i); }
    template <uint _i>
    inline const auto& operator [](const UInt<_i> i) const
    { return value(i); }

    inline auto& operator =(const This& t)
    { set(t); return *this; }
    inline auto& operator =(This&& t)
    { set(xx::move(t)); return *this; }

    /* ___________ */
    /* Comparation */
    inline auto eq(const This& t) const
    { return m_.x == t.m_.x ? m_.xs.eq(t.m_.xs) : false; }
    inline auto neq(const This& t) const
    { return m_.x != t.m_.x ? true : m_.xs.neq(t.m_.xs); }

    /* ____________________ */
    /* Simple value getters */
    template <uint _i>
    inline auto& value(const UInt<_i> i)
    { return tail(i).m_.x; }
    template <uint _i>
    inline const auto& value(const UInt<_i> i) const
    { return tail(i).m_.x; }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _i>
    inline auto& value()
    { return value(UInt<_i>()); }
    template <uint _i>
    inline const auto& value() const
    { return value(UInt<_i>()); }
#endif

    /* ____________________ */
    /* Base getters/setters */
    inline void get(This& t) const
    {
        t.set(*this);
    }
    inline void get(This&& t)
    {
        t.set(xx::move(*this));
    }
    inline void set(const This& t)
    {
        m_.x = t.m_.x;
        m_.xs.set(t.m_.xs);
    }
    inline void set(This&& t)
    {
        m_.x = xx::move(t.m_.x);
        m_.xs.set(xx::move(t.m_.xs));
    }

    /* ________________________________ */
    /* Base getters/setters (separated) */
    template <class... _Xs>
    inline void get(X& x, _Xs&... xs) const
    {
        x = m_.x;
        m_.xs.get(xs...);
    }
    template <class... _Xs>
    inline void get(X&& x, _Xs&&... xs)
    {
        x = xx::move(m_.x);
        m_.xs.get(xx::move(xs)...);
    }
    template <class... _Xs>
    inline void set(const X& x, const _Xs&... xs)
    {
        m_.x = x;
        m_.xs.set(xs...);
    }
    template <class... _Xs>
    inline void set(X&& x, _Xs&&... xs)
    {
        m_.x = xx::move(x);
        m_.xs.set(xx::move(xs)...);
    }

    /* _______________________ */
    /* Partial getters/setters */
    template <uint... _is>
    inline void get(const UInt<_is...>,
                    Custom<_is...>& t) const
    {
        t.set(*this);
    }
    template <uint... _is>
    inline void get(const UInt<_is...>,
                    Custom<_is...>&& t)
    {
        t.set(xx::move(*this));
    }
    template <uint... _is>
    inline void set(const UInt<_is...> is,
                    const Custom<_is...>& t)
    {
        t.invoke([this, is](const At<_is>&... xs) {
            set(is, xs...);
        });
    }
    template <uint... _is>
    inline void set(const UInt<_is...> is,
                    Custom<_is...>&& t)
    {
        t.invoke([this, is](At<_is>&... xs) {
            set(is, xx::move(xs)...);
        });
    }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(Custom<_is...>& t) const
    { get(UInt<_is...>(), t); }
    template <uint... _is>
    inline void get(Custom<_is...>&& t)
    { get(UInt<_is...>(), xx::move(t)); }
    template <uint... _is>
    inline void set(const Custom<_is...>& t)
    { set(UInt<_is...>(), t); }
    template <uint... _is>
    inline void set(Custom<_is...>&& t)
    { set(UInt<_is...>(), xx::move(t)); }
#endif

    /* ___________________________________ */
    /* Partial getters/setters (separated) */
    template <uint _i, uint... _is, class... _Xs>
    inline void get(const UInt<_i, _is...>,
                    At<_i>& x, _Xs&... xs) const
    {
        x = tail(UInt<_i>()).m_.x;
        get(UInt<_is...>(), xs...);
    }
    inline void get(const UInt<>) const
    { /* stub */ }

    template <uint _i, uint... _is, class... _Xs>
    inline void get(const UInt<_i, _is...>,
                    At<_i>&& x, _Xs&&... xs)
    {
        x = xx::move(tail(UInt<_i>()).m_.x);
        get(UInt<_is...>(), xx::move(xs)...);
    }
    inline void get(const UInt<>)
    { /* stub */ }

    template <uint _i, uint... _is, class... _Xs>
    inline void set(const UInt<_i, _is...>,
                    const At<_i>& x, const _Xs&... xs)
    {
        tail(UInt<_i>()).m_.x = x;
        set(UInt<_is...>(), xs...);
    }
    inline void set(const UInt<>)
    { /* stub */ }

    template <uint _i, uint... _is, class... _Xs>
    inline void set(const UInt<_i, _is...>,
                    At<_i>&& x, _Xs&&... xs)
    {
        tail(UInt<_i>()).m_.x = xx::move(x);
        set(UInt<_is...>(), xx::move(xs)...);
    }
    /* inline void set(const UInt<>&); */

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(At<_is>&... xs) const
    { get(UInt<_is...>(), xs...); }
    template <uint... _is>
    inline void get(At<_is>&&... xs)
    { get(UInt<_is...>(), xx::move(xs)...); }
    template <uint... _is>
    inline void set(const At<_is>&... xs)
    { set(UInt<_is...>(), xs...); }
    template <uint... _is>
    inline void set(At<_is>&&... xs)
    { set(UInt<_is...>(), xx::move(xs)...); }
#endif

    /* _________________________________ */
    /* Range getters/setters (separated) */
    // TODO: make true g/s for ranges
    template <uint _s, bool _r, class... _Xs>
    inline void getRange(const UInt<_s>,
                         const False,
                         X& x, _Xs&... xs) const
    {
        x = m_.x;
        getRange(UInt<_s - 1>(), Bool<_r>(), xs...);
    }
    template <bool _r, class... _Xs>
    inline void getRange(const UInt<0>,
                         const False) const
    { /* stub */ }

    /* _______________ */
    /* Merge functions */
    template <uint _s, bool _r, class... _Xs>
    inline const auto& merge(const UInt<_s>,
                             const Bool<_r> r,
                             const T<Extreme<_s, _r>, _Xs...>& t)
    {
        const auto& o = m_.xs.merge(UInt<_s - 1>(), r, t.tail(UInt<_r ? 0 : 1>()));
        m_.x = mux(UInt<_r ? 0 : 1>(), o, t).m_.x;
        return o.tail(UInt<_r ? 1 : 0>());
    }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             const T<_Xs...>& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             const T<X, _Xs...>& t)
    { return t; }

    template <uint _s, bool _r, class... _Xs>
    inline const auto& merge(const UInt<_s>,
                             const Bool<_r> r,
                             T<Extreme<_s, _r>, _Xs...>&& t)
    {
        const auto& o = m_.xs.merge(UInt<_s - 1>(), r, xx::move(t.tail(UInt<_r ? 0 : 1>())));
        m_.x = xx::move(mux(UInt<_r ? 0 : 1>(), o, t).m_.x);
        return o.tail(UInt<_r ? 1 : 0>());
    }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             T<_Xs...>&& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             T<X, _Xs...>&& t)
    { return t; }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const UInt<_i, _j, _s>,
                             const Bool<_r> r,
                             const _T& t)
    { return tail(UInt<_i>()).merge(UInt<_s>(), r, t.tail(UInt<_j>())); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const UInt<_i, _j, _s>,
                             const Bool<_r> r,
                             _T&& t)
    { return tail(UInt<_i>()).merge(UInt<_s>(), r, xx::move(t.tail(UInt<_j>()))); }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_s>(), Bool<_r>(), t); }

    template <uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_s>(), Bool<_r>(), xx::move(t)); }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_i, _j, _s>(), Bool<_r>(), t); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_i, _j, _s>(), Bool<_r>(), xx::move(t)); }
#endif

    /* ______ */
    /* Invoke */
    template <class _F>
    inline auto invoke(_F func)
    { return invokeRange(UInt<0, m_size>(), func); }
    template <class _F>
    inline auto invoke(_F func) const
    { return invokeRange(UInt<0, m_size>(), func); }

    /* ______________ */
    /* Partial invoke */
    template <uint... _is, class _F>
    inline auto invoke(const UInt<_is...>, _F func)
    -> ResultOf<_F(At<_is>&...)>
    { return func(value(UInt<_is>())...); }
    template <uint... _is, class _F>
    inline auto invoke(const UInt<_is...>, _F func) const
    -> ResultOf<_F(const At<_is>&...)>
    { return func(value(UInt<_is>())...); }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is, class _F>
    inline auto invoke(_F func)
    -> ResultOf<_F(At<_is>&...)>
    { return invoke(UInt<_is...>(), func); }
    template <uint... _is, class _F>
    inline auto invoke(_F func) const
    -> ResultOf<_F(At<_is>&...)>
    { return invoke(UInt<_is...>(), func); }
#endif

    /* ________________ */
    /* Invoke for range */
    template <uint _i, uint _s, class _F>
    inline auto invokeRange(const UInt<_i, _s> is, _F func)
    { return invokeRange(is, False(), func); }
    template <uint _i, uint _s, class _F>
    inline auto invokeRange(const UInt<_i, _s> is, _F func) const
    { return invokeRange(is, False(), func); }

    template <uint _i, uint _s, bool _r = false, class _F>
    inline auto invokeRange(const UInt<_i, _s>,
                            const Bool<_r>,
                            _F func)
    { return invoke(Expand<_i, _s, _r>(), func); }
    template <uint _i, uint _s, bool _r = false, class _F>
    inline auto invokeRange(const UInt<_i, _s>,
                            const Bool<_r>,
                            _F func) const
    { return invoke(Expand<_i, _s, _r>(), func); }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _i, uint _s, bool _r = false, class _F>
    inline auto invokeRange(_F func)
    { return invokeRange(UInt<_i, _s>(), Bool<_r>(), func); }
    template <uint _i, uint _s, bool _r = false, class _F>
    inline auto invokeRange(_F func) const
    { return invokeRange(UInt<_i, _s>(), Bool<_r>(), func); }
#endif

private:
    struct S
    {
        inline  S() = default;
        inline ~S() = default;

        inline S(const S& s_) :
            x(s_.x),
            xs(s_.xs)
        { }
        inline S(S&& s_) :
            x(xx::move(s_.x)),
            xs(xx::move(s_.xs))
        { }

        inline explicit S(const X& x_, const Xs& xs_) :
            x(x_),
            xs(xs_)
        { }
        inline explicit S(X&& x_, Xs&& xs_) :
            x(xx::move(x_)),
            xs(xx::move(xs_))
        { }

        X  x;
        Xs xs;
    };

    union U
    {
        inline  U() : xs() { }
        inline ~U() { xs.~Xs(); }

        inline U(const U& u_) :
            xs(u_.xs)
        { }
        inline U(U&& u_) :
            xs(xx::move(u_.xs))
        { }

        inline explicit U(const X& , const Xs& xs_) :
            xs(xs_)
        { }
        inline explicit U(X&&, Xs&& xs_) :
            xs(xx::move(xs_))
        { }

        X  x;
        Xs xs;
    };

    MuxEither<_union, U, S> m_;
};

/* Tuple container constraint*/
template <bool _union>
class Tuple<_union>
{
    template <bool, class...>
    friend class Tuple;

    template <class... _Xs>
    using T = Tuple<_union, _Xs...>;

    using This = T<>;

protected:
    /* ________________ */
    /* Subtuple getters */
    inline auto& tail(const UInt<0>)
    { return *this; }
    inline const auto& tail(const UInt<0>) const
    { return *this; }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _i>
    inline auto& tail()
    { return tail(UInt<_i>()); }
    template <uint _i>
    inline const auto& tail() const
    { return tail(UInt<_i>()); }
#endif

public:
    /* ____________ */
    /* Constructors */
    inline  Tuple() = default;
    inline ~Tuple() = default;

    inline Tuple(const This&)
    { }
    inline Tuple(This&&)
    { }

    /* _________ */
    /* Operators */
    inline auto operator ==(const This&) const
    { return true; }
    inline auto operator !=(const This&) const
    { return false; }

    inline auto& operator =(const This&)
    { return *this; }
    inline auto& operator =(This&&)
    { return *this; }

    /* ___________ */
    /* Comparation */
    inline auto eq(const This&) const
    { return true; }
    inline auto neq(const This&) const
    { return false; }

    /* ____________________ */
    /* Base getters/setters */
    inline void get(This&) const
    { }
    inline void get(This&&)
    { }
    inline void set(const This&)
    { }
    inline void set(This&&)
    { }

    /* ________________________________ */
    /* Base getters/setters (separated) */
    inline void get() const
    { }
    inline void set()
    { }

    /* _______________________ */
    /* Partial getters/setters */
    inline void get(const UInt<>,
                    This&) const
    { }
    inline void get(const UInt<>,
                    This&&)
    { }
    inline void set(const UInt<>,
                    const This&)
    { }
    inline void set(const UInt<>,
                    This&&)
    { }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(This& t) const
    { get(UInt<_is...>(), t); }
    template <uint... _is>
    inline void get(This&& t)
    { get(UInt<_is...>(), xx::move(t)); }
    template <uint... _is>
    inline void set(const This& t)
    { set(UInt<_is...>(), t); }
    template <uint... _is>
    inline void set(This&& t)
    { set(UInt<_is...>(), xx::move(t)); }
#endif

    /* ___________________________________ */
    /* Partial getters/setters (separated) */
    inline void get(const UInt<>) const
    { }
    inline void set(const UInt<>)
    { }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get() const
    { get(UInt<_is...>()); }
    template <uint... _is>
    inline void set()
    { set(UInt<_is...>()); }
#endif

    /* _______________ */
    /* Merge functions */
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             const T<_Xs...>& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline auto&& merge(const UInt<0>,
                        const Bool<_r>,
                        T<_Xs...>&& t)
    { return t; }

    template <bool _r, uint _j = 0, class _T>
    inline const auto& merge(const UInt<0, 0, _j>,
                             const Bool<_r>,
                             const _T& t)
    { return t; }
    template <bool _r, uint _j = 0, class _T>
    inline const auto& merge(const UInt<0, 0, _j>,
                             const Bool<_r>,
                             _T&& t)
    { return t; }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_s>(), Bool<_r>(), t); }
    template <uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_s>(), Bool<_r>(), xx::move(t)); }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_s, _i, _j>(), Bool<_r>(), t); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_s, _i, _j>(), Bool<_r>(), xx::move(t)); }
#endif

    /* ______ */
    /* Invoke */
    template <class _F>
    inline auto invoke(_F func)
    -> ResultOf<_F()>
    { return func(); }
    template <class _F>
    inline auto invoke(_F func) const
    -> ResultOf<_F()>
    { return func(); }

    template <class _F>
    inline auto invoke(const UInt<>, _F func)
    -> ResultOf<_F()>
    { return func(); }
    template <class _F>
    inline auto invoke(const UInt<>, _F func) const
    -> ResultOf<_F()>
    { return func(); }
};

/* Tuple aliases */
template <class... _Pack>
using Struct = Tuple<false, _Pack...>;
template <class... _Pack>
using Union = Tuple<true, _Pack...>;

}

#endif // TUPLE_H
