#ifndef BASIC_TOOL_TMP_H
#define BASIC_TOOL_TMP_H

#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QTextCodec>
#include <QDomDocument>

class StreamFop
{
  public:
  StreamFop() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  start();
  }
  ~StreamFop()
  {
   d->close();
  }
  bool clear()
  {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  qint64 write( const QByteArray dat )
  {
    d->write(dat); 
    return d->bytesAvailable();
  }
  
  void start() { 
    d->seek(0);
  }
  bool LoadFile( const QString file ) { 
    if (clear()) {
     QFile f(file); 
        if (f.exists()) {
          if (f.open(QFile::ReadOnly)) {
           d->write(f.readAll());
           f.close();
           start();
           return true;
          }
        }
    }
    return false;
  }
  bool PutOnFile( const QString file ) { 
      QFile f(file);
      start();
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(d->readAll());
        f.close();
        start();
        return bi > 0 ? true : false;
      }
   return false; 
  }
  QDomDocument Dom() { return doc; }
  QBuffer *device() { return d; }
  bool isValid() { return doc.setContent(stream(),false,0,0,0);  }
  QByteArray stream() { return d->data(); }
  QDomDocument doc;
  QBuffer *d;
}; 

static inline bool isvalidXmlencoding( const QString xmlfile  )
{
    QFile *xfile = new QFile( xmlfile );
    if (!xfile->exists()) {
    return false;
    }
    QString Firstline;
    bool validxml = false;
    if (xfile->open(QIODevice::ReadOnly))    {
             char buf[1024];
             qint64 lineLength = xfile->readLine(buf, sizeof(buf));
             Firstline = QString(buf);
             if (lineLength > 10 && Firstline.contains("encoding")) {
             validxml = true;
             }  
    }
    return validxml;
}

static inline QString oldMiniscribusSignature( QString streamxml   )
{
    QRegExp expression( "editor=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    expression.setMinimal(true);
    QString semencoding = "";
    int iPosition = 0;
    while( (iPosition = expression.indexIn( streamxml , iPosition )) != -1 ) {
        semencoding = expression.cap( 0 );
        semencoding = semencoding.mid(8,semencoding.size() - 9);
        iPosition += expression.matchedLength();
        /////////qDebug() << "### semencoding" << semencoding;
    }
    
    return semencoding;
}



static inline QString documentBuild( QString streamxml   )
{
    QRegExp expression( "build=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    expression.setMinimal(true);
    QString semencoding = "";
    int iPosition = 0;
    while( (iPosition = expression.indexIn( streamxml , iPosition )) != -1 ) {
        semencoding = expression.cap( 0 );
        semencoding = semencoding.mid(7,semencoding.size() - 8);
        iPosition += expression.matchedLength();
        /////////qDebug() << "### semencoding" << semencoding;
    }
    
    return semencoding;
}

/* correct codex from xml file read only first line */
static inline QTextCodec *GetcodecfromXml( const QString xmlfile  )
{
    QString semencoding = "UTF-8";
    QTextCodec *codecsin;
    QFile *xfile = new QFile( xmlfile );
    if (!xfile->exists()) {
    codecsin =  QTextCodec::codecForName(semencoding.toAscii());
    return codecsin;
    }
  
    QString Firstline;
    bool validxml = false;
    if (xfile->open(QIODevice::ReadOnly))    {
             char buf[1024];
             qint64 lineLength = xfile->readLine(buf, sizeof(buf));
             Firstline = QString(buf);
             if (lineLength > 10 && Firstline.contains("encoding")) {
             validxml = true;
             }  
    }
  
    if (!validxml ) {
    codecsin =  QTextCodec::codecForName(semencoding.toAscii());
    return codecsin;
    }
  
    QRegExp expression( "encoding=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    expression.setMinimal(true);
    int iPosition = 0;
    while( (iPosition = expression.indexIn( Firstline , iPosition )) != -1 ) {
        semencoding = expression.cap( 0 );
        semencoding = semencoding.mid(10,semencoding.size() - 11);
        iPosition += expression.matchedLength();
        qDebug() << "### semencoding" << semencoding;
    }
    if (iPosition == -1) {
    codecsin =  QTextCodec::codecForName("UTF-8");
    } else {
    codecsin =  QTextCodec::codecForName(semencoding.toAscii());
    }
    return codecsin;
}

static inline uint QTime_Null()
{
    QDateTime timer1( QDateTime::currentDateTime() ); 
    return timer1.toTime_t();
}

static inline QString Unique_Stamp()
{
    QDateTime timer1( QDateTime::currentDateTime() ); 
    QChar letter('A' + (qrand() % 26));
    QChar letter1('A' + (qrand() % 26));
    QString initstr = timer1.toString("zzzssmm");
    initstr.prepend("00000");
    initstr.prepend(letter);
    initstr.prepend(letter1);
    
    return initstr;
}



/* return int value from a unixtime date MMM YYY ... */
static inline int dateswap(QString form, uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QString numeric = fromunix.toString((const QString)form);
     bool ok; 
     return (int)numeric.toFloat(&ok);
}


/* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
/* Mail time compatible string date from unixtime  */
static inline QString UmanTimeFromUnix( uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QStringList RTFdays = QStringList() << "day_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
     QStringList RTFmonth = QStringList() << "month_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
     QDate timeroad(dateswap("yyyy",unixtime),dateswap("M",unixtime),dateswap("d",unixtime));
     QStringList rtfd_line;
     rtfd_line.clear();
     rtfd_line.append("Date: ");
     rtfd_line.append(RTFdays.at(timeroad.dayOfWeek()));
     rtfd_line.append(", ");
     rtfd_line.append(QString::number(dateswap("d",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(RTFmonth.at(dateswap("M",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(QString::number(dateswap("yyyy",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(fromunix.toString("hh:mm:ss"));
     rtfd_line.append("");
return QString(rtfd_line.join(""));
}



/* read the contenet of a local file as qstring */
static inline QString ReadFileUtf8Xml( const QString fullFileName )
{
    QString inside = "";
    QFile file(fullFileName); 
    if (file.exists()) {
                if (file.open(QFile::ReadOnly | QFile::Text)) {
                    inside = QString::fromUtf8(file.readAll());
                    file.close();
                }
    }

return inside;
}

/* remove a file if exist  */
static inline bool qt_unlink(const QString fullFileName)
{
    if (fullFileName.size() > 0) {
    QFile f( fullFileName );
	 if ( f.exists () ) {
            if (f.remove()) {
             return true;
         }
	}
    }
return false;
}

/* remove dir recursive */
static inline void DownDir_RM(const QString d)
{
   QDir dir(d);
   if (dir.exists())
   {
      const QFileInfoList list = dir.entryInfoList();
      QFileInfo fi;
      for (int l = 0; l < list.size(); l++)
      {
         fi = list.at(l);
         if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            DownDir_RM(fi.absoluteFilePath());
         else if (fi.isFile())
         {
            qt_unlink(fi.absoluteFilePath());
         } 
        
      }
      dir.rmdir(d);
      
   }
} 



/* random nummer */
static inline int FooNumer( int lowest, int highest )
{
    using namespace std;
    srand(QTime_Null()); 
    int random_integer; 
    int range=(highest-lowest)+1; 
    for(int index=0; index<10; index++){ 
        random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0)); 
    } 
return random_integer;
}


/* random QStringList */
static inline QStringList ShuffleRandomList( QStringList belist )
{
    QStringList eleborator;
    eleborator.clear();
    eleborator = belist;
    eleborator.sort();
    int base = 0;
    int destruct;
    for (int i = 0; i < eleborator.size(); ++i)  { 
        destruct = FooNumer(base,eleborator.size());
        if (destruct != i) {
        eleborator.swap(destruct,i);
        }
        eleborator.move(0,eleborator.size() - 1 );
    }
return   eleborator;  
}

/*  append write as internal debug log  ....   */
static inline bool fwriteAppend( const QString fullFileName , const QString xml)
{
    if (fullFileName.contains("/", Qt::CaseInsensitive)) {
    QString ultimacartellaaperta = fullFileName.left(fullFileName.lastIndexOf("/"))+"/";
    QDir dira(ultimacartellaaperta);
    if ( dira.mkpath(ultimacartellaaperta) ) { } else {
    return false;
    }
    }
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( fullFileName );
        if ( f.open( QFile::WriteOnly | QFile::Text |  QIODevice::Append ) )
        {
        QTextStream sw( &f );
        sw.setCodec(codecx);
        sw << xml;
        f.close();
        return true;
        }
        return false;
}




#endif













