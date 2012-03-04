LibLCTHW
========

This is the library that you finally create in my book Learn C The Hard Way found at:

http://c.learncodethehardway.org/

TODO
====

* Make tst_traverse abort if too deep.
* Have these be in a lcthw/*.h setup and that is installed.
* Add the RadixMap_sort_tail from Mongrel2 to study as an exercise.
* Look up whatever tail recursion is available in GCC or others.
* Don't implement a string lib, do a review of the key parts of bstrlib.


Algorithms Needed
=================

* binary search -- got in sarray.c and radixmap.c
* qsort -- used in sarray.c but maybe implement?
* heap -- don't have, could add
* radix sort -- have that, and in radixmap
* jenkin's hash -- need another one, djb's maybe?
* boyer-moore-horspool search -- bstrlib.c doesn't use this, so show how to make one
* longest common subsequence -- tst vs. sarray
* all common prefixes -- tst vs. sarray
* all substrings -- tst vs. sarray
* async file client/server -- ringbuffer, queue
* non-linear suffix tree delta -- that was cool, figure it out again
* XTEA encryption -- work on bstring, darray?
* timsort maybe?

