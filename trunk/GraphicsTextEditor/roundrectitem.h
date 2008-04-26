
#include <QtCore/qobject.h>
#include <QtCore/qtimeline.h>
#include <QtGui/qgraphicsitem.h>

#if QT_VERSION >= 0x040400

QT_FORWARD_DECLARE_CLASS(QGraphicsProxyWidget);

class RoundRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    RoundRectItem(const QRectF &rect, const QColor &color, QWidget *embeddedWidget = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;

    void setPixmap(const QPixmap &pixmap);

    qreal opacity() const;
    void setOpacity(qreal opacity);

Q_SIGNALS:
    void activated();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void updateValue(qreal value);

private:
    QColor col;
    QPixmap pix;
    QTimeLine timeLine;
    qreal lastVal;
    qreal opa;

    QGraphicsProxyWidget *proxyWidget;
};



#endif 






