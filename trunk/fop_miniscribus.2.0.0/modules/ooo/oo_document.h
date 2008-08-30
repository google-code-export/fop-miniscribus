/*******************************************************************************
 * class Document include needed file
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


#ifndef OO_DOCUMET_H
#define OO_DOCUMET_H

#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QVariant>
#include <QDomDocument>
#include <QtDebug>
#include <QDebug> 
#include <QSettings>
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QTextDocument>
#include <QTextFrameFormat>
#include <QTextFormat>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextListFormat>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextTable>
#include <QTextList>

namespace OOO {

class Document
{
  public:
  Document() 
  { 
     
  }
  ////operator QVariant() const {
  ////return QVariant::fromValue(*this);
  ////}   
  QByteArray content() const
  {
    return mContent;
  }

  QByteArray meta() const
  {
    return mMeta;
  }

  QByteArray styles() const
  {
    return mStyles;
  }
  QString lastErrorString() const
  {
    return mErrorString;
  }

QByteArray mStyles;
QByteArray mMeta;
QByteArray mContent;
QString mErrorString;
  
};


///////Q_DECLARE_METATYPE(Document); 


}
  
  
  
  
  
#endif // OO_DOCUMET_H

