#! /usr/bin/env bash

diff ~nimrodav/lint/.clang-format ./.clang-format
diff ~nimrodav/lint/.clang-tidy ./.clang-tidy

CLANG_TIDY=~nimrodav/clang+llvm-3.9.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang-tidy

CLANG_FORMAT=~nimrodav/clang+llvm-3.9.0-x86_64-linux-gnu-ubuntu-14.04/bin/clang-format 
#*.h
for f in *.c *.h ; do
  $CLANG_FORMAT $f -i
done
#
$CLANG_TIDY *.h *.c -- |& grep -v "warning generated" | grep -v "Suppressed"
