#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include "scribeapi.h"
#include "SessionManager.h"


/*

#include <QtGui>

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );

    QTextDocument d("This is some text");
    QTextEdit t;
    t.setDocument(&d);
    t.selectAll();

    const qreal spacingnow = t.textCursor().charFormat().fontLetterSpacing();
    bool ok;
    qreal space = QInputDialog::getDouble(0, QObject::tr("Font Letter Spacing"),
            QObject::tr("Space:"),spacingnow,90,2000, 2, &ok);
    if (space > 0 && ok) {
        QTextCursor c = t.textCursor();
        QTextCharFormat format = c.charFormat();
        qDebug() << "Setting spacing to" << space;
        format.setFontLetterSpacing(space);
        c.setCharFormat(format);
    }

    t.show();


    QTextDocument * doc = d.clone();


    for (QTextBlock srcBlock = d.firstBlock(), dstBlock = doc->firstBlock();
            srcBlock.isValid() && dstBlock.isValid();
            srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
        dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
    }

    QTextEdit t2;
    t2.setDocument(doc);
    t2.show();

    return app.exec();
}

*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApiSession *sx = ApiSession::instance();
    
    Panel *w = new Panel();
    w->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
