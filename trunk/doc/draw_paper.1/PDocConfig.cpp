#include "PDocConfig.h"


QRectF divideRect(const QRectF rect , const int part )
{
    bool horrizontal = qMax(rect.height(),rect.width()) == rect.width() ? true : false;
    const qreal hwi = qMax(rect.height(),rect.width()) / 2;

    if (horrizontal && part == 1 || horrizontal && part == 0) {
        return QRectF(rect.topLeft().x(),rect.topLeft().y(),hwi,rect.height());
    } else if (horrizontal && part !=0 || horrizontal && part !=1) {
        return QRectF(rect.topLeft().x() + hwi,rect.topLeft().y(),hwi,rect.height());
    } else if (!horrizontal && part == 0 || !horrizontal && part == 1) {
        return QRectF(rect.topLeft().x(),rect.topLeft().y(),rect.width(),hwi);
    } else if (!horrizontal && part != 0 || !horrizontal && part != 1) {
        return QRectF(rect.topLeft().x(),rect.topLeft().y() + hwi,rect.width(),hwi);
    } else {
        return rect;
    }
}

QRectF moveZero(const QRectF rect )
{
    return QRectF(0,0,rect.width(),rect.height());
}


bool isOnPage(const QRectF rect , const QPointF p , const qreal zoom )
{
    const qreal lmax = qMax(rect.height(),rect.width());
    const qreal lmin = qMax(rect.height(),rect.width());
    bool having = rect.contains(p);

    /*
    qDebug() << "-------------------------------------------------------------------";
    qDebug() << "### a topleft Y" << POINT_TO_CM(rect.top());
    qDebug() << "### max " << POINT_TO_CM(lmax);
    qDebug() << "### min " << POINT_TO_CM(lmin);
    qDebug() << "### a bottomLeft X " << POINT_TO_CM(rect.bottom());
    qDebug() << "### clic Y " << POINT_TO_CM(p.y());
    qDebug() << "### clic X " << POINT_TO_CM(p.x());
    qDebug() << "### zoom " << zoom;
    qDebug() << "### having " << having;
    qDebug() << "-------------------------------------------------------------------";
    */
    //////QApplication::restoreOverrideCursor();

    if (having) {
        return true;
    } else {
        QApplication::beep();
        return false;
    }

}


void paintWidged( QPainter *p , const QRectF rect , const QTransform trax )
{
    p->save();
    p->setPen(QPen(Qt::black,0.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p->setBrush(_SLIDERBGCOLOR_);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

void paintArea( QPainter *p , const QRectF rect , QColor col )
{
    p->save();
    p->setBrush(col);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}










void paintCursor( QPainter *p , const QRectF rect )
{
    bool horrizontal = rect.width() == SLIDERCURSORWI ? true : false;
    qreal midiline = SLIDERCURSORWI / 2;
    QPointF soco(QPointF(midiline,0) + rect.topLeft());
    QPointF socoend(QPointF(midiline,0) + rect.bottomLeft());

    if (!horrizontal) {
        soco =  rect.bottomLeft() - QPointF(0,midiline);
        socoend = soco + QPointF(rect.width(),0);
    }

    p->save();

    p->setOpacity(0.4);
    p->setBrush(QColor(Qt::green));
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->setOpacity(1.0);

    p->setBrush(Qt::red);

    p->setPen(QPen(Qt::red,1.5));
    if (horrizontal) {
        p->drawLine(soco,socoend);
    } else {
        p->drawLine(soco,socoend);
    }
    p->restore();
}

/*

QRectF cursorRectTop(  const QRectF rect , const qreal distance  , const QTransform trax )
{
    const QPointF init = rect.topLeft();

    const qreal marginLeftX = init.x() + ( bodyMargins.first * trax.m11() );
    const qreal marginRightX = init.x() +  rect.width()  - ( bodyMargins.second * trax.m11() );

    const qreal marginTopY = init.y() + ( bodyMargins.first * trax.m11() );
    const qreal marginToptX = init.y() +  rect.height()  - ( bodyMargins.second * trax.m11() );

    const QRectF cursor_1(marginLeftX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
    const QRectF cursor_2(marginRightX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());

    const QRectF cursor_3(init.x() ,marginTopY - (SLIDERCURSORWI / 2) ,rect.width(),SLIDERCURSORWI);
        const QRectF cursor_4(init.x() ,marginToptX - (SLIDERCURSORWI / 2),rect.width(),SLIDERCURSORWI);


}
*/

void paintScale( QPainter *p , const QRectF rect , QPair<qreal,qreal> bodyMargins , const QTransform trax )
{
    bool horrizontal = qMax(rect.height(),rect.width()) == rect.width() ? true : false;
    const qreal one_unit = CM_TO_POINT(1);
    const qreal one_unit_dec = MM_TO_POINT(1) * trax.m11();
    const QPointF init = rect.topLeft();
    qreal linemid = qMin(rect.height(),rect.width())  / 5;
    qreal wi = qMax(rect.height(),rect.width());

    const qreal marginLeftX = init.x() + ( bodyMargins.first * trax.m11() );
    const qreal marginRightX = init.x() +  rect.width()  - ( bodyMargins.second * trax.m11() );
    const qreal marginTopY = init.y() + ( bodyMargins.first * trax.m11() );
    const qreal marginToptX = init.y() +  rect.height()  - ( bodyMargins.second * trax.m11() );

    int fontsizetxt = 6;
    int sline = wi / one_unit;
    int slines = wi / one_unit_dec;

    p->save();
    p->setBrush(QColor(Qt::white));
    p->setPen(Qt::NoPen);
    p->drawRect(rect);

    QFont valFont("Arial",fontsizetxt, QFont::Normal);
    p->setFont(valFont);

    p->setPen(QPen(Qt::black,0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (horrizontal) {
        p->drawLine(init.x(),linemid + init.y(), init.x() + wi,linemid + init.y());
        p->setPen(QPen(Qt::black,0.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));




        int PressUnit = -1;
        int numsUnit = 0;
        for (int x = 0; x<=slines; x++)  {
            PressUnit++;
            const int LeftPointer2 = (x * one_unit_dec) + init.x();
            if (PressUnit == 0) {
                p->drawLine(LeftPointer2,linemid + init.y(),LeftPointer2,linemid * 3.5 + init.y());
            } else if (PressUnit == 5) {
                p->drawLine(LeftPointer2,linemid + init.y(),LeftPointer2,linemid * 3 + init.y());
            } else {
                p->drawLine(LeftPointer2,linemid + init.y() ,LeftPointer2,linemid * 2 + init.y());
            }

            if (PressUnit == 0 && numsUnit > 0) {
                const QString strnum = QString("%1").arg(numsUnit);
                QRectF valR(LeftPointer2 - fontsizetxt ,linemid * 3 + init.y() ,fontsizetxt * 2,fontsizetxt + 2);
                p->drawText(valR, Qt::AlignCenter , strnum);

            }


            if (PressUnit == 9) {
                PressUnit = -1;
                numsUnit++;
            }
        }
        const QRectF cursor_1(marginLeftX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        const QRectF cursor_2(marginRightX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        paintCursor(p,cursor_1);
        paintCursor(p,cursor_2);


    } else {
        p->setPen(QPen(Qt::black,0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p->drawLine(init.x() + linemid,init.y(),init.x() + linemid, init.y() + wi);
        p->setPen(QPen(Qt::black,0.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        int PressUnit = -1;
        int numsUnit = 0;

        for (int x = 0; x<=slines; x++)  {
            PressUnit++;

            const int TopPointer = (x * one_unit_dec) + init.y();
            if (PressUnit == 0) {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 3.5),TopPointer);
            } else if (PressUnit == 5) {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 3),TopPointer);
            } else {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 2),TopPointer);
            }


            if (PressUnit == 0 && numsUnit > 0) {
                const QString strnum = QString("%1").arg(numsUnit);
                QRectF valR(init.x() + (linemid * 2),TopPointer - fontsizetxt ,fontsizetxt * 2,fontsizetxt + 2);
                //////////////p->rotate ( 90 );
                p->drawText(valR, Qt::AlignCenter , strnum);
            }

            if (PressUnit == 9) {
                PressUnit = -1;
                numsUnit++;
            }



        }

        //////const QRectF cursor_1(marginLeftX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        const QRectF cursor_3(init.x() ,marginTopY - (SLIDERCURSORWI / 2) ,rect.width(),SLIDERCURSORWI);
        const QRectF cursor_4(init.x() ,marginToptX - (SLIDERCURSORWI / 2),rect.width(),SLIDERCURSORWI);
        paintCursor(p,cursor_3);
        paintCursor(p,cursor_4);



    }

    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(Qt::black,0.8));
    p->drawRect(rect);
    p->restore();

}


void paintShadow( QPainter *p , const QRectF rect )
{
    p->save();
    QRectF rightShadow(rect.right(), rect.top() + BorderShadow, BorderShadow, rect.height());
    QRectF bottomShadow(rect.left() + BorderShadow, rect.bottom(), rect.width(), BorderShadow);
    p->fillRect(rightShadow, _SHADOWCOLOR_);
    p->fillRect(bottomShadow, _SHADOWCOLOR_);
    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(Qt::black,0.8));
    p->drawRect(rect);
    p->restore();
}












