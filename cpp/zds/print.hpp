#pragma once

#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <sstream>
#include <type_traits>

namespace print_helper
{

    template <class...>
    using _void_t = void;

    template <class T, class = _void_t<>>
    struct _is_std_container : std::false_type
    {
    };

    template <class T>
    struct _is_std_container<T, _void_t<typename T::iterator>> : std::true_type
    {
    };

    template <class T, class OS, class = _void_t<>>
    struct _is_streamable_helper : std::false_type
    {
    };

    template <class T, class OS>
    struct _is_streamable_helper<
        T, OS, _void_t<decltype(std::declval<OS &>() << std::declval<const T &>())>>
        : std::true_type
    {
    };

    template <class T>
    struct _is_streamable
        : std::integral_constant<bool, bool(_is_streamable_helper<T, std::ostream>::value &&
                                            !std::is_same<T, bool>::value) &&
                                           !std::is_pointer<T>::value>
    {
    };

    template <class T>
    using _enable_if = typename std::enable_if<T::value, std::ostream>::type &;

    template <class T, size_t N>
    std::ostream &_print(std::ostream &os, const T (&v)[N]);

    std::ostream &_print(std::ostream &os, const char *v);
    std::ostream &_print(std::ostream &os, const std::string s);

    template <class T>
    std::ostream &_print(std::ostream &os, const T *p);

    template <class K, class V>
    std::ostream &_print(std::ostream &os, const std::pair<K, V> &v);

    template <class T>
    _enable_if<std::is_same<T, bool>> _print(std::ostream &os, const T &v);

    template <class T>
    _enable_if<_is_streamable<T>> _print(std::ostream &os, const T &v);

    template <class C>
    _enable_if<_is_std_container<C>> _print(std::ostream &os, const C &c);

    template <class T, class = _void_t<>>
    struct is_printable : std::false_type
    {
    };

    template <class T>
    struct is_printable<T, _void_t<decltype(_print(std::declval<std::ostream &>(),
                                                   std::declval<const T &>()))>>
        : std::true_type
    {
    };

    template <class C>
    _enable_if<_is_std_container<C>> _print(std::ostream &os, const C &c)
    {
        os << "{";

        auto it = c.begin();
        if (it != c.end())
        {
            _print(os, *it++);
        }

        while (it != c.end())
        {
            _print(os << ", ", *it++);
        }

        return os << "}";
    }

    template <class T>
    _enable_if<_is_streamable<T>> _print(std::ostream &os, const T &v)
    {
        return os << v;
    }

    template <class T>
    _enable_if<std::is_same<T, bool>> _print(std::ostream &os, const T &v)
    {
        return os << (v ? "true" : "false");
    }

    template <class K, class V>
    std::ostream &_print(std::ostream &os, const std::pair<K, V> &v)
    {
        return _print(_print(os, v.first) << ": ", v.second);
    }

    template <class T>
    std::ostream &_print(std::ostream &os, const T *p)
    {
        if (p == nullptr)
        {
            return os << "<nil>";
        }
        else if (std::is_pointer<std::remove_reference_t<T>>::value ||
                 is_printable<T>::value)
        {
            return _print(os << "*", *p);
        }
        else
        {
            return os << "*" << std::hex << p;
        }
    }

    template <class T, size_t N>
    std::ostream &_print(std::ostream &os, const T (&v)[N])
    {
        os << "[" << N << "]{";

        for (size_t i = 0; i < N; i++)
        {
            if (i > 0)
            {
                os << ", ";
            }
            _print(os, v[i]);
        }

        return os << "}";
    }

    std::ostream &_print(std::ostream &os, const char *v) { return os << v; }
    std::ostream &_print(std::ostream &os, const std::string s) { return os << s; }
}

template <class T>
std::ostream &osprint(std::ostream &os, const T &v)
{
    return print_helper::_print(os, v);
}

template <class T0, class... Args>
std::ostream &osprint(std::ostream &os, const T0 &v0, Args &&...args)
{
    return osprint(osprint(os, v0) << " ", std::forward<Args>(args)...);
}

template <class... Args>
void println(Args &&...args)
{
    osprint(std::cout, std::forward<Args>(args)...) << std::endl;
}

#endif // PRINT_HPP
