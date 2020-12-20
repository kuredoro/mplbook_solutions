#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <cstdio>

using namespace boost;

/*
 * IsMatch is used to resolve partial specialization ambiguity. 
 * The termination case is only specialized if IsMatch true, and all type traversal specializations, for when it's false.
 * Consider there to be no IsMatch
 * The second test case from the book uncovers the bug: 
 *           replace_type<int const*[10], int const, long>
 * Eventually, we'll reach replace_type<int const, int const, long>
 * But now we have 2 possible specs! 
 * The one that replaces type and the one that decomposes it.
 * Specializing with IsMatch solves this problem
 */

template <class T, 
         class OldType, 
         class NewType, 
         class IsMatch = typename is_same<T, OldType>::type
         >
struct replace_type
{
    typedef T type;
};

// Termination case
// ----------------

template <class OldType, class NewType>
struct replace_type<OldType, OldType, NewType, true_type>
{
    typedef NewType type;
};

// Type tree traversal
// -------------------

template <class T, class OldType, class NewType>
struct replace_type<T*, OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type* type;
};

template <class T, class OldType, class NewType>
struct replace_type<T[], OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type type[];
};

template <class T, size_t Size, class OldType, class NewType>
struct replace_type<T[Size], OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type type[Size];
};

template <class T, class OldType, class NewType>
struct replace_type<T const, OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type const type;
};

template <class T, class OldType, class NewType>
struct replace_type<T volatile, OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type volatile type;
};

template <class T, class OldType, class NewType>
struct replace_type<T&, OldType, NewType, false_type>
{
    typedef typename replace_type<T, OldType, NewType>::type& type;
};

template <class Ret, class OldType, class NewType>
struct replace_type<Ret(), OldType, NewType, false_type>
{
    typedef typename replace_type<Ret, OldType, NewType>::type type();
};

template <class Ret, class ...Args, class OldType, class NewType>
struct replace_type<Ret(Args...), OldType, NewType, false_type>
{
    typedef typename replace_type<Ret, OldType, NewType>::type type(typename replace_type<Args, OldType, NewType>::type...);
};



int main()
{
    static_assert(is_same<replace_type<void, void, int>::type, int>::value);
    static_assert(is_same<replace_type<void*, void, int>::type, int*>::value);
    static_assert(is_same<replace_type<int[], int, char>::type, char[]>::value);
    static_assert(is_same<replace_type<int[2], int, char>::type, char[2]>::value);
    static_assert(is_same<replace_type<int const, int, char>::type, char const>::value);
    static_assert(is_same<replace_type<int volatile, int, char>::type, char volatile>::value);
    static_assert(is_same<replace_type<int&, int, char>::type, char&>::value);
    static_assert(is_same<replace_type<int(), int, char>::type, char()>::value);
    static_assert(is_same<replace_type<int(int), int, char>::type, char(char)>::value);
    static_assert(is_same<replace_type<int(int, char, int, char), int, char>::type, char(char, char, char, char)>::value);

    static_assert(is_same<replace_type<int const*[10], int const, long>::type, long*[10]>::value);
    static_assert(is_same<replace_type<char& (*)(char&), char&, long&>::type, long& (*)(long&)>::value);
    return 0;
}
