#! /bin/bash

echo "\section{CódigoFuente}"

for path in $(find ./../../ -name '*.h') $(find ./../../ -name '*.cpp') 
do
    echo "\subsection*{"$(basename $path)"}"  	
    echo "\scriptsize\verbatiminput{$path}"
done;
