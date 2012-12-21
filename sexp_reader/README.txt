Background
==================
http://weibo.com/1929644930/zaKmuoRRJ
@马少平THU:
设计一个数据结构，存储类似这样的表：(a 123 (3.14 pi 5) d e 12 5.6), (2 3 (a b c (5 6 (b)) 9 9 2.7)，也就是说表中的元素数据类型不定，表的元素数和深度也不定。通过键盘输入建立这样的表。


I wrote a s-exp reader prototype, partially because I'll need the similar functionality in a personal project, partically because it is just fun.  
Do not support garbage collection yet

Compile
===================
$ make

The unit test depends on google's test framework. Makefile needs to be changed to for its library path.
$ make test

Run
===================
$ ./readsexp
> (a 123 (3.14 pi 5) d e 12 5.6)
value:
(a 123 (3.140000 pi 5) d e 12 5.600000)
> (2 3 (a b c (5 6 (b)) 9 9 2.7))
value:
(2 3 (a b c (5 6 (b)) 9 9 2.700000))
> 



