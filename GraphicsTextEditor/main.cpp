#include <QtGui>
#include "roundrectitem.h"
#include "textlayer.h"
#include <QtGui/qgraphicsview.h>

class Panel : public QGraphicsView
{
    Q_OBJECT
public:
    Panel(int width, int height)
    : width(width),
      height(height),
      flipLeft(false),
      flipped(true)
{
    
    setMinimumSize (1000,600);
    swapTimeLine = new QTimeLine(1300, this);
    swapTimeLine->setFrameRange(0,90);
    /////setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //////setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QRectF bounds((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
    scene = new QGraphicsScene(bounds, this);
    setScene(scene);

    ioq = new TextLayer(1,0,scene);
    ioq->setPos(QPointF(0,0));
    /* edit gui */
    embed = new QTextBrowser();
    embed->setText ( "sdfsdfsdfsdfsd fsd f sd f dsf ds f ds fds f ds f sd f dsf ds f dsf ds f ds fs f ds fs f sf ds f");
    embed->setReadOnly(false);
    embed->setOpenExternalLinks(false);
    backItem = new RoundRectItem(bounds, palette().color(QPalette::Window), embed);
    backItem->setTransform(QTransform().rotate(90, Qt::YAxis));
    scene->addItem(backItem);
    /* edit gui */
    
    fitInView(ioq, Qt::KeepAspectRatio);
    
    connect(backItem, SIGNAL(activated()), this, SLOT(flip()));
    connect(swapTimeLine, SIGNAL(frameChanged(int)), this, SLOT(updateFlipStep(int)));
    
}




void keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Q)   {
        flip();
        qDebug() << "###  Key_X  swap ";
        return;
    }
    
    
    QGraphicsView::keyPressEvent(event);
}

void resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(backItem, Qt::KeepAspectRatio);
}
QTextBrowser *embed;
TextLayer *ioq;
int width;
int height;
qreal xrot, yrot;
qreal xrot2, yrot2;
bool flipped;
bool flipLeft;
QTimeLine *swapTimeLine;
QGraphicsScene *scene;
RoundRectItem *baseItem;
RoundRectItem *backItem;

private slots:
    
void flip()
{
    if (swapTimeLine->state() == QTimeLine::Running) {
        return;
    }
    
    
     
    
    if (flipped) {
        swapTimeLine->setDirection(QTimeLine::Backward);
        swapTimeLine->start();
        flipped = false;
        embed->setDocument(ioq->document());
    } else {
        swapTimeLine->setDirection(QTimeLine::Forward);
        swapTimeLine->start();
        flipped = true;
        ioq->setDocument(embed->document()->clone());
    }
    
    
    
}

void updateFlipStep(int val)
{
    QTransform trans;
               trans.rotate(val, Qt::YAxis);
               trans.rotate(val, Qt::XAxis);
    qDebug() << "###  updateFlipStep val  " << val ;
    backItem->setTransform(trans);
    qDebug() << "###  flipped  " << flipped ;
    
    if (val == 0 ) {
    fitInView(backItem, Qt::KeepAspectRatio);
    }
}


};



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Panel panel(3, 3);
    panel.show();
    return app.exec();
}

#include "main.moc"












