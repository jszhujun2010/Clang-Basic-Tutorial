#Clang basic usage
This repository follow my [blog](http://jszhujun2010.farbox.com), which gives an introduction of Clang basics which includes Clang library(LibClang), LibTooling and Plugin(namely corresponding to folder LibExample, LibToolingExample and PluginExample). It is inspired by [this blog](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-i-introduction/), and I have modified some codes to be suitable for the newest Clang 3.8.

##What changes most?
Smart pointer in C++11 is becoming more and more popular because of its safety, and Clang seems to have changed all possible raw pointers to smart pointers since Clang 3.4. So, I make some changes in pointer types. Also, due to the rapid change of Clang API, I have changed some other codes. Finally, makefile has been modified for general purpose use.

##Usage
See details in readme.md in each subdirectory.
