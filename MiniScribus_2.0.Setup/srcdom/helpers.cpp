#include "helpers.h"

WorkPanel::WorkPanel( QWidget * parent )
 : QWorkspace(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
	  
	
}

void WorkPanel::paintEvent(QPaintEvent *e) 
{
		static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -50)
    };
    static const QPoint minuteHand[3] = {
        QPoint(4, 8),
        QPoint(-4, 8),
        QPoint(0, -75)
    };
    
     static const QPoint secondHand[3] = {
        QPoint(2, 4),
        QPoint(-2, 4),
        QPoint(0, -96)
    };
		
		QPixmap pic(":/img/vm_color.png");
	  panello =  pic.scaledToWidth(width() / 3.2 ,Qt::SmoothTransformation);
		
		QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen ( Qt::NoPen );
    painter.setBrush ( QBrush(Qt::white));
		painter.drawRect(rect());
		
		
	  QPointF dd = rect().center();
		
		qreal fromtop =  dd.rx(); 
		
		////////////qDebug() << "### fromtop 007d72 " << fromtop;
		///////////////////qDebug() << "### panello.height()  " << panello.height();
		
    QPointF ee(  panello.width() / 2  , panello.height() + (panello.height() / 2)  );
		painter.drawPixmap(dd - ee,panello);
		
		//////////painter.save();

    QColor hourColor("#0168b5");
    QColor minuteColor("#0168b5");
    QColor secondColor("#e94e2f");

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    
    
    QPen pen;
    pen.setStyle( Qt::SolidLine );
    pen.setWidth( 2 );
    pen.setColor( QColor(Qt::red) );
    painter.setPen( pen);
    /////////painter.drawEllipse(rect());
    
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    

    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    int radioore = 30.0 * ((time.hour() + time.minute() / 60.0));
    int radiosecondi = 360.0 * (time.second() / 60.0);
    
    
    painter.save();
    painter.rotate(radioore);
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
    
      for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

    //////painter.setPen(Qt::NoPen);
    /////painter.setBrush(minuteColor);
    
    int radiominuti = 6.0 * (time.minute() + time.second() / 60.0);
    /////////////std::cout << "sec->[X]" << radiosecondi << "-" << time.second() << std::endl;
    /////std::cout << "ore->[X]" << radioore << std::endl;

    painter.save();
    painter.rotate(radiominuti);
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(secondColor);
    
    painter.save();
    painter.rotate(radiosecondi);
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();

    
}
