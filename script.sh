#!/usr/bin/env bash

fff_len=`cat fff.h | wc -l`
# test/fff_test_c.c
gcc -E test/fff_test_c.c > out
comm -13 <(sort < fff.h | uniq) <(sort < out | uniq) > unique_to_out      # strip out fff.h include
awk 'FNR==NR{a[$1];next}($1 in a){print}'  unique_to_out fff.h > matched  # find unique matches
grep -Fxf  <(sort < matched | uniq) <(sort < fff.h | uniq) > coverage     # find coverage of matches in fff.h
cov_len=`cat coverage | wc -l`
echo "test/fff_test_c.c coverage:"
echo "$cov_len / $fff_len" | bc  -l

# test/fff_test_cpp.cpp
gcc -E test/fff_test_cpp.cpp -I . > out
comm -13 <(sort < fff.h | uniq) <(sort < out | uniq) > unique_to_out
awk 'FNR==NR{a[$1];next}($1 in a){print}'  unique_to_out fff.h > matched
grep -Fxf  <(sort < matched | uniq) <(sort < fff.h | uniq) > coverage
cov_len_2=`cat coverage | wc -l`
echo "test/fff_test_cpp.cpp coverage:"
echo "$cov_len_2 / $fff_len" | bc  -l

# total coverage
echo "TOTAL COVERAGE:"
echo "($cov_len + $cov_len_2) / ($fff_len * 2)" | bc -l
