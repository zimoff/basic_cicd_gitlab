#!/bin/bash
f=0
s=0
t=0
prog=./s21_cat
echo ">>> s21cat test: single key, single file <<<"
for filename in test_tabs.txt test_emptyline.txt test_chars.txt
do
    echo ___
    echo testing by file: $filename
    for key in "-b" "-e" "-n" "-s" "-v" "-t" "-bns" "-ebn" "-T" "-E" "--number" "--squeeze-blank" "--number-nonblank" 
    do
        cat $key $filename > tcat.txt
        $prog $key $filename > t21.txt

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


echo
echo ">>> s21cat test: multi key, single file <<<"
for filename in test_tabs.txt test_emptyline.txt test_chars.txt
    do
        echo ___
        echo testing by file: $filename
        for shortkey in "-b" "-e" "-n" "-s" "-v" "-t" "-bns" "-ebn" "-T" "-E"  
        do
            for longkey in "--number" "--squeeze-blank" "--number-nonblank"
                do

                cat $shortkey $longkey $filename > tcat.txt
                $prog $shortkey $longkey $filename > t21.txt

                if cmp t21.txt tcat.txt
                then
                    #echo comprasion SUCCESS by $shortkey $longkey
                    s=$[ $s + 1 ]
                    t=$[ $t + 1 ]
                else
                    echo comprasion FAILED by $shortkey $longkey
                    f=$[ $f + 1 ]
                    t=$[ $t + 1 ]
                fi
            done
        done
    done

echo
echo ">>> s21cat test: single key, multi file <<<"

multfile="test_tabs.txt test_emptyline.txt test_chars.txt"



echo ___
echo testing by file: $multfile 
for key in "-b" "-e" "-n" "-s" "-v" "-t" "-bns" "-ebn" "-T" "-E" "--number" "--squeeze-blank" "--number-nonblank" 
do
    cat $key $multfile > tcat.txt
    $prog $key $multfile > t21.txt

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

echo ____________
echo all tests: $t
echo failed tests: $f
echo success tests: $s

if [ $f -gt 0 ]
then
    echo 'fail'
    exit 1
fi
