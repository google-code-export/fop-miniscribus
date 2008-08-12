/*******************************************************************************
 * class OO_Handler
 *******************************************************************************
 * Copyright (C) 2008 by Peter Hohl  Porting to QT4
  * e-Mail: ppkciz@gmail.com   www.qmake.net
 * Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>  KDE Okular
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/


#ifndef OO_HANDLER_H
#define OO_HANDLER_H

#include <QDomText>
#include <QDomElement>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include "quazip.h"
#include "quazipfile.h"
#include "oo_document.h"
#include "styleparser.h"
#include "styleinformation.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QtGui/QTextBrowser>

namespace OOO {




//
class OO_Handler : public QObject
{
     Q_OBJECT
//
public:
OO_Handler( const QString filedoc );
QString  text();
bool ParseFiles();
inline QTextDocument *document() { return mTextDocument; }
inline QMap<QString,QByteArray> imagelist() { return ooimage; }
inline QMap<QString,QByteArray> fileziplist() { return ooFile; }
protected:
   void OpenFile( const QString filedoc );
   
   bool havingdoc;
   QString log;
   QStringList filelist;
   QMap<QString,QByteArray> ooimage; 
   QMap<QString,QByteArray> ooFile; 
private:
   bool convertBody( const QDomElement &element );
   bool convertText( const QDomElement &element );
   bool convertHeader( QTextCursor *cursor, const QDomElement &element );
   bool convertParagraph( QTextCursor *cursor, const QDomElement &element, const QTextBlockFormat &format = QTextBlockFormat(), bool merge = false );
   bool convertTextNode( QTextCursor *cursor, const QDomText &element, const QTextCharFormat &format );
   bool convertSpan( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format );
   bool convertList( const QDomElement &element );
   bool convertTable( const QDomElement &element );
   bool convertFrame( const QDomElement &element );
   bool convertLink( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format );
   bool convertAnnotation( QTextCursor *cursor, const QDomElement &element );
    QTextDocument *mTextDocument;
    QTextCursor *mCursor;
    StyleInformation *mStyleInformation;
    void setError( QString msg );
signals:
   void errorFound(QString);
public slots:

};



}
//
#endif // OO_HANDLER_H

