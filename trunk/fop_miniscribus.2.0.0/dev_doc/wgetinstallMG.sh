#! /bin/sh -e


######## http://heanet.dl.sourceforge.net/sourceforge/mingw/wget-1.9.1-mingwPORT.tar.bz2

tar -xvjf wget-1.9.1-mingwPORT.tar.bz2
rm wget-1.9.1-mingwPORT.tar.bz2
mkdir /usr/src
mkdir /usr/local
mkdir /usr/local/bin
cd wget-1.9.1/mingwPORT
cp wget.exe /usr/local/bin
./mingwPORT.sh
cd
rm -rf wget-1.9.1