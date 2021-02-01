#include <boost/mpl/int.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/at.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class Seq, int Index>
using at_iter_t = 
    typename mpl::advance<
        typename mpl::begin<Seq>::type,
        mpl::int_<Index>
    >::type;

template <class Seq, int Index, class X>
using replace_at_t =
    typename mpl::insert<
        typename mpl::erase<
            Seq, 
            at_iter_t<Seq, Index>
        >::type,
        at_iter_t<
            typename mpl::erase<
                Seq, 
                at_iter_t<Seq, Index>
            >::type,
            Index
        >,
        X
    >::type;

/*
 * So this is really cool. It is as if I'm having a mutative state, even 
 * though I'm in pure functional world!
 * I can't have
 * for (int i = 0; i < N; i++) {
 *     // do smthing
 * }
 * in TMP world..... but I can!
 * Look!!!
 * template <int Index = 0, int Size>
 * struct foo : foo<Index + 1, Size>
 *
 * template <>
 * struct foo<Size, Size> : whatever
 *
 * So via inheritance I'm propagating and alter state. 
 * Look for the code below: additionally to Index and Size, I've attached
 * Seq and PastMiddle. I mutate Seq all the way until I'm finished with it
 * and then I just "return" it (inherit from it in the terminal case).
 *
 * NOTE:
 * You could optimize this code by removing Size and terminating recursion
 * after iterating over the first half of the vector instead of all of it.
 */

template <class Seq, int Index = 0, int Size = mpl::size<Seq>::type::value, bool PastMiddle = Index >= Size / 2>
struct double_first_half;

template <class Seq, int Index, int Size>
struct double_first_half<Seq, Index, Size, false>
    : double_first_half<
        replace_at_t<
            Seq,
            Index,
            typename mpl::multiplies<
                mpl::int_<2>, 
                typename mpl::at<Seq, mpl::int_<Index>>::type
            >::type
        >,
        Index + 1,
        Size>
{};

template <class Seq, int Index, int Size>
struct double_first_half<Seq, Index, Size, true>
    : double_first_half<
        Seq,
        Index + 1,
        Size>
{};

template <class Seq, int Size>
struct double_first_half<Seq, Size, Size, true>
    : Seq
{};

static_assert(
    mpl::equal<
        double_first_half<mpl::vector_c<int, 2, 3, 4, 5>>::type,
        mpl::vector_c<int, 4, 6, 4, 5>
    >(),
    ""
);

int main()
{
}
