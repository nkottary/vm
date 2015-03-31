#!/bin/bash

rm *.vmc

declare -a fnames=("echo.vm" "hw.vm" "loop.vm" "odd_or_even.vm" "prime.vm" "max.vm")
declare -a inputs=("123" "" "" "32" "31" "")
declare -a outputs=($'123' $'\nHELLO WORLD!' $'1, 2, 3, 4, 5, 6, 7, 8, 9, 10, ' $'Even' $'prime' $'800')

#compilation
for fname in "${fnames[@]}"
do
    ./compiler_dbg $fname
    if [ ! -f $fname"c" ]; then
        echo "$fname not compiled"
        exit 0
    fi
done

echo "--------------------------------------------------"
echo "              Compilation Success!"
echo "--------------------------------------------------"

#IO test
for i in "${!fnames[@]}"
do
    output=`echo "${inputs[$i]}" | ./vm_dbg "${fnames[$i]}""c"`
    if [ "$output" != "${outputs[$i]}" ]; then
        echo "Test failed for ${fnames[$i]}"
        echo "Expected: ${outputs[$i]}"
        echo "Real: $output"
        exit 0
    fi
done

echo "--------------------------------------------------"
echo "                  Test Success!"
echo "--------------------------------------------------"
