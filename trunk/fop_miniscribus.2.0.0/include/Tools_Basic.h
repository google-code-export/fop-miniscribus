#ifndef BASIC_TOOL_TMP_H
#define BASIC_TOOL_TMP_H

#include <QtGui>
#include <QtCore>
#include <QPixmap>


static inline uint QTime_Null()
{
    QDateTime timer1( QDateTime::currentDateTime() ); 
    return timer1.toTime_t();
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




#endif













