/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTEXTOBJECT_H
#define QTEXTOBJECT_H

#include <QtCore/qobject.h>
#include <QtGui/qtextformat.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QTextObjectPrivate;
class QTextDocument;
class QTextDocumentPrivate;
class QTextCursor;
class QTextBlock;
class QTextFragment;
class QTextLayout;
class QTextList;

class Q_GUI_EXPORT QTextObject : public QObject
{
    Q_OBJECT

protected:
    explicit QTextObject(QTextDocument *doc);
    ~QTextObject();

    void setFormat(const QTextFormat &format);

public:
    QTextFormat format() const;
    int formatIndex() const;

    QTextDocument *document() const;

    int objectIndex() const;

    QTextDocumentPrivate *docHandle() const;

protected:
    QTextObject(QTextObjectPrivate &p, QTextDocument *doc);

private:
    Q_DECLARE_PRIVATE(QTextObject)
    Q_DISABLE_COPY(QTextObject)
    friend class QTextDocumentPrivate;
};

class QTextBlockGroupPrivate;
class Q_GUI_EXPORT QTextBlockGroup : public QTextObject
{
    Q_OBJECT

protected:
    explicit QTextBlockGroup(QTextDocument *doc);
    ~QTextBlockGroup();

    virtual void blockInserted(const QTextBlock &block);
    virtual void blockRemoved(const QTextBlock &block);
    virtual void blockFormatChanged(const QTextBlock &block);

    QList<QTextBlock> blockList() const;

protected:
    QTextBlockGroup(QTextBlockGroupPrivate &p, QTextDocument *doc);
private:
    Q_DECLARE_PRIVATE(QTextBlockGroup)
    Q_DISABLE_COPY(QTextBlockGroup)
    friend class QTextDocumentPrivate;
};

class Q_GUI_EXPORT QTextFrameLayoutData {
public:
    virtual ~QTextFrameLayoutData();
};

class QTextFramePrivate;
class Q_GUI_EXPORT QTextFrame : public QTextObject
{
    Q_OBJECT

public:
    explicit QTextFrame(QTextDocument *doc);
    ~QTextFrame();

    inline void setFrameFormat(const QTextFrameFormat &format);
    QTextFrameFormat frameFormat() const { return QTextObject::format().toFrameFormat(); }

    QTextCursor firstCursorPosition() const;
    QTextCursor lastCursorPosition() const;
    int firstPosition() const;
    int lastPosition() const;

    QTextFrameLayoutData *layoutData() const;
    void setLayoutData(QTextFrameLayoutData *data);

    QList<QTextFrame *> childFrames() const;
    QTextFrame *parentFrame() const;

    class Q_GUI_EXPORT iterator {
        QTextFrame *f;
        int b;
        int e;
        QTextFrame *cf;
        int cb;

        friend class QTextFrame;
        friend class QTextTableCell;
        friend class QTextDocumentLayoutPrivate;
        iterator(QTextFrame *frame, int block, int begin, int end);
    public:
        iterator();
        iterator(const iterator &o);
        iterator &operator=(const iterator &o);

        QTextFrame *parentFrame() const { return f; }

        QTextFrame *currentFrame() const;
        QTextBlock currentBlock() const;

        bool atEnd() const { return !cf && cb == e; }

        inline bool operator==(const iterator &o) const { return f == o.f && cf == o.cf && cb == o.cb; }
        inline bool operator!=(const iterator &o) const { return f != o.f || cf != o.cf || cb != o.cb; }
        iterator &operator++();
        inline iterator operator++(int) { iterator tmp = *this; operator++(); return tmp; }
        iterator &operator--();
        inline iterator operator--(int) { iterator tmp = *this; operator--(); return tmp; }
    };

    friend class iterator;
    // more Qt
    typedef iterator Iterator;

    iterator begin() const;
    iterator end() const;

protected:
    QTextFrame(QTextFramePrivate &p, QTextDocument *doc);
private:
    friend class QTextDocumentPrivate;
    Q_DECLARE_PRIVATE(QTextFrame)
    Q_DISABLE_COPY(QTextFrame)
};
Q_DECLARE_TYPEINFO(QTextFrame::iterator, Q_MOVABLE_TYPE);

inline void QTextFrame::setFrameFormat(const QTextFrameFormat &aformat)
{ QTextObject::setFormat(aformat); }

class Q_GUI_EXPORT QTextBlockUserData {
public:
    virtual ~QTextBlockUserData();
};

class Q_GUI_EXPORT QTextBlock
{
    friend class QSyntaxHighlighter;
public:
    inline QTextBlock(QTextDocumentPrivate *priv, int b) : p(priv), n(b) {}
    inline QTextBlock() : p(0), n(0) {}
    inline QTextBlock(const QTextBlock &o) : p(o.p), n(o.n) {}
    inline QTextBlock &operator=(const QTextBlock &o) { p = o.p; n = o.n; return *this; }

    inline bool isValid() const { return p != 0 && n != 0; }

    inline bool operator==(const QTextBlock &o) const { return p == o.p && n == o.n; }
    inline bool operator!=(const QTextBlock &o) const { return p != o.p || n != o.n; }
    inline bool operator<(const QTextBlock &o) const { return position() < o.position(); }

    int position() const;
    int length() const;
    bool contains(int position) const;

    QTextLayout *layout() const;
    void clearLayout();
    QTextBlockFormat blockFormat() const;
    int blockFormatIndex() const;
    QTextCharFormat charFormat() const;
    int charFormatIndex() const;

    QString text() const;

    const QTextDocument *document() const;

    QTextList *textList() const;

    QTextBlockUserData *userData() const;
    void setUserData(QTextBlockUserData *data);

    int userState() const;
    void setUserState(int state);

    int revision() const;
    void setRevision(int rev);

    bool isVisible() const;
    void setVisible(bool visible);

    int blockNumber() const;
    int firstLineNumber() const;

    void setLineCount(int count);
    int lineCount() const;

    class Q_GUI_EXPORT iterator {
        const QTextDocumentPrivate *p;
        int b;
        int e;
        int n;
        friend class QTextBlock;
        iterator(const QTextDocumentPrivate *priv, int begin, int end, int f) : p(priv), b(begin), e(end), n(f) {}
    public:
        iterator() : p(0), b(0), e(0), n(0) {}
        iterator(const iterator &o) : p(o.p), b(o.b), e(o.e), n(o.n) {}

        QTextFragment fragment() const;

        bool atEnd() const { return n == e; }

        inline bool operator==(const iterator &o) const { return p == o.p && n == o.n; }
        inline bool operator!=(const iterator &o) const { return p != o.p || n != o.n; }
        iterator &operator++();
        inline iterator operator++(int) { iterator tmp = *this; operator++(); return tmp; }
        iterator &operator--();
        inline iterator operator--(int) { iterator tmp = *this; operator--(); return tmp; }
    };

    // more Qt
    typedef iterator Iterator;

    iterator begin() const;
    iterator end() const;

    QTextBlock next() const;
    QTextBlock previous() const;

    inline QTextDocumentPrivate *docHandle() const { return p; }
    inline int fragmentIndex() const { return n; }

private:
    QTextDocumentPrivate *p;
    int n;
    friend class QTextDocumentPrivate;
    friend class QTextLayout;
};

Q_DECLARE_TYPEINFO(QTextBlock, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QTextBlock::iterator, Q_MOVABLE_TYPE);


class Q_GUI_EXPORT QTextFragment
{
public:
    inline QTextFragment(const QTextDocumentPrivate *priv, int f, int fe) : p(priv), n(f), ne(fe) {}
    inline QTextFragment() : p(0), n(0), ne(0) {}
    inline QTextFragment(const QTextFragment &o) : p(o.p), n(o.n), ne(o.ne) {}
    inline QTextFragment &operator=(const QTextFragment &o) { p = o.p; n = o.n; ne = o.ne; return *this; }

    inline bool isValid() const { return p && n; }

    inline bool operator==(const QTextFragment &o) const { return p == o.p && n == o.n; }
    inline bool operator!=(const QTextFragment &o) const { return p != o.p || n != o.n; }
    inline bool operator<(const QTextFragment &o) const { return position() < o.position(); }

    int position() const;
    int length() const;
    bool contains(int position) const;

    QTextCharFormat charFormat() const;
    int charFormatIndex() const;
    QString text() const;

private:
    const QTextDocumentPrivate *p;
    int n;
    int ne;
};

Q_DECLARE_TYPEINFO(QTextFragment, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

QT_END_HEADER

#endif // QTEXTOBJECT_H
