#include "mounttextprivate.h"
/* #####################################buggi ##################################################*/

TextWriter::MountTextPrivate( QObject *parent )
	: Layoutpainter( parent ),cursorWidth(1),preeditCursor(0),timeline(0),edit_enable(false),overwriteMode(false),
    cursorIsFocusIndicator(false),movecursor(false),navigatelink(false),cursor_position(-1)
{
	setParent(parent);
	Cursor_From_Event_Source = false;
	StartSelectionMouse = -1;
	paintline = timeline;
	_d = new QTextDocument(this);
	setContent(Qt::RichText,QString(),0);
	clipboard = QApplication::clipboard();
	QObject::connect(clipboard, SIGNAL(dataChanged() ), this, SLOT(int_clipboard_new()));
}
 
void TextWriter::int_clipboard_new()
{
	qDebug() << "### clipboard in ";
	_layout_1->dumpObjectTree();
	
	
}

void TextWriter::paste()
{
	  if (clipboard->text().size() > 600) {
		qApp->beep();
		qApp->beep();
		return;
		}
    const QMimeData *md = QApplication::clipboard()->mimeData();
    if (md)
        insertFromMimeData(md);
}


void TextWriter::edit( bool e)
{
	edit_enable = e;
	setBlinkingCursorEnabled(e);
	qDebug() << "### edit " << e;
}

void TextWriter::setxhtml( QString html )
{
	if (html.size() < 2) {
	return;
	}
	edit(false);
	
	qDebug() << "### setxhtml 0 " << html;
	
	lastrect = boundingRect();
	qDebug() << "### setxhtml " << __LINE__;
	if (_d) {
	_d->disconnect(this);
	_d->documentLayout()->disconnect(this);
	////_d->documentLayout()->setPaintDevice(0);
		qDebug() << "### setxhtml " << __LINE__;
	 if (_d->parent() == this) {
			////////delete _d;
		  qDebug() << "### setxhtml " << __LINE__;
	 }
	 
	_d = 0;
	}
	qDebug() << "### setxhtml " << __LINE__;
	setContent(Qt::RichText,html,0);
	qDebug() << "### setxhtml 1 " << html;
}


void TextWriter::setDocument(QTextDocument *document)
{
	if (_d == document) {
	return;
	}
	edit(false);
	
	lastrect = boundingRect();
	if (_d) {
	_d->disconnect(this);
	_d->documentLayout()->disconnect(this);
	_d->documentLayout()->setPaintDevice(0);
		
	
	if (_d->parent() == this) {
			//////////delete _d;
	 }
	 _d = 0;
	 edit(false);
  }
	setContent(Qt::RichText,QString(),document);
}


void TextWriter::setContent(Qt::TextFormat format, QString text, QTextDocument *document)
{
	  qDebug() << "### setContent " << __LINE__;
	  imagemaps.clear();
	
	  //////edit(false);
	  bool nondoc = false;
	  qDebug() << "### setContent " << __LINE__;
    if (!_d) {
			   qDebug() << "### setContent " << __LINE__;
			
        if (document) {
					 qDebug() << "### setContent in  " << __LINE__;
					_d = document;
					_d->setParent(this);
        } else {
					 qDebug() << "### setContent crea " << __LINE__;
					_d = new QTextDocument(this);
					nondoc = true;
        }
				//////_d->setUseDesignMetrics(true);
				///////_d->setMaximumBlockCount(20);

				QTextCursor _c(_d);
				
				qDebug() << "### setContent crea " << __LINE__;
				
				
				
				    if (!text.isEmpty()) {
							if (format == Qt::PlainText) {
								   qDebug() << "### setContent crea " << __LINE__;
                   _d->setPlainText(text);
                   /////////_d->setUndoRedoEnabled(false);
								   /////////_c.beginEditBlock();
                   //////////_/c.select(QTextCursor::Document);
								   //////// if (LastCharFormat.isValid () ) {
                      ///////// _c.setCharFormat(LastCharFormat);
										/////////}
                   ////////////_c.endEditBlock();
								   cursor_position = 0;
							} else {
								  qDebug() << "### setContent crea " << __LINE__;
								 _d->setHtml(text);
								 _d->setUndoRedoEnabled(false);
								 ///////////_c.movePosition(QTextCursor::End);
								 cursor_position = 0;
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
						qDebug() << "### setContent crea " << __LINE__;
				
						_c.setPosition(1,QTextCursor::MoveAnchor);
				    cursor_position = _c.position();
				///////_c.setPosition(cursor_position);
						qDebug() << "### cursor_positioioncursor_positioncursor_ " << _c.position();
				/* reconnect all */
						 C_cursor = _c; 
				/////_d = new QTextDocument(this);
				 
				/////////////////d->setUndoRedoEnabled(false);
				_layout_1 = _d->documentLayout();
				QObject::connect(_d, SIGNAL(modificationChanged(bool) ),this, SLOT(update() ));  /* paint area size? */
				QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
	      QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), parent(), SLOT(cursorPositionChanged(QTextCursor) ));
				QObject::connect(_layout_1, SIGNAL(documentSizeChanged(QSizeF) ), this, SLOT(DocsizeChanged(QSizeF) ));
				QObject::connect(_layout_1, SIGNAL(update(QRectF)), this, SIGNAL(updateRequest(QRectF)));
				QObject::connect(_d, SIGNAL(undoAvailable(bool)),this, SIGNAL(undoAvailable(bool)));
        QObject::connect(_d, SIGNAL(redoAvailable(bool)),this, SIGNAL(redoAvailable(bool)));
				_d->setUndoRedoEnabled(true);
		}
}



void TextWriter::undoAvailable( bool e )
{
	qDebug() << "### undoAvailable " << __LINE__ << e;
}
void TextWriter::redoAvailable( bool e )
{
	qDebug() << "### redoAvailable " << __LINE__ << e;
}

void TextWriter::DocsizeChanged( const QSizeF size )
{
	qDebug() << "### DocsizeChanged 0  " << size;
	qDebug() << "### DocsizeChanged  1 " << boundingRect();
	
}


void TextWriter::blockCount( int bb )
{
	 ////////////qDebug() << "### blockCountChanged " << bb;
	 lastrect = boundingRect();
	 //////////LastCharFormat = _c.charFormat();
}

void TextWriter::cursorPosition( const QTextCursor curs )
{
	cursor_position = curs.position();
	LastCharFormat = curs.charFormat();
	if (!curs.isCopyOf(C_cursor)) {
		Cursor_From_Event_Source = true;
	} else {
		C_cursor = curs;
	}
	GrepCursorData();
	qDebug() << "X-Y_Pos_Cursor ->" << X_Pos_Cursor  << "x" << Y_Pos_Cursor << C_cursor.position();
	NewCharformat(C_cursor);
}

QRectF TextWriter::actualcursorrect()
{
	    QTextCursor _c(_d);
	                _c.setPosition(cursor_position);
	return rectForPosition(_c.position());
}


QRectF TextWriter::cursorRect(const QTextCursor &cursor)
{
    if (cursor.isNull()) {
        return QRectF();
		}
    return rectForPosition(cursor.position());
}

QRectF TextWriter::Rect2cursorpoint( const QTextCursor &cursor  , const int first , const int second ) 
{
            QTextLine Zline = currentTextLine(cursor);
            if (!Zline.isValid()) {
              return QRectF();
            }
						
						/* check if more as one line !!! */

						////  Zline.height()       
						qreal px2 = Zline.cursorToX(qMax(second,first));
						qreal px1 = Zline.cursorToX(qMin(second,first));
						QRectF selectionrect( px1, Zline.rect().y() , px2 - px1 , Zline.height() ); 
						return selectionrect;
}

int  TextWriter::LinefromPosition(int position)
{
	              QTextCursor neon(_d);
	              neon.setPosition(position);
	              QTextLine Zline = currentTextLine(neon);
	
	          if (!Zline.isValid()) {
              return 0;
            }
						
						Zline.lineNumber();
}

QRectF TextWriter::rectForPosition(int position)
{
		QTextCursor neon(_d);
	              neon.setPosition(position);
	
	  QTextLine Zline = currentTextLine(neon);
	
            if (!Zline.isValid()) {
              return QRectF();
            }
	
		  qreal px2 = Zline.cursorToX(qMax(position,position + 1));
      qreal px1 = Zline.cursorToX(qMin(position,position + 1));
			QRectF selectionrect( px1, Zline.rect().y() , px2 - px1 , Zline.height() ); 
      return selectionrect;
}

QTextLine TextWriter::TextLineAt(int position)
{
	QTextCursor neon(_d);
	              neon.setPosition(position,QTextCursor::KeepAnchor);
	return currentTextLine(neon);
}

void TextWriter::wake_up( const int msec )
{
	 _d->adjustSize();
	 QTimer::singleShot(msec,this,SLOT(update())); 
}

void TextWriter::update()
{
	_d->adjustSize();
	lastrect = boundingRect();
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


void TextWriter::repaintCursor( bool allrect )
{
	   
	   if (allrect) {
			emit updateRequest(boundingRect());
		  return;
		 }
		 if (!line_rect_out.isNull()) {
      emit updateRequest(line_rect_out);
		 } else {
			 emit updateRequest(boundingRect());
		 }
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
				 setClipboardSelection();
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
		
		      if (DDline.isValid() && DDline.textLength() > 0) {
            C_cursor.select(QTextCursor::WordUnderCursor);   /////  return tc.selectedText();
						const int position = C_cursor.selectionStart();
            const int anchor = C_cursor.selectionEnd();
						const QTextCursor oldSelection = C_cursor;
						if (position != anchor && C_cursor.selectedText().size() > 0) {
					    cursor_position = anchor;
							position_selection_start = position;
							QRectF selectionXY = Rect2cursorpoint(oldSelection,position,anchor);
							if (!selectionXY.isNull()) {
							ClearSelections();
							TextHighlightSelect = selectionXY;
							cursorIsFocusIndicator = true;  /* draw selection highlight */
							emit docselectionChanged();
							}
							
						}
				    repaintCursor();
          } else {
						ClearSelections();
				    repaintCursor();
					}
  }
}

void TextWriter::ClearSelections()
{
	  cursorIsFocusIndicator = false;
    TextHighlightSelect = QRectF();
		selection_list.clear();
	  repaintCursor(true);
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


void TextWriter::selectionChanged(bool forceEmitSelectionChanged /*=false*/)
{
	  /////////////bool dataH = (cursor_position =! position_selection_start );
	  
    if (forceEmitSelectionChanged && C_cursor.hasSelection() ) {
			   FocusSelectionHighlight = QRectF();
         TextHighlightSelect = QRectF();
        emit docselectionChanged();
		} else {
			return;
		}
		qDebug() << "### position_selection_start/stop " << C_cursor.position() << "/" << C_cursor.anchor();
		qDebug() << "### position_selection_start/stop " << cursor_position << "/" << position_selection_start; 
		
		const int Cstart = qMin(C_cursor.position(),C_cursor.anchor());
		const int Cstop = qMax(C_cursor.position(),C_cursor.anchor());
		
		qDebug() << "### Cstart/stop " << Cstart << "/" << Cstop; 
		////////////  QTextLine QTextLayout::lineAt ( int i ) const 
		selection_list.clear();
		const int stopline  = LinefromPosition(Cstop);
		const int startline = LinefromPosition(Cstart);
		
		qDebug() << "### Cstart/stop line " << startline << "/" << stopline; 
		
		int lineNr = 0;       ////////QRectF Rli = block.layout()->boundingRect()
		QTextFrame  *Tframe = _d->rootFrame();
		
		    QTextFrame::iterator it;
        for (it = Tframe->begin(); !(it.atEnd()); ++it) {
					
					                QTextBlock para = it.currentBlock();
					                if (para.isValid()) {
														            QTextBlock::iterator de;
																				for (de = para.begin(); !(de.atEnd()); ++de) {
																					lineNr++;
																					 qDebug() << "### lineNr " << lineNr; 
																					 selection_list.insert(lineNr,para.layout()->boundingRect());
																				}
														
													}
					
				}
		
		if (selection_list.size() > 0) {
		cursorIsFocusIndicator = true;
		setBlinkingCursorEnabled(false);
		repaintCursor(true);  /* repaint all true */
		}
		
}


/* child from class TextLayer : public QObject, public QGraphicsItem */

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
	
	   if (edit_enable) {  /* writteln modus */
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
			
				if (cursortime) {
				CTX.cursorPosition = cursor_position;
				} else {
				CTX.cursorPosition = -1;
				}
	  }
		if (edit_enable) {
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
    _layout_1->draw(painter,CTX);
		/*dispay editable modus yes oderwise here not paint ! */              
}

/*

return;
	    
	

	QRectF lastrect;
  QTextCursor _c;
  QTextDocument *_d;
  int cursor_position;
  QRectF line_rect;
  QRectF line_rect_out;
  int line_nummer;
  qreal linehight;
  QTextLine DDline;
  QLineF CursorDrawLine;
	

	
///////////qDebug() << "### paint selection_list.size() " << selection_list.size();
	
bool playdoc = true;
QPen cursor_pen(QBrush(QColor(Qt::red)),1);
QPen line_pen(QBrush(QColor(Qt::black)),cursorWidth);
QColor testcolor("#a6ffc7");
			 testcolor.setAlpha(140);
			 
			
	
	      painter->save();
	      painter->setPen(Qt::NoPen);
	      painter->setBrush(BGpage);
        painter->drawRect(fulllayer);
	      if (_d) {
        _d->drawContents(painter,fulllayer);  
				}
        painter->restore();
				
				
				
	if (edit_enable) {
		  paintline++;
		                           
		           if (cursorIsFocusIndicator && !TextHighlightSelect.isEmpty() ) {
								  const qreal Ysel = TextHighlightSelect.topRight().y();
								  QPointF toprights = TextHighlightSelect.topRight();
								  toprights.setY( Ysel + 1 );
									painter->save();
									painter->setPen(Qt::NoPen);
									painter->setBrush(QBrush(testcolor));
									painter->drawRect(TextHighlightSelect);
									painter->restore();
								  if (paintline%2) {
								  painter->save();
								  painter->setPen(line_pen);
								  QLineF Afterselection(toprights,TextHighlightSelect.bottomRight());
									       Afterselection.setLength(linehight - 1);
									painter->drawLine(Afterselection);
								  painter->restore();
									}
									cursortime = false; 
								}
								
							 if (selection_list.size() > 0) {
									QMapIterator<int,QRectF> o(selection_list);
													 while (o.hasNext()) {
															   o.next();
																 QRectF Darea = o.value();
														     painter->save();
														     painter->setPen(Qt::NoPen);
									               painter->setBrush(QBrush(testcolor));
									               painter->drawRect(Darea);
														     painter->restore();
													 } 
													 
									cursortime = false;
									playdoc = false;
								}
		
	
            if (!CursorDrawLine.isNull()) {
								painter->save();
                if (cursortime && cursorOn ) {

									    if (playdoc) {
											painter->setBrush(QBrush(QColor(Qt::transparent)));
											painter->setPen(line_pen);
											painter->drawLine(CursorDrawLine);
											}
									
									
									}  else {
									  * remove cursor blink ! *
										if (playdoc) {
                     _d->drawContents(painter,line_rect_out);
										}
                }
								    dispay editable modus yes oderwise here not paint ! 
								   QPixmap pixbg(":/img/icon.png");
                   painter->drawPixmap(QPointF(boundingRect().width() - 40,8),pixbg);
								   painter->restore();
            }
	}

*/


void TextWriter::timerEvent(QTimerEvent *event)
{
	
	if (!edit_enable) {
     return;
	}
	
	cursortime = false;
	
    if (event->timerId() == cursorTimeLine.timerId()) {
        timeline++;
			
            if (timeline%2) {
							cursortime = true; 
            } else {
							cursortime = false;
            }
						 qDebug() << "### cursortime  " << cursortime << _d->isUndoRedoEnabled() << _d->isUndoAvailable();
						
				repaintCursor();
    }
}

void TextWriter::undo()
{
	   qDebug() << "### undo stack go ..... ";
    ////_d->undo(&C_cursor);
	  _d->undo();
}

void TextWriter::redo()
{
	  qDebug() << "### redo stack go ..... ";
    ///_d->redo(&C_cursor);
	  _d->redo();
}


void TextWriter::setBlinkingCursorEnabled(bool enable)
{
    if (enable && QApplication::cursorFlashTime() > 0) {
        cursorTimeLine.start( QApplication::cursorFlashTime() / 2,this);
		}  else {
        cursorTimeLine.stop();
        cursorOn = enable;
		}
		
		if (!enable) {
			if (cursorIsFocusIndicator) {   /* leave by release */
				ClearSelections();
	     }
			
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
				  emit cursor_newPos();
				}
    }
    return true;
}





void TextWriter::setCursorPosition(const QPointF &pos)
{
	 const int cursorPos = hitTest(pos, Qt::FuzzyHit);
   if (cursorPos == -1) {
   return;
	 } else {
		 QTextCursor c(_d);
     c.setPosition(cursorPos);
		 cursor_position = c.position();
		 Cursor_From_Event_Source = true;
		 C_cursor = c;
		 ////////qDebug() << "### cursor_" << cursorPos << "position hit to from pos " << _c.position() << " l.rect->" << line_rect << " l.nr." << line_nummer;
		 GrepCursorData();
	 }
}

QTextCursor TextWriter::_cForPosition()
{
	if (Cursor_From_Event_Source) {
		return C_cursor;
	}
	
    QTextCursor c(_d);
    c.setPosition(cursor_position);
	
	        if (position_selection_start !=0) {
					c.setPosition(qMin(cursor_position,position_selection_start) );
					c.select(QTextCursor::WordUnderCursor);
					position_selection_start = 0;
				  }
					
    return c;
}

void TextWriter::UpadateCursor( const int freezeon )
{
	 qDebug() << "### UpadateCursor  posz." << freezeon;
	 if (freezeon !=-1) {
	      QTextCursor c(_d);
        c.setPosition(freezeon);
		    cursor_position = c.position();
	 } else {
		     qApp->beep();
	       QTextCursor c(_d);
         c.setPosition(0);
		     cursor_position = c.position();
	 }
	 GrepCursorData();
}




void TextWriter::GrepCursorData()
{
	if (!editable()) {
	return;
	}
	if (!_d) {
		return;
	}
	if (!Cursor_From_Event_Source) {
	return;
	}
	///////  QRectF boundingRect()
	
		 DDline = currentTextLine(C_cursor);
		 line_rect = DDline.naturalTextRect();
		 line_rect_out = DDline.rect();
		 line_nummer = DDline.lineNumber();
		 linehight = DDline.height();
		 X_Pos_Cursor = DDline.cursorToX(C_cursor.position());
	   Y_Pos_Cursor =  qBound (0.,DDline.y(),boundingRect().bottom());
		 ///////////////qDebug() << "### mouse data  Ln." << line_nummer << " Cp." << cursor_position  << "|" << _c.position();
		 CursorDrawLine = QLineF(QPointF(X_Pos_Cursor,line_rect_out.y()),QPointF(X_Pos_Cursor,line_rect_out.y() + linehight));
	   //////////////emit updateRequest(line_rect_out);
	   
	
}













void TextWriter::copy()
{
	  qDebug() << "### copy 1  ";
    if (!C_cursor.hasSelection()) {
	    return;
		}
		
		qDebug() << "### copy 2  ";
		
    QMimeData *data = createMimeDataFromSelection();
    QApplication::clipboard()->setMimeData(data);
}


void TextWriter::setClipboardSelection()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (!C_cursor.hasSelection() || !clipboard->supportsSelection())
        return;
    QMimeData *data = createMimeDataFromSelection();
    clipboard->setMimeData(data, QClipboard::Selection);
}


QMimeData *TextWriter::createMimeDataFromSelection() const
{
	  if (Cursor_From_Event_Source) {
    const QTextDocumentFragment fragment(C_cursor);
    return new QTextEditMimeData(fragment);    /////QTextDocumentFragment::fromHtml(md->html())
		} else {
		return new QTextEditMimeData(QTextDocumentFragment::fromHtml("Cursor not load!"));
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






void  TextWriter::SetLayerMargin()
{
        if (_d) {
            QTextFrame  *Tframe = _d->rootFrame();
            QTextFrameFormat rootformats = Tframe->frameFormat();
            
             GetMargin *marge = new GetMargin(0);
             marge->setWindowTitle(tr("Set Layer margin"));  /* or page */
             marge->Set_margin( QRectF (rootformats.topMargin(),rootformats.rightMargin() ,rootformats.bottomMargin() ,rootformats.leftMargin() ) );
             int faxme = marge->exec();
             if (faxme == 1) {
                 QRectF setFrams = marge->Get_margin();
                 const qreal TopMargin = setFrams.x();
                 const qreal RightMargin = setFrams.y();
                 const qreal BottomMargin = setFrams.width();
                 const qreal LeftMargin = setFrams.height();
                 
                 rootformats.setBottomMargin(BottomMargin);
                 rootformats.setTopMargin(TopMargin);
                 rootformats.setRightMargin(RightMargin);
                 rootformats.setLeftMargin(LeftMargin);
                 rootformats.setPadding(0);
                 Tframe->setFrameFormat(rootformats);
             }
             
         }
}




void  TextWriter::RegisterImage( SPics e , bool insert )
{
    QApplication::restoreOverrideCursor();
    bool ok;
    QString txtinfo = QInputDialog::getText(0, tr("Image Description to blind people."),tr("Description:"), QLineEdit::Normal,e.name, &ok);
    if (txtinfo.size() > 0) {
    e.info = txtinfo.left(110);
    }
		/////////  QMap<QString,SPics> imagemaps;
    imagemaps.insert(e.name,e);
    _d->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
		
    if (insert) {
        QTextImageFormat format;
        format.setName( e.name );
        format.setHeight ( e.pix().height() );
        format.setWidth ( e.pix().width() );
        format.setToolTip(e.info);
        format.setProperty(_IMAGE_PICS_ITEM_,e);
        textCursor().insertImage( format );
    }
    
}

QMenu *TextWriter::StandardMenu( QWidget * inparent )
{
    QMenu *menu = new QMenu(inparent);
    QAction *a;
	  
	  if (editable()) {
	      QString text = tr("&Undo");
        a = menu->addAction(tr("&Undo") + ACCEL_KEYL(Z), this, SLOT(undo()));
        a->setEnabled(_d->isUndoAvailable());
        a = menu->addAction(tr("&Redo") + ACCEL_KEYL(Y), this, SLOT(redo()));
        a->setEnabled(_d->isRedoAvailable());
        menu->addSeparator();
			  a = menu->addAction(tr("Cu&t") + ACCEL_KEYL(X), this, SLOT(cut()));
        a->setEnabled(C_cursor.hasSelection());
		}
		    a = menu->addAction(tr("&Copy") + ACCEL_KEYL(C), this, SLOT(copy()));
        a->setEnabled(C_cursor.hasSelection());
	
		    menu->addSeparator();
        a = menu->addAction(tr("Select All") + ACCEL_KEYL(A), this, SLOT(selectAll()));
        a->setEnabled(!_d->isEmpty());
		
		if (editable()) {
				a = menu->addAction(tr("&Paste") + ACCEL_KEYL(V), this, SLOT(paste()));
        a->setEnabled(clipboard->text().size() > 1);
        a = menu->addAction(tr("Delete"), this, SLOT(deleteSelected()));
        a->setEnabled(C_cursor.hasSelection());
    }
		
		return menu;

}

void TextWriter::deleteSelected()
{
    if (editable()) {
    C_cursor.removeSelectedText();
		}
}

void  TextWriter::InsertImageonCursor()
{
	  if (!_d) {
		return;
		}
		
		_d->undo();
		return;
		
		
    QString file = QFileDialog::getOpenFileName(0, tr( "Choose Image to insert..." ), QString(setter.value("LastDir").toString()) , ImageFilterHaving() );
    if ( file.isEmpty() ) {
    return;
    }
    setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
    ImageonCursor(file);
}

void  TextWriter::ImageonCursor( QString file )
{
     QDateTime timer1( QDateTime::currentDateTime() );
     const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QFileInfo fixurl(file);
     const QString extension =  fixurl.completeSuffix().toLower();
     QByteArray              derangedata;
     QPixmap                 resultimage;
     QPixmap                 scaledsimage;
     QUrl                    imgresor;
     
        QFile f(fixurl.absoluteFilePath());
        if (f.open(QIODevice::ReadOnly)) {
            derangedata = f.readAll();
            f.close();
        }
  
        
        //////////////qDebug() << "### image" << derangedata.size();
        if (derangedata.size() < 1) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr( "Alert! image format." ), tr( "Unable to read file %1" ).arg(fixurl.fileName()) );  
        return;
        }
        /* read image */
        if (extension.contains("svg")) {
        resultimage = RenderPixmapFromSvgByte(  derangedata );
        } else if (extension.contains("ps") || extension.contains("eps")) {
            QApplication::restoreOverrideCursor();
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        resultimage = LoadPS( fixurl.absoluteFilePath() );
        } else if (extension.contains("pdf")) {
            QApplication::restoreOverrideCursor();
            int page = QInputDialog::getInteger(0, tr("Render Page Nr."),tr("Page:"),1, 1, 100, 1);
            int large = QInputDialog::getInteger(0, tr("Page scaled to width"),tr("Point unit:"),400, 10, 3000, 10);
            if (page > 0 && large > 0) {
                QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
                resultimage = LoadPDF( fixurl.absoluteFilePath() ,page,large);
            } else {
                QApplication::restoreOverrideCursor();
            return;
            }
        }  else {
        resultimage.loadFromData( derangedata );
        }
        
        /* read in */
        if ( resultimage.isNull() ) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr( "Alert! image format." ), tr( "Unable to render to image file %1 from type %2." ).arg(fixurl.fileName()).arg(extension) ); 
        return;
        }
        
         
         QByteArray bytes;
         QBuffer buffer(&bytes);
         buffer.open(QIODevice::WriteOnly);
         QApplication::restoreOverrideCursor();
				 int limitwiimage = _MAXIEDITWI_;
				 if (limitwiimage > boundingRect().width()) {
					 limitwiimage = boundingRect().width();
				 }
				 
				
				if (resultimage.width() > limitwiimage) {
             /* question widht */
           scaledsimage = resultimage.scaledToWidth( limitwiimage );
        } else {
           scaledsimage = resultimage.scaledToWidth( resultimage.width() ); 
        }
				
				
				
        if (_DRAWMODUS_WEB_ == 1) {
				scaledsimage.save(&buffer,"JPG",70);
				} else {
        scaledsimage.save(&buffer,"PNG",100);
				}
				
        if (!scaledsimage.isNull()) {
        const QString nami = Imagename(fixurl.baseName());
        SPics  xpix;
        xpix.name = nami;
				if (_DRAWMODUS_WEB_ == 1) {
				xpix.extension = QByteArray("JPG");
				} else {
        xpix.extension = QByteArray("PNG");
				}
				/* bytes having data images */
				xpix.set_pics(scaledsimage);
				
				RegisterImage(xpix,true);
				}
        QApplication::restoreOverrideCursor();
}






