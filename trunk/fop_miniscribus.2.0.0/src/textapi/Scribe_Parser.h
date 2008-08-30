#ifndef SCRIBE_PARSER_QTEXTDOCUMENT_H
#define SCRIBE_PARSER_QTEXTDOCUMENT_H


#include "Config.h"
#include "SessionManager.h"
#include <limits.h>
#include "Layer_Absolute_Floating.h"





/* Parser qtextdocument to save on other format */

class ScribeParser {
public:
  
typedef enum
{  
  Psave,
  PFopModus,
  Plinker
} ScribeParseModus;

    ScribeParser( QTextDocument * doc , ScribeParseModus e  = Plinker );
    inline QStringList internals() { return Internal_Destination_Links; }
    inline QStringList destinations() { return Clicks_Destination_Links; }
    inline QDomDocument *dom() { return document; }
    inline QMap<QString,SPics> resourceImage() { return resource; }
    inline RichDoc mimeDocument() { return mimeDoc; }
    QMap<QString,SPics> resource;
    RichDoc mimeDoc;
    ~ScribeParser();
  private:
QDomDocument *document;
QTextDocument *textDocument;
QTextCursor helper_cursor;
ScribeParseModus modus;
QStringList Internal_Destination_Links;
QStringList Clicks_Destination_Links;
  
void MemoonBlock( QTextCursor c ,  QVariant data , const int id );
void processFrame(QDomElement appender ,  QTextFrame *frame);
void processBlock( QDomElement appender ,  QTextBlock   block );
void processFragment( QDomElement appender ,  QTextFragment  fragment );
void processTable( QDomElement appender , QTextTable *table);
void processTableCell( QDomElement appender , QTextTableCell cell );
};



class Rotater : public QWidget
{
    Q_OBJECT
//
public:
    Rotater( int rotaten , QWidget *parent  )
    {
        gridLayout = new QGridLayout(this);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label = new QLabel(tr("Rotate �:"),this);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 25));
        hboxLayout->addWidget(label);
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hboxLayout->addItem(spacerItem);
        lcdNumber = new QLCDNumber(this);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setMaximumSize(QSize(16777215, 25));
        lcdNumber->setAutoFillBackground(false);
        lcdNumber->setFrameShadow(QFrame::Sunken);
        lcdNumber->display(rotaten);
        hboxLayout->addWidget(lcdNumber);
        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

        dial = new QDial(this);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setFocusPolicy(Qt::NoFocus);
        dial->setContextMenuPolicy(Qt::NoContextMenu);
        dial->setNotchesVisible(true);
        dial->setMaximum(360);
        dial->setValue(rotaten);
        gridLayout->addWidget(dial, 1, 0, 1, 1);
        connect(dial, SIGNAL(dialMoved(int)),this, SLOT(NewValue(int)));

    }

    ~Rotater()
    {
        dial->disconnect(this);
    }


    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QSpacerItem *spacerItem;
    QLCDNumber *lcdNumber;
    QDial *dial;
private:
signals:
    void rotater(int);
public slots:

    void NewValue( const int x )
    {
        lcdNumber->display(x);
        emit rotater(x);
    }



};




class FrameStyler : public QWidget
{
    Q_OBJECT
//
public:
    FrameStyler( QTextFrame *base , QWidget *parent  )
            : QWidget( parent ),frame(base)
    {
        QTextFrameFormat Ftf = frame->frameFormat();
        QTextDocument *dd = base->document();
        /////QTextFrame  *RootFrame = dd->rootFrame();
        qreal largoss = dd->pageSize().rwidth();
        QTextLength langeframles = Ftf.width();
        if (langeframles.type() == QTextLength::FixedLength)
        {
            largoss = langeframles.rawValue();
        }



        gridLayout = new QGridLayout(this);
        hboxLayout = new QHBoxLayout();
        vboxLayout = new QVBoxLayout();
        label = new QLabel(tr("Border style:"),this);
        vboxLayout->addWidget(label);
        label_3 = new QLabel(tr("Border color:"),this);
        vboxLayout->addWidget(label_3);
        label_2 = new QLabel(tr("Float:"),this);
        vboxLayout->addWidget(label_2);
        vboxLayout->addWidget(new QLabel(tr("Border width:"),this));
        vboxLayout->addWidget(new QLabel(tr("Frame width:"),this));
        vboxLayout->addWidget(new QLabel(tr("Padding:"),this));



        hboxLayout->addLayout(vboxLayout);
        vboxLayout1 = new QVBoxLayout();
        comboBox = new QComboBox(this); ///// style
        comboBox->addItem(tr("Border none"),QTextFrameFormat::BorderStyle_None);
        comboBox->addItem(tr("Border Dotted"),QTextFrameFormat::BorderStyle_Dotted);
        comboBox->addItem(tr("Border Double"),QTextFrameFormat::BorderStyle_Double);
        comboBox->addItem(tr("Border Solid"),QTextFrameFormat::BorderStyle_Solid);
        comboBox->addItem(tr("Border Dashed"),QTextFrameFormat::BorderStyle_Dashed);
        comboBox->addItem(tr("Border DotDash"),QTextFrameFormat::BorderStyle_DotDash);
        comboBox->addItem(tr("Border DotDotDash"),QTextFrameFormat::BorderStyle_DotDotDash);
        comboBox->addItem(tr("Border Groove"),QTextFrameFormat::BorderStyle_Groove);
        comboBox->addItem(tr("Border Ridge"),QTextFrameFormat::BorderStyle_Ridge);
        comboBox->addItem(tr("Border Inset"),QTextFrameFormat::BorderStyle_Inset);
        comboBox->addItem(tr("Border Outset"),QTextFrameFormat::BorderStyle_Outset);
        comboBox->setCurrentIndex(comboBox->findData(Ftf.borderStyle()));


        QPixmap pix(22, 22);
        vboxLayout1->addWidget(comboBox);
        comboBox_3 = new QComboBox(this); //// color
        QStringList colorNames = QColor::colorNames();
        foreach (QString name, colorNames)
        {
            pix.fill(QColor(name));
            comboBox_3->addItem(pix,QString("color %1").arg(name),QColor(name));
        }
        comboBox_3->setCurrentIndex(comboBox_3->findData(Ftf.borderBrush().color()));

        vboxLayout1->addWidget(comboBox_3);
        comboBox_2 = new QComboBox(this);   /// float
        comboBox_2->addItem(tr("Float inline"),QTextFrameFormat::InFlow);
        comboBox_2->addItem(tr("Float left"),QTextFrameFormat::FloatLeft);
        comboBox_2->addItem(tr("Float right"),QTextFrameFormat::FloatRight);

        comboBox_2->setCurrentIndex(comboBox->findData(Ftf.position()));

        vboxLayout1->addWidget(comboBox_2);
        /* border widht */
        borderwi = new QSpinBox(this);
        borderwi->setMaximum(20);
        borderwi->setValue(Ftf.border());
        vboxLayout1->addWidget(borderwi);

        framewi = new QSpinBox(this);
        framewi->setMaximum(dd->idealWidth());
        framewi->setValue(largoss);
        vboxLayout1->addWidget(framewi);

        paddingwi = new QSpinBox(this);
        paddingwi->setMaximum(50);
        paddingwi->setValue(Ftf.padding());
        vboxLayout1->addWidget(paddingwi);


        hboxLayout->addLayout(vboxLayout1);
        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);
        QObject::connect(comboBox_3, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(borderwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(paddingwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(framewi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));






    }
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QVBoxLayout *vboxLayout1;
    QComboBox *comboBox;
    QComboBox *comboBox_3;
    QComboBox *comboBox_2;
    QSpinBox *borderwi;
    QSpinBox *paddingwi;
    QSpinBox *framewi;

private:
    QTextFrame *frame;
signals:
public slots:

    void FormatUpdate()
    {
        QTextFrameFormat Ftf = frame->frameFormat();
        Ftf.setWidth ( framewi->value() );
        Ftf.setPosition(QTextFrameFormat::Position(comboBox_2->itemData(comboBox_2->currentIndex()).toInt()));
        Ftf.setBorderStyle(QTextFrameFormat::BorderStyle(comboBox->itemData(comboBox->currentIndex()).toInt()));
        Ftf.setBorderBrush(QBrush(comboBox_3->itemData(comboBox_3->currentIndex()).value<QColor>()));
        Ftf.setBorder(borderwi->value());
        Ftf.setPadding(paddingwi->value());
        frame->setFrameFormat(Ftf);
    }

};




class LoadGetImage : public QHttp
{
    Q_OBJECT
//
public:
    LoadGetImage( int nr , QUrl url_send );
    void Start();
    inline int Get_id()
    {
        return cid;
    }
    inline int Htpp_id()
    {
        return Http_id;
    }
    inline QPixmap pics()
    {
        return resultimage;
    }
    int cid;
    int Http_id;
    QHttpRequestHeader header;
    QUrl url;
    QPixmap resultimage;
signals:
    void take(int);
public slots:
    void ImageReady( bool error );
};


class Gloader : public QThread
{
    Q_OBJECT

public:
    void Setting( QObject *parent , int id , QUrl url_send );
protected:
    void run();
signals:
private:
    int cid;
    QUrl url;
    LoadGetImage *Rhttp;
    QObject* receiver;
};






class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene( QObject * parent = 0 );
    GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
    void clear();
    void setSceneRect(  const QRectF & rect );
    void clearSelection();
    void SetVisibleArea( const QRectF area );
    qreal zmax();
    qreal zmin();
    void removeIdItem( const int idchunk );
    AbsoluteLayer *itemAbsolute( QGraphicsItem * item );
signals:
    void SelectOn(QGraphicsItem*);
    void MakeVisible(QRectF);
public slots:
protected:
    ////////void dragMoveEvent ( QGraphicsSceneDragDropEvent * e );
    bool WakeUp( const QPointF incomming );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void keyPressEvent(QKeyEvent *e);
    QGraphicsItem *bridge;
    void storno(); /* reset */
};















#endif 


