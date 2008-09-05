#! /bin/sh -e


mkdir -p /C/Programming/libs/src
mkdir -p /C/Programming/libs/include
mkdir -p /C/Programming/libs/lib


cd /C/Programming/libs/src
wget http://www.zlib.net/zlib-1.2.3.tar.gz
tar xzf zlib-1.2.3.tar.gz
rm zlib-1.2.3.tar.gz
cd zlib-1.2.3
./configure --prefix=$PWD/../../
make
make install



cd /C/Programming/libs/src


wget http://ftp.gnu.org/gnu/libiconv/libiconv-1.11.tar.gz
tar xzf libiconv-1.11.tar.gz
rm libiconv-1.11.tar.gz
cd libiconv-1.11
./configure --enable-static --disable-shared --prefix=$PWD/../../
make
make install

cd /C/Programming/libs/src


wget http://www.exiv2.org/exiv2-0.12.tar.gz
tar xzf exiv2-0.12.tar.gz
rm exiv2-0.12.tar.gz
cd exiv2-0.12
./configure --enable-static --disable-shared --prefix=$PWD/../../ \
CXXFLAGS="${CXXFLAGS} -DSUPPRESS_WARNINGS" --with-zlib=$PWD/../../
make
make install



cd /C/Programming/libs/src

wget ftp://xmlsoft.org/libxml2/libxml2-2.6.26.tar.gz
tar xzf libxml2-2.6.26.tar.gz
rm libxml2-2.6.26.tar.gz
cd libxml2-2.6.26
./configure --enable-static --disable-shared --with-iconv=$PWD/../../ --prefix=$PWD/../../
make
make install

cd /C/Programming/libs/src

wget ftp://xmlsoft.org/libxslt/libxslt-1.1.17.tar.gz
tar xzf libxslt-1.1.17.tar.gz
rm libxslt-1.1.17.tar.gz
cd libxslt-1.1.17
./configure --enable-static --disable-shared --with-libxml-prefix=$PWD/../../ --prefix=$PWD/../../
make
make install


cd /C/Programming/libs/src

