#ifndef ANIMATIONWRITER_H
#define ANIMATIONWRITER_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>

#include <QImage>
#include <QIODevice>
#include <png.h>






class AnimationWriterData 
{
public:
    AnimationWriterData(QIODevice* d) : dev(d)
    {
	  framerate = 1000;
    }
    void setFrameRate(int d) { framerate = d; }
    virtual ~AnimationWriterData() { }
    virtual void setImage(const QImage& src)=0;
    virtual bool canCompose() const { return false; }
    virtual void composeImage(const QImage&, const QPoint& ) { }
		
protected:
    int framerate;
    QIODevice* dev;
};





class AnimationWriter
{
public:
    AnimationWriter( const QString& filename, const char* format="MNG" );
    ~AnimationWriter();
    bool okay() const;
    void setFrameRate(int);
    void appendBlankFrame();
    void appendFrame(const QImage&);
    void appendFrame(const QImage&, const QPoint& offset);

private:
    QImage prev;
    QIODevice* dev;
    AnimationWriterData* d;
};






//
#endif // ANIMATIONWRITER_H

