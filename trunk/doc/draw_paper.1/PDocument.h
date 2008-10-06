#ifndef PDOCUMENT_H
#define PDOCUMENT_H

#include "PDocConfig.h"
#include <QTextDocument>
#include <QDomDocument>
#include <QTextDocumentFragment>
#include <QDrag>


class StreamDoc
{
public:
    StreamDoc()
            :d(new QBuffer())
    {
        d->open(QIODevice::ReadWrite);
        start();
    }
    ~StreamDoc()
    {
        d->close();
    }
    bool clear()
    {
        d->write(QByteArray());
        return d->bytesAvailable() == 0 ? true : false;
    }
    qint64 write( const QByteArray dat )
    {
        d->write(dat);
        return d->bytesAvailable();
    }

    void start() {
        d->seek(0);
    }
    bool LoadFile( const QString file ) {
        if (clear()) {
            QFile f(file);
            if (f.exists()) {
                if (f.open(QFile::ReadOnly)) {
                    QByteArray xx;
                    xx.append(f.readAll());
                    d->write(xx);
                    f.close();
                    start();
                    return true;
                }
            }
        }
        return false;
    }
    bool PutOnFile( const QString file ) {
        QFile f(file);
        start();
        if (f.open(QFile::WriteOnly)) {
            uint bi = f.write(d->readAll());
            f.close();
            start();
            return bi > 0 ? true : false;
        }
        return false;
    }
    QDomDocument Dom() {
        return doc;
    }
    QBuffer *device() {
        return d;
    }
    bool isValid() {
        return doc.setContent(stream(),false,0,0,0);
    }
    QByteArray stream() {
        return d->data();
    }
    QDomDocument doc;
    QBuffer *d;
};


class QTextEditMimeData : public QMimeData
{
public:
    inline QTextEditMimeData(const QTextDocumentFragment &aFragment) : fragment(aFragment) {}
    virtual QStringList formats() const;
protected:
    virtual QVariant retrieveData(const QString &mimeType, QVariant::Type type) const;
private:
    void setup() const;
    mutable QTextDocumentFragment fragment;
};




class PDocument : public QTextDocument
{
    Q_OBJECT

public:
    PDocument();

    enum PageFormat {
        PPortrait = 1000,
        PLandscape = 2000
    };

    void init();
    QSizeF size();
    QRectF boundingRect() const;
    void style();
    void paintPage( const int index  , QPainter * painter  , const QRectF viewarea );
    QPointF pageIndexTopLeft( const int index  );
    void openFile( const QString file );
    void setHtml ( const QString & html );
    PageFormat pageformat();
    void setFormat( PageFormat e );
    qreal pageFaktor() const;

    PageFormat p_format;
signals:
    void formatChance();
};








#endif // PDOCUMENT_H

