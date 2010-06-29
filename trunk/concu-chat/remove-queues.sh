#! /bin/bash

qids=$(ipcs -q | sed -n 's/ \+/ /pg' | grep '.* .* .* 644' | cut -d ' ' -f 2)
for id in $qids
do
    echo "Removing queue $id"
    ipcrm -q $id
done;

rm debug/queues/*.queue
