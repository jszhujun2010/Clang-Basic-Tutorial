# This works for only for source installed only.
# It seems that clang++ has some problem, so g++ is used.
# Note that I add -std=c++11 option here to support smart pointer.
CXX := g++
CXXFLAGS := -fno-rtti -O0 -g -std=c++11
PLUGIN_CXXFLAGS := -fpic

# Your LLVM source directory and build directory
LLVM_SRC_DIR := /home/workspace/Tools/llvm
LLVM_DIR := /home/workspace/Tools/build

# This is why we need source installed LLVM
CLANG_INCLUDES := \
		-I$(LLVM_SRC_DIR)/tools/clang/include \
		-I$(LLVM_DIR)/tools/clang/include

# LLVM C++ options and LLVM link options
LLVM_CXXFLAGS := `${LLVM_DIR}/bin/llvm-config --cxxflags`
LLVMLDFLAGS := `${LLVM_DIR}/bin/llvm-config --ldflags --libs --system-libs`

# Plugins shouldn't link LLVM and Clang libs statically, because they are
# already linked into the main executable (opt or clang). LLVM doesn't like its
# libs to be linked more than once because it uses globals for configuration
# and plugin registration, and these trample over each other.
LLVMLDFLAGS_NOLIBS := `${LLVM_DIR}/bin/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared


SOURCES = PluginExample.cpp 

SOLIB = $(SOURCES:.cpp=.so)


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


.PHONY: all
all: $(SOLIB)

# $@ stands for object file, $< is the first dependence source file(.cpp)
%.so: %.cpp
	$(CXX) $(PLUGIN_CXXFLAGS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -o $@ $< $(PLUGIN_LDFLAGS) $(LLVMLDFLAGS_NOLIBS)

# make clean manually
.PHONY: clean
clean:
	-rm -f *.so
