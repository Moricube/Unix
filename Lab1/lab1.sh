#!/bin/sh
trap cleanup 1 2 3 6

cleanup() {
    echo "Сработал clenup"
    if [ $tempPath != "" ]; then
        rm -d tempPath
        echo Temp directory by path $tempPath was removed
    fi
}

string="Вывести мне вот сюда Output: helloworld.cpp пожалуйста"

output="Output:"

count=0
findOutput=false
fileName=""
tempPath=""

for i in $string; do
    count=$((count+1))

    if [ $findOutput = true ]; then
        fileName=$i
        findOutput=false
    fi

    if [ $i = $output ]; then
        findOutput=true        
    fi

done

if [ $fileName != "" ]; then
    echo Find Output: fileName $fileName
fi

tempPath=$(mktemp -d)

cp $fileName $tempPath

tempPathFile=$tempPath
tempPathFile=$tempPathFile/$fileName

echo tempPathFile: $tempPathFile

g++ -o $tempPath/$fileName.exe $tempPathFile

gResult=$?

cp $tempPath/$fileName.exe $PWD

rm -d -Rf $tempPath

echo Temp directory by path $tempPath was removed

echo g++ еxit code: $gResult
