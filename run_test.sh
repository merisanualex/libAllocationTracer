#!/bin/bash

# gdb -ex run --eval-command=quit --args env
# LD_PRELOAD="./libAllocationTracer.so /usr/lib64/libjemalloc.so.2" ./test/base/BaseTest
LD_PRELOAD="./libAllocationTracer.so /usr/lib64/libjemalloc.so.2" ./test/stl/STLTest
