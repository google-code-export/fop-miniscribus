#ifndef BASE_QBASEFILE_H
#define BASE_QBASEFILE_H
#include <QString>
#include <QtDebug>
#include <QDebug> 
#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QTextCodec>
#include <QDateTime>
#include <QDate>
#include <QStringList>
#include <QRegExp>
#include <QUrl>
#include <QDomNode>
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QProcess>
#include <QStringList>
#include <QSettings>
#include <QActionGroup>
#include <QColorDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextCursor>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QFontDatabase>
#include <QTextBlockFormat>
#include <QTextListFormat>
#include <QTextFormat>
#include <QTextList>
#include <QTextCodec>
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#include <QErrorMessage>
#include <QProcess>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QInputDialog>
#include <QMenu>
#include "qxml.h"
#include <QtSql>
#include <QSqlQuery>
#include <QWidget>
#include <QObject>

#include <iostream> 
#include <ctime> 
#include <cstdlib>
#include "qxml.h"
#include <QMessageBox>
#include "main.h"


class Base_QBaseFile : public QWidget
{
    Q_OBJECT
    
public:
   Base_QBaseFile( QWidget *parent = 0 )
    : QWidget(parent)
{
     
}

QString datetr(QString form, uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     return fromunix.toString((const QString)form);
}

/* return int value from a unixtime date MMM YYY ... */
int dateswap(QString form, uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QString numeric = fromunix.toString((const QString)form);
     bool ok; 
     return (int)numeric.toFloat(&ok);
}

uint QTime_Null()
{
    QDateTime timer1( QDateTime::currentDateTime() ); 
    return timer1.toTime_t();
}

QString PasteTime()
{
    return UmanTimeFromUnix(QTime_Null());
}


/* display a mail date format */
QString UmanTimeFromUnix( uint unixtime )
{
     /* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QStringList RTFdays = QStringList() << "day_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
     QStringList RTFmonth = QStringList() << "month_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
     QDate timeroad(dateswap("yyyy",unixtime),dateswap("M",unixtime),dateswap("d",unixtime));
     /*qDebug() << "### RTFdays " << RTFdays.at(timeroad.dayOfWeek());
     qDebug() << "### RTFmonth " << RTFmonth.at(dateswap("M",unixtime));
     qDebug() << "### yyyy " << dateswap("yyyy",unixtime);
     qDebug() << "### M " << dateswap("M",unixtime);
     qDebug() << "### d " << dateswap("d",unixtime);*/
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
     /*qDebug() << "### mail rtf Date format " << rtfd_line.join("");*/
return QString(rtfd_line.join(""));
}



/* open file url mail ecc on other programm */
void OpenUrl_File_Dir_Dektop( QString item )
{
    #if defined(Q_WS_WIN)
    QProcess p;
    QStringList s;
    s << "url.dll,FileProtocolHandler" << item;
    p.startDetached(QString("rundll32.exe") , s );
    #endif
    #if defined Q_WS_MAC
    QProcess m;
    QStringList macs;
    macs << item;  /* oeffnet der default browser */
    m.startDetached(QString("open") , macs );
    #endif 
}


void Copy_To(QString inputfile, QString outfile)
{
    bool actioncpy = false;
    QFile f(inputfile);
    if ( f.exists()  and f.size() > 0) {
           if (!f.copy(outfile)) {
               QApplication::restoreOverrideCursor(); 
               QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(outfile));    
           } else {
               f.remove();
           }
	} else {
        QApplication::restoreOverrideCursor(); 
        QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(outfile));
    }
}

bool is_numeric(QString incomming)
{
    incomming.replace(QString(" "), QString("")); /* trimm */
    QString str2 = incomming;
    bool ok; 
    str2.toFloat(&ok); 
return ok;
}

/*
QString fileName = QFileDialog::getSaveFileName(this, "Export XML",QDir::homePath(), "*.xml");
*/
private:
    

};













#endif // QBASEFILE_H



