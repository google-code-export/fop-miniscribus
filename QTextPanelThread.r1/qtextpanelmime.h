#ifndef QTEXTPANELMIME_H
#define QTEXTPANELMIME_H

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include <QDomDocument>

#include "config.h"
#include "qtextpanelimage.h"
#include <QtGui/qtextdocument.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qmenu.h>
#include <QtCore/qrect.h>
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtGui/qtextdocumentfragment.h>
#include <QTextLine>
#include <QSvgRenderer>

#include <QGraphicsScene>

#include <QHttp>
#include <QThread>



/* help functions */
QPixmap bruschChess(qreal chesswidth = 50.);
/* allowed char on file name image to save */
QString imageName(const  QString txt);
/* 12mm to point , 12cm , 2inch */
/* metric conversion from and to */
qreal FopInt(const QString datain);
qreal pointTo(qreal unit , const QString unita);
qreal toUnit(qreal unit , const QString unita);
qreal toPoint(qreal unit , const QString unita);

bool Cache(const QString dirpath);    /* mkdir */
QString convertPath(QString path);    /* QDir::toNativeSeparators  */
bool fwriteUtf8(const QString file , QString xml);
QString ReadFile(const QString fullFileName); /* read utf8 text */
/* read the content of a local file as QByteArray*/
QByteArray StreamFromFile(const QString fullFileName);
QString decodeBase64(QString xml);
QString encodeBase64(QString xml);
void OpenDeskBrowser(QUrl loc);    /* open any file or url */

QPixmap RenderPixmapFromSvgByte(QByteArray streams);

QRectF M_PagesizeMake(QPrinter::PageSize psize , bool landscape = true);

QRectF CenterRectSlaveFromMaster(const QRectF Master , QRectF Slave);

QPixmap ImagefromMime(const QMimeData *mime);

QIcon createColorToolButtonIcon(const QString &imageFile,QColor color);
QIcon createColorIcon(QColor color);


QString getGSLinuxPath(QString apps = QString("gs"));
QString getGSDefaultExeName();
double getGSVersion();
double JavaVersion();
int callGS(const QStringList args);
QPixmap LoadPS(QString fn , const QString arguments_append = QString());
QPixmap LoadPDF(QString fn, int Page, int w);



class QTextEditMimeData : public QMimeData
{
	private:
		void setup() const;
		mutable QTextDocumentFragment fragment;
	protected:
		virtual QVariant retrieveData(const QString &mimeType, QVariant::Type type) const;
	public:
		inline QTextEditMimeData(const QTextDocumentFragment &aFragment) : fragment(aFragment) {}
		virtual QStringList formats() const;
};

class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent * e);
		bool WakeUp(const QPointF incomming);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent * e);
		void mousePressEvent(QGraphicsSceneMouseEvent * e);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
		void keyPressEvent(QKeyEvent *e);
		QGraphicsItem *bridge;
		void storno(); /* reset */
	public:
		GraphicsScene(QObject * parent = 0);
		GraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
		void clear();
		void setSceneRect(const QRectF & rect);
		void clearSelection();
		void SetVisibleArea(const QRectF area);
		qreal zmax();
		qreal zmin();
	signals:
		void SelectOn(QGraphicsItem*);
		void MakeVisible(QRectF);
};










class LoadGetImage : public QHttp
{
	Q_OBJECT

	public:
		int cid;
		int Http_id;
		QHttpRequestHeader header;
		QUrl url;
		QPixmap resultimage;
		LoadGetImage(int nr , QUrl url_send);
		void Start();
		inline int Get_id() {return cid;}
		inline int Htpp_id() {return Http_id;}
		inline QPixmap pics() {return resultimage;}

	public slots:
		void ImageReady(bool error);

	signals:
		void take(int);
};

class Gloader : public QThread
{
	Q_OBJECT

	private:
		int cid;
		QUrl url;
		LoadGetImage *Rhttp;
		QObject* receiver;

	protected:
		void run();

	public:
		void Setting(QObject *parent , int id , QUrl url_send);
};

class FrameStyler : public QWidget
{
	Q_OBJECT

	private:
		QTextFrame *frame;
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

	public:
		FrameStyler(QTextFrame *base , QWidget *parent) : QWidget(parent),frame(base)
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
			vboxLayout1->addWidget(comboBox);

			QPixmap pix(22, 22);

			comboBox_3 = new QComboBox(this); //// color

			QStringList colorNames = QColor::colorNames();

			foreach(QString name, colorNames)
			{
				pix.fill(QColor(name));
				comboBox_3->addItem(pix,QString("color %1").arg(name),QColor(name));
			}

			comboBox_3->setCurrentIndex(comboBox_3->findData(Ftf.borderBrush().color()));

			vboxLayout1->addWidget(comboBox_3);
			comboBox_2 = new QComboBox(this);   /// float
			comboBox_2->addItem(tr("Float inline"), QTextFrameFormat::InFlow);
			comboBox_2->addItem(tr("Float left"), QTextFrameFormat::FloatLeft);
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

	public slots:
		void FormatUpdate()
		{
			QTextFrameFormat Ftf = frame->frameFormat();
			Ftf.setWidth(framewi->value());
			Ftf.setPosition(QTextFrameFormat::Position(comboBox_2->itemData(comboBox_2->currentIndex()).toInt()));
			Ftf.setBorderStyle(QTextFrameFormat::BorderStyle(comboBox->itemData(comboBox->currentIndex()).toInt()));
			Ftf.setBorderBrush(QBrush(comboBox_3->itemData(comboBox_3->currentIndex()).value<QColor>()));
			Ftf.setBorder(borderwi->value());
			Ftf.setPadding(paddingwi->value());
			frame->setFrameFormat(Ftf);
		}
};

class Rotater : public QWidget
{
	Q_OBJECT

	public:
		QGridLayout *gridLayout;
		QHBoxLayout *hboxLayout;
		QLabel *label;
		QSpacerItem *spacerItem;
		QLCDNumber *lcdNumber;
		QDial *dial;

		Rotater(int rotaten , QWidget *parent)
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

	public slots:
		void NewValue(const int x)
		{
			lcdNumber->display(x);
			emit rotater(x);
		}

	signals:
		void rotater(int);
};

#endif // QTEXTPANELMIME_H
