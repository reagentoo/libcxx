#ifndef TUPLE_H
#define TUPLE_H

#include <type_traits>

#include <mux.h>
#include <range.h>
#include <variadic_traits.h>

namespace xx {

/* =============== */
/* Tuple container */
template <class... _Pack>
class Tuple
{
    template <class...>
    friend class Tuple;
    using This = Tuple<_Pack...>;

    // alias stub for build with clang
    template <class... _Xs>
    using Tpl = Tuple<_Xs...>;

    template <uint _i>
    using At = MuxType<_i, _Pack...>;

    template <uint... _i>
    using Custom = Tuple<At<_i>...>;

    template <uint _i, uint _s, bool _r = false>
    using Expand = RangeExpand<_i, _s, _r>;

    template <uint _i, bool _r = false>
    using Extreme = At<_i ? _r ? (_i - 1) : 0 : 0>;

    template <class _T>
    using ResultOf = typename std::result_of<_T>::type;

    using X = Head<_Pack...>;
    using Xs = Tail<Tpl, _Pack...>;

    static constexpr uint m_size = sizeof...(_Pack);

protected:
    /* ________________ */
    /* Subtuple getters */
    template <uint _i>
    inline auto& tail(const UInt<_i>)
    { return m_xs.tail(UInt<_i - 1>()); }
    inline auto& tail(const UInt<0>)
    { return *this; }

    template <uint _i>
    inline const auto& tail(const UInt<_i>) const
    { return m_xs.tail(UInt<_i - 1>()); }
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
    inline Tuple() {}

    inline Tuple(const This& t)
    { set(t); }
    inline Tuple(This&& t)
    { set(std::move(t)); }

    inline Tuple(const _Pack&... xs)
    { set(xs...); }
    inline Tuple(_Pack&&... xs)
    { set(std::move(xs)...); }

    /* _________ */
    /* Operators */
    inline auto operator ==(const This& t) const
    { return eq(t); }
    inline auto operator !=(const This& t) const
    { return !eq(t); }

    template <uint _i>
    inline auto& operator [](const UInt<_i>)
    { return value(UInt<_i>()); }
    template <uint _i>
    inline const auto& operator [](const UInt<_i>) const
    { return value(UInt<_i>()); }

    inline auto& operator =(const This& t)
    { set(t); return *this; }
    inline auto& operator =(This&& t)
    { set(std::move(t)); return *this; }

    /* ___________ */
    /* Comparation */
    inline auto eq(const This& t) const
    { return m_x == t.m_x ? m_xs.eq(t.m_xs) : false; }

    /* ____________________ */
    /* Simple value getters */
    template <uint _i>
    inline auto& value(const UInt<_i>)
    { return tail(UInt<_i>()).m_x; }
    template <uint _i>
    inline const auto& value(const UInt<_i>) const
    { return tail(UInt<_i>()).m_x; }

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
        t.m_x = m_x;
        m_xs.get(t.m_xs);
    }
    inline void set(const This& t)
    {
        m_x = t.m_x;
        m_xs.set(t.m_xs);
    }
    inline void set(This&& t)
    {
        m_x = std::move(t.m_x);
        m_xs.set(std::move(t.m_xs));
    }

    /* ________________________________ */
    /* Base getters/setters (separated) */
    template <class... _Xs>
    inline void get(X& x, _Xs&... xs) const
    {
        x = m_x;
        m_xs.get(xs...);
    }
    template <class... _Xs>
    inline void set(const X& x, const _Xs&... xs)
    {
        m_x = x;
        m_xs.set(xs...);
    }
    template <class... _Xs>
    inline void set(X&& x, _Xs&&... xs)
    {
        m_x = std::move(x);
        m_xs.set(std::move(xs)...);
    }

    /* _______________________ */
    /* Partial getters/setters */
    template <uint... _is>
    inline void get(const UInt<_is...>,
                    Custom<_is...>& t) const
    {
        t.invoke([this](At<_is>&... xs) {
            get(UInt<_is...>(), xs...);
        });
    }
    template <uint... _is>
    inline void set(const UInt<_is...>,
                    const Custom<_is...>& t)
    {
        t.invoke([this](const At<_is>&... xs) {
            set(UInt<_is...>(), xs...);
        });
    }
    template <uint... _is>
    inline void set(const UInt<_is...>,
                    Custom<_is...>&& t)
    {
        t.invoke([this](At<_is>&... xs) {
            set(UInt<_is...>(), std::move(xs)...);
        });
    }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(Custom<_is...>& t) const
    { get(UInt<_is...>(), t); }
    template <uint... _is>
    inline void set(const Custom<_is...>& t)
    { set(UInt<_is...>(), t); }
    template <uint... _is>
    inline void set(Custom<_is...>&& t)
    { set(UInt<_is...>(), std::move(t)); }
#endif

    /* ___________________________________ */
    /* Partial getters/setters (separated) */
    template <uint _i, uint... _is, class... _Xs>
    inline void get(const UInt<_i, _is...>,
                    At<_i>& x, _Xs&... xs) const
    {
        x = tail(UInt<_i>()).m_x;
        get(UInt<_is...>(), xs...);
    }
    inline void get(const UInt<>) const
    { /* stub */ }

    template <uint _i, uint... _is, class... _Xs>
    inline void set(const UInt<_i, _is...>,
                    const At<_i>& x, const _Xs&... xs)
    {
        tail(UInt<_i>()).m_x = x;
        set(UInt<_is...>(), xs...);
    }
    inline void set(const UInt<>)
    { /* stub */ }

    template <uint _i, uint... _is, class... _Xs>
    inline void set(const UInt<_i, _is...>,
                    At<_i>&& x, _Xs&&... xs)
    {
        tail(UInt<_i>()).m_x = std::move(x);
        set(UInt<_is...>(), std::move(xs)...);
    }
    /* inline void set(const UInt<>&); */

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(At<_is>&... xs) const
    { get(UInt<_is...>(), xs...); }
    template <uint... _is>
    inline void set(const At<_is>&... xs)
    { set(UInt<_is...>(), xs...); }
    template <uint... _is>
    inline void set(At<_is>&&... xs)
    { set(UInt<_is...>(), std::move(xs)...); }
#endif

    /* _________________________________ */
    /* Range getters/setters (separated) */
    // TODO: make true g/s for ranges
    template <uint _s, bool _r, class... _Xs>
    inline void getRange(const UInt<_s>,
                         const Bool<false>,
                         X& x, _Xs&... xs) const
    {
        x = m_x;
        getRange(UInt<_s - 1>(), Bool<_r>(), xs...);
    }
    template <bool _r, class... _Xs>
    inline void getRange(const UInt<0>,
                         const Bool<false>) const
    { /* stub */ }

    /* _______________ */
    /* Merge functions */
    template <uint _s, bool _r, class... _Xs>
    inline const auto& merge(const UInt<_s>,
                             const Bool<_r>,
                             const Tuple<Extreme<_s, _r>, _Xs...>& t)
    {
        const auto& o = m_xs.merge(UInt<_s - 1>(), Bool<_r>(), t.tail(UInt<_r ? 0 : 1>()));
        m_x = mux(UInt<_r ? 0 : 1>(), o, t).m_x;
        return o.tail(UInt<_r ? 1 : 0>());
    }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             const Tuple<_Xs...>& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             const Tuple<X, _Xs...>& t)
    { return t; }

    template <uint _s, bool _r, class... _Xs>
    inline const auto& merge(const UInt<_s>,
                             const Bool<_r>,
                             Tuple<Extreme<_s, _r>, _Xs...>&& t)
    {
        const auto& o = m_xs.merge(UInt<_s - 1>(), Bool<_r>(), std::move(t.tail(UInt<_r ? 0 : 1>())));
        m_x = std::move(mux(UInt<_r ? 0 : 1>(), o, t).m_x);
        return o.tail(UInt<_r ? 1 : 0>());
    }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             Tuple<_Xs...>&& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline const auto& merge(const UInt<0>,
                             const Bool<_r>,
                             Tuple<X, _Xs...>&& t)
    { return t; }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const UInt<_i, _j, _s>,
                             const Bool<_r>,
                             const _T& t)
    { return tail(UInt<_i>()).merge(UInt<_s>(), Bool<_r>(), t.tail(UInt<_j>())); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const UInt<_i, _j, _s>,
                             const Bool<_r>,
                             _T&& t)
    { return tail(UInt<_i>()).merge(UInt<_s>(), Bool<_r>(), std::move(t.tail(UInt<_j>()))); }

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_s>(), Bool<_r>(), t); }

    template <uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_s>(), Bool<_r>(), std::move(t)); }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_i, _j, _s>(), Bool<_r>(), t); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_i, _j, _s>(), Bool<_r>(), std::move(t)); }
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
    inline auto invokeRange(const UInt<_i, _s>, _F func)
    { return invokeRange(UInt<_i, _s>(), Bool<false>(), func); }
    template <uint _i, uint _s, class _F>
    inline auto invokeRange(const UInt<_i, _s>, _F func) const
    { return invokeRange(UInt<_i, _s>(), Bool<false>(), func); }

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
    X  m_x;
    Xs m_xs;
};

/* Tuple container constraint*/
template <>
class Tuple<>
{
    template <class...>
    friend class Tuple;
    using This = Tuple<>;

    template <class _T>
    using ResultOf = typename std::result_of<_T>::type;

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
    inline Tuple()
    {}

    inline Tuple(const This&) {}
    inline Tuple(This&&) {}

    /* _________ */
    /* Operators */
    inline auto operator ==(const This&) const { return true; }
    inline auto operator !=(const This&) const { return false; }

    inline auto& operator =(const This&)
    { return *this; }
    inline auto& operator =(This&&)
    { return *this; }

    /* ___________ */
    /* Comparation */
    inline auto eq(const This&) const
    { return true; }

    /* ____________________ */
    /* Base getters/setters */
    inline void get(This&) const {}
    inline void set(const This&) {}
    inline void set(This&&) {}

    /* ________________________________ */
    /* Base getters/setters (separated) */
    inline void get() const {}
    inline void set() {}

    /* _______________________ */
    /* Partial getters/setters */
    inline void get(const UInt<>,
                    This&) const
    {}

    inline void set(const UInt<>,
                    const This&)
    {}
    inline void set(const UInt<>,
                    This&&)
    {}

#ifdef ENABLE_PLAIN_WRAPPERS
    template <uint... _is>
    inline void get(This& t) const
    { get(UInt<_is...>(), t); }

    template <uint... _is>
    inline void set(const This& t)
    { set(UInt<_is...>(), t); }
    template <uint... _is>
    inline void set(This&& t)
    { set(UInt<_is...>(), std::move(t)); }
#endif

    /* ___________________________________ */
    /* Partial getters/setters (separated) */
    inline void get(const UInt<>) const
    {}

    inline void set(const UInt<>)
    {}

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
                             const Tuple<_Xs...>& t)
    { return t; }
    template <bool _r, class... _Xs>
    inline auto&& merge(const UInt<0>,
                        const Bool<_r>,
                        Tuple<_Xs...>&& t)
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
    { return merge(UInt<_s>(), Bool<_r>(), std::move(t)); }

    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(const _T& t)
    { return merge(UInt<_s, _i, _j>(), Bool<_r>(), t); }
    template <uint _i, uint _j, uint _s, bool _r, class _T>
    inline const auto& merge(_T&& t)
    { return merge(UInt<_s, _i, _j>(), Bool<_r>(), std::move(t)); }
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

}

#endif // TUPLE_H