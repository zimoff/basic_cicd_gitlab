#!/bin/bash
f=0
s=0
t=0
pattern=abc
my_prog=./s21_grep
test_prog=grep
echo ">>> s21grep test: single key, single file <<<"
for filename in test_abc.txt test_elit.txt
do
    echo ___
    echo testing by file: $filename
    for key in  "-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" 
    do
        $test_prog $key $pattern $filename > tcat.txt
        $my_prog $key $pattern $filename > t21.txt

        if cmp t21.txt tcat.txt
        then
            #echo comprasion SUCCESS by $key
            s=$[ $s + 1 ]
            t=$[ $t + 1 ]
        else
            echo comprasion FAILED by $key
            f=$[ $f + 1 ]
            t=$[ $t + 1 ]
        fi
    done
done


echo ______________________________________
echo ">>> s21grep test: two key, single file <<<"
for filename in test_abc.txt test_elit.txt
do
    echo ___
    echo testing by file: $filename
    for key1 in   "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" 
    do
        for key2 in   "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" 
        do
            $test_prog $key1 $key2 $pattern $filename > tcat.txt
            $my_prog $key1 $key2 $pattern $filename > t21.txt

            if cmp t21.txt tcat.txt
            then
                #echo comprasion SUCCESS by $key
                s=$[ $s + 1 ]
                t=$[ $t + 1 ]
            else
                echo comprasion FAILED by $key1 $key2 file: $filename
                echo command: $test_prog $key1 $key2 $pattern $filename 
                f=$[ $f + 1 ]
                t=$[ $t + 1 ]
            fi
        done
    done
done
echo ____________
echo all tests: $t
echo failed tests: $f
echo success tests: $s

if [ $f -gt 10 ]
then
    echo 'fail'
    exit 1
fi
