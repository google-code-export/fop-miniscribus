#include "BookTree.h"



using namespace ApacheFop;




BookTree::~BookTree()
{
   
}


BookTree::BookTree( QWidget* parent )
	: QTreeView( parent )
{
   onRowCurrent =-1;
   currentLinker.clear();
   xmlmode = new BookMarkModelRead("a.fo");
   QAbstractItemModel *pma = xmlmode->bookModel();
   insertModel( pma , xmlmode->linkList() );
   
    
}

void BookTree::insertModel( QAbstractItemModel *m , QStringList avaiablelist )
{
   setModel(m);
   if (avaiablelist.size() > 0) {
    /* session append others to delegate */
   currentLinker = avaiablelist;
   }
   setItemDelegate(new BooksDelegate(this,currentLinker));
   expandAll();
   setColumnWidth (0,180);
   setColumnWidth (2,25);
   setDragEnabled ( false );
    setAcceptDrops ( false);
    setDragDropOverwriteMode ( false );
   setIndentation (15);
   setIconSize ( QSize(11,11) );
    onRowCurrent =-1;
    ///////setColumnHidden (2,true);
   connect(this, SIGNAL(clicked(QModelIndex)), this , SLOT(activeTree(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this , SLOT(activeTree(QModelIndex)));
    connect(this, SIGNAL(entered(QModelIndex)), this , SLOT(activeTree(QModelIndex)));
    connect(this, SIGNAL(activated(QModelIndex)), this , SLOT(activeTree(QModelIndex)));
    
}

void BookTree::activeTree(const QModelIndex  index )
{
    if (!index.isValid()) {
      return;
    }
    QModelIndex firstcolumn = index.sibling(index.row(),0);
    if (index.column() != 0 && firstcolumn.column() == 0) {
    touchCell = firstcolumn;  
    } else {
    touchCell = index;
    }
    onRowCurrent = touchCell.row();
    ///////////////const QString atext = touchCell.data(Qt::DisplayRole).toString();
    ///////////qDebug() << "### click  -> " << atext << "-" << touchCell.internalId() << "-" << touchCell.column();
}

void BookTree::insertSubOnCurrent()
{
    if (onRowCurrent == -1) {
    /* select a item !!! */
    return;
    }
    if (!touchCell.isValid()) {
    /* select a item !!! */
      return;
    }
    const int level = touchCell.data(Qt::UserRole).toInt();
    QModelIndex ilevdata = touchCell.sibling(touchCell.row(),touchCell.column() + 2);
    const int currlevel = ilevdata.data(Qt::DisplayRole).toInt();
    
    QStandardItemModel *pm = qobject_cast<QStandardItemModel *>(model());
    QStandardItem *ix =  pm->itemFromIndex(touchCell);
    QList<QStandardItem *> oLine = xmlmode->newLine(currlevel + 1);
    ix->appendRow(oLine);
    
    QStandardItemModel *pam = qobject_cast<QStandardItemModel *>(model());
    ModelDomRead *ponte = new ModelDomRead(pam);
    QByteArray dataxml = ponte->Domdocument()->toByteArray(1);
    xmlmode->openStream(dataxml);
    QAbstractItemModel *pma = xmlmode->bookModel();
    insertModel( pma , xmlmode->linkList() );
    delete &touchCell;
}


void BookTree::removeOnCurrent()
{
    if (onRowCurrent == -1) {
    /* select a item !!! */
    return;
    }
    if (!touchCell.isValid()) {
    /* select a item !!! */
      return;
    }
    const int r = touchCell.row();
    QStandardItemModel *pm = qobject_cast<QStandardItemModel *>(model());
    const QString removelinkt = touchCell.data(Qt::UserRole).toString();
    //////qDebug() << "### removelinkt " << removelinkt;
    ModelDomRead *ponte = new ModelDomRead(pm,Mommit,removelinkt);
    QByteArray dataxml = ponte->Domdocument()->toByteArray(1);
    ////////qDebug() << "### dataxml" << dataxml;
    xmlmode->openStream(dataxml);
    QAbstractItemModel *pma = xmlmode->bookModel();
    insertModel( pma , xmlmode->linkList() );
    delete &touchCell;
}




void BookTree::contextMenuEvent ( QContextMenuEvent * e )
{
    QMenu *menu = new QMenu();
    menu->addAction(tr( "New root item" ), this , SLOT( InsertOnRoot() ) );
    menu->addAction(tr( "New sub item here" ), this , SLOT( insertSubOnCurrent() ) );
    menu->addAction(tr( "Remove this here" ), this , SLOT( removeOnCurrent() ) );
    menu->addAction(tr( "Display xml bookmark" ), this , SLOT( showXmlBooks() ) );
    menu->exec( QCursor::pos() );
    delete menu;
}

void BookTree::InsertOnRoot()
{
    QStandardItemModel *pm = qobject_cast<QStandardItemModel *>(model());
    QList<QStandardItem *> oLine = xmlmode->newLine(0);
    pm->invisibleRootItem()->appendRow(oLine);
    insertModel( pm );
    ////////////qDebug() << "### InsertOnRoot    ";
}


void BookTree::showXmlBooks()
{
    
    QStandardItemModel *pm = qobject_cast<QStandardItemModel *>(model());
    ModelDomRead *ponte = new ModelDomRead(pm);
    XMLTextEdit *sHtml = new XMLTextEdit(0);
	sHtml->setWindowFlags ( Qt::Window );
	sHtml->setMinimumSize ( 450 , 500 );
    sHtml->setPlainText( ponte->Domdocument()->toString(1) );
    sHtml->show();
    
}







int BookTree::maximumID()
{
    
}

















