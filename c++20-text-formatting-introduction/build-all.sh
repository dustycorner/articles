#!/bin/bash

if [ ! -d prgs ]
then
    mkdir prgs
fi

function buildit
{
    NAME=$1
    cppfile=code/${NAME}.cpp
    prgfile=prgs/${NAME}
    outfile=output/${NAME}.out
    if [ ! -f $prgfile ]
    then
        prgtime="0"
    else
        prgtime=$(stat -c "%Y" $prgfile)
    fi
    cpptime=$(stat -c "%Y" $cppfile)
    if [ $cpptime -gt $prgtime ]
    then
        echo "Building $NAME"
        g++ --std=c++17 $cppfile -lfmt -o $prgfile
        prgtime=$(stat -c "%Y" $prgfile)
    else
        echo "$NAME up-to-date"
    fi
    if [ ! -f $outfile ]
    then
        outtime="0"
    else
        outtime=$(stat -c "%Y" $outfile)
    fi
    if [ $prgtime -gt $outtime ]
    then
        echo "Creating $outfile"
        $prgfile >$outfile
    fi
}

buildit align-fill
buildit bad-format
buildit basic-formatting
buildit bool-format
buildit char-format
buildit float-format
buildit float-inf-nan
buildit formatted_size
buildit format_to
buildit format_to_n
buildit integer-format
buildit locale
buildit pointer-format
buildit printf-vs-format
buildit repeated-param
buildit sign-zero
buildit string-format
buildit unused-args
buildit vlog
buildit widths
