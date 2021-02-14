> Because `std::for_each` is the most basic algorithm in the standard library, you may be wondering why we didn't say anything about its compile time counterpart. The fact is that unlike, for example, transform, the algorithm deos not have a pure compile time counterpart. Can you offer an explanation for that fact?

`std::for_each` mutates the elements of a sequence in-place. This is not possible to do in compile time, because compile-time data is immutable. `mpl::transform` is the closest in terms of semantics algorithm to the `std::for_each`.
