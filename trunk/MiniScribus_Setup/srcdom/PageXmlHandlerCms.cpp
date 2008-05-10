#include "PageXmlHandlerCms.h"



PageXmlHandler::PageXmlHandler( QDomDocument newformat , QDomDocument oldformat )
  :grundierung(QColor(Qt::white))
{
	Page_Base.Load_Doc(newformat);  /* valid or not */
	Page_Base.save();
	Language_B = AvaiableLanguage();
	doc_a = newformat;
	CMS_Base.Load_Stream(oldformat.toByteArray(1));
	CMS_Base.openfile_small(Page_Base.doc);
	doc_b = oldformat;
	CMS_Base.RewriteFrominit();
	ModelBase();
}

PageXmlHandler::PageXmlHandler()
  :grundierung(QColor(Qt::white))
{
	//////SwissLang defaultpage;
	////Page_Base = defaultpage;  /* valid or not */
	Language_B = AvaiableLanguage();
	Page_Base.save();
	doc_a = Page_Base.doc;
	CMS_Base.openfile_small(Page_Base.doc);
	CMS_Base.RewriteFrominit();  /* fill doc 1 */
	doc_b = CMS_Base.doc1;;
	ModelBase();
}
QString PageXmlHandler::current_title()
{
 return Page_Base.GetTag("tit_no").toString();
}

void PageXmlHandler::ModelBase()
{
	bold_base_font = qApp->font();
	flags_yes = Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	flags_no = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	bold_base_font.setBold ( true );
	model = new QStandardItemModel();
	   
	       QStandardItem *Hitem1 = new QStandardItem(tr("Name"));
	       QStandardItem *Hitem2 = new QStandardItem(tr("Value"));
         Hitem1->setData(bold_base_font,Qt::FontRole);
	       Hitem2->setData(bold_base_font,Qt::FontRole);
         model->setHorizontalHeaderItem(0,Hitem1);
	       model->setHorizontalHeaderItem(1,Hitem2);
	
	       model->invisibleRootItem()->appendRow(TagValue("cat",tr("Category:")));
	       model->invisibleRootItem()->appendRow(TagName("tit_",tr("Title:"),5));
	       model->invisibleRootItem()->appendRow(TagName("desc_",tr("Description:"),15));
	       model->invisibleRootItem()->appendRow(TagName("note_",tr("Note:")));
	       model->invisibleRootItem()->appendRow(TagName("autor_",tr("Autor:")));
				 model->invisibleRootItem()->appendRow(TagValue("active",tr("Priority:")));
	       model->invisibleRootItem()->appendRow(TagValue("start",tr("Start time:")));
	       model->invisibleRootItem()->appendRow(TagValue("stop",tr("Stop time:")));
	       model->invisibleRootItem()->appendRow(TagValue("worktext",tr("Source work:")));
				 model->invisibleRootItem()->appendRow(TagValue("bgcolor",tr("Background color:")));
	
}

QList<QStandardItem *> PageXmlHandler::TagValue( const QString shortname , const QString HumanName )
{
	QStandardItem *noedit = new QStandardItem(HumanName);
	noedit->setFlags(flags_no);
	noedit->setData(bold_base_font,Qt::FontRole);
	noedit->setData(grundierung,Qt::BackgroundColorRole);
	
	
	
	QList<QStandardItem *> diritto;
	
	
	QStandardItem *Hitem2 = new QStandardItem(CMS_Base.Root_Tag(shortname).toString());
				      Hitem2->setFlags(flags_yes);
              Hitem2->setToolTip ( shortname );
				      Hitem2->setData(shortname,TAGID);
				      Hitem2->setData(2,T_FORMAT);
	            Hitem2->setData(CMS_Base.Root_Tag(shortname),OLDVALUE);
	
	 diritto.append(noedit);
	 diritto.append(Hitem2);
	
	return diritto;
}



void PageXmlHandler::UpdateTag( const QString name , const QString value , bool clone )
{
	if (Page_Base.GetTag(name).toString() != value) { 
		 if (clone) {
			Page_Base.cloneitem(name,QVariant(value));
		 } else {
	   Page_Base.SetValue(name,QVariant(value));
		 }
		 /////////////qDebug() << "### update start 55 " << value;
		 Page_Base.savefile( "muster.xml"  );  /* debug to se its */
		 ModelBase();  /* remake model */
		 emit updated();
		 emit datachanged(Page_Base.save());
	}
	
	
}




void PageXmlHandler::UpdateStream( const QModelIndex index )
{
	  if (!index.isValid()) {
    return;
    }
		const int format = index.data(T_FORMAT).toInt();
		QString oldvar = index.data(OLDVALUE).toString();
		QVariant value = index.data(Qt::DisplayRole);
		QString tag = index.data(TAGID).toString();
		qDebug() << "### UpdateStream " << tag << "/" << value;
	
}



QList<QStandardItem *> PageXmlHandler::TagName( const QString shortname , const QString HumanName , const int mv )
{
	QStandardItem *noedit = new QStandardItem();
	noedit->setFlags(flags_no);
	noedit->setData(5,T_FORMAT);
	
	QStandardItem *Hitem0 = new QStandardItem(HumanName);
	               Hitem0->setFlags(flags_no);
	               Hitem0->setData(bold_base_font,Qt::FontRole);
	               Hitem0->setData(grundierung,Qt::BackgroundColorRole);
	QList<QStandardItem *> diritto;
	QList<QStandardItem *> lines;
	diritto.append(Hitem0);
	diritto.append(noedit);
	
	    for (int i = 0; i < Language_B.size(); ++i)  {
				QString shortn = Language_B.at(i).toLower();
				lines.clear();
				shortn = shortn.left(2);
				const QString TagValueName = shortname+shortn;
				const QString value = Page_Base.GetTag(TagValueName).toString();
				     QStandardItem *Hitem1 = new QStandardItem(Language_B.at(i)+":");
							Hitem1->setFlags(flags_no);
				      Hitem1->setData(bold_base_font,Qt::FontRole);
	            QStandardItem *Hitem2 = new QStandardItem(value);
				      Hitem2->setFlags(flags_yes);
              Hitem2->setToolTip ( TagValueName );
				      Hitem2->setData(TagValueName,TAGID);
				      Hitem2->setData(1,T_FORMAT);
				      Hitem2->setData(mv,MINIMUMCHAR);
				      Hitem2->setData(value,OLDVALUE);
				      lines.append(Hitem1);
				      lines.append(Hitem2);
				      Hitem0->appendRow(lines);
			}
	
	return diritto;
}






PageFirstTree::PageFirstTree(  PageXmlHandler * device , QWidget* parent )
	: QTreeView( parent ),dev(device)
{
	setItemDelegateForColumn(1,new AttriDelegate(this));
  init();
}
void PageFirstTree::swapdevice( PageXmlHandler * device )
{
	dev = device;
	setWindowTitle ( dev->current_title()  );
	init();
}

void PageFirstTree::init()
{
	if (dev) {
	dev->disconnect(this);
	///////connect(this, SIGNAL(clicked(QModelIndex) ), this , SLOT(ActiveTre(QModelIndex) ));
	connect(dev, SIGNAL(updated() ), this , SLOT(reloads() ));
	reloads();
	setWindowTitle ( dev->current_title()  );
	}
}


void PageFirstTree::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
	   if (topLeft == bottomRight) {
			 
			 QAbstractItemModel *mone = const_cast<QAbstractItemModel*>(topLeft.model());
			 
			 const int format = topLeft.data(PageXmlHandler::T_FORMAT).toInt();
			 if (format != 1) {
			  dev->UpdateStream(topLeft);
				return;
			 }
			 
			 const int minimsize = topLeft.data(PageXmlHandler::MINIMUMCHAR).toInt();
			 QString oldvar = topLeft.data(PageXmlHandler::OLDVALUE).toString();
			 
			 
			 QString tag = topLeft.data(PageXmlHandler::TAGID).toString();
			 const QString cellTxt = topLeft.data(Qt::DisplayRole).toString();
			 if (minimsize < 1) {
			 dev->UpdateTag(tag,cellTxt);
			 } else if (minimsize > 0) {
				 
				  if (cellTxt.size() >= minimsize) {
						dev->UpdateTag(tag,cellTxt);
					} else {
						QMessageBox::warning(0,tr("Format error"),tr("Minimum %1 chars to save!").arg(minimsize));
						mone->setData(topLeft,oldvar,Qt::DisplayRole);
						///////bottomRight.setData(oldvar,Qt::EditRole);
					  return;
					}
				 
			 }
		 }
		 
		 if (dev) {
			setWindowTitle ( dev->current_title()  );
		 }
}

void PageFirstTree::reloads()
{
	  qDebug() << "### reloads ";
    setModel(dev->Fmodel());
	  expandAll();
    ////////setAlternatingRowColors (true);
}

void PageFirstTree::ActiveTre( const QModelIndex index )
{
    if (!index.isValid()) {
    return;
    }
    QVariant vv = index.data(PageXmlHandler::TAGID);
    if (!vv.isNull() ) {
       QString tag = vv.toString();
       const QString cellTxt = index.data(Qt::DisplayRole).toString();
       ////////////////qDebug(QString("tag %2 %1 ok .......").arg(cellTxt).arg(tag).toAscii());
    }
}








