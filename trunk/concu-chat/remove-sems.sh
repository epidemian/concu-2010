#! /bin/bash

semids=$(ipcs -s | sed -n 's/ \+/ /pg' | grep '.* .* .* 666' | cut -d ' ' -f 2)
for id in $semids
do
    echo "Removing semaphore $id"
    ipcrm -s $id
done;

