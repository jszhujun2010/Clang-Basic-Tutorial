#!/bin/bash
clang -Xclang -load \
      -Xclang /home/workspace/MyProject_test/PluginExample/PluginExample.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.c
