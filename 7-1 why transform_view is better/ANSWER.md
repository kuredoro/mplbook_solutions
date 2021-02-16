> Our implementation of `zip_iterator` uses `transform` to generate its nested `::type`, but the one in MPL uses `transform_view` instead. What advantage does theMPL approach have?

```cpp
template <class IteratorSeq>
struct zip_iterator
{
    using category = mpl::forward_iterator_tag;

    using type =
        typename mpl::transform<
            IteratorSeq,
            mpl::deref<_>
        >::type;
};
```
vs.
```cpp
template <class IteratorSeq>
struct zip_iterator
{
    using category = mpl::forward_iterator_tag;

    using type =
        mpl::transform_view
            IteratorSeq,
            mpl::deref<_>
        >;
};
```

With `mlp::transform` we eagerly do `mpl::size<IteratorSeq>::type` transformations (template instantiations) at least, but the user of the `zip_view` might not want to use all of the iterators. In this case the library would waste template instantiations on nothing. Basically, we with `transform_view` the potential user's code will do exactly as much template instantiations as it needs. No more no less.

*Althogh, if you have a practical example, where you ***don't*** need all of the iterators, it would be great. Be sure to make an issue on github, if you came up with one, I would like to hear it (or if you wanna just talk with me, that's fine too, cuz I want somebody to talk with me :P)!*
