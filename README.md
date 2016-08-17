# GCOMP - a compression stack written from scratch

So far the compression stack plan is:

* Run Length Encoding

* Burrows-Wheeler Transform

* Move to Front Algorithm

* Run Length Encoding

* Arithmetic Coding


Currently only the arithmetic coding is working. I plan on updating it to use markov chains and change the encoding to be predicitive. Currently the arithmetic coding is geting a 55% compression rate on the test file. This is significantly worse than using the built in compression on the file system.
