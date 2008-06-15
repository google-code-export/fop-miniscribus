#include "qvcursor.h"

QVCursor::QVCursor(QWidget *parent) 
  :QWidget(parent),units("mm"),dimfontsize(8),Cursor_1_X(15),Cursor_2_X(600)
{
    ColText = Qt::black;
}


void QVCursor::paintEvent(QPaintEvent *)
{
    
    
    paintScale();
    paintCursor();
}

void QVCursor::paintCursor()
{
    QPainter painter(this);  ///QRect ( int x, int y, int width, int height )
    const int large = width();
    painter.setWindow(0, 0,large,22);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    
    
    QRectF cursor(Cursor_1_X,12,5,18);
    painter.setPen(QPen(Qt::red,1.5));
    painter.drawLine(Cursor_1_X, 1,Cursor_1_X, 21);
    painter.drawLine(Cursor_2_X, 1,Cursor_2_X, 21);
    
    painter.setPen(QPen(Qt::darkRed,1));
    QRectF rectangle_0(Cursor_1_X - 5,18,10,15); 
    QRectF rectangle_1(Cursor_1_X - 5,-10,10,15); 
    
    QRectF brectangle_0(Cursor_2_X - 5,18,10,15); 
    QRectF brectangle_1(Cursor_2_X - 5,-10,10,15); 
    
    
    painter.drawEllipse(rectangle_0);
    painter.drawEllipse(rectangle_1);
    
    painter.drawEllipse(brectangle_0);
    painter.drawEllipse(brectangle_1);
    
}


void QVCursor::paintScale()
{
    const int large = width();
    const qreal scaleMesure = Pointo(large,units);
    const qreal lineseparator_0 = FopInt(QString("10%1").arg(units));
    const qreal lineseparator_1 = lineseparator_0 / 10;
    QPainter painter(this);
    painter.setWindow(0, 0,large,22);
    //////painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#ece4c7"));
    painter.drawRect(rect());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::darkGray,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(0,1,large,1);
    const uint linessumme_0 = large / lineseparator_0;
    const uint linessumme_1 = large / lineseparator_1;
    int PressUnit = -1;
    painter.setPen(QPen(ColText,0.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int i = 0; i<=linessumme_1; i++)
    {
        PressUnit++;
        
        QString strnu = QString("%1").arg(i);
        const int LeftPointer1 = i * lineseparator_1 - 0.4;
        if (PressUnit > 0 && PressUnit < 5 || PressUnit > 5 && PressUnit < 10) {
        painter.drawLine(LeftPointer1,5,LeftPointer1,6);  
        } else if (PressUnit == 5) {
        painter.drawLine(LeftPointer1,5,LeftPointer1,8);   
        }
        
        if (PressUnit == 9) {
          PressUnit = -1;
        }
    }
    painter.setPen(QPen(ColText,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont valFont("Arial", dimfontsize, QFont::Normal);
    painter.setFont(valFont);
    for (int i = 0; i<=linessumme_0; i++)
    {
        const int LeftPointer0 = i * lineseparator_0 - 0.5;
        painter.drawLine(LeftPointer0,5,LeftPointer0,10);
        if (i > 0) {
        QString val = QString("%1").arg(i*10);
        QRectF valR(LeftPointer0 - (lineseparator_0 / 2),11,lineseparator_0,dimfontsize + 2);
        painter.drawText(valR, Qt::AlignCenter , val);
        }
    }
    
    painter.setPen(QPen(Qt::darkGray,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(0,21,large,21);
    
    painter.end();
}

QSize QVCursor::minimumSizeHint() const
{
    return QSize(100,45);
}

QSize QVCursor::sizeHint() const
{
    return QSize(700,45);
}


