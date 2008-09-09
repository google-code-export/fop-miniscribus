
#include <QtGui>
#include <QTextDocument>
#include <QTextCursor>

class Writteln : public QObject
{
    Q_OBJECT
//
public:
 Writteln()
{
   d = new QTextDocument("This is a table merge sample; ");
}
QTextDocument *document()
{
    QTextCursor cur(d);
    cur.movePosition(QTextCursor::End);
    cur.insertBlock();
    cur.beginEditBlock();
    QTextTableFormat tf; 
    tf.setBorder ( 0.4 );
    tf.setAlignment ( Qt::AlignTop | Qt::AlignHCenter );
    tf.setCellPadding ( 2 );
    tf.setCellSpacing ( 2 );
    tf.setWidth ( QTextLength(QTextLength::PercentageLength,90) );
    
    const int trow = 10;
    const int tcool = 4;
    QTextTable *qtable = cur.insertTable(trow,tcool,tf);
    QString celltext;
    
    QTextTableCell cell;
    
    bool tmake ;
     for (int r = 0; r < r; ++r)  {
         for (int c = 0; c < c; ++c)  {
             
                      if (r == 3 && c == 0) {
                      /* span on 4 cell*/
                       qtable->mergeCells(3,0,1,4);
                       cell = qtable->cellAt(r,c);
                       celltext = QString("r%1c%2").arg(cell.row()).arg(cell.column ());
                          
                       qDebug() << "### celltext " << celltext;
                          
                        QTextCursor tmpcur  = cell.firstCursorPosition();
                        tmpcur.insertBlock();
                        tmpcur.beginEditBlock();
                        tmpcur.insertText(celltext); 
                        tmpcur.endEditBlock();
                          
                      } else if ( r != 3) {
                          cell = qtable->cellAt(r,c);
                         celltext = QString("r%1c%2").arg(cell.row()).arg(cell.column ());
                         qDebug() << "### celltext " << celltext; 
                        QTextCursor tmpcur  = cell.firstCursorPosition();
                            tmpcur.insertBlock();
                            tmpcur.beginEditBlock();
                            tmpcur.insertText(celltext); 
                            tmpcur.endEditBlock();
                          
                      }
         }
     }
    cur.endEditBlock();
    cur.movePosition(QTextCursor::End);
    cur.insertBlock();
    cur.beginEditBlock();
    cur.insertText("Table end ......");
    cur.endEditBlock();
    
    
    return d->clone();
}
protected:
private:
    QTextDocument *d;
signals:
public slots:

};


int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    Writteln *dd = new Writteln();
    QTextEdit t;
    t.setDocument(dd->document());
    t.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    return app.exec();
}

#include "main.moc"











