#include "mounttextprivate.h"

TextWriter::TextWriter(QObject *parent )
  : Layoutpainter(this),cursorIsFocusIndicator(false),edit_enable(false),cursorOn(false),timeline(0),
	overwriteMode(false),StartSelectionMouse(-1)
{
	bridge = parent;
	_d = new QTextDocument;
	C_cursor = QTextCursor(_d);
	///////setContent(Qt::AutoText,QString(),doc);
  setBlinkingCursorEnabled(false);
	edit(false);
  ///////////qDebug() << "### TextWriter init  html in size ->" << _d->toHtml().size();
	clipboard = QApplication::clipboard();
}

void TextWriter::setDocument ( QTextDocument * document , QObject *parent )
{
	bridge = parent;
	if (document->toHtml().size() < 22) {
	qApp->beep();
	qApp->beep();
	qApp->beep();
	return;
	}
	  if (_d) {
		_d->clear();
	  _d->disconnect(this);
	  _d->documentLayout()->disconnect(this);
			if (_d->parent() == this) {
        delete _d;
		  }
		_d = 0;
		}
		
	setContent(Qt::AutoText,QString(),document);
}

void TextWriter::RegisterResource( QMap<QString,SPics> list )
{
	             QMapIterator<QString,SPics> i(list);
                 while (i.hasNext()) {
                     i.next();
									    SPics e  = i.value();
									    qDebug() << "### TextWriter insert 4 addResource " << e.name << e.pix().isNull();
						         _d->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
									   imagemaps.insert(e.name,e);
								 }
	
}

void TextWriter::setContent(Qt::TextFormat format, QString text, QTextDocument *document)
{
	if (_d == document) {
	return;
	}
	imagemaps.clear();  /* image resource */
	/* register image if found on doc before clone !!!! */
	if (document) {
		     QTextFrame  *Tframe = document->rootFrame();
				 QTextFrame::iterator it;
         for (it = Tframe->begin(); !(it.atEnd()); ++it) {
				               QTextBlock para = it.currentBlock();
					             if (para.isValid()) {
												   QTextBlock::iterator de;
														for (de = para.begin(); !(de.atEnd()); ++de) {
																QTextFragment fr = de.fragment();
																if (fr.isValid()) {
																	 QTextCharFormat base = fr.charFormat();
																	 QTextImageFormat pico = base.toImageFormat();
																				if (pico.isValid()) {
																						const QString hrefadress = pico.name();
																					  qDebug() << "### hrefadress register " << hrefadress;
																					  QVariant xx = pico.property(_IMAGE_PICS_ITEM_); 
																					   if (!xx.isNull()) {
                                                SPics pic = xx.value<SPics>();
																							  qDebug() << "### TextWriter insert 1 " << pic.name;
																							  imagemaps.insert(pic.name,pic);
                                             }
																					
																				}
																}
																				 
														}
												 
											 }
				 }
	}
	cursor_position = 0;
	bool nondoc = false;
	if (!_d) {
        if (document) {
					_d = document;
					if (imagemaps.size() > 0) {
						  QMapIterator<QString,SPics> i(imagemaps);
                 while (i.hasNext()) {
                     i.next();
									    SPics e  = i.value();
									    qDebug() << "### TextWriter insert 2 addResource " << e.name << e.pix().isNull();
						         _d->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
								 }
						
					}
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
				QObject::connect(clipboard, SIGNAL(dataChanged() ), this, SLOT(int_clipboard_new()));
				/////QObject::connect(_d, SIGNAL(modificationChanged(bool) ),this, SLOT(update() ));  /* paint area size? */
				QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
	      ////////QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(NewCharformat(QTextCursor) ));
				//////////QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ),bridge, SLOT(cursorPositionChanged(QTextCursor) ));
				QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)),this,SIGNAL(updateRequest(QRectF)));
				/////QObject::connect(_d, SIGNAL(undoAvailable(bool)),this, SIGNAL(undoAvailable(bool)));
        ///////QObject::connect(_d, SIGNAL(redoAvailable(bool)),this, SIGNAL(redoAvailable(bool)));
				_d->setUndoRedoEnabled(true);
						if (nondoc) {
							/* no document  set plain text or html not doc precision format as default color */
							
						}
						
						
				repaintCursor();
				emit q_cursor_newPos();
						
	}
					
		
}


 
void TextWriter::int_clipboard_new()
{
	///////////qDebug() << "### clipboard in ";
}

void TextWriter::paint_doc(  QPainter * painter ,
                         const QStyleOptionGraphicsItem *option , 
                         QBrush BGpage , 
                         const QRectF fulllayer , QPen BorderFiller ,
                         bool alternate )
{
	  painter->save();
		painter->setPen(BorderFiller);
		painter->setBrush(BGpage);
		painter->drawRect(fulllayer);
		painter->restore();
	  painter->save();
	
		const QColor backcolor = BGpage.color();
    const QColor contrastbackcolor(
        backcolor.red()   > 127 ? 0 : 255,
        backcolor.green() > 127 ? 0 : 255,
        backcolor.blue()  > 127 ? 0 : 255); 
	
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
				if (cursortime) {  /* cursor blink yes vom timer QApplication::cursorFlashTime() */
				painter->setPen(QPen(QBrush(contrastbackcolor),1));
				CTX.cursorPosition = C_cursor.position();
				} else {
				painter->setPen(Qt::NoPen);
				CTX.cursorPosition = -1;
				}
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
    _d->documentLayout()->draw(painter,CTX);
				
			if (editable()) {  /* writteln modus icon */
				QPixmap pixbg(":/img/icon.png");
				painter->drawPixmap(QPointF(fulllayer.width() - 40,8),pixbg);
			}
				
				
		painter->restore();
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




TextWriter::~TextWriter()
{
  delete _d;
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
	  C_cursor.setPosition(C_cursor.position());
		emit updateRequest(boundingRect());
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

void TextWriter::deleteSelected()
{
    if (editable()) {
    C_cursor.removeSelectedText();
		}
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
					  if (ev->buttons() == Qt::LeftButton) {
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
		///////setBlinkingCursorEnabled(false);
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

QMenu *TextWriter::StandardMenu( QWidget * inparent )
{
    QMenu *menu = new QMenu(inparent);
    QAction *a;
	  
	  if (editable()) {
	      QString text = tr("&Undo");
        a = menu->addAction(tr("&Undo") + ACCEL_KEYL(Z), this, SLOT(undo()));
        a->setEnabled(_d->isUndoAvailable());
			  a->setIcon(QIcon(":/img/undo.png"));
			
        a = menu->addAction(tr("&Redo") + ACCEL_KEYL(Y), this, SLOT(redo()));
        a->setEnabled(_d->isRedoAvailable());
			  a->setIcon(QIcon(":/img/redo.png"));
        menu->addSeparator();
			
			  QMenu *charsfo = TextMenu(inparent);
        QMenu *blockfo = BlockMenu(inparent);
				QMenu *tablesfo = TableMenu(inparent);
			
         menu->addAction(charsfo->menuAction()); 
         menu->addAction(blockfo->menuAction()); 
			   menu->addAction(tablesfo->menuAction()); 
			   menu->addSeparator();
			
			
			
			  a = menu->addAction(tr("Cu&t") + ACCEL_KEYL(X), this, SLOT(cut()));
        a->setEnabled(C_cursor.hasSelection());
			  a->setIcon(QIcon(":/img/editcut.png"));
		}
		    a = menu->addAction(tr("&Copy") + ACCEL_KEYL(C), this, SLOT(copy()));
        a->setEnabled(C_cursor.hasSelection());
		    a->setIcon(QIcon(":/img/editcopy.png"));
	
		    menu->addSeparator();
        a = menu->addAction(tr("Select All") + ACCEL_KEYL(A), this, SLOT(selectAll()));
        a->setEnabled(!_d->isEmpty());
		
		if (editable()) {
				a = menu->addAction(tr("&Paste") + ACCEL_KEYL(V), this, SLOT(paste()));
        a->setEnabled(clipboard->text().size() > 1);
			  a->setIcon(QIcon(":/img/editpaste.png"));
        a = menu->addAction(tr("Delete"), this, SLOT(deleteSelected()));
        a->setEnabled(C_cursor.hasSelection());
			  a->setIcon(QIcon(":/img/delete.png"));
			
			  a = menu->addAction(tr("Insert image"), this, SLOT(InsertImageonCursor()));
			  a->setIcon(QIcon(":/img/thumbnail.png"));
			
			  a = menu->addAction(tr("Layer margin"), this, SLOT(SetLayerMargin()));
			  a->setIcon(QIcon(":/img/view_top_bottom.png"));
			  
				a = menu->addAction(tr("Insert frame"), this, SLOT(FosInsertFrame()));
			
			
    }
		
		return menu;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                          EVENTS                                                 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void  TextWriter::InsertImageonCursor()
{
	  if (!_d) {
		return;
		}
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









