# GCOMP - a compression stack written from scratch

I've decided to write a compression library and only attempt to use the C++ standard library. In the past I've attempted a similar model using Keras, but what is the fun in using someone else's (way better) statistical model?

Currently the model is just an arithmetic compressor and a place holder at that. The placeholder is leftover from learning how to write a compressor for [This article on Dr. Dobbs](http://www.drdobbs.com/cpp/data-compression-with-arithmetic-encodin/240169251)

This model is basically going to be a worse version of [PAQ](https://en.wikipedia.org/wiki/PAQ)

So far the compression stack plan is:

* Arithmetic Coding as the base

* Predictive algorithms:

  * Hidden Markov Model (trained on input stream)
  
  * LSTMs trained on n grams dataset at byte and word level 
  
  * BD-LSTMs trained on input stream at byte and word level
  
  * character frequencies in file
  
  * character frequencies for english language
 
  * consider using sentiment analysis (im not sure the effects this would have. does the emotion protrayed in a sentence allow you to better predict the types of descriptors an author might use when a character is speaking for example?)
 
* Use a context mixing neural network to combine these into a single prediction (another choice is random forest).

If we want to get even crazier we could attempt to use a dictionary mapping of common words that have a semantically or syntatically similar words to allow us to reuse bytes (similar to a trie). 

Some benchmarks to compare against one day:
1. https://www.maximumcompression.com/
2. https://en.wikipedia.org/wiki/Calgary_corpus
