#!/bin/bash

rm *.vmc

declare -a  fnames=("echo.vm" "hw.vm"           "loop.vm"                          "odd_or_even.vm" "odd_or_even.vm" "prime.vm" "prime.vm"   "max.vm")
declare -a  inputs=("123"     ""                ""                                 "32"             "33"             "31"       "32"         "")
declare -a outputs=($'123'    $'\nHELLO WORLD!' $'1, 2, 3, 4, 5, 6, 7, 8, 9, 10, ' $'Even'          $'Odd'           $'prime'   $'not prime' $'800')

#compilation
for fname in "${fnames[@]}"
do
    ./compiler_dbg $fname
    if [ $? -ne 0 ]; then
        echo "\n$fname not compiled."
        exit -1
    fi
done

echo "--------------------------------------------------"
echo "              Compilation Success!"
echo "--------------------------------------------------"

#IO test
for i in "${!fnames[@]}"
do
    output=`echo "${inputs[$i]}" | ./vm_dbg "${fnames[$i]}""c"`
    if [ $? -ne 0 ]; then
        echo "\nVM error for ${fnames[$i]}."
        exit -1
    fi
    if [ "$output" != "${outputs[$i]}" ]; then
        echo "\nTest failed for ${fnames[$i]}"
        echo "\nExpected: ${outputs[$i]}"
        echo "\nReal: $output"
        exit -1
    fi
done

echo "--------------------------------------------------"
echo "                  Test Success!"
echo "--------------------------------------------------"
exit 0
