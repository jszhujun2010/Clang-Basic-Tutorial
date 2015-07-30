# This works for both binary installed and source installed
# It seems that clang++ has some problem, so g++ is used.
# Note that I add -std=c++11 option here to support smart pointer
CXX := g++
CXXFLAGS := -fno-rtti -O0 -g -std=c++11

# Your own LLVM build path
LLVM_DIR := /home/workspace/Tools/build

# LLVM C++ options and LLVM link options
LLVM_CXXFLAGS := `${LLVM_DIR}/bin/llvm-config --cxxflags`
LLVMLDFLAGS := `${LLVM_DIR}/bin/llvm-config --ldflags --libs --system-libs`

SOURCES = test1.cpp 

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
				-lclangTooling\
				-lclangFrontendTool\
				-lclangFrontend\
				-lclangDriver\
				-lclangSerialization\
				-lclangCodeGen\
				-lclangParse\
				-lclangSema\
				-lclangStaticAnalyzerFrontend\
				-lclangStaticAnalyzerCheckers\
				-lclangStaticAnalyzerCore\
				-lclangAnalysis\
				-lclangARCMigrate\
				-lclangRewriteFrontend\
				-lclangRewrite\
				-lclangEdit\
				-lclangAST\
				-lclangLex\
				-lclangBasic\
				$(shell $(LLVMCONFIG) --libs)\
				$(shell $(LLVMCONFIG) --system-libs)\
                -lcurses

all: $(OBJECTS) $(EXES)

#Not obvious, see my blog
%: %.o
	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

#make clean manually
clean:
	-rm -f $(EXES) $(OBJECTS) *~
