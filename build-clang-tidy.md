
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_LINKER=gold -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_PARALLEL_LINK_JOBS=1 .. 
make check-clang-tools -j $(nproc)


run test
./bin/clang-tidy -extra-arg=-std=c++17 --checks="-*,misc-auto-to-explicit-return-type" ../../clang-tools-extra/test/clang-tidy/misc-auto-to-explicit-return-type.cpp
