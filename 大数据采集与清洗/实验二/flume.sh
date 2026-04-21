#! /bin/bash

function listenFromAvro(){
    `flume-ng agent -c /usr/local/flume/conf -f /usr/local/flume/conf/avro.conf -n a1 -Dflume.root.logger=INFO,console`
}

function listenFromNetcat(){
    `flume-ng agent -c /usr/local/flume/conf/ -f /usr/local/flume/conf/example.conf --n a1 -Dflume.root.logger=INFO,console`
}

function listenFromDir(){
    echo -e "\n\n"
    read -p "Please Input the Directory Path: " dirPath

    if [ ! -d $dirPath ];
    then
        echo "The Directory Does Not Exist!"
        exit 1
    fi

    sed -i "s:a1.sources.r1.spoolDir =.*:a1.sources.r1.spoolDir = $dirPath:g" /usr/local/flume/conf/dir.conf

    `flume-ng agent -c /usr/local/flume/conf -f /usr/local/flume/conf/dir.conf -n a1 -Dflume.root.logger=INFO,console`
}

function listenFromFile(){
    echo -e "\n\n"
    read -p "Please Input the File Path: " filePath

    if [ ! -e $filePath ];
    then
        echo "The File Does Not Exist!"
        exit 1
    fi

    sed -i "s:a1.sources.r1.command = tail -F.*:a1.sources.r1.command = tail -F $filePath:g" /usr/local/flume/conf/data.conf

    `flume-ng agent -c /usr/local/flume/conf -f /usr/local/flume/conf/data.conf -n a1 -Dflume.root.logger=INFO,console`
}

function writeToHDFS(){
    `flume-ng agent -c /usr/local/flume/conf -f /usr/local/flume/conf/hdfs.conf -n a1 -Dflume.root.logger=INFO,console`
}

echo "-------------------------------"
echo "Welcome to This System"
echo "Input 1~5 For:"
echo -e "\t1: Listen From AvroSource"
echo -e "\t2: Listen From NetcatSource"
echo -e "\t3: Listen From Directory"
echo -e "\t4: Listen From File"
echo -e "\t5: Write To HDFS"
echo "-------------------------------"
read -p "Please Enter Your Choice: " choice

case "${choice}" in
    "1")
        listenFromAvro
        ;;
    "2")
        listenFromNetcat
        ;;
    "3")
        listenFromDir
        ;;
    "4")
        listenFromFile
        ;;
    "5")
        writeToHDFS
        ;;
    *)
        echo "Your Input Is Invalid!"
        ;;
esac
