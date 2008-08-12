
http://www.qt-apps.org/usermanager/search.php?username=patrik08&action=contents
http://www.qt-apps.org/usermanager/search.php?username=patrik08&action=profile


Usage to post image on a QWidget or other

imagePost/src1/apngreader.h

/* 
FrameReader( QObject *reciver , const QString apngfile = QString(), uint defaulttime = 1000 );
QObject *reciver reciver from event
QString apngfile file to load or null ore direct from resource 
uint defaulttime  time to play one image /* if apng format evry image has is time */
*/
FrameReader *xdev = new FrameReader(this,QString("Kubus.png") /* filename */ );



or


FrameReader *xdev = new FrameReader(this) /* filename */ );
xdev->append(QImage("1.png"));
xdev->append(QImage("2.png"));
xdev->start();

Peter Hohl
www.ciz.ch

