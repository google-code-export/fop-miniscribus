#include "mounttextprivate.h"
/* #####################################buggi ##################################################*/



TextWriter::TextWriter(QTextDocument *doc , QObject *parent )
  : Layoutpainter(this),cursorIsFocusIndicator(false),edit_enable(false),cursorOn(false),timeline(0),
	overwriteMode(false),StartSelectionMouse(-1)
{
	bridge = parent;
	_d = new QTextDocument(this);
	setContent(Qt::AutoText,QString(),doc);
  setBlinkingCursorEnabled(false);
	edit(false);
  emit q_cursor_newPos();
  qDebug() << "### TextWriter init  html in size ->" << _d->toHtml().size();
  
}

void TextWriter::paint_doc(  QPainter * painter ,
                         const QStyleOptionGraphicsItem *option , 
                         QBrush BGpage , 
                         const QRectF fulllayer ,  
                         bool alternate )
{
	  /* Layer Background draw! */
		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(BGpage);
		painter->drawRect(fulllayer);
		painter->restore();
	
	   if (edit_enable) {  /* writteln modus icon */
				painter->save();
				QPixmap pixbg(":/img/icon.png");
				painter->drawPixmap(QPointF(boundingRect().width() - 40,8),pixbg);
				painter->restore();
			}
	
	  /* rect from frame root QTextDocument*/
	  QRectF doc_rect = boundingRect();
		QColor BackHightlight("#a6ffc7");
					 BackHightlight.setAlpha(140);
		QAbstractTextDocumentLayout::PaintContext CTX;    
		CTX.selections;
		 if (doc_rect.isValid()) {
					painter->setClipRect(doc_rect, Qt::IntersectClip);
					CTX.clip = doc_rect;
		 }
		CTX.palette.setColor(QPalette::Highlight,BackHightlight);
		CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
		/* blink cursor from timer event ! */
		if (edit_enable) {
				if (cursortime) {  /* cursor blink vom timer QApplication::cursorFlashTime() */
				CTX.cursorPosition = cursor_position;
				} else {
				CTX.cursorPosition = -1;
				}
				
				if (C_cursor.hasSelection()) {
					QAbstractTextDocumentLayout::Selection Internal_selection;
					Internal_selection.cursor = C_cursor;
								QPalette::ColorGroup cg = cursorIsFocusIndicator ? QPalette::Active : QPalette::Inactive;
								Internal_selection.format.setBackground(CTX.palette.brush(cg, QPalette::Highlight));
								Internal_selection.format.setForeground(CTX.palette.brush(cg, QPalette::HighlightedText));
								Internal_selection.format.setProperty(QTextFormat::FullWidthSelection, true);
				   CTX.selections.append(Internal_selection);
				}
	  }
    _d->documentLayout()->draw(painter,CTX);
		/* _d = QTextDocument dispay editable modus yes oderwise here not paint ! */              
}

int TextWriter::hitTest(const QPointF &point, Qt::HitTestAccuracy accuracy) const
{
    return _d->documentLayout()->hitTest(point, accuracy);
}

QTextLine TextWriter::currentTextLine(const QTextCursor &cursor)
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


QRectF TextWriter::boundingRect() const
{
	if (_d) {
	_d->adjustSize();
	QAbstractTextDocumentLayout *Layout = _d->documentLayout();
	return Layout->frameBoundingRect(_d->rootFrame());
	} else {
		return QRectF();
	}
}

void TextWriter::setContent(Qt::TextFormat format, QString text, QTextDocument *document)
{
	  imagemaps.clear();
	  cursor_position = 0;
	  bool nondoc = false;
    if (_d->isEmpty()) {
        if (document) {
					_d = document;
        } else {
					_d = new QTextDocument;
					 nondoc = true;
        }
				C_cursor = QTextCursor(_d);
				    if (!text.isEmpty()) {
							if (format == Qt::PlainText) {
                   _d->setPlainText(text);
							} else {
								 _d->setHtml(text);
								 
							}
						}
						
						QTextFrame  *Tframe = _d->rootFrame();
						QTextFrameFormat rootformats = Tframe->frameFormat();
						if (!lastrect.isNull()) {
							_d->setTextWidth(lastrect.width());
							rootformats.setWidth(lastrect.width()); 
						}
						if (nondoc) { 
						rootformats.setBottomMargin(0);
						rootformats.setTopMargin(0);
						rootformats.setRightMargin(0);
						rootformats.setLeftMargin(0);
						rootformats.setPadding(0);
						}
            Tframe->setFrameFormat(rootformats);
				
						C_cursor.setPosition(0,QTextCursor::MoveAnchor);
				    cursor_position = C_cursor.position();
						
				/////QObject::connect(_d, SIGNAL(modificationChanged(bool) ),this, SLOT(update() ));  /* paint area size? */
				QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
	      QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ),bridge, SLOT(cursorPositionChanged(QTextCursor) ));
				QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)),bridge,SIGNAL(updatearea(QRectF)));
				/////QObject::connect(_d, SIGNAL(undoAvailable(bool)),this, SIGNAL(undoAvailable(bool)));
        ///////QObject::connect(_d, SIGNAL(redoAvailable(bool)),this, SIGNAL(redoAvailable(bool)));
				_d->setUndoRedoEnabled(true);
		} else {
			 lastrect = boundingRect();
			_d->disconnect(this);
	    _d->documentLayout()->disconnect(this);
	    _d->documentLayout()->setPaintDevice(0);
      _d->clear();
			return setContent(format,text,document);
		}
}






void TextWriter::edit( bool e)
{
	edit_enable = e;
	setBlinkingCursorEnabled(e);
	qDebug() << "### edit " << e;
}

void TextWriter::updateRequest( const QRectF area )
{
	emit q_update(area);
	
}

QTextDocument *TextWriter::document()
{
  return _d; 
}

QTextCursor TextWriter::textCursor()
{
  return C_cursor; 
}


TextWriter::~TextWriter()
{
  delete _d;
}


void TextWriter::setBlinkingCursorEnabled(bool enable)
{
    if (enable && QApplication::cursorFlashTime() > 0) {
        cursorTimeLine.start( QApplication::cursorFlashTime() / 2,this);
		}  else {
        cursorTimeLine.stop();
        cursorOn = enable;
		}
		
		if (!edit_enable) {
			if (cursorIsFocusIndicator) {   /* leave by release */
				////ClearSelections();
	     }
			
		}
}

void TextWriter::ClearSelections()
{
	  cursorIsFocusIndicator = false;
    TextHighlightSelect = QRectF();
	  repaintCursor(true);
}

void TextWriter::selectAll()
{
    const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
    C_cursor.select(QTextCursor::Document);
	  cursor_position = C_cursor.position();
	  position_selection_start = C_cursor.anchor();
	  cursorIsFocusIndicator = true;
	  qDebug() << "### position_selection_start/stop selectAll" << cursor_position << "/" << position_selection_start; 
    selectionChanged(selectionLength != qAbs(C_cursor.position() - C_cursor.anchor()));
}

void TextWriter::cut()
{
    if (!C_cursor.hasSelection()) {
	    return;
		}
    copy();
    C_cursor.removeSelectedText();
	  ClearSelections();
}

void TextWriter::paste()
{
    const QMimeData *md = QApplication::clipboard()->mimeData();
    if (md) {
        insertFromMimeData(md);
		}
}

void TextWriter::copy()
{
    if (!C_cursor.hasSelection()) {
	    return;
		}
    QMimeData *data = createMimeDataFromSelection();
    QApplication::clipboard()->setMimeData(data);
}

void TextWriter::selectionChanged(bool forceEmitSelectionChanged /*=false*/)
{
    if (forceEmitSelectionChanged && C_cursor.hasSelection() ) {
         TextHighlightSelect = QRectF();
         repaintCursor(true);
		} else {
			return;
		}
		
}

void TextWriter::setCursorPosition(const QPointF &pos)
{
	 const int cursorPos = hitTest(pos, Qt::FuzzyHit);
   if (cursorPos == -1) {
   return;
	 } else {
     C_cursor.setPosition(cursorPos);
		 cursor_position = C_cursor.position();
		 ////////qDebug() << "### cursor_" << cursorPos << "position hit to from pos " << _c.position() << " l.rect->" << line_rect << " l.nr." << line_nummer;
		 //////GrepCursorData();
	 }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                          EVENTS                                                 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextWriter::timerEvent(QTimerEvent *event)
{
	
	if (!edit_enable) {
     ////return;
	}
	
	cursortime = false;
	
    if (event->timerId() == cursorTimeLine.timerId()) {
        timeline++;
			
            if (timeline%2) {
							cursortime = true; 
							emit q_cursor_newPos();
            } else {
							cursortime = false;
            }
						 /////////qDebug() << "### cursortime  " << cursortime << _d->isUndoRedoEnabled() << _d->isUndoAvailable();
						
				repaintCursor();
    }
}

void TextWriter::tkeyPressEvent(QKeyEvent *e)
{
	  if(!_d) {
		return;
		}
		
		if (C_cursor.isNull())  {
			return;
		}
		bool navigatelink = false;
		
		
	
#ifndef QT_NO_SHORTCUT
    if (e == QKeySequence::SelectAll) {
            e->accept();
            selectAll();
            return;
    }
#endif
#ifndef QT_NO_CLIPBOARD
    else if (e == QKeySequence::Copy) {
            e->accept();
            copy();
            return;
    }
#endif
		
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_B) {
            e->accept();
			qDebug() << "### a BoldText / BoldText " << textCursor().position();
            BoldText();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_I) {
            e->accept();
            ItalicText();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_U) {
            e->accept();
            UnderlineText();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Z) {
            e->accept();
            undo();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Y) {
            e->accept();
            redo();
						return;
		}
	
	if (false) {
	} else if (cursorMoveKeyEvent(e)) {
		goto accept_move_cursor;  /* jump line */
	} else if (e == QKeySequence::Undo) {
            undo();
    }
    else if (e == QKeySequence::Redo) {
           redo();
    } else if (e == QKeySequence::Cut) {
           cut();
			     position_selection_start = 0;
    }
    else if (e == QKeySequence::Paste) {
           paste();
			     position_selection_start = 0;
    } else if (e->key() == Qt::Key_Backspace && !(e->modifiers() & ~Qt::ShiftModifier)) {
        QTextBlockFormat blockFmt = C_cursor.blockFormat();
        QTextList *list = C_cursor.currentList();
        if (list && C_cursor.atBlockStart()) {
            list->remove(C_cursor.block());
        } else if (C_cursor.atBlockStart() && blockFmt.indent() > 0) {
            blockFmt.setIndent(blockFmt.indent() - 1);
            C_cursor.setBlockFormat(blockFmt);
        } else {
            C_cursor.deletePreviousChar();
        }
        goto accept;
    } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (e->modifiers() & Qt::ControlModifier)
            C_cursor.insertText(QString(QChar::LineSeparator));
        else
            C_cursor.insertBlock();
        e->accept();
        goto accept;
    } else if (navigatelink) {
					if ((e->key() == Qt::Key_Return
							 || e->key() == Qt::Key_Enter
	#ifdef QT_KEYPAD_NAVIGATION
							 || e->key() == Qt::Key_Select
	#endif
							 )
							&& C_cursor.hasSelection()) {

							e->accept();
							/////activateLinkUnderCursor();
							return;
					}
			} else if (e == QKeySequence::Delete) {
				       if (C_cursor.hasSelection()) {
				       QString remtxt = C_cursor.selectedText();
					     for (int i = 0; i < remtxt.size(); ++i) {
								 C_cursor.deleteChar();
							 }
							   position_selection_start = 0;
						   } else {
								 C_cursor.deleteChar();
							 }
				
        
    }
    else if (e == QKeySequence::DeleteEndOfWord) {
        C_cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
    }
    else if (e == QKeySequence::DeleteStartOfWord) {
        C_cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
    }
    else if (e == QKeySequence::DeleteEndOfLine) {
        QTextBlock block = C_cursor.block();
        if (C_cursor.position() == block.position() + block.length() - 2)
            C_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        else
            C_cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
    } else {
			goto process;
		}
		goto accept;
	
	
process:
    {
        QString text = e->text();
        if (!text.isEmpty() && (text.at(0).isPrint() || text.at(0) == QLatin1Char('\t'))) {
            if (overwriteMode
                // no need to call deleteChar() if we have a selection, insertText
                // does it already
                && !C_cursor.hasSelection()
                && !C_cursor.atBlockEnd())
                C_cursor.deleteChar();

            C_cursor.insertText(text);
						cursor_position = C_cursor.position();
           repaintCursor(false);  /* expanded */
        } else if (!text.isEmpty() && C_cursor.hasSelection() ) {
					     QString remove = C_cursor.selectedText();    ///// QTextCursor::deletePreviousChar ()
					     ///////qDebug() << "### remove  " << remove;
					     for (int i = 0; i < remove.size(); ++i) {
								 //////qDebug() << "### remove  " << remove.at(i);
								 C_cursor.deletePreviousChar();
							 }
					     C_cursor.insertText(text);
					repaintCursor(false);  /* expanded */
				}  else {
            e->ignore();
            return;
        }
    }
	
	/* ########## section accept ################*/
	accept:
	{
				e->accept();
	      _d->adjustSize();
	      cursor_position = C_cursor.position();
	      setBlinkingCursorEnabled(true);
				/////////repaintCursor();  /* expanded */
	/* ########## section accept ################*/
	}
	
	accept_move_cursor:
	{
	      e->accept();  /* position update ok! */
	      _d->adjustSize();
        cursor_position = C_cursor.position();
		    ClearSelections();
	      setBlinkingCursorEnabled(true);
				//////////repaintCursor();  /* expanded */
	}
	      
}


void TextWriter::procesevent( QEvent *e )
{
	  QMatrix matrix;
    matrix.translate(controlOffset().x(), controlOffset().y());
	
	   switch (e->type()) {
			 
			    case QEvent::GraphicsSceneMouseMove: {
						if (edit_enable) { 
						QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
						tmouseMoveEvent(e, ev->button(), ev->pos() );
						return;
						}
            break; }
					case QEvent::GraphicsSceneHoverMove: {
            if (edit_enable) 
	          setBlinkingCursorEnabled(false);
						return;
					break; }
					
					case QEvent::KeyPress:
						if (edit_enable) {
						setBlinkingCursorEnabled(false);
            tkeyPressEvent(static_cast<QKeyEvent *>(e));
						return;
						}
            break;
						
				case QEvent::GraphicsSceneMouseDoubleClick: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
					  ///////////qDebug() << "### 1 MouseButtonDblClick";
					  if (  ev->modifiers() == Qt::NoModifier ) {
            tmouseDoubleClickEvent(e, ev->button(), ev->pos() );
						}
            break;
					  return;
					}
				case QEvent::GraphicsSceneMouseRelease: {
             QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
             tmouseReleaseEvent(e, ev->button(), ev->pos() );
					break; }
				
				case QEvent::GraphicsSceneMousePress: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
					  qDebug() << "### 1 GraphicsSceneMousePress";
					  if (ev->buttons() == Qt::LeftButton && edit_enable) {
            tmousePressEvent(ev->button(), ev->pos() , ev->modifiers(), ev->buttons(),ev->screenPos());
						return;
						}
            break; }
		 }
		 
	   if (edit_enable && !cursorOn)  {
			setBlinkingCursorEnabled(true);
		 }
	
}

void TextWriter::tmouseMoveEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	  const int cursorPos = _d->documentLayout()->hitTest(pos,Qt::ExactHit);
	
	  if (StartSelectionMouse != -1 && cursorPos > 0) {
			if (StartSelectionMouse != cursorPos) {
	       setBlinkingCursorEnabled(false);
				 const int stopat = qMax(StartSelectionMouse,cursorPos); 
				 const int startat = qMin(StartSelectionMouse,cursorPos);
				/* selection tracer */
			   //////////////qDebug() << "### tmouseMoveEvent 1a from-to #######" << StartSelectionMouse << "|" << cursorPos;
				 cursorIsFocusIndicator = true;
				 if (stopat == cursorPos) {
					   /* direction -> left-right */
						 C_cursor.setPosition(StartSelectionMouse);
						 for (int i = StartSelectionMouse; i < cursorPos; ++i) {
						 C_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
						 }
						 
						 cursor_position = stopat;
						 
			   } else {
					  /* direction <- right-left */
					    /* not possibel on QGraphicsItem + QTextDocument */
						  ///////C_cursor.setPosition(qMin(StartSelectionMouse,cursorPos));
						 ///////for (int i = StartSelectionMouse; i < cursorPos; ++i) {
						  ///////C_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
						//////// }
					  selectAll();
					  return;
				 }
				 ///////setClipboardSelection();
				 repaintCursor();
			}
		} 
}

void TextWriter::tmousePressEvent(Qt::MouseButton button, const QPointF &pos, Qt::KeyboardModifiers modifiers,
                                          Qt::MouseButtons buttons, const QPoint &globalPos)
{
	qDebug() << "### mousePressEvent in ";
	lastrect = boundingRect();
	setCursorPosition(pos);
	
	cursortime = true;
	
	if (StartSelectionMouse == cursor_position) {
		  ClearSelections();
		  repaintCursor();
		  return;
	} else {
		cursortime = false;
		setBlinkingCursorEnabled(false);
		cursortime = false;
	}
	StartSelectionMouse = cursor_position;
	   /* fast  display not alternate */
	repaintCursor();
}

void TextWriter::tmouseReleaseEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	 ////////////qDebug() << "### tmouseReleaseEvent 1 ############################################";
	 StartSelectionMouse = -1;
}


void TextWriter::tmouseDoubleClickEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	///////////////qDebug() << "### MouseButtonDblClick 1 ############################################";
	if (button != Qt::LeftButton) {
		e->ignore();
		return;
	}
  StartSelectionMouse = -1;
	
	cursortime = true;   /* fast  display not alternate */
	setCursorPosition(pos);

	lastrect = boundingRect();
	if (lastrect.contains(pos) ) { 
		
		      if (currentTextLine(C_cursor).rect().isValid() && currentTextLine(C_cursor).textLength() > 0) {
            C_cursor.select(QTextCursor::WordUnderCursor);   /////  return tc.selectedText();
						const int position = C_cursor.selectionStart();
            const int anchor = C_cursor.selectionEnd();
						const QTextCursor oldSelection = C_cursor;
						if (position != anchor && C_cursor.selectedText().size() > 0) {
					    cursor_position = anchor;
							position_selection_start = position;
							QRectF selectionXY = currentTextLine(C_cursor).rect();
							ClearSelections();
							selectionChanged(selectionXY.isValid());
						}
				    repaintCursor();
          } else {
						ClearSelections();
				    repaintCursor();
					}
  }
}


bool TextWriter::cursorMoveKeyEvent(QKeyEvent *e)
{


   
    const QTextCursor oldSelection = C_cursor;
    const int oldCursorPos = oldSelection.position();

    QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
    QTextCursor::MoveOperation op = QTextCursor::NoMove;

		if (e->key() == Qt::Key_Right) {
            op = QTextCursor::Right;
    } else if (e->key() == Qt::Key_Left) {
            op = QTextCursor::Left;
    } else if (e == QKeySequence::MoveToPreviousChar) {
            op = QTextCursor::Left;
    }
    else if (e == QKeySequence::SelectNextChar) {
           op = QTextCursor::Right;
           mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousChar) {
            op = QTextCursor::Left;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectNextWord) {
            op = QTextCursor::WordRight;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousWord) {
            op = QTextCursor::WordLeft;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfLine) {
            op = QTextCursor::StartOfLine;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfLine) {
            op = QTextCursor::EndOfLine;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfBlock) {
            op = QTextCursor::StartOfBlock;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfBlock) {
            op = QTextCursor::EndOfBlock;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfDocument) {
            op = QTextCursor::Start;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfDocument) {
            op = QTextCursor::End;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousLine) {
            op = QTextCursor::Up;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectNextLine) {
            op = QTextCursor::Down;
            mode = QTextCursor::KeepAnchor;
            {
                QTextBlock block = C_cursor.block();
                QTextLine line = currentTextLine(C_cursor);
                if (!block.next().isValid()
                    && line.isValid()
                    && line.lineNumber() == block.layout()->lineCount() - 1)
                    op = QTextCursor::End;
            }
    }
    else if (e == QKeySequence::SelectNextLine) {
            op = QTextCursor::Down;
            mode = QTextCursor::KeepAnchor;
            {
                QTextBlock block = C_cursor.block();
                QTextLine line = currentTextLine(C_cursor);
                if (!block.next().isValid()
                    && line.isValid()
                    && line.lineNumber() == block.layout()->lineCount() - 1)
                    op = QTextCursor::End;
            }
    }
    else if (e == QKeySequence::MoveToNextWord) {
            op = QTextCursor::WordRight; ///// 16777236
    }
    else if (e == QKeySequence::MoveToPreviousWord) {
            op = QTextCursor::WordLeft;
    } else if (e->key() == Qt::Key_Left) {
            op = QTextCursor::WordLeft;
    } else if (e->key() == Qt::Key_Right) {
            op = QTextCursor::WordRight;
    } else if (e == QKeySequence::MoveToEndOfBlock) {
            op = QTextCursor::EndOfBlock;
    }
    else if (e == QKeySequence::MoveToStartOfBlock) {
            op = QTextCursor::StartOfBlock;
    }
    else if (e == QKeySequence::MoveToNextLine) {
            op = QTextCursor::Down;
    }
    else if (e == QKeySequence::MoveToPreviousLine) {
            op = QTextCursor::Up;
    }
    else if (e == QKeySequence::MoveToPreviousLine) {
            op = QTextCursor::Up;
    }
    else if (e == QKeySequence::MoveToStartOfLine) {
            op = QTextCursor::StartOfLine;
    }
    else if (e == QKeySequence::MoveToEndOfLine) {
            op = QTextCursor::EndOfLine;
    }
    else if (e == QKeySequence::MoveToStartOfDocument) {
            op = QTextCursor::Start;
    }
    else if (e == QKeySequence::MoveToEndOfDocument) {
            op = QTextCursor::End;
    }

		
    else {
        return false;
    }
		_d->adjustSize();
		
		const bool moved = C_cursor.movePosition(op,QTextCursor::MoveAnchor);
		if (moved) {
        if (C_cursor.position() != oldCursorPos) {
					cursor_position = C_cursor.position(); 
					ClearSelections();
				  emit q_cursor_newPos();
				}
    }
    return true;
}


void TextWriter::repaintCursor( bool allrect )
{
	   
	   if (allrect) {
			emit updateRequest(boundingRect());
		  return;
		 }
		 
		 if (currentTextLine(C_cursor).rect().isValid()) {
			 emit updateRequest(currentTextLine(C_cursor).rect());
		 } else {
			 emit updateRequest(boundingRect());
		 }
}

void TextWriter::insertFromMimeData(const QMimeData *source)
{
 
    if (!_d) {
	  return;
		}

    bool hasData = false;
    QTextDocumentFragment fragment;
		
    if (source->hasFormat(QLatin1String("application/x-qrichtext")) ) {
        // x-qrichtext is always UTF-8 (taken from Qt3 since we don't use it anymore).
        fragment = QTextDocumentFragment::fromHtml(QString::fromUtf8(source->data(QLatin1String("application/x-qrichtext"))), _d );
        hasData = true;
    } else if (source->hasHtml()) {
        fragment = QTextDocumentFragment::fromHtml(source->html(), _d);
        hasData = true;
    } else {
        QString text = source->text();
        if (!text.isNull()) {
            fragment = QTextDocumentFragment::fromPlainText(text);
            hasData = true;
        }
    }
		
    fragment = QTextDocumentFragment::fromPlainText(source->text());
		
		qDebug() << "### insertFromMimeData " << hasData;
		
    if (hasData) {
        C_cursor.insertFragment(fragment);
			  boundingRect();  /* shake size ! */
			  ClearSelections();
        repaintCursor(true);
		}
}

QMimeData *TextWriter::createMimeDataFromSelection() const
{
    const QTextDocumentFragment fragment(C_cursor);
    return new QTextEditMimeData(fragment);    /////QTextDocumentFragment::fromHtml(md->html())
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                          EVENTS                                                 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////





