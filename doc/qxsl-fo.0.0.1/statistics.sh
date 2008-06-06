#!/bin/sh

cppcount=`find ./* -name "*.cpp" | wc -l`
echo "count of *.cpp files :$cppcount"