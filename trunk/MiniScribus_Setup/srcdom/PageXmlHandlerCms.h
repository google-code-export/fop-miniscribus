#ifndef PAGEXMLHANDLER_H
#define PAGEXMLHANDLER_H

#include "xmlhighlighter.h"
#include "richdoc_structure.h"
#include "pics_structure.h"
#include "swisslang_structure.h"
#include "ModelSwap.h"
#include "cmspage_structure.h"





class PageXmlHandler : public QObject
{
     Q_OBJECT
//
public:
   PageXmlHandler();
   PageXmlHandler( QDomDocument newformat , QDomDocument oldformat );
   inline QStandardItemModel *Fmodel() { return model; }
   void UpdateTag( const QString name , const QString value , bool clone = false );
   enum SAVEVARIANT { TAGID = Qt::UserRole + 1 ,
                     OLDVALUE = Qt::UserRole + 2 , 
                     MINIMUMCHAR = Qt::UserRole + 3 ,
                     T_FORMAT = Qt::UserRole + 4 };
   QString current_title();
   QDomDocument doc_a;
   QDomDocument doc_b;
   QColor grundierung;
   protected:
   bool SwissFormatValid;
   bool CmsFormatValid;
   
   SwissLang Page_Base;
   CMSDoc    CMS_Base;
   
   QStringList Language_B;
   Qt::ItemFlags flags_yes;
   Qt::ItemFlags flags_no;
   
   void ModelBase();  /* new format model */
   QStandardItemModel *model;  /* new format model */
   QList<QStandardItem *> TagValue( const QString shortname , const QString HumanName );
   QList<QStandardItem *> TagName( const QString shortname , const QString HumanName , const int mv = 0 );
private:
   QFont bold_base_font;
signals:
   void updated();
   void datachanged(QString);
public slots:
   void UpdateStream( const QModelIndex index );

};


class AttriDelegate : public QItemDelegate
{
    Q_OBJECT
    
public:
AttriDelegate( QObject *parent  )
    :QItemDelegate(parent)
{

}
void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  /* persisten editor here */
   const int format = index.data(PageXmlHandler::T_FORMAT).toInt();
   QPen line_pen(QBrush(Qt::red),1,Qt::DotLine,Qt::RoundCap);
   QPen line_pen2(QBrush(Qt::blue),1,Qt::DotLine,Qt::RoundCap);
   
   if ( format == 2 || format == 1 ) {
    painter->save();
    painter->setBrush(QBrush(QColor("#faffd7")));
       if (format == 2) {
		   painter->setPen(line_pen);
       } else {
       painter->setPen(line_pen2); 
       }
    painter->drawRect(option.rect);
		painter->restore(); 
   }
   
   QItemDelegate::paint(painter,option,index);
}

QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
        QString tag = index.data(PageXmlHandler::TAGID).toString();
         const int format = index.data(PageXmlHandler::T_FORMAT).toInt();
        if (format !=1 && tag == "active") {
        qDebug() << "### format " << format;
           QSpinBox *editor = new QSpinBox(parent);      
           editor->setMaximum(100);
           return editor;
           
        } else if ( format !=1 && tag == "worktext" ) {
            QComboBox *editor = new QComboBox(parent);
            editor->addItem("false");
            editor->addItem("true");
            return editor;           

        }  else if ( format !=1 && tag == "bgcolor" ) {



        }  else if (format !=1 && tag == "start" || format !=1 && tag == "stop") {
           
           QDateTimeEdit *editor = new QDateTimeEdit(parent);
           editor->setDisplayFormat("dd.MM.yyyy");
           editor->setCalendarPopup(true);
           return editor;
           
        }  else {
        QLineEdit *editor = new QLineEdit(parent); 
        return editor;
        }
}
void setEditorData(QWidget *editor, const QModelIndex &index) const 
{
    if (!index.isValid()) {
        return;
    } else {
       
       QString dateuser = index.model()->data(index, Qt::DisplayRole).toString();
       
         QString tag = index.data(PageXmlHandler::TAGID).toString();
         const int format = index.data(PageXmlHandler::T_FORMAT).toInt();
       
         if (format !=1 && tag == "active") {
         QSpinBox *editorax = qobject_cast<QSpinBox *>(editor);
         editorax->setValue(index.model()->data(index, Qt::DisplayRole).toInt());
            
         } else if ( format !=1 && tag == "worktext" ) {
            QComboBox *editorax = qobject_cast<QComboBox *>(editor);
            if (dateuser !="0") {
               editorax->setCurrentIndex(1); 
            }
            
         }  else if (format !=1 && tag == "start" || format !=1 && tag == "stop") {
            
            
            QDateTimeEdit *editorrun = qobject_cast<QDateTimeEdit *>(editor);
            
            if (dateuser !="0") {
            editorrun->setDate(QDate::fromString(index.model()->data(index, Qt::EditRole).toString(), "dd.MM.yyyy")); 
            } else {
            editorrun->setDate(QDate::currentDate ()); 
            }
                       
         }  else {
         QLineEdit *editorax = qobject_cast<QLineEdit *>(editor);
         editorax->setText(index.model()->data(index, Qt::DisplayRole).toString());
         }
    }
}



void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const 
{
    if (index.column() == 0) {
        return;
     }  else {
         QString tag = index.data(PageXmlHandler::TAGID).toString();
         const int format = index.data(PageXmlHandler::T_FORMAT).toInt();
        if (format !=1 && tag == "active") {
           QSpinBox *editorax = qobject_cast<QSpinBox *>(editor);
           model->setData(index,editorax->text());
           
         } else if ( format !=1 && tag == "worktext" ) {
            QComboBox *editorax = qobject_cast<QComboBox *>(editor);
            model->setData(index,editorax->currentIndex());
            
         } else if (format !=1 && tag == "start" || format !=1 && tag == "stop") {
            
              QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
              model->setData(index,dateEditor->date().toString("dd.MM.yyyy"));
            
        }  else {
        
        QLineEdit *editorax = qobject_cast<QLineEdit *>(editor);
        model->setData(index,editorax->text());
        }
    }
}


signals:
public slots:
private:
};


class PageFirstTree : public QTreeView
{
     Q_OBJECT
//
public:
PageFirstTree( PageXmlHandler * device = 0 , QWidget * parent = 0 );
void swapdevice( PageXmlHandler * device );
protected:
void init();
void dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight );
PageXmlHandler *dev;
signals:
public slots:
   void ActiveTre( QModelIndex index );
   void reloads();

};

















//
#endif // PAGEXMLHANDLER_H

