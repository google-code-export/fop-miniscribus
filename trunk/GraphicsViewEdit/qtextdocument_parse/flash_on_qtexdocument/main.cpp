#include <QtGui>

static const int ObjectName = 853;
static const int FlashReferenceID = 322;

#define _LINK_COLOR_ \
             QColor("#dc0000")

static inline QTextBlockFormat default_block_style( QTextBlockFormat pf = QTextBlockFormat() )
{
 /* i not find on qt to remove default 12point default space !!! */
 pf.setBottomMargin(0);
 pf.setTopMargin(0);
 pf.setRightMargin(0);
 pf.setIndent(0);
 pf.setLeftMargin(0);
 pf.setProperty(FlashReferenceID,0);
 pf.setProperty(ObjectName,0);
 return pf;
}

static inline QTextCharFormat default_char_style( QTextCharFormat pf = QTextCharFormat() )
{
	  QFont font = QApplication::font();
    font.setPointSize(10);
	  pf.setFont ( font );
		pf.setProperty(FlashReferenceID,0);
    pf.setProperty(ObjectName,0);
	  pf.setVerticalAlignment ( QTextCharFormat::AlignTop );
    return pf;
}






class FlashItem
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 1 };
    FlashItem() {
    wi = 400;  /* width */
    hi = 290;  /* height  */
    url = "erno";
    bg = QColor(Qt::blue);
    rchunk = QByteArray("erno");
    }
    operator QVariant() const {
    return QVariant::fromValue(*this);
    }
		/* is default setting no url no edit params */
		bool isValid()
		{
			return url == "erno" ? false : true;
		}
		/* pack params on a linktext */
		QString linktranslate()
		{
		 return QString("mediafla://%1|%2|%3|%4|end|").arg(url).arg(wi).arg(hi).arg(bg.name());
		}
		/* restore from linktext */
		void Load( const QString mediaflaurl )
		{
			if (mediaflaurl.startsWith("mediafla://")) {
				  QString Luri = mediaflaurl;
				  QStringList items = Luri.replace("mediafla://","").split("|");
				  if (items.size() > 4) {
						 url = items.at(0);
						 wi = qBound(100,items.at(1).toInt(),3000);
						 hi = qBound(100,items.at(2).toInt(),3000);
						 bg = QColor(items.at(3));
					}
					///////qDebug() << "### items list " << items;
			}
		}
		
    QSizeF sizeF()
    {
    return QSizeF(wi,hi); 
    }
		QString Tag()
		{
		  return QString("<object classid=\"clsid:d27cdb6e-ae6d-11cf-96b8-444553540000\" codebase=\"http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,0,0\" width=\""+QString::number(wi)+"\" height=\""+QString::number(hi)+"\" align=\"middle\">\n"
        "<param name=\"movie\" value=\"/multimedia/flvplayer.swf?file=data/"+url+".flv&amp;autoStart=true\" />\n"
        "<param name=\"quality\" value=\"high\" />\n"
        "<param name=\"bgcolor\" value=\""+bg.name()+"\" />\n"
        "<embed src=\"/multimedia/flvplayer.swf?file=data/"+url+".flv&autoStart=true\" quality=\"high\" bgcolor=\"#ffffff\" width=\""+QString::number(wi)+"\" height=\""+QString::number(hi)+"\" align=\"middle\" allowscriptaccess=\"sameDomain\" type=\"application/x-shockwave-flash\" pluginspage=\"http://www.macromedia.com/go/getflashplayer\" />\n"
        "</object>\n");
			  /* data samba linux and webdav */
		}
		/* to load as dom childs from root elements */
		QString stDom()
		{
			return QString("<dummyroot>%1</dummyroot>").arg(Tag());
		}
		
    FlashItem& operator=( const FlashItem& d )
    {
      wi = d.wi;
      hi = d.hi;
      url = d.url;
      bg = d.bg;
      rchunk = d.rchunk;
      return *this;
    }
		QSize minimum()
		{
			return QSize(300,140);
		}
    /* draw param from movie width x height + url */
    QPixmap pics()  {
    QPixmap pixmap(wi,hi);
    pixmap.fill(bg);
    QPainter p(&pixmap);
    //// QPointF ( qreal x, qreal y )
    p.setPen(QPen(Qt::black,4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    const int linessumme_1 = 12;
    const int divisor = wi / linessumme_1;
    const int border = divisor;
    QRectF BBleft(0,0,border,hi);
    QRectF BBright(wi-border,0,border,hi);
    p.setBrush(QBrush(Qt::lightGray));
    p.drawRect(BBleft);
    p.drawRect(BBright);
    for (int i = 0; i<=linessumme_1; i++) 
    {
       if (i > 0) {
         int TopY = divisor * i;
         p.drawLine(QPointF(0,TopY),QPointF(border,TopY));
         p.drawLine(QPointF(wi-border,TopY),QPointF(wi,TopY));
       }           
        
    }
    QFont font = QApplication::font();
    font.setPixelSize(30);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(pixmap.rect(), Qt::AlignCenter,
                      QString("Multimedia: %1x%2")      /* \xb0 */
                      .arg(wi)
                      .arg(hi));
    font.setPixelSize(12);
    p.setFont(font);
    p.drawText(pixmap.rect(),Qt::AlignBottom | Qt::AlignCenter ,QString("File: ")+url);
    return pixmap;
   }
    
QColor bg;
QByteArray rchunk;
int wi;
int hi;
QString url;
};

Q_DECLARE_METATYPE(FlashItem);


class FlashReference : public QObject, public QTextObjectInterface
{
	Q_OBJECT
 	Q_INTERFACES(QTextObjectInterface)
	
	
public:
 	FlashReference( FlashItem flock , QObject* par = 0)
 		: QObject(par),params(flock) {  }
	QSizeF intrinsicSize(QTextDocument* doc, int posInDoc,
		const QTextFormat &fmt)
	{
		  Q_UNUSED(doc)
			Q_UNUSED(posInDoc)
      QVariant xx = fmt.property(FlashReferenceID); 
      Q_ASSERT(!xx.isNull());
      FlashItem fla = xx.value<FlashItem>();
			return fla.sizeF();
	}
	void drawObject(QPainter* p, const QRectF &rect, QTextDocument* doc,
		   int posInDoc, const QTextFormat &fmt)
	{
		  Q_UNUSED(doc)
			Q_UNUSED(posInDoc)
      QVariant xx = fmt.property(FlashReferenceID); 
      Q_ASSERT(!xx.isNull());
      FlashItem fla = xx.value<FlashItem>();
			p->drawPixmap(rect.toRect(), fla.pics() );
	}
private:
	FlashItem params;
};




class MediaAttribute : public QDialog 
{
    Q_OBJECT
//
public:
 MediaAttribute( FlashItem edit , QWidget *parent = 0 )
 :QDialog(parent),xx(edit)
{
	  QPixmap pixmap(80, 80);
    pixmap.fill(edit.bg);
	  QIcon colorBG(pixmap);
	  setWindowTitle("Multimedia Parameter");
	  resize(260, 187);
    setMaximumSize(QSize(330, 220));
    gridLayout = new QGridLayout(this);
    groupBox = new QGroupBox(tr("Multimedia item"),this);
		gridLayout1 = new QGridLayout(groupBox);
    hboxLayout = new QHBoxLayout();
    vboxLayout = new QVBoxLayout();
    vboxLayout->addWidget(new QLabel(tr("Width:"),groupBox));
    vboxLayout->addWidget(new QLabel(tr("Height:"),groupBox));
    vboxLayout->addWidget(new QLabel(tr("Fla File name:"),groupBox));
    vboxLayout->addWidget(new QLabel(tr("BackgroundColor:"),groupBox));
    hboxLayout->addLayout(vboxLayout);
    vboxLayout1 = new QVBoxLayout();
    spinBox = new QSpinBox(groupBox);
		spinBox->setMinimum(edit.minimum().width());
    spinBox->setMaximum(3000);
    vboxLayout1->addWidget(spinBox);
    spinBox_2 = new QSpinBox(groupBox);
		spinBox_2->setMinimum(edit.minimum().height());
    spinBox_2->setMaximum(3000);
    vboxLayout1->addWidget(spinBox_2);
    lineEdit = new QLineEdit(groupBox);
		lineEdit->setText ( edit.url );
    vboxLayout1->addWidget(lineEdit);
    toolButton = new QToolButton(groupBox);
    toolButton->setIcon ( colorBG );
    vboxLayout1->addWidget(toolButton);
    hboxLayout->addLayout(vboxLayout1);
    gridLayout1->addLayout(hboxLayout, 0, 0, 1, 1);
    gridLayout->addWidget(groupBox, 0, 0, 1, 1);
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pushButton = new QPushButton(tr("OK"),this);
    hboxLayout1->addWidget(pushButton);
    pushButton_2 = new QPushButton(tr("Close"),this);
    hboxLayout1->addWidget(pushButton_2);
    gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);
		
		
		
		spinBox_2->setValue ( edit.sizeF().height() );
		spinBox->setValue ( edit.sizeF().width() );
		
		
		
		
		connect(pushButton, SIGNAL(clicked()),this, SLOT(save()));
    connect(pushButton_2, SIGNAL(clicked()),this, SLOT(reject()));
		connect(toolButton, SIGNAL(clicked()),this, SLOT(coloritem()));
		
 
}
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QVBoxLayout *vboxLayout1;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QHBoxLayout *hboxLayout1;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
FlashItem movie() { return xx; }
FlashItem xx;
protected:
private:
signals:
public slots:
	void save()
{
	  QString urlin = lineEdit->text().trimmed();
	  urlin = urlin.replace(".fla","");
	  urlin = urlin.replace(".FLA","");
	  xx.wi = spinBox->value();
		xx.hi = spinBox_2->value();
		xx.url = urlin;
	  accept();
}
void coloritem()
{
	   bool ok;
	   QRgb col = QColorDialog::getRgba(xx.bg.rgba(),&ok);
     if (!ok) {
        return; 
     }
		 xx.bg = QColor( col );
		 QPixmap pixmap(80, 80);
     pixmap.fill(xx.bg);
	   QIcon colorBG(pixmap);
		 toolButton->setIcon ( colorBG );
}
};






class widget : public QTextEdit
{
	Q_OBJECT
	
public:
	widget(QWidget* parent = 0)
		: QTextEdit(parent)
	{
    FlashItem One;
		          One.url = tr("Your uri from file.");
      
		FlashReference* flahandler = new FlashReference(One,this);
		document()->documentLayout()->registerHandler(QTextCharFormat::UserObject + 1,flahandler);
		QTextCharFormat FlaStruct;
		FlaStruct.setObjectType(QTextCharFormat::UserObject + 1);
    FlaStruct.setProperty(FlashReferenceID,One);
    FlaStruct.setProperty(ObjectName,FlashReferenceID);
		
		textCursor().insertText("Title page...");
		
    textCursor().insertBlock(default_block_style());
		textCursor().insertText(QString(QChar::ObjectReplacementCharacter),FlaStruct);
    textCursor().insertBlock(default_block_style());
    textCursor().insertBlock(default_block_style());
		textCursor().insertText("Hello fla movie reference",default_char_style());
		 textCursor().insertBlock(default_block_style());
		 textCursor().insertBlock(default_block_style());
		textCursor().insertText(QString(QChar::ObjectReplacementCharacter),FlaStruct);
		
	}
  
void contextMenuEvent(QContextMenuEvent *event)
{
    QTextCharFormat currCharForm = textCursor().charFormat();
    QMenu *menu = createStandardContextMenu();
    if (currCharForm.property(ObjectName).toInt() == FlashReferenceID) {
        QVariant xx = currCharForm.property(FlashReferenceID); 
        if (!xx.isNull()) {
					FlashItem fla = xx.value<FlashItem>();
					if (fla.isValid()) {
					menu->addAction(tr("Edit Multimedia Parameter"),this, SLOT(MultimediaEdit()));
					}
        }
    }
		menu->addAction(tr("Retranslate"),this, SLOT(RelinkFlaChunk()));
		menu->addAction(tr("Restore from link"),this, SLOT(Restore()));
    menu->exec(event->globalPos());
    delete menu;
}
  
public slots:
	
void MultimediaEdit() {
	QTextCharFormat currCharForm = textCursor().charFormat();
	if (currCharForm.property(ObjectName).toInt() == FlashReferenceID) {
		       QVariant xx = currCharForm.property(FlashReferenceID); 
        if (!xx.isNull()) {
					  FlashItem fla = xx.value<FlashItem>();
					  if (!fla.isValid()) {
							return;
						}
						qDebug() << "### edititem  " << fla.Tag();
					  MediaAttribute *apiM = new MediaAttribute(fla,this);
					  if (apiM->exec() == QDialog::Accepted ) {
							currCharForm.setProperty(FlashReferenceID,apiM->movie());  
              textCursor().setCharFormat(currCharForm);
						}
				}
	}
}

/* pack from flash to link */
void RelinkFlaChunk() 
{
	/* dont set permission to insert FlashItem on table copy item return image */
	///////int QString::count ( QChar ch, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const
	QTextDocument *d = document()->clone();
	QTextCursor helper = QTextCursor(d);
	int xli = 1;
	for ( QTextBlock block = d->begin(); block.isValid(); block = block.next(), xli++) {
               QTextBlock::iterator li;
				       for (li = block.begin(); !(li.atEnd()); ++li) {
								  QTextFragment lifr = li.fragment();
                  if (lifr.isValid()) {
										 QTextCharFormat currCharForm = lifr.charFormat();
										 QVariant xx = currCharForm.property(FlashReferenceID); 
										 if (!xx.isNull()) {
											  FlashItem fla = xx.value<FlashItem>();
												if (fla.isValid()) {
													////////qDebug() << "### found  " << fla.linktranslate();
													helper.setPosition(lifr.position());
													helper.setPosition(lifr.position() + lifr.length(),QTextCursor::KeepAnchor);
													QTextCharFormat format = default_char_style();
													format.setAnchor(true);
                          format.setAnchorHref(fla.linktranslate()); 
													helper.setCharFormat(format);
												}
										 }
									}
								 
							 }
		
	}
	
	QString hh = d->toHtml("utf-8");
	///////qDebug() << "### result  " << hh;
	setDocument(d);
}

/* from linkt to flash */
void Restore() 
{
	QTextDocument *d = document()->clone();
	
	FlashItem One;
	FlashReference* flahandler = new FlashReference(One,this);
	d->documentLayout()->registerHandler(QTextCharFormat::UserObject + 1,flahandler);
	
	QTextCursor helper = QTextCursor(d);
	int xli = 1;
	for ( QTextBlock block = d->begin(); block.isValid(); block = block.next(), xli++) {
               QTextBlock::iterator li;
				       for (li = block.begin(); !(li.atEnd()); ++li) {
								  QTextFragment fr = li.fragment();
                  if (fr.isValid()) {
										 QTextCharFormat base = fr.charFormat();
										           if (base.isAnchor()) {
																    const QString href = base.anchorHref();
																	  qDebug() << "### link found ..... " << href;
																	  /* default color from link and insert fla flash item to edit */
																    
																    if (href.startsWith("mediafla://")) {
																			  /* translate from link to flash objekt */
																			FlashItem One;
		                                  One.Load(href);
																			QTextCharFormat FlaStruct;
																			FlaStruct.setObjectType(QTextCharFormat::UserObject + 1);
																			FlaStruct.setProperty(FlashReferenceID,One);
																			FlaStruct.setProperty(ObjectName,FlashReferenceID);
																			helper.setPosition(fr.position());
                                      helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
																			helper.setCharFormat(FlaStruct);
																		} else {
																			/* normal link */
																	    base.setForeground(QBrush(_LINK_COLOR_));
                                      base.setUnderlineStyle(QTextCharFormat::SingleUnderline);
																		  helper.setPosition(fr.position());
                                      helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
																	    helper.setCharFormat(base);
																		}
																}
							
									}
								 
							 }
		
	}
	
	setDocument(d);
}



};



int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	widget w;
	w.show();
	return a.exec();
	
}

#include "main.moc"









