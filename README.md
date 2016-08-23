# GCOMP - a compression stack written from scratch

So far the compression stack plan is:

* Burrows-Wheeler Transform

* Move to Front Algorithm

* Run Length Encoding

* Adaptive Arithmetic Coding (with predictive modeling)

Currently I only have the arithmetic coding algorithm working and I'm getting about a 55% compression ratio on text files. Hoping to increase the compression ratio with the transform and better predictive modeling. 
