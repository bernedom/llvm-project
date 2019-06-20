
# clang-tidy check: `auto` to explicit return value 

This fork of the llvm project contains a custom clang tidy check that converts functions declared to return `auto` to return the explicit type, if this the type does not depend on any template arguments supplied. 

## running the check 

```
./llvm/build/bin/clang-tidy -extra-arg=-std=c++17 --header-filter=.* --checks="-*,misc-auto-to-explicit-return-type" <sourcefile.h> -- --std=c++17 
```


to apply the fixed

```
./llvm/build/bin/clang-tidy -extra-arg=-std=c++17 --header-filter=.* --checks="-*,misc-auto-to-explicit-return-type" <sourcefile.h> -fix -- --std=c++17 
```

## building the check

```
cd llvm
mkdir build 
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_LINKER=gold -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_PARALLEL_LINK_JOBS=1 .. 
make check-clang-tools -j $(nproc)
```



## LLVM Compiler Infrastructure

This directory and its subdirectories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and runtime environments.
