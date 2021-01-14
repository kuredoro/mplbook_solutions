#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <vector>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

int main()
{
    /* 1 */
    typedef mpl::lambda<mpl::lambda<_1>>::type t1;
    /*
     * lambda will take the placeholder expression inside it and turn it
     * into a metafunction class with apply template. apply<Arg>::type
     * will substitute the Arg into mpl::lambda<_1> and **will call
     * ::type on it**. And since the contents of the lambda are no
     * longer a placeholder expression, the type is returned
     * unchanged.
     */
    static_assert(
        boost::is_same<
            mpl::apply<t1, int>::type,
            int
        >::value
    );

    /* 2 */
    typedef mpl::apply<_1, mpl::plus<_1, _2>>::type t2;
    /*
     * mpl::plus placeholder expression will replace _1 placeholder.
     * So t2 == mpl::plus<_1, _2>
     */
    static_assert(
        mpl::apply<t2, mpl::int_<2>, mpl::int_<3>>::type::value == 5
    );
    
    /* 3 */
    typedef mpl::apply<_1, std::vector<int>>::type t3;
    /*
     * _1 will be replaced by vector<int>, so t3 is vector<int>
     */
    static_assert(
        boost::is_same<t3, std::vector<int>>::value
    );

    /* 4 */
    typedef mpl::apply<_1, std::vector<_1>>::type t4;
    /*
     * _1 will be literally replaced by vector<_1>. So t4 is vector<_1>
     * placeholder expression.
     */
    static_assert(
        boost::is_same<
            mpl::apply<t4, int>::type,
            std::vector<int>
        >::value
    );

    /* 5 */
    typedef mpl::apply<mpl::lambda<_1>, std::vector<int>>::type t5;
    /* Attempst 1
     * ----------
     * apply wil literally replace _1 with vector<int>, making
     * t5 = lambda<vector<int>>. t5::type is a metafunction class with
     * ::apply taking no arguments and returning vector<int>
     */
    // static_assert(
    //     boost::is_same<
    //         t5::type::template apply<>::type,
    //         std::vector<int>
    //     >::value
    // );

    /* Attempt 2
     * ---------
     * According to the documentation of mpl::apply, the first argument
     * is enclosed into a mpl::lambda<...>::type  and then this
     * metafunction class is passed to apply_wrap that works with the
     * classes only.
     * So, this is just like the first typedef!
     * lambda<_1>::apply<vector<int>> will substite _1 with
     * vector<int> giving lambda<vector<int>>. ::type on it will
     * return vector<int> unchanged!
     * So, t5 is just vector<int>
     */
    static_assert(
        boost::is_same<
            t5,
            std::vector<int>
        >::value
    );

    /* 6 */
    typedef mpl::apply<mpl::lambda<_1>, std::vector<_1>>::type t6;
    /*
     * apply will substitute _1 with vector<_1>, giving
     * lambda<vector<_1>>. ::type on it will give a metafunction
     * class that will accept one argument and will return vector
     * of this type. 
     *
     * In other words, ::apply will substitute _1 with
     * the argument and will call type on it. Since vector<_1>
     * doesn't have the ::type, it will be returned unchanged.
     */
    static_assert(
        boost::is_same<
            t6::apply<int>::type,
            std::vector<int>
        >::value
    );

    /* 7 */
    typedef mpl::apply<mpl::lambda<_1>, mpl::plus<_1, _2>>::type t7;
    /*
     * Equivalent to plus_f.
     */
    static_assert(
        mpl::apply<t7, mpl::int_<2>, mpl::int_<3>>::type::value == 5
    );

    /* 8 */
    typedef mpl::apply<_1, mpl::lambda<mpl::plus<_1, _2>>>::type t8;
    /*
     * _1 will take first argument and return it. Hence, t8 is
     * lambda<plus<_1, _2>>
     */
    static_assert(
        mpl::apply<t8::type, mpl::int_<2>, mpl::int_<3>>::type::value == 5
    );
    return 0;
}
