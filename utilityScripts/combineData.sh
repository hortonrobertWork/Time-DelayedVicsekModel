#!/bin/bash
for size in data2/observables/*; 
do for delay in $size/*;
    do file=$(echo $delay.txt | sed 's|/||g');
        cat $delay/* > $file; 
    done; 
done