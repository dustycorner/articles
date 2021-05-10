#!/bin/bash

# Make sure all programs built
make

# File to hold the median times from each program
mediansfile="medians.txt"
if [ -f $mediansfile ]
then
    rm $mediansfile
fi

# Run each program 101 times and then grab the median value
numruns=101
lastidx=$((numruns-1))
mednum=$(($lastidx/2))
for i in *.opt
do
    echo "Running $i"
    timefile="$(basename $i .opt).times"
    if [ -f $timefile ]
    then
        rm $timefile
    fi
    j=0
    while [[ $j -lt ${numruns} ]]
    do
        ./$i 2>>$timefile >/dev/null
        (( j += 1 ))
    done
    sort -n $timefile >$timefile.tmp
    mv $timefile.tmp $timefile
    median=$(head -$mednum $timefile | tail -1)
    mode=$(sort $timefile | uniq -c | sort -n | tail -1 | sed -E -e 's/ *([0-9]+)+ +([0-9]+)/\2 (\1)/')
    echo "median=$median, mode=$mode : $i" >>$mediansfile
done

mediansfile="medians.noopt.txt"
if [ -f $mediansfile ]
then
    rm $mediansfile
fi

for i in *.noopt
do
    echo "Running no-opt $i"
    timefile=$i.times
    if [ -f $timefile ]
    then
        rm $timefile
    fi
    j=0
    while [[ $j -lt ${numruns} ]]
    do
        ./$i 2>>$timefile >/dev/null
        (( j += 1 ))
    done
    sort -n $timefile >$timefile.tmp
    mv $timefile.tmp $timefile
    median=$(head -$mednum $timefile | tail -1)
    mode=$(sort $timefile | uniq -c | sort -n | tail -1 | sed -E -e 's/ *([0-9]+)+ +([0-9]+)/\2 (\1)/')
    echo "median=$median, mode=$mode : $i" >>$mediansfile
done
