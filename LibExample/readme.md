#LibClang usage
This file contains a brief introduction about the given test1.cpp file and LibClang usage.

##Source
Actually, this file comes from [this tutorial](http://swtv.kaist.ac.kr/courses/cs453-fall13/Clang%20tutorial%20v4.pdf).
Due to the rapid change of Clang API, I have made some changes to ensure it can work under the newest Clang 3.8.
BTW, this tutorial gives a nice introduction of basic Clang concepts, even though a little outdated.

##Function of the source file
The source file test1.cpp can walk through the program test.c's AST, print all the function declarations and statement class name(a little ugly).

##Usage(comand)
./test1 test.c