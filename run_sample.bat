@echo off
cd /d D:\C++
echo 4 > sample.txt
echo 100 200 300 400 >> sample.txt
echo 3 >> sample.txt
echo 50 50 50 50 >> sample.txt
echo 200 300 200 300 >> sample.txt
echo 900 150 389 399 >> sample.txt
做题草稿.exe < sample.txt
del sample.txt
