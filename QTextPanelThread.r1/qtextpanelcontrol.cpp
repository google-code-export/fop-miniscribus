#include "qtextpanelcontrol.h"
#include "qtextpanelimage.h"
#include "qtextpaneltable.h"
#include "qtextpanelmargin.h"

static QRectF boundingRectOfFrame(const QTextCursor &cursor)
{
	QRectF r;
	QTextFrame *frame = cursor.currentFrame();
	return frame->document()->documentLayout()->frameBoundingRect(frame);
}


static void drawPageShadow( QPainter * painter , const int index , PanelPageSize  e )
{
    const QRectF pagen =  e.PageExternal(index);
    painter->save();
    QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
    QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
    painter->fillRect(rightShadow, Qt::darkGray);
    painter->fillRect(bottomShadow, Qt::darkGray);
    painter->restore();
}


static void drawPageGround( QPainter * painter , const int index , PanelPageSize  e )
{
    const QRectF pagen =  e.PageExternal(index);
    painter->save();
    painter->setBrush(QColor(Qt::white));
    painter->setPen(QPen(Qt::black,0.3));
    painter->drawRect(pagen);
    painter->restore();
}


QMap<QString,QVariant> loadResourceImage( const QTextDocument *doc )
{
    QMap<QString,QVariant> imagelist;
    QVector<QTextFormat> iter = doc->allFormats();
         for (int i = 0; i < iter.size(); ++i) {
             QTextFormat pline = iter.at(i);
             if (pline.isImageFormat() ) {
                 QTextImageFormat ipic = pline.toImageFormat();
                 /////////////qDebug() << "### ipic  ->" << ipic.name();
                 const QVariant xx = doc->resource(QTextDocument::ImageResource,QUrl(ipic.name()));
                 if (!xx.isNull()) {
                     imagelist.insert(ipic.name(),xx);
                     /////QImage qi = xx.value<QImage>();
                     //////QPixmap qp = xx.value<QPixmap>();
                 }                     
             }  
         }
   return imagelist;
}








/* contains link on block */
static bool HavingLink(const QTextBlock para)
{
	QTextBlock::iterator li;
	for (li = para.begin(); !(li.atEnd()); ++li)
	{
		QTextFragment lifrag = li.fragment();
		if (lifrag.isValid())
		{
			const QTextCharFormat format = lifrag.charFormat();
			if (format.isAnchor())
			{
				return true;
			}
		}
	}
	return false;
}

QTextPanelControl::QTextPanelControl(DisplayModus _modus_)
		: QObject(),_d(new QTextDocument),PageTotal(1),eventline(0),timeline(0),
		OnPageClick(1),cursortime(true),cursorOn(false),overwriteMode(false),FullDocSelect(false),PlayCursorMode(false)
{
	SceneTimerLine = 0;
	_d->setUndoRedoEnabled(false);
	Modus = _modus_;
	QTextOption opt;
	opt.setUseDesignMetrics(true);
	opt.setTabStop(8);
	opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	_d->setDefaultTextOption(opt);
	Page_Width = QTextPanelData::instance()->CurrentPageFormat().width();
	Page_Height = 22.5;
	Page_Edit_Rect =  QRectF(0,0,Page_Width,Page_Height);
	ALL_Page_Edit_Rect = Page_Edit_Rect;
	RangeSelection = qMakePair(0,0);
}

/*

####################################################################################
################################ Auxiliary tool#####################################
####################################################################################

*/

bool QTextPanelControl::CheckedAlign(const int a)
{
	QTextBlock bf = textCursor().block();
	QTextBlockFormat format = bf.blockFormat();
	const Qt::Alignment cur = format.alignment();
	if (a == 1200 && cur == Qt::AlignLeft)
	{
		return true;
	}
	else  if (a == 1201 && cur == Qt::AlignRight)
	{
		return true;
	}
	else  if (a == 1202 && cur == Qt::AlignHCenter)
	{
		return true;
	}
	else  if (a == 1203 && cur == Qt::AlignJustify)
	{
		return true;
	}

	return false;
}



void QTextPanelControl::MaketextAlign()
{
	QAction *ali = qobject_cast<QAction *>(sender());
	if (!ali)
	{
		return;
	}
	const int a = ali->data().toInt();

	/*
		BLOCK_ALIGN_LEFT = 1200,
	    BLOCK_ALIGN_RIGHT = 1201,
	    BLOCK_ALIGN_CENTER = 1202,
	    BLOCK_ALIGN_JUSTIFY = 1203,
	*/

	QTextBlock bf = textCursor().block();
	QTextBlockFormat format = bf.blockFormat();

	Qt::Alignment al = Qt::AlignLeft;

	if (a == 1200) {al = Qt::AlignLeft;}
	else if (a == 1202) {al = Qt::AlignHCenter;}
	else if (a == 1201) {al = Qt::AlignRight;}
	else if (a == 1203) {al = Qt::AlignJustify;}

	format.setAlignment(al);
	textCursor().setBlockFormat(format);
}

///////QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)), this, SLOT(LayoutRepaint(QRectF)));
/* big rect !!!!! */
void QTextPanelControl::LayoutRepaint(const QRectF docrect)
{
	return;

	(void)boundingRect();   /* change page if need */
	if (!boundingRect().intersects(docrect))
	{
		QRectF aread = boundingRect().intersected(docrect);
		redir_update(aread);
		return;
	}
	QRectF areadu = QRectF(traposin(docrect.topLeft())  ,  docrect.size());
	////////qDebug() << "### LayoutRepaint  b " << docrect;
	redir_update(areadu);
}

void QTextPanelControl::redir_update(QRectF area)
{

	const QRect needed = area.toRect();
	if (needed.width() > boundingRect().width())
	{
		return;
	}
	if (!boundingRect().toRect().intersects(needed))
	{
		repaintCursor();
		return;
	}
	if (OverloadRectUpdate == needed)
	{
		/* free it on 400 ms */
		return;
	}
	OverloadRectUpdate = needed;
	////////qDebug() << "### redir_update  " << needed;
	emit q_update(needed);
}

QRectF QTextPanelControl::boundingRect()
{
	if (Modus == FLAT)
	{
		return _d->documentLayout()->frameBoundingRect(_d->rootFrame());
	}
	return ALL_Page_Edit_Rect;
}

/* new cursor position .....  */
void QTextPanelControl::cursorPosition(const QTextCursor curs)
{
	if (!_d->isUndoRedoEnabled())
	{
		_d->setUndoRedoEnabled(true);
	}


	cursor_position = curs.position();
	LastCharFormat = curs.charFormat();
	if (curs.isCopyOf(controlTextCursor))
	{

	}
	else
	{
		controlTextCursor.setPosition(curs.position());
		cursor_position = controlTextCursor.position();
		////////WakeUpElasticSize();
	}


	/*
	  qmenu go session
	 remkake bold color checked .....

	  */

	emit q_cursor_newPos();
	EnsureVisibleCursor();
}

void QTextPanelControl::setBlinkingCursorEnabled(bool enable)
{
	edit_enable = enable;

	if (enable && cursorTimeLine.isActive())
	{
		cursorOn = true;
		emit q_update(boundingRect().toRect());
		return;
	}


	if (enable && QApplication::cursorFlashTime() > 0)
	{
		cursorTimeLine.start(QApplication::cursorFlashTime() / 2,this);
	}
	else
	{
		cursorTimeLine.stop();
	}

	emit q_update(boundingRect().toRect());
	if (cursorTimeLine.isActive())
	{
		cursorOn = true;
		emit q_update_scene();
	}
	else
	{
		cursorOn = false;
		emit q_update(boundingRect().toRect());
	}
}


QTextCursor QTextPanelControl::textCursor()
{
	return controlTextCursor;
}


QTextDocument *QTextPanelControl::document()
{
	return _d;
}

void QTextPanelControl::timerEvent(QTimerEvent *event)
{
	if (!edit_enable)
	{
		return;
	}
	/* activate undo redo only at the first blink cursor */
	if (!_d->isUndoRedoEnabled())
	{
		_d->setUndoRedoEnabled(true);
	}

	if (event->timerId() == cursorTimeLine.timerId())
	{
		cursortime = cursortime == true ? false : true;
		repaintCursor();
		SceneTimerLine++;


		if (SceneTimerLine == 4)
		{
			SceneTimerLine = 0;
			emit q_update_scene();
			///////qDebug() << "### SceneTimerLine " << SceneTimerLine;
		}
	}

	if (cursor_position != controlTextCursor.position())
	{
		cursor_position = controlTextCursor.position();
		emit q_cursor_newPos();
	}
}

void QTextPanelControl::ResetClickTimer()
{
	if (dragClickTimer.isActive())
	{
		dragClickTimer.stop();
		DragFill = false;
		//////qDebug() << "### ResetClickTimer DragFill go " << DragFill;
	}
	if (trippleClickTimer.isActive())
	{
		trippleClickTimer.stop();
	}
}

QLineF QTextPanelControl::BlinkCursorLine()
{
	QLineF CursorDrawLine(QPointF(0,0),QPointF(0,10));  /* error line */
	/* QTextDocument *_d;  */
	const QRectF xxtmp = _d->documentLayout()->blockBoundingRect(textCursor().block());
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	QTextLine TTline = currentTextLine(textCursor());
	if (TTline.isValid())
	{
		int pos = textCursor().position() - textCursor().block().position();
		const qreal TextCursorStartTop = TTline.lineNumber() * TTline.height() + xxtmp.top();
		const qreal TextCursorStartLeft = TTline.cursorToX(pos) + root_format.leftMargin() + root_format.padding();
		CursorDrawLine = QLineF(QPointF(TextCursorStartLeft,TextCursorStartTop),QPointF(TextCursorStartLeft,TextCursorStartTop + TTline.height()));
	}
	return CursorDrawLine;
}

QLineF QTextPanelControl::ViewBlinkedCursorLine()
{
	if (Modus == FLAT)
	{
		return BlinkCursorLine();
	}

	(void)CurrentBlockRect();
	QLineF cursorLiner = BlinkCursorLine();
	qreal increment = currentPageNumber * InterSpace;
	if (isBottomBlock)
	{
		increment = increment + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}
	cursorLiner.translate(QPointF(0,increment));
	return cursorLiner;
}

QRectF QTextPanelControl::CurrentBlockRect()
{
	const QRectF xxtmp = _d->documentLayout()->blockBoundingRect(textCursor().block());
	if (Modus == FLAT)
	{
		return xxtmp;
	}

	QLineF cursorLiner = BlinkCursorLine();

	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();

	const int page_a1 = cursorLiner.p2().y() / Page_Edit_Rect.height();
	const int page_a2 = cursorLiner.p2().y() / (Page_Edit_Rect.height() - root_format.bottomMargin() - root_format.padding());
	isBottomBlock = page_a1 == page_a2 ? false : true;
	currentPageNumber = cursorLiner.p2().y() / Page_Edit_Rect.height();
	if (isBottomBlock)
	{
		currentPageNumber = qMax(page_a1,page_a2);
	}
	/////////////////////qDebug() << "### currentPageNumber " << currentPageNumber << " bottom->" << isBottomBlock;
	qreal spacer =  currentPageNumber * InterSpace;
	if (isBottomBlock)
	{
		spacer = spacer + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}

	const qreal InternalSpace = Page_Width - root_format.leftMargin() - root_format.rightMargin();


	const qreal largoblocco = qBound(InternalSpace,xxtmp.width(),InternalSpace);
	qreal altoblocco = xxtmp.height() * 1.888888;
	if (isBottomBlock)
	{
		altoblocco = xxtmp.height() + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}
	QRectF blockrr(root_format.leftMargin(),xxtmp.topLeft().y() + spacer , largoblocco , altoblocco);
	return blockrr;
}

qreal QTextPanelControl::SlicedPage(const int page)
{
	return (page + 1) * Page_Edit_Rect.height();
}

void QTextPanelControl::FrameHandler()
{
	const qreal spacer =  currentPageNumber * InterSpace;
	QTextFrame  *RootFrame = _d->rootFrame();
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	if (controlTextCursor.currentTable())
	{
		QTextTable *table = controlTextCursor.currentTable();
		QRectF tre = _d->documentLayout()->frameBoundingRect(table);
		emit q_update(QRect(tre.left() , tre.top() + spacer , tre.width() , tre.height()));
		return;
	}
	else if (controlTextCursor.currentFrame() && controlTextCursor.currentFrame() != RootFrame)
	{
		QRectF tre = boundingRectOfFrame(textCursor());
		emit q_update(QRect(tre.left() , tre.top() + spacer , tre.width() * 1.2 , tre.height()));
	}
	else if (selectionLength > 999)
	{
		emit q_update_scene();
	}
	else
	{
		QRectF par = CurrentBlockRect();
		/* large paragraph update !!!!!! */
		emit q_update(QRect(0,par.top() - par.height(), Page_Width , par.height() * 2.1));
	}
}

void QTextPanelControl::EnsureVisibleCursor()
{
	const QRectF paragraphrect = CurrentBlockRect();
	QRectF large(-10,paragraphrect.top() - 25,Page_Width + 10,paragraphrect.height() + 100);
	if (Modus == PAGES)
	{
		emit q_visible(large);
	}
	else
	{
		emit q_visible(paragraphrect);
	}
}

QTextLine QTextPanelControl::currentTextLine(const QTextCursor &cursor)
{
	const QTextBlock block = cursor.block();
	if (!block.isValid())
		return QTextLine();

	const QTextLayout *layout = block.layout();
	if (!layout)
		return QTextLine();
	const int relativePos = cursor.position() - block.position();
	return layout->lineForTextPosition(relativePos);
}

QPointF QTextPanelControl::traposin(const QPointF &pos)
{

	////////////qDebug() << "### traposin  modus  " << Modus;

	if (Modus == FLAT)
	{
		return pos;
	}

	if (!boundingRect().contains(pos))
	{
		return QPointF(0,0);
	}

	const int currentpageisY =  pos.y() / Page_Height ;
	if (currentpageisY == 0)
	{
		return pos;
	}
	const qreal spacer = currentpageisY * InterSpace;   /* summ space */
	const qreal PosY = pos.y() - spacer; /* decrement space */
	return QPointF(pos.x(),PosY);
}

void QTextPanelControl::CursorMovetoPosition(const QPointF &pos)
{
	const int cursorPosFozze = _d->documentLayout()->hitTest(pos,Qt::FuzzyHit);
	if (cursorPosFozze != -1)
	{
		controlTextCursor = QTextCursor(_d);
		controlTextCursor.setPosition(cursorPosFozze);
		cursor_position = cursorPosFozze;
		cursortime = true; /* fast display */
	}

}

void QTextPanelControl::gotoNextTableCell()
{
	QTextTable *table = textCursor().currentTable();
	if (!table)
	{
		return;
	}
	QTextTableCell cell = CellOnPosition(textCursor().position());

	int newColumn = cell.column() + cell.columnSpan();
	int newRow = cell.row();

	if (newColumn >= table->columns())
	{
		newColumn = 0;
		++newRow;
		if (newRow >= table->rows())
			table->insertRows(table->rows(), 1);
	}

	cell = table->cellAt(newRow, newColumn);
	const int moveto  = cell.firstCursorPosition().position();
	controlTextCursor.setPosition(moveto);
	cursor_position = moveto;
	cursortime = true; /* fast display */
}

void QTextPanelControl::gotoPreviousTableCell()
{
	QTextTable *table = textCursor().currentTable();
	if (!table)
	{
		return;
	}

	QTextTableCell cell = CellOnPosition(textCursor().position());

	int newColumn = cell.column() - 1;
	int newRow = cell.row();

	if (newColumn < 0)
	{
		newColumn = table->columns() - 1;
		--newRow;
		if (newRow < 0)
			return;
	}

	cell = table->cellAt(newRow, newColumn);
	const int moveto  = cell.firstCursorPosition().position();
	controlTextCursor.setPosition(moveto);
	cursor_position = moveto;
	cursortime = true; /* fast display */
}



/*

####################################################################################
################################ Auxiliary tool#####################################
####################################################################################


####################################################################################
################################ Input Event #######################################
####################################################################################

*/

void QTextPanelControl::selectAll()
{
	if (selectionStartPosition == controlTextCursor.anchor())
	{
		return;
	}

	controlTextCursor.select(QTextCursor::Document);
	cursor_position = controlTextCursor.position();
	selectionStartPosition = controlTextCursor.anchor();
	FullDocSelect = true;
	emit q_update_scene();
}

void QTextPanelControl::deleteSelected()
{
	if (Edit_On())
	{
		controlTextCursor.removeSelectedText();
	}
}

void QTextPanelControl::cut()
{
	if (!Edit_On())
	{
		return;
	}

	if (!controlTextCursor.hasSelection())
	{
		QApplication::beep();
		return;
	}
	copy();
	controlTextCursor.removeSelectedText();
	/////SwapSelectionsCursor();
}

void QTextPanelControl::paste()
{
	if (!Edit_On())
	{
		return;
	}
	InsertMimeDataOnCursor(clipboard->mimeData());
	emit q_update_scene();
}

void QTextPanelControl::int_clipboard_new()
{
	QTextPanelData::instance()->SaveMimeTmp();   /* clone a copy on session before next incomming */


	////////////////qDebug() << "### clipboard fill  ";
}

void QTextPanelControl::copy()
{
	if (!controlTextCursor.hasSelection())
	{
		QApplication::beep();
		return;
	}
	QMimeData *data = createMimeDataFromSelection();
	clipboard->setMimeData(data);
}


void QTextPanelControl::undo()
{
	if (!Edit_On())
	{
		return;
	}

	_d->undo();
	////////qDebug() << "### undo ";
}

void QTextPanelControl::redo()
{
	if (!Edit_On())
	{
		return;
	}
	_d->redo();
}

bool QTextPanelControl::getoverwriteMode() const
{
	return overwriteMode;
}
/* swap Insert key */
void QTextPanelControl::SwapOverwriteMode()
{
	overwriteMode = overwriteMode == false ? true : false;
}

/* swap cursor mode */
void QTextPanelControl::SwapCursorMode()
{
	PlayCursorMode = PlayCursorMode == false ? true : false;
}


bool QTextPanelControl::cursorMoveKeyEvent(QKeyEvent *e)
{
	const QTextCursor oldSelection = controlTextCursor;
	const int oldCursorPos = oldSelection.position();

	QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
	QTextCursor::MoveOperation op = QTextCursor::NoMove;

	/* first compare QKeySequences */
	if (e == QKeySequence::MoveToPreviousChar)
	{
		op = QTextCursor::Left;
	}
	else if (e == QKeySequence::SelectNextChar)
	{
		op = QTextCursor::Right;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectPreviousChar)
	{
		op = QTextCursor::Left;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectNextWord)
	{
		op = QTextCursor::WordRight;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectPreviousWord)
	{
		op = QTextCursor::WordLeft;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectStartOfLine)
	{
		op = QTextCursor::StartOfLine;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectEndOfLine)
	{
		op = QTextCursor::EndOfLine;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectStartOfBlock)
	{
		op = QTextCursor::StartOfBlock;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectEndOfBlock)
	{
		op = QTextCursor::EndOfBlock;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectStartOfDocument)
	{
		op = QTextCursor::Start;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectEndOfDocument)
	{
		op = QTextCursor::End;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectPreviousLine)
	{
		op = QTextCursor::Up;
		mode = QTextCursor::KeepAnchor;
	}
	else if (e == QKeySequence::SelectNextLine)
	{
		op = QTextCursor::Down;
		mode = QTextCursor::KeepAnchor;
		{
			QTextBlock block = controlTextCursor.block();
			QTextLine line = currentTextLine(controlTextCursor);
			if (!block.next().isValid()
			      && line.isValid()
			      && line.lineNumber() == block.layout()->lineCount() - 1)
				op = QTextCursor::End;
		}
	}
	else if (e == QKeySequence::SelectNextLine)
	{
		op = QTextCursor::Down;
		mode = QTextCursor::KeepAnchor;
		{
			QTextBlock block = controlTextCursor.block();
			QTextLine line = currentTextLine(controlTextCursor);
			if (!block.next().isValid()
			      && line.isValid()
			      && line.lineNumber() == block.layout()->lineCount() - 1)
				op = QTextCursor::End;
		}
	}
	else if (e == QKeySequence::MoveToNextWord)
	{
		op = QTextCursor::WordRight; ///// 16777236
	}
	else if (e == QKeySequence::MoveToPreviousWord)
	{
		op = QTextCursor::WordLeft;
	}
	else if (e->key() == Qt::Key_Left)
	{
		op = QTextCursor::WordLeft;
	}
	else if (e->key() == Qt::Key_Right)
	{
		op = QTextCursor::WordRight;
	}
	else if (e == QKeySequence::MoveToEndOfBlock)
	{
		op = QTextCursor::EndOfBlock;
	}
	else if (e == QKeySequence::MoveToStartOfBlock)
	{
		op = QTextCursor::StartOfBlock;
	}
	else if (e == QKeySequence::MoveToNextLine)
	{
		op = QTextCursor::Down;
	}
	else if (e == QKeySequence::MoveToPreviousLine)
	{
		op = QTextCursor::Up;
	}
	else if (e == QKeySequence::MoveToPreviousLine)
	{
		op = QTextCursor::Up;
	}
	else if (e == QKeySequence::MoveToStartOfLine)
	{
		op = QTextCursor::StartOfLine;
	}
	else if (e == QKeySequence::MoveToEndOfLine)
	{
		op = QTextCursor::EndOfLine;
	}
	else if (e == QKeySequence::MoveToStartOfDocument)
	{
		op = QTextCursor::Start;
	}
	else if (e == QKeySequence::MoveToEndOfDocument)
	{
		op = QTextCursor::End;
	}
	/* after comparing all the possible key sequences, compare individual keys*/
	else if (e->key() == Qt::Key_Right)
	{
		op = QTextCursor::Right;
	}
	else if (e->key() == Qt::Key_Left)
	{
		op = QTextCursor::Left;
	}
	else
	{
		return false;
	}

	/////////_d->adjustSize();   /* destroy page count */
	repaintCursor(true);

	const bool moved = controlTextCursor.movePosition(op, QTextCursor::MoveAnchor);

	if (moved)
	{
		if (controlTextCursor.position() != oldCursorPos)
		{
			cursor_position = controlTextCursor.position();
			emit q_cursor_newPos();
		}
	}

	if (mode == QTextCursor::KeepAnchor)
	{
		/* if it wasn't selected before, defines where the selection started */
		if (selectionStartPosition == -1)
		{
			selectionStartPosition = oldCursorPos;
		}

		int newCursorPos = controlTextCursor.position();
		controlTextCursor.setPosition(selectionStartPosition);

		if (newCursorPos > selectionStartPosition)
		{
			controlTextCursor.movePosition(QTextCursor::NextCharacter, mode, newCursorPos - selectionStartPosition);
		}
		else
		{
			controlTextCursor.movePosition(QTextCursor::PreviousCharacter, mode, selectionStartPosition - newCursorPos);
		}

		cursor_position = controlTextCursor.position();
		RangeSelection = qMakePair(controlTextCursor.position(),controlTextCursor.anchor());

		emit q_cursor_newPos();
	}
	else
	{
		/* resets the selection in case user didn't use a key sequence to keep the selection */
		ClearCurrentSelection();
	}

	return true;
}

/* after key press */
void QTextPanelControl::Controller_keyReleaseEvent(QKeyEvent * e)
{
	if (trippleClickTimer.isActive())
	{
		trippleClickTimer.stop();
	}
	FrameHandler();
	cursortime = true;
}

void QTextPanelControl::Controller_keyPressEvent(QKeyEvent * e)
{
	//////////qDebug() << "### Controller_keyPressEvent  " << e->text() << e->key();
	ResetClickTimer();
	cursortime = false;

	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S)
	{
		return;
	}
	if (e->key() == Qt::Key_Insert)
	{
		SwapOverwriteMode();
		e->accept();
		return;
	}
	if (e->key() == Qt::Key_ScrollLock)
	{
		SwapCursorMode();
		e->accept();
		return;
	}
	if (controlTextCursor.currentTable() && e->key() == Qt::Key_Tab || controlTextCursor.currentTable() && e->key() == Qt::Key_Right)
	{
		gotoNextTableCell();
		e->accept();
		return;
	}
	if (controlTextCursor.currentTable() && e->key() == Qt::Key_Left)
	{
		gotoPreviousTableCell();
		e->accept();
		return;
	}

	LastCharFormat = controlTextCursor.charFormat();

	/* fast access */

	if (e == QKeySequence::SelectAll || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_A)
	{
		e->accept();
		selectAll();
		return;
	}
	else if (e == QKeySequence::Copy || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C)
	{
		e->accept();
		copy();
		return;
	}
	else if (e == QKeySequence::Paste || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_V)
	{
		e->accept();
		paste();
		return;
	}
	else if (e == QKeySequence::Cut || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C)
	{
		e->accept();
		cut();
		return;
	}

	/* CTRL + B sets style of the selected text to bold */
	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_B)
	{
		LastCharFormat.setFontWeight(!controlTextCursor.charFormat().font().bold()  ? QFont::Bold : QFont::Normal);
		controlTextCursor.setCharFormat(LastCharFormat);
		e->accept();
		return;
	}

	/* CTRL + I sets style of the selected text to italic */
	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_I)
	{
		LastCharFormat.setFontItalic(!LastCharFormat.font().italic() ? true : false);
		controlTextCursor.setCharFormat(LastCharFormat);
		e->accept();
		return;
	}

	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_J)
	{
		InsertImageonCursor();
		e->accept();
		return;
	}

	if ((e->modifiers() & Qt::AltModifier) && e->key() == Qt::Key_S)
	{
		showhtml();
		return;
	}

	/* CTRL + U sets style of the selected text to underline */
	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_U)
	{
		LastCharFormat.setUnderlineStyle(!LastCharFormat.font().underline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline);
		controlTextCursor.setCharFormat(LastCharFormat);
		e->accept();
		return;
	}

	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_H)
	{
		controlTextCursor.insertFragment(QTextDocumentFragment::fromHtml("<hr>"));
		e->accept();
		return;
	}
    /* need to stress Paint on developed time */
    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F)
	{
        FontText();
		e->accept();
		return;
	}
    if ((e->modifiers() & Qt::AltModifier) && e->key() == Qt::Key_F)
	{
        TXcolor();
		e->accept();
		return;
	}

	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Z || e == QKeySequence::Undo)
	{
		e->accept();
		undo();
		return;
	}

	if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Y || e == QKeySequence::Redo)
	{
		e->accept();
		redo();
		return;
	}

	//////////////return;

	/* move action on cursor ? */
	if (cursorMoveKeyEvent(e))
	{
		e->accept();
		cursor_position = controlTextCursor.position();
		EnsureVisibleCursor();  /* big steep move */

		if (FullDocSelect)
		{
			q_update_scene();
		}

		return;
	}

	/* fast access end  */

	if (e->key() == Qt::Key_Backspace && !(e->modifiers() & ~Qt::ShiftModifier))
	{

		QTextBlockFormat blockFmt = controlTextCursor.blockFormat();

		if (blockFmt.bottomMargin() == 12)
		{
			blockFmt.setBottomMargin(0);
		}

		if (blockFmt.topMargin() == 12)
		{
			blockFmt.setTopMargin(0);
		}



		QTextList *list = controlTextCursor.currentList();
		if (list && controlTextCursor.atBlockStart())
		{
			list->remove(controlTextCursor.block());
		}
		else if (controlTextCursor.atBlockStart() && blockFmt.indent() > 0)
		{
			blockFmt.setIndent(blockFmt.indent() - 1);
			controlTextCursor.setBlockFormat(blockFmt);
		}
		else
		{
			controlTextCursor.deletePreviousChar();
		}
		goto accept;
	}
	else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
	{

		QTextBlockFormat blockFmt = controlTextCursor.blockFormat();


		if (e->modifiers() & Qt::ControlModifier)
		{
			if (Op != FOP)
			{
				controlTextCursor.insertText(QString(QChar::LineSeparator));
			}
			else
			{
				controlTextCursor.insertBlock();
			}

			if (blockFmt.bottomMargin() == 12)
			{
				blockFmt.setBottomMargin(0);
			}

			if (blockFmt.topMargin() == 12)
			{
				blockFmt.setTopMargin(0);
			}

			blockFmt.setIndent(blockFmt.indent() - 1);
			controlTextCursor.setBlockFormat(blockFmt);

		}
		else
		{
			controlTextCursor.insertBlock();   /* default format can take from setting */
		}
		e->accept();
		goto accept;
	}
	else if (e == QKeySequence::Delete)
	{

		if (controlTextCursor.hasSelection())
		{
			QString remtxt = controlTextCursor.selectedText();
			for (int i = 0; i < remtxt.size(); ++i)
			{
				controlTextCursor.deleteChar();
			}
			////////////SwapSelectionsCursor();  /* clear */
		}
		else
		{
			controlTextCursor.deleteChar();
		}

		goto accept;
	}
	else if (e == QKeySequence::DeleteEndOfWord)
	{
		controlTextCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
		controlTextCursor.deleteChar();
		goto accept;
	}
	else if (e == QKeySequence::DeleteStartOfWord)
	{
		controlTextCursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
		controlTextCursor.deleteChar();
		goto accept;
	}
	else if (e == QKeySequence::DeleteEndOfLine)
	{
		QTextBlock block = controlTextCursor.block();
		if (controlTextCursor.position() == block.position() + block.length() - 2)
		{
			controlTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
		}
		else
		{
			controlTextCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
			controlTextCursor.deleteChar();
		}
		goto accept;
	}
	else
	{
		/* go insert text ......................*/
		goto process;
	}



process:
	{
		QString text = e->text();
		if (!text.isEmpty() && (text.at(0).isPrint() || text.at(0) == QLatin1Char('\t')))
		{

			if (getoverwriteMode() && !controlTextCursor.hasSelection() && !controlTextCursor.atBlockEnd())
			{
				controlTextCursor.deleteChar();
			}

			controlTextCursor.insertText(text);
			cursor_position = controlTextCursor.position();
			e->accept();

			if (FullDocSelect)
			{
				q_update_scene();
			}

			return;

		}
		else if (!text.isEmpty() && controlTextCursor.hasSelection())
		{
			QString remove = controlTextCursor.selectedText();
			for (int i = 0; i < remove.size(); ++i)
			{
				controlTextCursor.deletePreviousChar();
			}

			controlTextCursor.insertText(text);

			if (FullDocSelect)
			{
				q_update_scene();
			}

			return;

		}
		else
		{
			e->ignore();
			QApplication::beep();
			return;
		}
	}



	/* ########## section accept ################*/
accept:
	{
		cursor_position = controlTextCursor.position();
		/* ########## section accept ################*/
	}

	if (FullDocSelect)
	{
		q_update_scene();
	}

}

void QTextPanelControl::repaintCursor(bool allrect)
{

	////////qDebug() << "### repaintCursor " << __LINE__ << eventline;

	if (allrect)
	{
		emit q_update_scene();
		return;
	}
	if (controlTextCursor.hasComplexSelection() && controlTextCursor.currentTable())
	{
		QTextTable *table = controlTextCursor.currentTable();
		QRectF tablerect = _d->documentLayout()->frameBoundingRect(table);
		emit q_update(tablerect.toRect());
		return;
	}
	QRectF paragraphrect = CurrentBlockRect();
	//////////QRectF paragraphrect = PAGE_MODEL.PageInternal(OnPageClick);
	if (paragraphrect.isValid())
	{
		emit q_update(paragraphrect.toRect());
	}
	else
	{
		emit q_update(_d->documentLayout()->frameBoundingRect(_d->rootFrame()).toRect());
	}

}

bool QTextPanelControl::procesevent(QEvent *e)
{
	eventline++;
	bool FoundEvent = false;
	QGraphicsSceneEvent *ev;


	switch (e->type())
	{

	case QEvent::GraphicsSceneMouseMove:
	case QEvent::GraphicsSceneMousePress:
	case QEvent::GraphicsSceneMouseRelease:
	case QEvent::GraphicsSceneMouseDoubleClick:
	case QEvent::GraphicsSceneContextMenu:
	case QEvent::GraphicsSceneHoverEnter:
	case QEvent::GraphicsSceneHoverMove:
	case QEvent::GraphicsSceneHoverLeave:
	case QEvent::GraphicsSceneHelp:
	case QEvent::GraphicsSceneDragEnter:
	case QEvent::GraphicsSceneDragMove:
	case QEvent::GraphicsSceneDragLeave:
	case QEvent::GraphicsSceneDrop:
	{
		ev = static_cast<QGraphicsSceneEvent *>(e);
		Gwi = ev->widget();
		break;
	}
	default:
		return false;
		break;
	};

	if (!ev)
	{
		return false;
	}

	if (e->type() == QEvent::UngrabMouse)
	{
		//////////selectionStartPosition = -1;
		/////////////controlTextCursor.clearSelection();
		//////////qDebug() << "### UngrabMouse   " << e->type();
		FoundEvent = true;
	}
	if (e->type() == QEvent::GrabMouse)
	{
		//////////selectionStartPosition = -1;
		///////controlTextCursor.clearSelection();
		////////qDebug() << "### GrabMouse   " << e->type();
		FoundEvent = true;
	}
	else if (ev->type() == QEvent::GraphicsSceneDragMove)
	{
		DragFill = true;
		QGraphicsSceneDragDropEvent *prespos = static_cast<QGraphicsSceneDragDropEvent *>(e);
		const QPointF posi = traposin(prespos->pos());
		CursorMovetoPosition(posi);
		ResetClickTimer();
		FrameHandler();
		FoundEvent = true;
	}
	else if (ev->type() == QEvent::GraphicsSceneDrop && DragFill)
	{
		///////////qDebug() << "### QEvent::GraphicsSceneDrop   " << e->type();
		QGraphicsSceneDragDropEvent *prespos = static_cast<QGraphicsSceneDragDropEvent *>(e);
		const QPointF posi = traposin(prespos->pos());
		controlTextCursor.clearSelection();
		CursorMovetoPosition(posi);

		if (dragClickTimer.isActive())
		{
			dragClickTimer.stop();
		}
		DragFill = false;
		selectionStartPosition = -1;

		/* clear selection and paste */
		if (!IsSelfPlacePaste())
		{
			InsertMimeDataOnCursor(prespos->mimeData());
		}
		FrameHandler();
		FoundEvent = true;
	}
	else if (ev->type() == QEvent::GraphicsSceneMousePress)
	{
		QGraphicsSceneMouseEvent *prespos = static_cast<QGraphicsSceneMouseEvent *>(e);
		BaseMousePressEvent(traposin(prespos->pos()),prespos);
		FoundEvent = true;
		FullDocSelect = false;
	}
	else if (ev->type() == QEvent::GraphicsSceneMouseDoubleClick)
	{
		QGraphicsSceneMouseEvent *dblclickpos = static_cast<QGraphicsSceneMouseEvent *>(e);
		const QPointF posi = traposin(dblclickpos->pos());
		BaseDoubleClickEvent(posi,dblclickpos);
		FoundEvent = true;
		FullDocSelect = false;
	}
	else if (ev->type() == QEvent::GraphicsSceneMouseMove)
	{
		QGraphicsSceneMouseEvent *movepos = static_cast<QGraphicsSceneMouseEvent *>(e);
		const QPointF posi = traposin(movepos->pos());
		const int CursorPosition = _d->documentLayout()->hitTest(posi,Qt::FuzzyHit);
		BaseMoveEvent(CursorPosition,posi);
		FoundEvent = true;
	}
	else if (ev->type() == QEvent::GraphicsSceneMouseRelease)
	{
		QGraphicsSceneMouseEvent *movepos = static_cast<QGraphicsSceneMouseEvent *>(e);
		const QPointF posi = traposin(movepos->pos());
		BaseMouseReleaseEvent(posi,movepos->button());
		FoundEvent = true;
	}


	if (FoundEvent)
	{
		e->setAccepted(true);
	}
	return FoundEvent;
}

void QTextPanelControl::ClearCurrentSelection()
{
	selectionStartPosition = -1;
	controlTextCursor.clearSelection();
}



void QTextPanelControl::BaseDoubleClickEvent(const  QPointF posi , const QGraphicsSceneMouseEvent * event)
{
	ResetClickTimer();

	if (event->buttons() != Qt::LeftButton)
	{
		return;
	}
	const QTextCursor oldSelection = controlTextCursor;
	PointPositionOnDoc = posi;
	CursorMovetoPosition(posi);
	selectionStartPosition = -1;
	controlTextCursor.clearSelection();
	QTextLine line = currentTextLine(controlTextCursor);
	bool doEmit = false;
	if (line.isValid() && line.textLength())
	{
		controlTextCursor.select(QTextCursor::WordUnderCursor);
		doEmit = true;
	}
	trippleClickTimer.start(qApp->doubleClickInterval(),this);
	if (doEmit)
	{
		repaintCursor();
	}

}

bool QTextPanelControl::IsSelfPlacePaste()
{
	const int Sx1 = RangeSelection.first - 2;
	const int Sx2 = RangeSelection.second + 2;
	bool goup = false;
	bool godown = false;
	const int foundits = controlTextCursor.position();
	if (foundits < Sx1 && foundits < Sx2)
	{
		/* go up */
		goup = true;
	}
	if (foundits > Sx1 && foundits > Sx2)
	{
		/* go down */
		godown = true;
	}
	if (goup || godown)
	{
		return false;
	}
	else
	{
		return true;
	}
}



void QTextPanelControl::StartDragOperation()
{
	DragFill = false;
	if (!controlTextCursor.hasSelection())
	{
		return;
	}
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	if (selectionLength > 0)
	{
		RangeSelection = qMakePair(controlTextCursor.position(),controlTextCursor.anchor());
	}


	/////////////qDebug() << "### StartDragOperation->selectionLength" << selectionLength;
	QMimeData *data = createMimeDataFromSelection();
	if (data)
	{
		QApplication::clipboard()->setMimeData(data);
		QDrag *drag = new QDrag(Gwi);   /* QWidget *Gwi;  from event */
		drag->setMimeData(data);
		drag->setHotSpot(QPoint(-25,-25));
		/* try to make a QPixmap from mime html fragment or image drag drop */
		const QPixmap playdragicon = ImagefromMime(data);
		if (!playdragicon.isNull())
		{
			drag->setPixmap(playdragicon);
		}
		if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
		{
			DragFill = true;
			emit q_startDrag(PointPositionOnDoc);
		}
	}
}


void QTextPanelControl::BaseMousePressEvent(const  QPointF posi , const QGraphicsSceneMouseEvent *epress)
{
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	//////////////qDebug() << "### BaseMousePressEvent selectionLength->" << selectionLength;
	if (selectionLength > 0)
	{
		RangeSelection = qMakePair(controlTextCursor.position(),controlTextCursor.anchor());
	}

	PointPositionOnDoc = posi;

	if (trippleClickTimer.isActive() && selectionLength > 0 ||
	      dragClickTimer.isActive() && selectionLength > 0)
	{

		trippleClickTimer.stop();
		dragClickTimer.stop();

		(void)StartDragOperation();

		if (DragFill)
		{
			/* ctrl down? */
			if (epress->modifiers() == Qt::ControlModifier)
			{
				controlTextCursor.removeSelectedText();
			}


			dragClickTimer.start(qApp->doubleClickInterval(),this);
			return;
		}
		return;

	}

	CursorMovetoPosition(posi);
	selectionStartPosition = controlTextCursor.position();
	DragFill = false;
	controlTextCursor.clearSelection();
}
void QTextPanelControl::BaseMouseReleaseEvent(const  QPointF posi , Qt::MouseButton button)
{

	const int TMPCursorPosition = _d->documentLayout()->hitTest(posi,Qt::FuzzyHit);
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	if (selectionLength > 0)
	{
		RangeSelection = qMakePair(controlTextCursor.position(),controlTextCursor.anchor());
	}
	//////////////////qDebug() << "### BaseMouseReleaseEvent  " << selectionLength;
	if (button == Qt::LeftButton)
	{
		LastReleasePoint = posi;
		ResetClickTimer();

		if (!controlTextCursor.hasSelection())
		{
			selectionStartPosition = -1;
		}
		else
		{
			/* can start drag here ???????? */
			dragClickTimer.start(qApp->doubleClickInterval(),this);
		}



	}
}



void QTextPanelControl::BaseMoveEvent(const int cursorpos, QPointF moveposition)
{
	////////////// qDebug() << "### BaseMoveEvent  aa " << cursorpos;

	if (cursorpos >= INT_MAX || cursorpos ==  -1)
	{
		return;
	}

	///////////////// qDebug() << "### BaseMoveEvent bb " << cursorpos;


	const int cursorPosFozze = cursorpos;
	cursortime = false;
	const int stopat = qMax(selectionStartPosition,cursorPosFozze);
	const int startat = qMin(selectionStartPosition,cursorPosFozze);
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	if (selectionLength > 0)
	{
		RangeSelection = qMakePair(controlTextCursor.position(),controlTextCursor.anchor());
	}

	if (selectionStartPosition >= 0 && cursorPosFozze >= 0 && !controlTextCursor.currentTable())
	{
		/////////////qDebug() << "### selezione  " << selectionStartPosition <<  " to "  << cursorPosFozze;
		if (stopat == cursorPosFozze)
		{
			/* move >>>>>>>>>>>>>>>>>>>>>>>>>  */
			controlTextCursor.setPosition(startat);
			for (int i = startat; i < cursorPosFozze; ++i)
			{
				controlTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
			}
			cursor_position = stopat;
			FrameHandler();
			LastReleasePoint = moveposition;
			return;
		}
		else
		{
			controlTextCursor.setPosition(stopat);
			const int diffs = stopat - cursorPosFozze;
			for (int i = 0; i < diffs; ++i)
			{
				controlTextCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
			}
			cursor_position = stopat;
			FrameHandler();
			LastReleasePoint = moveposition;
			return;
		}
	}

	if (selectionStartPosition >= 0 && controlTextCursor.currentTable())
	{
		QTextTable *table = controlTextCursor.currentTable();
		QTextTableCell firstcell = CellOnPosition(startat);
		QTextTableCell lastcell = CellOnPosition(stopat);
		if (firstcell.isValid() && lastcell.isValid())
		{
			int fcellrow = firstcell.row();
			int fcellcool = firstcell.column();
			int numRows = qBound(1,lastcell.row() - fcellrow,table->rows());
			int numColumns = qBound(1,lastcell.column() - fcellcool,table->columns());
			///////qDebug() << "### nnrow / nncool " << numRows << numColumns;
			controlTextCursor.selectedTableCells(&fcellrow,&numRows,&fcellcool,&numColumns);
			controlTextCursor.setPosition(firstcell.firstPosition());
			controlTextCursor.setPosition(lastcell.lastPosition(), QTextCursor::KeepAnchor);
			cursor_position = stopat;
			FrameHandler();
			LastReleasePoint = moveposition;
			return;
		}
	}

	/* is comming here no selection !!! */
	selectionStartPosition =-1;
	LastReleasePoint = QPointF(-1,-1);
	controlTextCursor.clearSelection();
}


/*

####################################################################################
################################ Input Event #######################################
####################################################################################

*/

QTextTableCell QTextPanelControl::CellOnPosition(const int posi)
{
	////////////qDebug() << "### cell  OnPosition " << posi;
	if (posi != -1)
	{
		QTextCursor tmpcursor(_d);
		tmpcursor.setPosition(posi);
		if (!tmpcursor.currentTable())
		{
			return QTextTableCell();
		}
		return tmpcursor.currentTable()->cellAt(tmpcursor);
	}
	else
	{
		return QTextTableCell();
	}
}

QMimeData *QTextPanelControl::createMimeDataFromSelection()
{
	QTextCharFormat base = controlTextCursor.charFormat();
	QString txt;

	if (controlTextCursor.hasSelection())
	{
		txt = controlTextCursor.selectedText();
	}

	QTextImageFormat pico = base.toImageFormat();
	if (pico.isValid())
	{
		QVariant xx = pico.property(_IMAGE_PICS_ITEM_);
		if (!xx.isNull())
		{
			TPics pic = xx.value<TPics>();
			QList<TPics> li;
			li.append(pic);
			QString Sdd = SaveImageGroup(li);
			QMimeData *mimeData = new QMimeData;
			mimeData->setData("application/x-picslists",Sdd.toUtf8());
			return mimeData;
		}
	}

	const QTextDocumentFragment fragment(controlTextCursor);
	if (fragment.isEmpty() && txt.size() > 0)
	{
		QMimeData *xm = new QMimeData();
		xm->setText(txt);
		return xm;
	}
	else if (fragment.isEmpty() && txt.isEmpty())
	{
		QMimeData *xm = new QMimeData();
		xm->setText(QString("...Selection...Error..."));
		return xm;
	}
	else
	{
		return new QTextEditMimeData(fragment);    /////QTextDocumentFragment::fromHtml(md->html())
	}
}


void QTextPanelControl::InsertMimeDataOnCursor(const QMimeData *md)
{
	QTextDocumentFragment fragment;
	ResetClickTimer();

	if (md->hasUrls())
	{
		QList<QUrl> urls = md->urls();
		for (int i = 0; i < urls.size(); ++i)
		{
			QUrl gettyurl(urls.at(i));
			/* window is slow to write  event is to fast */
			//////////qDebug() << "### gettyurl " << gettyurl.toLocalFile();

			if (gettyurl.scheme() == "file")
			{
				ImageonCursor(gettyurl.toLocalFile());
			}
			else if (gettyurl.scheme() == "http")
			{
				Gloader *grephttp = new Gloader();
				grephttp->Setting(this,i,gettyurl);
				grephttp->start(QThread::LowPriority);
			}
		}

		controlTextCursor.clearSelection();
		repaintCursor(true);
		return;
	}

	if (md->hasFormat(QLatin1String("application/x-picslists")))
	{
		QByteArray dd = md->data("application/x-picslists");
		QList<TPics> li = OpenImageGroup(QString(dd));
		for (int i=0; i<li.size(); i++)
		{
			TPics conni = li[i];
			RegisterImage(conni,true);
		}
		controlTextCursor.clearSelection();
		repaintCursor(true);
		return;
	}

	if (md->hasImage())
	{
		QDateTime timer1(QDateTime::currentDateTime());
		const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
		QPixmap aspixmape = qvariant_cast<QPixmap>(md->imageData());
		if (!aspixmape.isNull())
		{
			insertPixmap(aspixmape);
		}
		controlTextCursor.clearSelection();
		repaintCursor(true);
		return;
	}

	QString cosa;
	bool hasData;

	if (md->hasFormat(QLatin1String("application/x-qrichtext")))
	{
		cosa = md->data(QLatin1String("application/x-qrichtext"));
		// x-qrichtext is always UTF-8 (taken from Qt3 since we don't use it anymore).
		fragment = QTextDocumentFragment::fromHtml(QString::fromUtf8(md->data(QLatin1String("application/x-qrichtext"))));
		hasData = true;
		//////////qDebug() << "### application/x-qrichtext ";
	}
	else if (md->formats().contains("text/html"))
	{
		///////////qDebug() << "### text/html";
		cosa = QString::fromUtf8(md->data(QLatin1String("text/html")));
		fragment = QTextDocumentFragment::fromHtml(md->html());
		//////////qDebug() << "### cosa " << cosa;
		controlTextCursor.insertFragment(fragment);
		controlTextCursor.clearSelection();
		repaintCursor(true);
		EnsureVisibleCursor();
		return;
	}
	else if (md->formats().contains("text/plain"))
	{
		cosa = QString::fromUtf8(md->data(QLatin1String("text/plain")));
		fragment = QTextDocumentFragment::fromPlainText(cosa);
		///////////qDebug() << "### text/plain";
		hasData = true;
	}


	if (hasData && cosa.size() > 0)
	{
		controlTextCursor.insertFragment(fragment);
		controlTextCursor.clearSelection();
		repaintCursor(true);
		EnsureVisibleCursor();
	}
	else
	{
		QApplication::beep();
	}

}





void QTextPanelControl::insertPixmap(QPixmap p)
{
	QPixmap scaledsimage;
	QTextFrame  *Tframe = _d->rootFrame();
	QTextFrameFormat rootformat= Tframe->frameFormat();
	const int margininside = rootformat.leftMargin() + rootformat.rightMargin() + rootformat.padding();
	int limitwiimage = Page_Width;

	if (limitwiimage > boundingRect().width())
	{
		limitwiimage = boundingRect().width() - margininside ;
	}

	if (p.width() > limitwiimage)
	{
		/* question width */
		bool ok;
		int iw = QInputDialog::getInteger(0, tr("Image to width in layer dimension!"),
		                                  tr("Point:"), limitwiimage, 20, limitwiimage, 1, &ok);
		if (iw > 0 && ok)
		{
			scaledsimage = p.scaledToWidth(iw);
		}
		else
		{
			return;
		}
	}
	else
	{
		scaledsimage = p;
	}

	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	if (_DRAWMODUS_WEB_ == 1)
	{
		scaledsimage.save(&buffer,"JPG",70);
	}
	else
	{
		scaledsimage.save(&buffer,"PNG",100);
	}

	QDateTime timer1(QDateTime::currentDateTime());
	const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));

	if (!scaledsimage.isNull())
	{
		const QString nami = imageName(TimestampsMs);
		TPics  xpix;
		xpix.name = nami;
		if (_DRAWMODUS_WEB_ == 1)
		{
			xpix.extension = QByteArray("JPG");
		}
		else
		{
			xpix.extension = QByteArray("PNG");
		}
		/* bytes having data images */
		xpix.set_pics(scaledsimage);
		RegisterImage(xpix,true);
	}
}

void  QTextPanelControl::RegisterImage(TPics e , bool insert)
{
	QApplication::restoreOverrideCursor();
	TPics base;

	bool ok;
	if (e.info == base.info)
	{
		QString txtinfo = QInputDialog::getText(0, tr("Image Description to blind people."),tr("Description:"), QLineEdit::Normal,e.name, &ok);
		if (txtinfo.size() > 0)
		{
			e.info = txtinfo.left(110);
		}
	}
	else
	{
		/* having desc  !!!  */
	}

	QTextPanelData::instance()->ImagePageList.insert(e.name,e);
	_d->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
	if (insert)
	{
		QTextImageFormat format;
		format.setName(e.name);
		format.setHeight(e.pix().height());
		format.setWidth(e.pix().width());
		format.setToolTip(e.info);
		format.setProperty(_IMAGE_PICS_ITEM_,e);
		textCursor().insertImage(format);
	}
}

void  QTextPanelControl::InsertImageonCursor()
{
	QString file = QFileDialog::getOpenFileName(0, tr("Choose Image to insert..."), QString(setter.value("LastDir").toString()) , ImageFilterHaving());
	if (file.isEmpty())
	{
		return;
	}
	setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
	ImageonCursor(file);
}

void  QTextPanelControl::ImageonCursor(const QString file)
{
	QDateTime timer1(QDateTime::currentDateTime());
	const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QFileInfo fixurl(file);
	const QString extension =  fixurl.completeSuffix().toLower();
	QByteArray              derangedata;
	QPixmap                 resultimage;
	QPixmap                 scaledsimage;
	QUrl                    imgresor;

	QFile f(fixurl.absoluteFilePath());
	if (f.open(QIODevice::ReadOnly))
	{
		derangedata = f.readAll();
		f.close();
	}

	//////////////qDebug() << "### image" << derangedata.size();
	if (derangedata.size() < 1)
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(0, tr("Alert! image format."), tr("Unable to read file %1").arg(fixurl.fileName()));
		return;
	}
	/* read image */
	if (extension.contains("svg"))
	{
		resultimage = RenderPixmapFromSvgByte(derangedata);
	}
	else if (extension.contains("ps") || extension.contains("eps"))
	{
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		resultimage = LoadPS(fixurl.absoluteFilePath());
	}
	else if (extension.contains("pdf"))
	{
		QApplication::restoreOverrideCursor();
		int page = QInputDialog::getInteger(0, tr("Render Page Nr."),tr("Page:"),1, 1, 100, 1);
		int large = QInputDialog::getInteger(0, tr("Page scaled to width"),tr("Point unit:"),400, 10, 3000, 10);
		if (page > 0 && large > 0)
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			resultimage = LoadPDF(fixurl.absoluteFilePath() ,page,large);
		}
		else
		{
			QApplication::restoreOverrideCursor();
			return;
		}
	}
	else
	{
		resultimage.loadFromData(derangedata);
	}

	/* read in */
	if (resultimage.isNull())
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(0, tr("Alert! image format."), tr("Unable to render to image file %1 from type %2.").arg(fixurl.fileName()).arg(extension));
		return;
	}

	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	QApplication::restoreOverrideCursor();
	int limitwiimage = Page_Width;
	if (limitwiimage > boundingRect().width())
	{
		limitwiimage = boundingRect().width();
	}
	if (resultimage.width() > limitwiimage)
	{
		/* question widht */
		scaledsimage = resultimage.scaledToWidth(limitwiimage);
	}
	else
	{
		scaledsimage = resultimage.scaledToWidth(resultimage.width());
	}

	if (_DRAWMODUS_WEB_ == 1)
	{
		scaledsimage.save(&buffer,"JPG",70);
	}
	else
	{
		scaledsimage.save(&buffer,"PNG",100);
	}

	if (!scaledsimage.isNull())
	{
		const QString nami = imageName(fixurl.baseName());
		TPics  xpix;
		xpix.name = nami;
		if (_DRAWMODUS_WEB_ == 1)
		{
			xpix.extension = QByteArray("JPG");
		}
		else
		{
			xpix.extension = QByteArray("PNG");
		}
		/* bytes having data images */
		xpix.set_pics(scaledsimage);

		RegisterImage(xpix,true);
	}
	QApplication::restoreOverrideCursor();
}

QString QTextPanelControl::ImageFilterHaving() const
{
	QString filter;
	filter = tr("All supported Types") + " (";
	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	for (int x = 0; x < formats.count(); ++x)
	{

		filter += QString("*.%1").arg(QString(formats[ x ]));
		if (x != formats.count() - 1)
			filter += " ";
	}
	filter += ");;";
	QString filterSimple;
	double gsversion = getGSVersion();
	//////qDebug() << "### args " << gsversion;
	filterSimple += tr("Scalable Vector Graphics") + " (*.svg *.svg.gz);;";
	if (gsversion > 6.5)
	{
		filterSimple += tr("PostScript Vector Graphics") + " (*.ps *.eps);;";
		filterSimple += tr("PDF Page units") + " (*.pdf);;";
	}

	for (int y = 0; y < formats.count(); ++y)
	{
		QString form(formats[ y ]);
		if (form == "bmp")
		{
			filterSimple += tr("Windows Bitmap") + " (*.bmp)";
		}
		else if (form == "gif")
		{
			filterSimple += tr("Graphic Interchange Format") + " (*.gif)";
		}
		else if (form == "jpeg" || form == "jpg")
		{

			if (!filterSimple.contains(form))
				filterSimple += tr("Joint Photographic Experts Group") + " (*.jpeg *.jpg)";
		}
		else if (form == "mng")
		{
			filterSimple += tr("Multiple-image Network Graphics") + " (*.mng)";
		}
		else if (form == "png")
		{
			filterSimple += tr("Portable Network Graphics") + " (*.png)";
		}
		else if (form == "pbm" || form == "ppm")
		{
			if (!filterSimple.contains(form))
				filterSimple += tr("Portable Bitmap") + " (*.pbm *.ppm)";
		}
		else if (form == "pgm")
		{
			filterSimple += tr("Portable Graymap") + " (*.pgm)";
		}
		else if (form == "xbm" || form == "xpm")
		{
			if (!filterSimple.contains(form))
				filterSimple += tr("X11 Bitmap") + " (*.xbm *.xpm)";
		}
		else if (form == "ico")
		{
			filterSimple += tr("Icon Image File Format") + " (*.ico)";
		}
		else if (form == "jp2" || form == "j2k")
		{
			if (!filterSimple.contains(form))
				filterSimple += tr("JPEG 2000") + " (*.jp2 *.j2k)";
		}
		else if (form == "tif" || form == "tiff")
		{
			if (!filterSimple.contains(form))
				filterSimple += tr("Tagged Image File Format") + " (*.tif *.tiff)";
		}
		else
		{
			filterSimple += tr("Unknown Format") + QString(" (*.%1)").arg(form);
		}

		if (y != formats.count() - 1 && !filterSimple.endsWith(";;"))
			filterSimple += ";;";
	}

	filterSimple = filterSimple.left(filterSimple.length() - 2);
	filter += filterSimple;

	return filter;
}

void QTextPanelControl::in_image(int id)
{
	QApplication::restoreOverrideCursor();
	LoadGetImage *ht = qobject_cast<LoadGetImage *>(sender());

	if (ht)
	{
		QPixmap  imagen = ht->pics();
		insertPixmap(imagen);
	}
}

void QTextPanelControl::showhtml()
{
	int modus = 0;
	QAction *ali = qobject_cast<QAction *>(sender());
	if (ali)
	{
		modus = ali->data().toInt();
	}

	/*
		SHOW_SOURCE_HTML = 703,
		SHOW_SOURCE_SCRIBE = 704,
		SHOW_SOURCE_FOP = 705,
	*/


	QDomDocument *sxml;
	if (modus == 703)
	{
		sxml = new QDomDocument();
		sxml->setContent(_d->toHtml("utf-8"),false);
	}
	else if (modus == 704)
	{
		ScribeParser *parsen = new ScribeParser(_d, ScribeParser::Psave);
		sxml = parsen->dom();
	}
	else if (modus == 705)
	{
		ScribeParser *parsen = new ScribeParser(_d,ScribeParser::Psave);
		sxml = parsen->dom();
	}
	else
	{
		sxml = new QDomDocument();
		sxml->setContent(_d->toHtml("utf-8"),false);
	}
}


ScribePage::ScribePage(PanelPageSize e) : QTextPanelControl(PAGES)
{
	PageTotal = 1;
	_d = new QTextDocument();
	SwapPageModel(e,true);
}


/*
####################################################################################
################################ Paint section #####################################
####################################################################################
*/

void ScribePage::paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget)
{
	PageTotal = _d->pageCount();
	const int PageSumm = qBound(1,_d->pageCount(),MaximumPages);
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	const QRectF ActiveBlock = CurrentBlockRect();   /* discovery qtextcursor living page  currentPageNumber  */

	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::lightGray);
	painter->drawRect(boundingRect());
	painter->restore();
	for (int i = 0; i < PageSumm; ++i)
	{
		const QPointF topleft = PageIndexTopLeft(i);  /* page top left point */
		DrawPage(i,painter);
	}
}


void ScribePage::paintPage(const int index  , QPainter * painter , bool printer )
{
    
    const QRectF pagen =  Model().PageExternal(index);
    if (!printer) {
    /*  draw ground */
    painter->save();
    painter->setBrush(QColor(Qt::white));
    painter->setPen(QPen(Qt::black,0.3));
    painter->drawRect(pagen);
    painter->restore();
    }
    /*  draw ground */
    const QPointF topleft = PageIndexTopLeft(index);
    QAbstractTextDocumentLayout::PaintContext CTX;
    /*  draw txt */
    CTX.cursorPosition = -1;
	CTX.palette.setColor(QPalette::Text, Qt::black);
	const QRectF body = QRectF(0, topleft.y() ,Page_Edit_Rect.width(),Page_Edit_Rect.height()); /* on view */
	QRectF view(0, index * body.height(), body.width(), body.height());
        painter->save();
        /* printer not translate ??? */
		painter->translate(body.left(), body.top() - index * body.height());
		painter->setClipRect(view);
		CTX.clip = view;
		_d->documentLayout()->draw(painter,CTX);
		painter->restore();
    /*  draw txt */
    /*  draw shadow  */
    if (!printer) {
    painter->save();
    QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
    QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
    painter->fillRect(rightShadow, Qt::darkGray);
    painter->fillRect(bottomShadow, Qt::darkGray);
    painter->restore();
    /*  draw shadow  */
    }
    
    
}

void ScribePage::DrawPage(const int index  , QPainter * painter )
{
	const QPointF topleft = PageIndexTopLeft(index);
	QAbstractTextDocumentLayout::PaintContext CTX;
	CTX.palette.setColor(QPalette::Text, Qt::black);
	const QRectF body = QRectF(0, topleft.y() ,Page_Edit_Rect.width(),Page_Edit_Rect.height()); /* on view */
	QRectF view(0, index * body.height(), body.width(), body.height());    /* on doc */
    drawPageGround(painter,index,Model());
    
	if (!Edit_On())
	{
        /* no cursor */
		painter->save();
		painter->translate(body.left(), body.top() - index * body.height());
		painter->setClipRect(view);
		CTX.clip = view;
		_d->documentLayout()->draw(painter,CTX);
		painter->restore();
		return;
	}

	/* draw cursor active page no !edit no print only display edit! */
	painter->save();
	painter->translate(body.left(), body.top() - index * body.height());
	painter->setClipRect(view);
	CTX.clip = view;

	QColor BackHightlight("#0072ab");
	BackHightlight.setAlpha(180);   /* original 150 */


	CTX.palette.setColor(QPalette::Text, Qt::black);
	CTX.palette.setColor(QPalette::Highlight,BackHightlight);
	CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
	CTX.selections;
	CTX.clip = view;
	CTX.cursorPosition = -1;
	/* play cursor */

	if (!PlayCursorMode)
	{

		if (cursortime)
		{
			CTX.cursorPosition = controlTextCursor.position();
		}


		if (controlTextCursor.hasSelection())
		{
			QAbstractTextDocumentLayout::Selection Internal_selection;
			Internal_selection.cursor = controlTextCursor;
			cursorIsFocusIndicator = true;
			QPalette::ColorGroup cg = cursorIsFocusIndicator ? QPalette::Active : QPalette::Inactive;
			Internal_selection.format.setBackground(CTX.palette.brush(cg, QPalette::Highlight));
			Internal_selection.format.setForeground(CTX.palette.brush(cg, QPalette::HighlightedText));
			Internal_selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			CTX.selections.append(Internal_selection);
		}
	}

	_d->documentLayout()->draw(painter,CTX);
	painter->restore();

	if (PlayCursorMode && !controlTextCursor.currentTable() && cursortime && Edit_On())
	{
        /* ScrLk  on */
		painter->save();
		const QLineF cursorLiner2 = ViewBlinkedCursorLine();
		painter->setPen(QPen(Qt::red,0.6));
		painter->setBrush(Qt::red);
		painter->drawLine(cursorLiner2);
		painter->restore();
	}

	/*
	QLineF cursorLiner = BlinkCursorLine();
	QLineF cursorLiner2 = ViewBlinkedCursorLine();

	painter->save();
	painter->setPen( QPen(Qt::red,3) );
	painter->setBrush(Qt::red);
	painter->drawEllipse(cursorLiner.p2(),5,5);

	painter->setPen( QPen(Qt::green,3) );
	painter->setBrush(Qt::green);
	painter->drawEllipse(cursorLiner2.p2(),5,5);

	painter->restore();
	*/
}

QPointF ScribePage::PageIndexTopLeft(const int index)
{
	return Model().PageIndexTopLeft(index);
}

/*
####################################################################################
################################ Paint section #####################################
####################################################################################

*/

bool ScribePage::AllowedPosition(const QPointF inpos)
{
	bool permission = false;
	for (int i = 0; i < _d->pageCount(); ++i)
	{
		QRectF sssx = PAGE_MODEL.PageInternal(i);
		if (sssx.contains(inpos))
		{
			OnPageClick = i + 1;
			return true;
		}
	}

	return permission;
}

QRectF ScribePage::boundingRect()
{
	const int page = qBound(1,_d->pageCount(),MaximumPages);
	const qreal spacer = page + ((page - 1) * InterSpace);
	const qreal pagesummhi = page * Page_Height;
	if (PageTotal != page)
	{
		PageTotal = page;
        startCache();

		for (int i = OnPageClick; i < _d->pageCount(); ++i)
		{
			const QPointF topleft = PageIndexTopLeft(i);
			QRectF PaperAreas(topleft,QSizeF(Page_Width,Page_Height));
			emit q_update(PaperAreas.toRect());
		}
		emit q_update_scene();
	}
	return QRectF(0,0,Page_Width,pagesummhi + spacer);
}

QRectF ScribePage::GroupboundingRect()
{
	const QRectF onlypage = boundingRect();
	return QRectF(0,0,onlypage.width() + BorderShadow,onlypage.height() + BorderShadow);
}

void ScribePage::SwapPageModel(PanelPageSize e , bool firstime )
{
	QTextPanelData::instance()->SetPageFormat(e);
	PAGE_MODEL = e;
	QTextOption opt;
	opt.setUseDesignMetrics(true);
	opt.setTabStop(8);
	opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	_d->setDefaultTextOption(opt);

	PAGE_MODEL.HandlePrint(_d); /* set page format margin  */
	Q_ASSERT(_d->pageSize().isValid());
	_d->setUseDesignMetrics(true);
	Page_Width = PAGE_MODEL.G_regt.width();
	Page_Height = PAGE_MODEL.G_regt.height();
	Page_Edit_Rect = PAGE_MODEL.G_regt;
	_d->setPageSize(QSizeF(Page_Width,Page_Height));
	Q_ASSERT(_d->pageSize().isValid());
	(void)_d->documentLayout(); /* reform margin wake up */
	PageTotal = _d->pageCount();
    if (!firstime) {
    startCache();
    }
	q_update(boundingRect().toRect());
}



void ScribePage::setDocument(const QTextDocument * document , FileHandlerType Type , bool oncache)
{
	if (_d)
	{
		_d->clear();
		_d->disconnect(this);
		_d->documentLayout()->disconnect(this);
		_d = 0;
	}
	else
	{
		_d = new QTextDocument(0);
	}

	clipboard = QApplication::clipboard();
	Op = Type;
	_d = const_cast<QTextDocument*>(document);

	for (QTextBlock srcBlock = document->firstBlock(), dstBlock = _d->firstBlock();
	      srcBlock.isValid() && dstBlock.isValid();
	      srcBlock = srcBlock.next(), dstBlock = dstBlock.next())
	{
		dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
	}
	_d->setUndoRedoEnabled(false);
	PanelPageSize DefaultSizeDoc;
	SwapPageModel(DefaultSizeDoc,true);
	PageTotal = _d->pageCount();
	Q_ASSERT(PageTotal > 0);
	controlTextCursor = QTextCursor(_d);
	controlTextCursor.setPosition(0,QTextCursor::MoveAnchor);
	setBlinkingCursorEnabled(true);
    if (oncache) {
    qDebug() << "### start cache theard ";
    startCache();  /* make first image cache */
    }
	QObject::connect(clipboard, SIGNAL(dataChanged()), this, SLOT(int_clipboard_new()));
	QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(cursorPosition(QTextCursor)));
	QObject::connect(_d, SIGNAL(modificationChanged(bool)), this, SLOT(ChangeFormatDoc(bool)));
	QObject::connect(_d, SIGNAL(documentLayoutChanged()), this, SLOT(ChangeFormatDoc()));
	QObject::connect(_d, SIGNAL(contentsChange(int,int,int)), this, SLOT(SessionUserInput(int,int,int)));
}

void ScribePage::setCacheDocument(const QTextDocument * document )
{
    _d = const_cast<QTextDocument*>(document);
	for (QTextBlock srcBlock = document->firstBlock(), dstBlock = _d->firstBlock();
	      srcBlock.isValid() && dstBlock.isValid();
	      srcBlock = srcBlock.next(), dstBlock = dstBlock.next())
	{
		dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
	}
    
}


void ScribePage::startCache()
{
    if (!allPageCache.isNull()) {
    allPageCache = QImage();
    /* remove before next paint grab */
    }
    
    /*  from running theard is ok */
    ////////this,Model(),_d->clone()
    
    QRichPage streamDoc;
    streamDoc.load(_d,Model());
    cacheJob = new CachePainter(streamDoc);
    cacheJob->start (QThread::HighPriority);
    qRegisterMetaType<PanelPageSize>("PanelPageSize");
    qRegisterMetaType<QPicture>("QPicture");
    connect(cacheJob, SIGNAL(cgenerator(QImage)), this, SLOT(docCache(QImage)));
    connect(cacheJob, SIGNAL(cstatus(int,int)), this, SLOT(cstatus(int,int)));
    
}

void ScribePage::cstatus( const int currentdraw , const int tot )
{
    
    qDebug() << "### page draw ->   " << currentdraw << "/" << tot;
}

void ScribePage::docCache( QImage img )
{
    if (!img.isNull()) {
    allPageCache = img;
    }
    if (cacheJob) {
    cacheJob = NULL;
    }
    emit q_update_scene();
}




void ScribePage::PageUpdate()
{
	SwapPageModel(PAGE_MODEL);
	ChangeFormatDoc(true);
	ALL_Page_Edit_Rect = this->boundingRect();
	emit q_update_scene();
}

/* story board of text input */
void ScribePage::SessionUserInput(int position, int charsRemoved, int charsAdded)
{

	ALL_Page_Edit_Rect = this->boundingRect();
	///////qDebug() << "### SessionUserInput...." << position << charsRemoved << charsAdded;
	const int selectionLength = qAbs(controlTextCursor.position() - controlTextCursor.anchor());
	if (charsRemoved > 0 && selectionLength > 0)
	{
		/* reformat text before drag clear all drag ! but  not selection */
		ChangeFormatDoc(true);
	}
}

void QTextPanelControl::ChangeFormatDoc(bool e)
{
	///////qDebug() << "### ChangeFormatDoc ...." << e;
	if (e)
	{
		ResetClickTimer();
		DragFill = false;
	}
	/* big modification change ?? */
}

void QTextPanelControl::UnderlineText()
{
	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	format.setUnderlineStyle(c.charFormat().underlineStyle() == QTextCharFormat::NoUnderline ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline);
	c.setCharFormat(format);
}

void QTextPanelControl::StrickText()
{
	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	QFont f = format.font();
	f.setStrikeOut(f.strikeOut() == false ? true : false);
	format.setFont(f);
	c.setCharFormat(format);
}

void QTextPanelControl::OverlineText()
{
	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	QFont f = format.font();
	f.setOverline(f.overline() == false ? true : false);
	format.setFont(f);
	c.setCharFormat(format);
}

void QTextPanelControl::BoldText()
{
	QTextCharFormat format = textCursor().charFormat();
	///////////format.setFontWeight(actionBold->isChecked() ? QFont::Bold : QFont::Normal);
	format.setFontWeight(!textCursor().charFormat().font().bold()  ? QFont::Bold : QFont::Normal);
	textCursor().setCharFormat(format);
}

void QTextPanelControl::FontText()
{
	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	QFont f = format.font();

	bool ok;
	QFont fontyour = QFontDialog::getFont(&ok,f,0);
	if (ok)
	{
		format.setFont(fontyour);
		c.setCharFormat(format);
	}
}

QStringList QTextPanelControl::HubBlockids()
{
	ScribeParser *parsen = new ScribeParser(document(),ScribeParser::Plinker);
	return parsen->internals();
}


void  QTextPanelControl::LinkText()
{

}

void QTextPanelControl::ParaBGcolor()
{
	QRgb col = QColorDialog::getRgba(textCursor().charFormat().background().color().rgb());
	QColor col2 = QColor(col);
	int trans = qAlpha(col);
	col2.setAlpha(trans);

	if (!col2.isValid()) {return;}

	QTextCursor c = textCursor();
	QTextBlockFormat format = c.block().blockFormat();
	format.setProperty(_BG_CHAR_ALPHA_,trans);
	format.setBackground(QBrush(col2));
	c.setBlockFormat(format);
}

void QTextPanelControl::FontsLetterSpacing()
{
	const qreal spacingnow = textCursor().charFormat().fontLetterSpacing();
	bool ok;
	qreal space = QInputDialog::getDouble(0, tr("Font Letter Spacing"),
	                                      tr("Space:"),spacingnow,90,2000, 2, &ok);
	if (space > 0 && ok)
	{
		QTextCursor c = textCursor();
		QTextCharFormat format = c.charFormat();
		format.setFontLetterSpacing(space);
		c.setCharFormat(format);
	}
}

void QTextPanelControl::BGcolor()
{
	QRgb col = QColorDialog::getRgba(textCursor().charFormat().background().color().rgb());
	QColor col2 = QColor(col);
	int trans = qAlpha(col);
	col2.setAlpha(trans);

	if (!col2.isValid()) {return;}

	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	format.setProperty(_BG_CHAR_ALPHA_,trans);
	format.setBackground(QBrush(col2));
	c.setCharFormat(format);
}

void QTextPanelControl::TXcolor()
{
	QRgb col = QColorDialog::getRgba(textCursor().charFormat().foreground().color().rgb());
	QColor col2 = QColor(col);
	int trans = qAlpha(col);
	col2.setAlpha(trans);

	if (!col2.isValid()) {return;}

	QTextCursor c = textCursor();
	QTextCharFormat format = c.charFormat();
	format.setProperty(_TXT_CHAR_ALPHA_,trans);
	format.setForeground(QBrush(col2));
	c.setCharFormat(format);
}


void  QTextPanelControl::SetParaMargin()
{
	QTextCursor c = textCursor();
	QTextBlock textblocc = c.block();

	GetMargin *marge = new GetMargin(0);
	marge->setWindowTitle(tr("Set Paragraph BlockFormat margin"));
	QTextBlockFormat formatibb = textblocc.blockFormat();
	QRectF bbmargin(formatibb.topMargin(), formatibb.rightMargin() ,formatibb.bottomMargin() , formatibb.leftMargin());

	marge->Set_margin(bbmargin);
	int faxme = marge->exec();

	if (faxme == 1)
	{

		QRectF setFrams = marge->Get_margin();
		const qreal TopMargin = setFrams.x();
		const qreal RightMargin = setFrams.y();
		const qreal BottomMargin = setFrams.width();
		const qreal LeftMargin = setFrams.height();

		formatibb.setLeftMargin(LeftMargin);
		formatibb.setBottomMargin(BottomMargin);
		formatibb.setTopMargin(TopMargin);
		formatibb.setRightMargin(RightMargin);
		textCursor().setBlockFormat(formatibb);
	}
}

void  QTextPanelControl::SwapUnbrekableBlock()
{
	QTextCursor c = textCursor();
	QTextBlock textblocc = c.block();
	QTextBlockFormat formatibb = textblocc.blockFormat();
	formatibb.setNonBreakableLines(formatibb.hasProperty(QTextFormat::BlockNonBreakableLines) == true ? false : true);
	c.setBlockFormat(formatibb);
}

void  QTextPanelControl::SetElementMargin()
{
	QTextFrame  *RootFrame = document()->rootFrame();
	GetMargin *marge = new GetMargin(0);
	int caseformat = 0;
	marge->setWindowTitle(tr("Set Paragraph BlockFormat margin"));
	QTextCursor c = textCursor();
	QTextBlock textblocc = c.block();
	QTextBlockFormat formatibb = textblocc.blockFormat();
	QRectF bbmargin(formatibb.topMargin(), formatibb.rightMargin() ,formatibb.bottomMargin() , formatibb.leftMargin());

	if (c.currentFrame() && c.currentFrame() != RootFrame)
	{
		QTextFrameFormat form = c.currentFrame()->frameFormat();
		bbmargin = QRectF(form.topMargin(),form.rightMargin(),form.bottomMargin(),form.leftMargin());
		marge->setWindowTitle(tr("Set inline frame margin"));
		caseformat = 1;
	}

	if (c.currentTable())
	{
		QTextTableFormat form = c.currentTable()->format();
		bbmargin = QRectF(form.topMargin(),form.rightMargin(),form.bottomMargin(),form.leftMargin());
		marge->setWindowTitle(tr("Set Table margin"));
		caseformat = 2;
	}

	marge->Set_margin(bbmargin);

	int faxme = marge->exec();

	if (faxme == 1)
	{
		QRectF setFrams = marge->Get_margin();
		const qreal TopMargin = setFrams.x();
		const qreal RightMargin = setFrams.y();
		const qreal BottomMargin = setFrams.width();
		const qreal LeftMargin = setFrams.height();
		if (caseformat == 0)
		{
			formatibb.setLeftMargin(LeftMargin);
			formatibb.setBottomMargin(BottomMargin);
			formatibb.setTopMargin(TopMargin);
			formatibb.setRightMargin(RightMargin);
			textCursor().setBlockFormat(formatibb);
		}
		else if (caseformat == 1)
		{
			QTextFrameFormat foframe = c.currentFrame()->frameFormat();
			foframe.setLeftMargin(LeftMargin);
			foframe.setBottomMargin(BottomMargin);
			foframe.setTopMargin(TopMargin);
			foframe.setRightMargin(RightMargin);
			c.currentFrame()->setFrameFormat(foframe);
		}
		else if (caseformat == 2)
		{
			QTextTableFormat fotable = c.currentTable()->format();
			fotable.setLeftMargin(LeftMargin);
			fotable.setBottomMargin(BottomMargin);
			fotable.setTopMargin(TopMargin);
			fotable.setRightMargin(RightMargin);
			c.currentTable()->setFormat(fotable);
		}
	}
}

void ScribePage::ParaBlockPageBreackPolicyInsert()
{
	///////if (Modus != PAGES) {
	////////return;   /* only on pages draw modus not flat */
	////////}

	QTextFrame  *RootFrame = document()->rootFrame();

	QTextCursor c = textCursor();
	QTextBlock bb = c.block();
	QTextBlockFormat bbformat = bb.blockFormat();
	QTextFormat::PageBreakFlags actual = bbformat.pageBreakPolicy();

	if (c.currentTable())
	{
		actual = c.currentTable()->format().pageBreakPolicy();
	}
	if (c.currentFrame() && c.currentFrame() != RootFrame)
	{
		actual = c.currentFrame()->frameFormat().pageBreakPolicy();
	}

	QStringList items;
	items << tr("PageBreak Auto") << tr("PageBreak AlwaysBefore") << tr("PageBreak AlwaysAfter");
	int index  = 0;

	if (actual == QTextFormat::PageBreak_AlwaysBefore)
	{
		index = 1;
	}
	else if (actual == QTextFormat::PageBreak_AlwaysAfter)
	{
		index = 2;
	}

	bool ok;
	QString item = QInputDialog::getItem(0, tr("Get PageBreakFlags"), tr("Set Policy:"), items,index, false, &ok);

	if (ok && item.size() > 0)
	{
		int choise = items.indexOf(item);
		if (choise == 1)
		{
			actual = QTextFormat::PageBreak_AlwaysBefore;
		}
		else if (choise == 2)
		{
			actual = QTextFormat::PageBreak_AlwaysAfter;
		}
		else
		{
			actual = QTextFormat::PageBreak_Auto;
		}

		if (c.currentTable())
		{
			QTextTableFormat fotable = c.currentTable()->format();
			fotable.setPageBreakPolicy(actual);
			c.currentTable()->setFormat(fotable);
		}
		else if (c.currentFrame() && c.currentFrame() != RootFrame)
		{
			QTextFrameFormat foframe = c.currentFrame()->frameFormat();
			foframe.setPageBreakPolicy(actual);
			c.currentFrame()->setFrameFormat(foframe);
		}
		else
		{
			bbformat.setPageBreakPolicy(actual);
			c.setBlockFormat(bbformat);
		}
		PageUpdate();
	}
}


///// ico color textCursor().currentFrame()->frameFormat().background().color()
void  QTextPanelControl::SetFrameBGColor()
{
	if (textCursor().currentFrame())
	{
		QTextFrame *frame = textCursor().currentFrame();
		QTextFrameFormat Ftf = frame->frameFormat();
		bool ok;
		/* get color */
		QColor col = QColorDialog::getRgba(Ftf.background().color().rgba(),&ok, 0);
		if (!col.isValid())
		{
			return;
		}
		QBrush stylesin(col);
		Ftf.setBackground(stylesin);
		frame->setFrameFormat(Ftf);
	}
}

void QTextPanelControl::FosInsertFrame()
{
	QColor BackHightlight("#0072ab");
	BackHightlight.setAlpha(60);


	QTextFrameFormat frame;
	frame.setBorder(1);
	frame.setBorderBrush(QBrush(Qt::red));
	frame.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
	frame.setBackground(BackHightlight);
	QTextLength mesure(QTextLength::FixedLength,180);
	frame.setWidth(mesure);
	frame.setPadding(10);
	frame.setPosition(QTextFrameFormat::FloatRight);
	textCursor().insertFrame(frame);
}

void  QTextPanelControl::SetColumLarge()
{
	if (textCursor().currentTable())
	{
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		QTextTableFormat tbforms = textCursor().currentTable()->format();
		int cellcoolcursoris = existingcell.column(); /* int value start from zero */
		bool ok;
		int LargeSet = QInputDialog::getInteger(0, tr("Set Cell Width"),
		                                        tr("Point Length:"),Get_Cell_Width(tbforms,cellcoolcursoris), 1, 2000, 1, &ok);
		if (ok && LargeSet > 0)
		{
			QVector<QTextLength> constraints = tbforms.columnWidthConstraints();
			for (int i = 0; i < constraints.size(); ++i)
			{
				if (i == cellcoolcursoris)
				{
					constraints.replace(i,QTextLength(QTextLength::FixedLength, LargeSet));
				}
			}
			tbforms.setColumnWidthConstraints(constraints);
			textCursor().currentTable()->setFormat(tbforms);
		}
	}
}

qreal QTextPanelControl::Get_Cell_Width(QTextTableFormat TableFormat , int position)
{
	qreal notfound = 0;
	QVector<QTextLength> constraints = TableFormat.columnWidthConstraints();
	for (int i = 0; i < constraints.size(); ++i)
	{
		if (i == position)
		{
			QTextLength langecell = constraints.value(i);
			if (langecell.type() == QTextLength::FixedLength)
			{
				return langecell.rawValue();
			}
		}
	}

	return notfound;
}

void  QTextPanelControl::MergeCellByCursorPosition()
{
	if (textCursor().currentTable())
	{
		textCursor().currentTable()->mergeCells(textCursor());
	}
}

void QTextPanelControl::RemoveCoolByCursorPosition()
{
	if (textCursor().currentTable())
	{
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		int cellcoolcursoris = existingcell.column(); /* int value start from zero */
		int cellrowcursoris = existingcell.row(); /* int value start from zero */
		textCursor().currentTable()->removeColumns(cellcoolcursoris,1);
	}
}

void  QTextPanelControl::RemoveRowByCursorPosition()
{
	if (textCursor().currentTable())
	{
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		int cellcoolcursoris = existingcell.column(); /* int value start from zero */
		int cellrowcursoris = existingcell.row(); /* int value start from zero */
		textCursor().currentTable()->removeRows(cellrowcursoris,1);
	}
}

void  QTextPanelControl::AppendTableRows()
{
	bool ok = false;
	if (textCursor().currentTable())
	{
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		int cellcoolcursoris = existingcell.column(); /* int value start from zero */
		int cellrowcursoris = existingcell.row(); /* int value start from zero */
		int approwtot = QInputDialog::getInteger(0, tr("Append NR. line row"),tr("Row:"), 1, 1, 100, 1, &ok);
		if (ok && approwtot > 0)
		{
			textCursor().currentTable()->insertRows(cellrowcursoris + 1,approwtot);
		}
	}
}

void  QTextPanelControl::AppendTableCools()
{
	bool ok = false;
	if (textCursor().currentTable())
	{
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		int cellcoolcursoris = existingcell.column(); /* int value start from zero */
		int cellrowcursoris = existingcell.row(); /* int value start from zero */
		int appcooltot = QInputDialog::getInteger(0, tr("Table append Column"),tr("Cool:"), 1, 1, 10, 1, &ok);
		if (ok && appcooltot > 0)
		{
			textCursor().currentTable()->insertColumns(cellcoolcursoris + 1,appcooltot);
		}
	}
}

void  QTextPanelControl::SetTableCellColor()
{
	if (textCursor().currentTable())
	{
		bool ok;
		QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
		/* reformat this -> existingcell */
		QTextCharFormat existformat = existingcell.format();
		/* get color */
		QColor col = QColorDialog::getRgba(textCursor().currentTable()->cellAt(textCursor()).format().background().color().rgba(),&ok, 0);
		if (!col.isValid())
		{
			return;
		}
		QBrush stylesin(col);
		existformat.setBackground(stylesin);
		existingcell.setFormat(existformat);
	}
}
void QTextPanelControl::UpdateTableFormat()
{
	if (!textCursor().currentTable()) {return;}

	Table_Setting::self(0)->SetFormat(textCursor().currentTable());
	const int xx = Table_Setting::self(0)->exec();
	QTextTableFormat newformine = Table_Setting::self(0)->GetNewFormats();
	if (newformine.isValid() && xx == 1)
	{
		textCursor().currentTable()->setFormat(newformine);
	}
}



void  QTextPanelControl::CreateanewTable()
{
	QString subtext, collx, rowx,largo;
	bool ok;
	int colonne = QInputDialog::getInteger(0, tr("New Table cool"),tr("Cool:"), 3, 1, 10, 1, &ok);
	int righe = QInputDialog::getInteger(0, tr("New Table row"),tr("Row:"), 3, 1, 100, 1, &ok);
	largo = "100%";
	if (colonne > 0 && righe > 0)
	{
		QStringList tables;
		tables.clear();
		tables.append(QString("<table border=\"1\" align=\"left\" width=\"%1\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#ffffff\">").arg(largo));
		for (int i=0;i<righe;i++)
		{
			tables.append(QString("<tr>"));
			for (int o=0;o<colonne;o++)
			{
				tables.append(QString("<td><p></p></td>"));
			}
			tables.append(QString("</tr>"));
		}
		tables.append(QString("</table>"));

		subtext = tables.join("\n");
		QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(subtext);
		textCursor().insertFragment(fragment);
	}
}

void QTextPanelControl::MaketableColorBG()
{
	if (!textCursor().currentTable()) {return;}

	bool ok;
	QColor col = QColorDialog::getRgba(textCursor().currentTable()->format().background().color().rgba(),&ok,0);

	if (!col.isValid())
	{
		return;
	}
	else
	{
		QTextTableFormat taform = textCursor().currentTable()->format();
		taform.setBackground(col);
		textCursor().currentTable()->setFormat(taform);
	}
}




/* #################################### floating text layer  ###########################################################  */
LayerText::LayerText() : QTextPanelControl(FLAT)
{
	PageTotal = 1;
	QTextDocument *dummy = new QTextDocument();
	dummy->setHtml("<p>Floating Layer2...</p>");    /////  ReadFile("a.html")
	setDocument(dummy,FOP);
}


void LayerText::SetRect(const QRectF re)
{
	_d->setPageSize(re.size());
	Page_Edit_Rect = re;
	Page_Width = Page_Edit_Rect.width();
	Page_Height = Page_Edit_Rect.height();

}


void LayerText::paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget)
{
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	const QRectF stxt = _d->documentLayout()->frameBoundingRect(Tframe);

	QAbstractTextDocumentLayout::PaintContext CTX;
	painter->save();
	painter->setClipRect(stxt);
	CTX.clip = stxt;
	QColor BackHightlight("#0072ab");
	BackHightlight.setAlpha(180);   /* original 150 */
	CTX.palette.setColor(QPalette::Text, Qt::black);
	CTX.palette.setColor(QPalette::Highlight,BackHightlight);
	CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
	CTX.cursorPosition = -1;
	if (cursorTimeLine.isActive() && cursortime)
	{
		CTX.cursorPosition = textCursor().position();
	}
	if (textCursor().hasSelection())
	{
		QAbstractTextDocumentLayout::Selection Internal_selection;
		Internal_selection.cursor = textCursor();
		cursorIsFocusIndicator = true;
		QPalette::ColorGroup cg = cursorIsFocusIndicator ? QPalette::Active : QPalette::Inactive;
		Internal_selection.format.setBackground(CTX.palette.brush(cg, QPalette::Highlight));
		Internal_selection.format.setForeground(CTX.palette.brush(cg, QPalette::HighlightedText));
		Internal_selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		CTX.selections.append(Internal_selection);
	}
	_d->documentLayout()->draw(painter,CTX);
	painter->restore();
}

void LayerText::setDocument(const QTextDocument * document , FileHandlerType Type)
{
	if (_d)
	{
		_d->clear();
		_d->disconnect(this);
		_d->documentLayout()->disconnect(this);
		_d = 0;
	}
	else
	{
		_d = new QTextDocument(0);
	}
	clipboard = QApplication::clipboard();
	Op = Type;
	_d = const_cast<QTextDocument*>(document);

	for (QTextBlock srcBlock = document->firstBlock(), dstBlock = _d->firstBlock();
	      srcBlock.isValid() && dstBlock.isValid();
	      srcBlock = srcBlock.next(), dstBlock = dstBlock.next())
	{
		dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
	}

	QTextFrame  *Tframe = _d->rootFrame();
	QTextFrameFormat Ftf = Tframe->frameFormat();
	Ftf.setLeftMargin(0);
	Ftf.setBottomMargin(0);
	Ftf.setTopMargin(0);
	Ftf.setRightMargin(0);
	Ftf.setPadding(0);
	Tframe->setFrameFormat(Ftf);


	_d->setUndoRedoEnabled(false);
	controlTextCursor = QTextCursor(_d);
	controlTextCursor.setPosition(0,QTextCursor::MoveAnchor);
	setBlinkingCursorEnabled(true);
	QObject::connect(clipboard, SIGNAL(dataChanged()), this, SLOT(int_clipboard_new()));
	QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(cursorPosition(QTextCursor)));
	///////QObject::connect(_d, SIGNAL(modificationChanged(bool)), this, SLOT(ChangeFormatDoc(bool)));
	///////QObject::connect(_d, SIGNAL(documentLayoutChanged()), this, SLOT(ChangeFormatDoc()));
	/////QObject::connect(_d, SIGNAL(contentsChange(int,int,int)), this, SLOT(SessionUserInput(int,int,int)));
	///////////QObject::connect(_d, SIGNAL(q_pageupdate()), this, SLOT(PageUpdate()));
}


/*   Parser QTextDocument  */
ScribeParser::ScribeParser(QTextDocument *doc, ScribeParseModus e)
{
	textDocument = doc;
	QTextFrame *root = doc->rootFrame();
	helper_cursor = QTextCursor(doc);

	Internal_Destination_Links.clear();
	modus = e;
	QDomImplementation implementation;
	QDomDocumentType docType = implementation.createDocumentType("scribe-document", "scribe","www.trolltech.com/scribe");

	document = new QDomDocument(docType);
	QDomProcessingInstruction process = document->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	document->appendChild(process);

	QDomElement documentElement = document->createElement("document");
	document->appendChild(documentElement);
	if (root)
	{
		processFrame(documentElement,root);
	}
}

void ScribeParser::processFrame(QDomElement appender ,  QTextFrame *frame)
{
	//~ qDebug() << "### processFrame   ";

	QDomElement frameElement = document->createElement("frame");
	frameElement.setAttribute("begin", frame->firstPosition());
	frameElement.setAttribute("end", frame->lastPosition());
	appender.appendChild(frameElement);
	QTextFrame::iterator it;
	for (it = frame->begin(); !(it.atEnd()); ++it)
	{
		QTextFrame *childFrame = it.currentFrame();
		QTextBlock childBlock = it.currentBlock();

		if (childFrame)
		{
			if (QTextTable *table = qobject_cast<QTextTable *>(it.currentFrame()))
			{
				processTable(frameElement,table);
			}
			else
			{
				processFrame(frameElement, childFrame);
			}
		}
		else if (childBlock.isValid())
		{
			processBlock(frameElement, childBlock);
		}
	}

}

///////////int QTextBlock::position () const
void ScribeParser::MemoonBlock(QTextCursor c ,  QVariant data , const int id)
{
	QTextBlockFormat bf = c.blockFormat();
	bf.setProperty(id,data);
	c.setBlockFormat(bf);

}


void ScribeParser::processBlock(QDomElement appender ,  QTextBlock   block)
{

	const int idnumerate = block.blockNumber();
	const QString blokstxt = Qt::escape(block.text());
	QString humantxt = imageName(blokstxt.toUpper());
	humantxt.truncate(4);
	const QString InternDestName = QString("%1_").arg(idnumerate) + humantxt.leftJustified(10, '0');
	if (modus == Plinker && blokstxt.size() > 1 && InternDestName.size() > 9)
	{
		QTextCharFormat chformat = block.charFormat();

		if (chformat.isAnchor())
		{
			QStringList dests = chformat.anchorNames();
			for (int i = 0; i < dests.size(); ++i)
			{
				Internal_Destination_Links.append(dests.at(i));
			}

			if (dests.contains(InternDestName))
			{
				/* having insert all link and block bookmark */
				Internal_Destination_Links.append(InternDestName);

			}
			else
			{
				dests << InternDestName;
				chformat.setAnchor(true);
				chformat.setAnchorNames(dests);
				chformat.setToolTip(InternDestName);
				////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
				//////chformat.setForeground(QBrush(_LINK_COLOR_));
				Internal_Destination_Links.append(InternDestName);
				helper_cursor.setPosition(block.position());
				helper_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
				helper_cursor.setCharFormat(chformat);
				MemoonBlock(helper_cursor,InternDestName,BookMarkInternalID);
			}
		}
		else
		{
			QStringList linker;
			linker << InternDestName;
			chformat.setAnchor(true);
			chformat.setAnchorNames(linker);
			chformat.setToolTip(InternDestName);
			/////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
			/////chformat.setForeground(QBrush(_LINK_COLOR_));
			helper_cursor.setPosition(block.position());
			helper_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
			helper_cursor.setCharFormat(chformat);
			MemoonBlock(helper_cursor,InternDestName,BookMarkInternalID);
		}
	}

	QDomElement blockElement = document->createElement("block");
	blockElement.setAttribute("block_number",idnumerate);
	blockElement.setAttribute("position", block.position());
	blockElement.setAttribute("length", block.length());
	appender.appendChild(blockElement);

	QTextBlock::iterator it;
	for (it = block.begin(); !(it.atEnd()); ++it)
	{
		QTextFragment fragment = it.fragment();
		if (fragment.isValid())
		{
			processFragment(blockElement,fragment);
		}
	}
}

void ScribeParser::processFragment(QDomElement appender ,  QTextFragment  fragment)
{
	//~ qDebug() << "### processFragment   ";

	QTextCharFormat chformat = fragment.charFormat();
	if (chformat.isAnchor() && chformat.anchorNames().size() > 0)
	{
		/////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		//////chformat.setForeground(QBrush(_INTERNAL_LINK_COLOR_));
		////helper_cursor.setPosition(fragment.position() + fragment.length(),QTextCursor::KeepAnchor);
		///////helper_cursor.setCharFormat(chformat);
	}
	else if (chformat.isAnchor() &&
	         chformat.anchorHref().size() > 0 &&
	         !Clicks_Destination_Links.contains(chformat.anchorHref()))
	{
		Clicks_Destination_Links.append(chformat.anchorHref());
	}

	if (modus == Plinker)
	{
		if (chformat.isAnchor())
		{
			QStringList dests = chformat.anchorNames();
			for (int i = 0; i < dests.size(); ++i)
			{
				Internal_Destination_Links.append(dests.at(i));
			}
		}
	}

	QDomElement fragmentElement = document->createElement("fragment");
	appender.appendChild(fragmentElement);
	fragmentElement.setAttribute("length", fragment.length());
	fragmentElement.setAttribute("position", fragment.position());
	///////blockElement.setAttribute("length", block.length());
	QDomText fragmentText = document->createTextNode(fragment.text());
	fragmentElement.appendChild(fragmentText);
}

void ScribeParser::processTable(QDomElement appender , QTextTable *table)
{
	//~ qDebug() << "### processTable   ";

	QDomElement element = document->createElement("table");

	for (int row = 0; row < table->rows(); ++row)
	{
		for (int column = 0; column < table->columns(); ++column)
		{
			QTextTableCell cell = table->cellAt(row, column);
			processTableCell(element, cell);
		}
	}
	appender.appendChild(element);
}


void ScribeParser::processTableCell(QDomElement appender , QTextTableCell cell)
{

	//~ qDebug() << "### processTableCell   ";

	QDomElement element = document->createElement("cell");
	element.setAttribute("row", cell.row());
	element.setAttribute("column", cell.column());

	QTextFrame::iterator it;
	for (it = cell.begin(); !(it.atEnd()); ++it)
	{

		QTextFrame *childFrame = it.currentFrame();
		QTextBlock childBlock = it.currentBlock();

		if (childFrame)
		{
			processFrame(element, childFrame);
		}
		else if (childBlock.isValid())
		{
			processBlock(element, childBlock);
		}
	}
	appender.appendChild(element);
}

ScribeParser::~ScribeParser()
{

}







CachePainter::CachePainter( QRichPage p )
  : QThread(0)
{
  page = p;
  setTerminationEnabled(true);
}

///////////////   24 32 
void CachePainter::run()
{
    QTime st = QTime::currentTime();
	QTextOption opt;
	opt.setUseDesignMetrics(true);
	opt.setTabStop(8);
	opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    
    QTextDocument *doc = new QTextDocument;
    doc->setHtml ( QString(page.html) );
    page.imageToDoc(doc);
	doc->setDefaultTextOption(opt);
	page.model.HandlePrint(doc); /* set page format margin  */
	doc->setUseDesignMetrics(true);
	qreal Page_Width = page.model.G_regt.width();
	qreal Page_Height = page.model.G_regt.height();
	doc->setPageSize(QSizeF(Page_Width,Page_Height));
	(void)doc->documentLayout(); /* reform margin wake up */
	const int pageCount = qBound(1,doc->pageCount(),MaximumPages);
    qDebug() << "### create doc a ->" << st.msecsTo ( QTime::currentTime() );
    const qreal spacer = pageCount + ((pageCount - 1) * InterSpace);
    const qreal pagesummhi = pageCount * Page_Height;
    const QRectF grect(0,0,Page_Width,pagesummhi + spacer);
    const int hi = grect.height() + 100;
    const int wi = Page_Width + 100;
    QImage img(wi,hi,QImage::Format_ARGB32 );
    img.fill(QColor(Qt::lightGray).rgb());
    QPainter painter(&img);
    painter.setRenderHint(QPainter::TextAntialiasing);
    
    
    ///////QAbstractTextDocumentLayout::PaintContext CTX;
    //////CTX.cursorPosition = -1;
    ////CTX.palette.setColor(QPalette::Text, Qt::black);
    
    
    for (int x = 0; x < pageCount; ++x)  {
        const QRectF pagen =  page.model.PageExternal(x);
        const QPointF topleft = page.model.PageIndexTopLeft(x);
        drawPageGround(&painter,x,page.model);
	    /////////////const QRectF body = QRectF(0, topleft.y() ,Page_Width,Page_Height); /* on view */
	    //////////QRectF view(0, x * body.height(), body.width(), body.height());
        ////////////painter.save();
        /* printer not translate ??? */
		//////painter.translate(body.left(), body.top() - x * body.height());
		/////////painter.setClipRect(view);
		////////CTX.clip = view;
		//////////////doc->documentLayout()->draw(&painter,CTX);
		/////////////painter.restore(); 
        drawPageShadow(&painter,x,page.model);        
    }
    painter.end();
    qDebug() << "### create doc b ->" << st.msecsTo ( QTime::currentTime() );
    qDebug() << "###  must go false ->" << img.isNull();
    emit cgenerator(img);
    exit();
}












