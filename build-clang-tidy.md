
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_LINKER=gold -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_PARALLEL_LINK_JOBS=1 .. 
make check-clang-tools -j $(nproc)


run test
./bin/clang-tidy -extra-arg=-std=c++17 --checks="-*,misc-auto-to-explicit-return-type" ../../clang-tools-extra/test/clang-tidy/misc-auto-to-explicit-return-type.cpp


clang-check -ast-dump -extra-arg=-std=c++17 ../../clang-tools-extra/test/clang-tidy/misc-auto-to-explicit-return-type.cpp -- > ../../ast-dump.txt
clang-query -extra-arg=-std=c++17 ../../clang-tools-extra/test/clang-tidy/misc-auto-to-explicit-return-type.cpp --

set bind-root false	
match functionDecl(isConstexpr()).bind("x")
match functionDecl(isConstexpr(), unless(isImplicit()), returns(autoType())).bind("x")
match cxxStaticCastExpr(hasSourceExpression(functionDecl(isConstexpr(), unless(isImplicit()), returns(autoType())))).bind("cast")

unless --> not

https://devblogs.microsoft.com/cppblog/exploring-clang-tooling-part-2-examining-the-clang-ast-with-clang-query/


~/Code/llvm-project/llvm/build/bin/clang-tidy -extra-arg=-std=c++17 --header-filter=.* --checks="-*,misc-auto-to-explicit-return-type" ./main.cc -- --std=c++17 --