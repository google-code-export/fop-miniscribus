#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QColor>
#include <QTextEdit>
#include <QtCore>
#include <QtGui>
#include <QtGui>
#include <QtCore>
#include <QWidget>
#include <QPair>
#include <QTextDocument>
#include <QPair>
#include <QMap>
#include <QDateTime>
#include <QFile>
#include <QtCore> 
#include <QCoreApplication>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <QInputDialog>
#include <QMenu>
#include <QString>
#include <QtDebug>
#include <QDebug> 
#include <QDesktopServices>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QObject>
#include <QDateTime>
#include <QDate>
#include <QImageReader>
#include <QPixmap>
#include <QSettings>
#include <QTimer>
#include <QProgressDialog>
#include <QPainter>
#include <QPixmap>
#include <QUrl>
#include <QColorDialog>
#include <QMessageBox>
#include <QWorkspace>
#include <QDomDocument>
#include <QtDebug>
#include <QDebug> 
#include <QSettings>
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBrowser>
#include <QTextBlockFormat>
#include <QTextFrame>
#include <QTextTable>



class XmlHighlighter : public QSyntaxHighlighter
{
public:
	XmlHighlighter(QObject* parent);
	XmlHighlighter(QTextDocument* parent);
	XmlHighlighter(QTextEdit* parent);
	~XmlHighlighter();

    enum HighlightType
	{
		SyntaxChar,
        ElementName,
		Comment,
		AttributeName,
		AttributeValue,
		Error,
		Other
	};
  
	void setHighlightColor(HighlightType type, QColor color, bool foreground = true);
	void setHighlightFormat(HighlightType type, QTextCharFormat format);

protected:
	void highlightBlock(const QString& rstrText);
	int  processDefaultText(int i, const QString& rstrText);

private:
	void init();

	QTextCharFormat fmtSyntaxChar;
    QTextCharFormat fmtElementName;
	QTextCharFormat fmtComment;
	QTextCharFormat fmtAttributeName;
	QTextCharFormat fmtAttributeValue;
	QTextCharFormat fmtError;
	QTextCharFormat fmtOther;

	enum ParsingState
	{
		NoState = 0,
		ExpectElementNameOrSlash,
		ExpectElementName,
		ExpectAttributeOrEndOfElement,
		ExpectEqual,
		ExpectAttributeValue
	};

	enum BlockState
	{
		NoBlock = -1,
		InComment,
		InElement
	};

	ParsingState state;
};





class XMLTextEdit : public QTextEdit
{
     Q_OBJECT
//
public:
XMLTextEdit( QWidget * parent = 0 );
bool Conform();
QDomDocument xml_document();
inline QString text() const 
{
  return QTextEdit::toPlainText();
}
QMenu *createOwnStandardContextMenu();
protected:
  void contextMenuEvent ( QContextMenuEvent * e );
  bool event( QEvent *event );
private:
  XmlHighlighter *highlight;
signals:
public slots:
  void Syntaxcheck();
  void setPlainText( const QString txt );
};

















#endif // XMLHIGHLIGHTER_H
