#include <type_traits>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace mpl = boost::mpl;

// Dunno what the exercise wanted here...
template <class T>
using is_data_member_pointer =
    std::is_member_object_pointer<T>;

struct test
{
    int a;
    int* b;
};

static_assert(is_data_member_pointer<int test::*>::value);
static_assert(is_data_member_pointer<int* test::*>::value);
static_assert(!is_data_member_pointer<int test::**>::value);
static_assert(is_data_member_pointer<double test::*>::value);


template <class T>
using is_pointer_to_function = typename mpl::and_<std::is_pointer<T>, std::is_function<std::remove_pointer_t<T>>>::type;

static_assert(
    is_pointer_to_function<void(*)(int)>::value
);

static_assert(
    !is_pointer_to_function<void(int)>::value
);

static_assert(
    !is_pointer_to_function<int*>::value
);


template <class T>
using is_reference_to_function_pointer = 
    typename mpl::and_<
        std::is_reference<T>,
        is_pointer_to_function<std::remove_reference_t<T>>
    >::type;

static_assert(
    is_reference_to_function_pointer<void(*&)(int)>::value
);

static_assert(
    is_reference_to_function_pointer<void(*&&)(int)>::value
);

static_assert(
    !is_reference_to_function_pointer<void(*)(int)>::value
);

static_assert(
    !is_reference_to_function_pointer<void(&)(int)>::value
);


template <class T>
using is_reference_to_non_const =
    typename mpl::and_<
        std::is_reference<T>,
        mpl::not_<
            std::is_const<std::remove_reference_t<T>>
        >
    >::type;

static_assert(
    !is_reference_to_non_const<int>::value
);

static_assert(
    is_reference_to_non_const<int&>::value
);

static_assert(
    !is_reference_to_non_const<int const &>::value
);

static_assert(
    is_reference_to_non_const<int&&>::value
);

static_assert(
    !is_reference_to_non_const<int const &&>::value
);

static_assert(
    is_reference_to_non_const<void(&)(int)>::value
);

int main()
{
}
