#!/usr/bin/bash
echo 'deploy!!!!!!!!'
#scp ./src/cat/s21_cat desirema@192.168.100.10:~/
#scp ./src/grep/s21_grep desirema@192.168.100.10:~/
scp ./cat/s21_cat desirema@192.168.100.10:~/
scp ./grep/s21_grep desirema@192.168.100.10:~/
ssh desirema@192.168.100.10 "sudo mv s21_cat s21_grep /usr/local/bin"
