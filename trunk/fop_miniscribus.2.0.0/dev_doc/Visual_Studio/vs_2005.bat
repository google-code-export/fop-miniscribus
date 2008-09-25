@echo off
REM to run after VS command line
REM Miniscrubus Visual Studio build need libxslt  libxml2 only before QT version 4.5 > after is included
REM QT XSLT http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/
REM http://swik.net/Trolltech/Trolltech+Labs+Blogs/XSL-T+and+Qt/ce11z  info
REM get http://www.winkde.org/pub/kde/ports/win32/releases/unstable/4.0.83/libxslt-1.1.23-1-lib.tar.bz2
REM get http://www.winkde.org/pub/kde/ports/win32/releases/unstable/4.0.83/libxml2-2.6.32-1-lib.tar.bz2
SET QTDIR=C:\dev\qt_4.4.1vs
REM  QTDIR Place
CALL "C:\dev\vs\VC\vcvarsall.bat" x86
REM SDK and external libs
SET INCLUDE=C:\dev\ms_sdk\Include;C:\dev\externallib\include;%INCLUDE%
SET LIB=C:\dev\ms_sdk\Lib;C:\dev\externallib\lib;%LIB%
SET PATH=%QTDIR%\bin;%QTDIR%\lib;%PATH%


