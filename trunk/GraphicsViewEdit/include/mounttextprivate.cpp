/*******************************************************************************
 * class TextWriter
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/


#include "mounttextprivate.h"

TextWriter::TextWriter(QObject *parent )
  : Layoutpainter(this),cursorIsFocusIndicator(false),edit_enable(false),cursorOn(false),timeline(0),
	overwriteMode(false),StartSelectionMouse(-1),Ram_Dat(false),DragFill(false)
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

/* wake up new format from current text !!!! */
void TextWriter::NewCharformat( QTextCursor cursor )
{
    QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    ////////QTextBlock bf = textCursor().block();
    QFont f = format.font();
     ///////////qDebug() << "### Layoutpainter cursor in .........." << f.bold() << " .............................................";
    actionBold->setChecked( f.bold() );
	  actionLink->setChecked( format.anchorHref().size() > 0 );
    actionItalic->setChecked( f.italic() );
    actionUnderline->setChecked(f.underline() );
    MakealignmentChanged(textCursor().block().blockFormat().alignment());
    ///////actionOverline->setChecked( f.overline() );
    actionStricktext->setChecked( f.strikeOut() );
    actionTextColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",format.foreground().color()));
    actionBackColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",format.background().color()));
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
									   ///////// qDebug() << "### TextWriter insert 4 addResource " << e.name << e.pix().isNull();
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
																					  //////qDebug() << "### hrefadress register " << hrefadress;
																					  QVariant xx = pico.property(_IMAGE_PICS_ITEM_); 
																					   if (!xx.isNull()) {
                                                SPics pic = xx.value<SPics>();
																							  //////////qDebug() << "### TextWriter insert 1 " << pic.name;
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
									   /////////// qDebug() << "### TextWriter insert 2 addResource " << e.name << e.pix().isNull();
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
				span_border = new SpanBorder(this);
				C_cursor.setPosition(0,QTextCursor::MoveAnchor);
				cursor_position = C_cursor.position();
				QObject::connect(clipboard, SIGNAL(dataChanged() ), this, SLOT(int_clipboard_new()));
				/////QObject::connect(_d, SIGNAL(modificationChanged(bool) ),this, SLOT(update() ));  /* paint area size? */
				QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
				QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(NewCharformat(QTextCursor) ));
						
	      ////////QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(NewCharformat(QTextCursor) ));
				//////////QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ),bridge, SLOT(cursorPositionChanged(QTextCursor) ));
				QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)),this,SIGNAL(updateRequest(QRectF)));
				/////QObject::connect(_d, SIGNAL(undoAvailable(bool)),this, SIGNAL(undoAvailable(bool)));
        ///////QObject::connect(_d, SIGNAL(redoAvailable(bool)),this, SIGNAL(redoAvailable(bool)));
				_d->setUndoRedoEnabled(true);
				_d->documentLayout()->registerHandler(QTextCharFormat::UserObject + 1,span_border);
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
	Ram_Dat = true;
}

void TextWriter::paint_doc(  QPainter * painter ,
                         const QStyleOptionGraphicsItem *option , 
                         QBrush BGpage , 
                         const QRectF fulllayer , QPen BorderFiller ,
                         bool currentprintrender )
{
	  bool makeborder = false;
	  qreal borderWWI = 0.;
	  qreal halfborder;
	
	  const QColor backcolor = BGpage.color();
    const QColor contrastbackcolor(
        backcolor.red()   > 127 ? 0 : 255,
        backcolor.green() > 127 ? 0 : 255,
        backcolor.blue()  > 127 ? 0 : 255); 
	
	
	         if (BorderFiller.widthF() > 0) {
           borderWWI	= BorderFiller.widthF();
					 halfborder =  borderWWI / 2;
					 }
					 
		
		QPen line_pen(QBrush(Qt::red),1,Qt::DotLine,Qt::RoundCap);		 
	  painter->save();
		if (halfborder > 1.1) {	
    painter->setPen(Qt::NoPen);
		makeborder = true;
		} else {			
		painter->setPen(BorderFiller);
		}		 
		painter->setBrush(BGpage);
		painter->drawRect(fulllayer);
		painter->restore();
	  painter->save();
		
	  /* rect from frame root QTextDocument*/
	  const  QRectF doc_rect = boundingRect();
		QColor BackHightlight("#a6ffc7");
					 BackHightlight.setAlpha(140);
		QAbstractTextDocumentLayout::PaintContext CTX;    
		CTX.selections;
		painter->setClipRect(fulllayer, Qt::IntersectClip);
		CTX.clip = doc_rect;
	
		 
		CTX.palette.setColor(QPalette::Highlight,BackHightlight);
		CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
		/* blink cursor from timer event ! */
		if (edit_enable) {
				if (cursortime) {  /* cursor blink yes vom timer QApplication::cursorFlashTime() */
				/////////////////painter->setPen(QPen(QBrush(contrastbackcolor),1));
				CTX.cursorPosition = C_cursor.position();
				} else {
				////////////////painter->setPen(Qt::NoPen);
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
		painter->restore();
				
			if (makeborder) {
				/* draw dick border  BorderFiller.brush()  width()  height() */
				QRectF smalit = fulllayer;
				       smalit.setWidth(  fulllayer.width() - halfborder  );
				       smalit.setHeight ( fulllayer.height() - halfborder  );
				       smalit.setTopLeft(QPointF(halfborder,halfborder));
				painter->save();
				painter->setPen(BorderFiller);
				painter->setBrush(Qt::NoBrush);
				painter->drawRect(smalit);
				painter->restore();
			}
				
				
				
				
				
				
				
				
				
			if (editable() && !currentprintrender ) {  /* writteln modus icon */
				QPixmap pixbg(":/img/icon.png");
				painter->save();
				painter->drawPixmap(QPointF(fulllayer.width() - 40,8),pixbg);
				painter->restore();
			}
			
			
			
			
			
			//////////////painter->save();
			/////////painter->setBrush(QBrush(Qt::white));
      ///////////QTextLayout Latooltips( "ToolTip sample dsafgdsgfsdgsdgsgsdgsgsdgdsg gsdg sdgsdgsdgs " );
      ////////////Latooltips.beginLayout();
      /////////////QTextLine line = Latooltips.createLine();
      //////////line.setLineWidth( 100 );
      ///////////line.setPosition( QPointF( 0, 0 ) );
			////////////Latooltips.setTextOption(QTextOption(Qt::AlignRight));
      ////////////Latooltips.endLayout();
			
			/////////////QPointF  pointxy(fulllayer.width() - line.width() - 5 ,45);
			///////////painter->drawRect(line.rect());
			
      ///////////Latooltips.draw(painter,pointxy);
			/////////////////painter->restore();
				
				
		
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
	
	
	   if (cursorOn) {
			return;
		 }
		 
		 cursorOn = enable;

    if (enable && QApplication::cursorFlashTime() > 0) {
        cursorTimeLine.start( QApplication::cursorFlashTime() / 2,this);
		}  else {
        cursorTimeLine.stop();
        
		}
}

void TextWriter::ClearSelections()
{
	   //////////qDebug() << "### ClearSelections call " << cursorIsFocusIndicator;
	
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
	  ////////////qDebug() << "### position_selection_start/stop selectAll" << cursor_position << "/" << position_selection_start; 
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
		
		/* test if a image !!!! */
		
    QMimeData *data = createMimeDataFromSelection();
    QApplication::clipboard()->setMimeData(data);
}

void TextWriter::selectionChanged(bool forceEmitSelectionChanged /*=false*/)
{
    if (forceEmitSelectionChanged && C_cursor.hasSelection() ) {
         TextHighlightSelect = QRectF();
         repaintCursor(true);
		} else {
			ClearSelections();
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
				//////qDebug() << "### cursortime  " << cursortime;
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
		QTextCharFormat format = C_cursor.charFormat();
		
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
						format.setFontWeight(!format.font().bold()  ? QFont::Bold : QFont::Normal);
						C_cursor.setCharFormat(format);
						e->accept();
						repaintCursor();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_I) {
			      format.setFontItalic(!format.font().italic() ? true : false );
			      C_cursor.setCharFormat(format);
            e->accept();
            repaintCursor();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_U) {
            format.setUnderlineStyle(!format.font().underline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline );
			      C_cursor.setCharFormat(format);
            e->accept();
            repaintCursor();
						return;
		}
		
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_H) {
            C_cursor.insertFragment(QTextDocumentFragment::fromHtml("<hr>"));
            e->accept();
            repaintCursor();
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
			 
			     case QEvent::QEvent::GraphicsSceneDrop: {
						if (edit_enable) { 
						QGraphicsSceneDragDropEvent *edd = static_cast<QGraphicsSceneDragDropEvent *>(e);
						setCursorPosition(edd->pos());
						insertFromMime(edd->mimeData());
						return;
							
						}
						return;
						break; }
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
					  Fevent = ev->widget();
					  ///////////qDebug() << "### 1 MouseButtonDblClick";
					  if (  ev->modifiers() == Qt::NoModifier ) {
            tmouseDoubleClickEvent(e, ev->button(), ev->pos() );
						}
            break;
					  return;
					}
				case QEvent::GraphicsSceneMouseRelease: {
             QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
					   Fevent = ev->widget();
             tmouseReleaseEvent(e, ev->button(), ev->pos() );
					break; }
				
				case QEvent::GraphicsSceneMousePress: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
					  ///////////////qDebug() << "### 1 GraphicsSceneMousePress";
					  Fevent = ev->widget();
					  if (ev->buttons() == Qt::LeftButton) {
            tmousePressEvent(ev->button(), ev->pos() , ev->modifiers(), ev->buttons(),ev->screenPos());
						return;
						} 
            break; }
		 }
		 
	   if (edit_enable)  {
			setBlinkingCursorEnabled(true);
		 }
	
}

QTextTableCell TextWriter::OnPosition( const int posi )
{
	////////////qDebug() << "### cell  OnPosition " << posi;
	       if (posi != -1) {
	        QTextCursor tmpcursor(_d);
					tmpcursor.setPosition(posi);
	        if ( !tmpcursor.currentTable() ) {
						return QTextTableCell();
					}
					return tmpcursor.currentTable()->cellAt(tmpcursor);
				 }  else {
					 return QTextTableCell();
				 }
}



void TextWriter::tmouseMoveEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	///////////qDebug() << "### mouseMoveEvent in ";
	///////////////qDebug() << "### move drag   " << DragFill;
	const int cursorPos = _d->documentLayout()->hitTest(pos,Qt::ExactHit) + 1;
	
	if (C_cursor.hasSelection() && DragFill) {
		/////////////qDebug() << "### move drag on  " << cursorPos;
		//////setCursorPosition(pos);
		////////repaintCursor();
		cursortime = false;
		repaintCursor();
		return;
	}

	const int cursorPosFozze = _d->documentLayout()->hitTest(pos,Qt::FuzzyHit);
	const int stopat = qMax(StartSelectionMouse,cursorPos); 
	const int startat = qMin(StartSelectionMouse,cursorPos);
	  ////////////qDebug() << "### start/stop cell position " << StartSelectionMouse << cursorPos << cursorPosFozze;
	 
	
	
	
	  if (StartSelectionMouse != -1 && cursorPos > 0 && !C_cursor.currentTable()) {
			if (StartSelectionMouse != cursorPos) {
	       setBlinkingCursorEnabled(false);
				 
				 
				 
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
		
		
		 if ( C_cursor.currentTable() ) {
			QTextTable *table = C_cursor.currentTable();
			QTextTableCell firstcell = OnPosition(StartSelectionMouse);
			QTextTableCell lastcell = OnPosition(cursorPosFozze);
			if ( firstcell.isValid() && lastcell.isValid() ) {
			int fcellrow = firstcell.row();
			int fcellcool = firstcell.column();
			////////////qDebug() << "### row / cool " << fcellrow << fcellcool;
			/////////////   qBound ( const T & min, const T & value, const T & max ) 
      ///////////C_cursor.selectedTableCells(fcellrow,lastcell.row() - fcellrow,lastcell.column() -  fcellcool);
			int numRows = qBound(1,lastcell.row() - fcellrow,table->rows());
			int numColumns = qBound(1,lastcell.column() - fcellcool,table->columns());
			//////////////qDebug() << "### nnrow / nncool " << numRows << numColumns;
			C_cursor.selectedTableCells(&fcellrow,&numRows,&fcellcool,&numColumns);
			C_cursor.setPosition(firstcell.firstPosition());
			C_cursor.setPosition(lastcell.lastPosition(), QTextCursor::KeepAnchor);
			cursor_position = stopat;
			setBlinkingCursorEnabled(false);
			QRectF tablerect = _d->documentLayout()->frameBoundingRect(table);
			emit updateRequest(tablerect);
			return;
			}
		}
}

QPixmap TextWriter::PicfromMime( QMimeData *mime )
{
	QPixmap one;
	QStringList dli = mime->formats();
	if (dli.contains("application/x-picslists")) {
		        QByteArray dd = mime->data("application/x-picslists"); 
            QList<SPics> li = OpenImageGroup(QString(dd.data()));
		        if (li.size() > 0) {
            SPics primoi = li.first();
            return primoi.pix();
						}
		
	}
	
	if (dli.contains("text/html")) {
		QByteArray dd = mime->data("text/html"); 
		QTextDocument *picdoc = new QTextDocument();
		picdoc->setHtml ( QString(dd.data()) );
		
		           QMapIterator<QString,SPics> i(imglist());
                 while (i.hasNext()) {
                     i.next();
									    SPics e  = i.value();
						         picdoc->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
								 }
		
		
		    QTextFrame  *Tframe = picdoc->rootFrame();
				QTextFrameFormat rootformats = Tframe->frameFormat();
				rootformats.setBottomMargin (0); 
				rootformats.setTopMargin(0); 
				rootformats.setRightMargin (0); 
				rootformats.setLeftMargin (0); 
				rootformats.setPadding (0); 
				Tframe->setFrameFormat ( rootformats );
		    picdoc->adjustSize();
		    /* paint doc */
		    QAbstractTextDocumentLayout *Layout = picdoc->documentLayout();
	      QRectF docirec = Layout->frameBoundingRect(picdoc->rootFrame()); 
		    QPixmap PicsDocument(docirec.size().toSize());
				PicsDocument.fill(Qt::white);
		    QRectF clip(0, 0,PicsDocument.width(),PicsDocument.height());
				QPainter *p = new QPainter(&PicsDocument);
        p->setRenderHint(QPainter::Antialiasing, true);
        picdoc->drawContents(p,clip);
				p->end();
				return PicsDocument;
	}
	
	return one;
}


void TextWriter::tmousePressEvent(Qt::MouseButton button, const QPointF &pos, Qt::KeyboardModifiers modifiers,
                                          Qt::MouseButtons buttons, const QPoint &globalPos)
{
	///////////qDebug() << "### mousePressEvent in ";
	///////qDebug() << "### Press drag   " << DragFill << " mouse selection " << C_cursor.hasSelection();
	if (C_cursor.hasSelection() && modifiers == Qt::ShiftModifier && edit_enable) {
							   DragFill = true;
		             cursortime = false;
							   //////////qDebug() << "### drag can fill  ";
		             setBlinkingCursorEnabled(false);
								 repaintCursor();
		             QMimeData *DDmime = createMimeDataFromSelection();
								 QPixmap ddpic = PicfromMime(DDmime);
		             QDrag *drag = new QDrag(Fevent);
                 drag->setMimeData(DDmime); 
		             if (!ddpic.isNull()) {
                 drag->setPixmap(ddpic); 
                 	delete &ddpic;								 
                 }
								 if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
								 return;         /* */
                 }
		
	} else {
		setCursorPosition(pos);
	}
	
	lastrect = boundingRect();
	
	cursortime = true;
	
	
	
	if (StartSelectionMouse == cursor_position) {
		  /////////////qDebug() << "### =========================  start pos  ";
		  ClearSelections();
		  setBlinkingCursorEnabled(true);
		  repaintCursor();
		  return;
	} else {
		setBlinkingCursorEnabled(false);
		StartSelectionMouse = cursor_position;
		repaintCursor();
	}
	
	   /* fast  display not alternate */
	
}

void TextWriter::tmouseReleaseEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	 /////////qDebug() << "### tmouseReleaseEvent 1 ############################################";
	///////////// qDebug() << "### Release drag   " << DragFill;
	 
	
	 StartSelectionMouse = -1;
	 DragFill = false;
}


void TextWriter::tmouseDoubleClickEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos)
{
	//////////////qDebug() << "### MouseButtonDblClick 1 ############################################";
	
	if (button != Qt::LeftButton) {
		e->ignore();
		return;
	}
  StartSelectionMouse = -1;
	cursortime = true;   /* fast  display not alternate */
	setCursorPosition(pos);
	lastrect = boundingRect();

		
		      if (currentTextLine(C_cursor).rect().isValid() && currentTextLine(C_cursor).textLength() > 0) {
            C_cursor.select(QTextCursor::WordUnderCursor);   /////  return tc.selectedText();
						const int position = C_cursor.selectionStart();
            const int anchor = C_cursor.selectionEnd();
						
						///////////qDebug() << "selectedText " << C_cursor.selectedText();
						
						
						const QTextCursor oldSelection = C_cursor;
						if (position != anchor && C_cursor.selectedText().size() > 0) {
					    cursor_position = anchor;
							position_selection_start = position;
							QRectF selectionXY = currentTextLine(C_cursor).rect();
							selectionChanged(selectionXY.isValid());   /////   line 414
						}
				    repaintCursor();
          } else {
						ClearSelections();
				    repaintCursor();
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
			emit q_update(boundingRect());
		  return;
		 }
		 
		 GrepCursorData();
		 
		 
		if (C_cursor.hasComplexSelection() && C_cursor.currentTable()) {
        QTextTable *table = C_cursor.currentTable();
        QRectF tablerect = _d->documentLayout()->frameBoundingRect(table);
				emit q_update(tablerect);
		}
			
		 
		 if (paragraphrect.isValid()) {
			 emit q_update(paragraphrect);
		 } else {
			 emit q_update(boundingRect());
		 }
}

void TextWriter::GrepCursorData()
{
	if (!editable()) {
	return;
	}
	if (!_d) {
		return;
	}
	
	   paragraphrect = _d->documentLayout()->blockBoundingRect(textCursor().block());
	   //////qDebug() << "### mouse pararec  " << paragraphrect;
		 DDline = currentTextLine(textCursor());
		 line_rect = DDline.naturalTextRect();
		 line_rect_out = DDline.rect();
		 line_nummer = DDline.lineNumber();
		 linehight = DDline.height();
	   cursor_position = textCursor().position();
		 X_Pos_Cursor = paragraphrect.x();  //////DDline.cursorToX(cursor_position);
	   Y_Pos_Cursor =  qBound (0.,paragraphrect.y(),boundingRect().bottom());
		 ///////////////qDebug() << "### mouse data  Ln." << Y_Pos_Cursor << " Cp." << cursor_position  << "|" << textCursor().position();
		 CursorDrawLine = QLineF(QPointF(X_Pos_Cursor,Y_Pos_Cursor),QPointF(X_Pos_Cursor,paragraphrect.bottom()));
}



void TextWriter::insertFromMimeData(const QMimeData *source)
{
 
    if (!_d) {
	  return;
		}

    bool hasData = false;
    QTextDocumentFragment fragment;
		
		/* check image */
		
		if ( source->hasImage() || source->hasFormat(QLatin1String("application/x-picslists")) ) {
			  insertFromMime(source);  /* image handler */
			  ClearSelections();
        repaintCursor(true);
			 return;
		}
		
		
		
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
		
		///////////qDebug() << "### insertFromMimeData " << hasData;
		
    if (hasData) {
        C_cursor.insertFragment(fragment);
			  boundingRect();  /* shake size ! */
			  ClearSelections();
        repaintCursor(true);
		}
}

QMimeData *TextWriter::createMimeDataFromSelection() const
{
	 QTextCharFormat base = C_cursor.charFormat();
	 QTextImageFormat pico = base.toImageFormat(); 
	 if (pico.isValid()) { 
		           QVariant xx = pico.property(_IMAGE_PICS_ITEM_); 
		           if (!xx.isNull()) {
								   SPics pic = xx.value<SPics>();
								   QList<SPics> li;
								                li.append(pic);
								   QString Sdd = SaveImageGroup(li);
								   QMimeData *mimeData = new QMimeData;
                   mimeData->setData("application/x-picslists",Sdd.toUtf8());
								   return mimeData;
							 }
							 
				     const QString hrefadress = pico.name();
				      /* grab pixmap */
				     /////QVariant x2 = _d->loadResource(QTextDocument::ImageResource,QUrl(hrefadress));
            /////if (!x2.isNull()) {
               //// QMimeData *mimeData = new QMimeData;
							 //// mimeData->setImageData(x2);
							  /////return mimeData;
						 /////}
	 }
	const QTextDocumentFragment fragment(C_cursor);
	return new QTextEditMimeData(fragment);    /////QTextDocumentFragment::fromHtml(md->html())
}

QList<QAction *> TextWriter::MainActions()
{
	///7 action ! 
	QList<QAction *> flyactions;
	                 flyactions.clear();

	  actionUndo = new QAction(tr("&Undo") + ACCEL_KEYL(Z), this);
    actionUndo->setIcon(QIcon(":/img/undo.png"));
	  actionUndo->setEnabled(_d->isUndoAvailable());
	  connect(actionUndo, SIGNAL(triggered()),this,SLOT(undo()));
	
	  actionRedo = new QAction(tr("&Undo") + ACCEL_KEYL(Z), this);
    actionRedo->setIcon(QIcon(":/img/redo.png"));
	  actionRedo->setEnabled(_d->isRedoAvailable());
	  connect(actionRedo, SIGNAL(triggered()),this,SLOT(redo()));

		actionBold = new QAction(tr("Bold text CTRL+B"),this);
    const QIcon icon = QIcon(QString::fromUtf8(":/img/textbold.png"));
    actionBold->setIcon(icon);
    actionBold->setCheckable(true);
	  connect(actionBold, SIGNAL(triggered()),this,SLOT(BoldText()));
    
    actionFonts = new QAction(tr("Text Font"),this);
    const QIcon icon9 = QIcon(QString::fromUtf8(":/img/textpointer.png"));
    actionFonts->setIcon(icon9);
	  connect(actionFonts, SIGNAL(triggered()),this,SLOT(FontText()));
		
    actionItalic = new QAction(tr("Italic Text CTRL+I"),this);
    const QIcon icon1 = QIcon(QString::fromUtf8(":/img/textitalic.png"));
    actionItalic->setIcon(icon1);
	  actionItalic->setCheckable(true);
	  connect(actionItalic, SIGNAL(triggered()),this,SLOT(ItalicText()));
    
    actionUnderline = new QAction(tr("Text underline CTRL+U"),this);
    const QIcon icon2 = QIcon(QString::fromUtf8(":/img/textunder.png"));
    actionUnderline->setIcon(icon2);
	  actionUnderline->setCheckable(true);
	  connect(actionUnderline, SIGNAL(triggered()),this,SLOT(UnderlineText()));
		
		actionBackColor = new QAction(tr("Text background color"),this);
    actionBackColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",Qt::white));
	  connect(actionBackColor, SIGNAL(triggered()),this,SLOT(BGcolor()));
    
    actionTextColor = new QAction(tr("Text color"),this);
    actionTextColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",Qt::black));
	  connect(actionTextColor, SIGNAL(triggered()),this,SLOT(TXcolor()));
		
		actionBlockMargin = new QAction(tr("Block paragraph margin"),this);
    actionBlockMargin->setIcon(QIcon(QString::fromUtf8(":/img/view_text.png")));
    connect(actionBlockMargin, SIGNAL(triggered()),this,SLOT(SetTextBlockMargin()));
		
		
		grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(MaketextAlign(QAction *)));
    
    actionAlignLeft = new QAction(QIcon(":/img/textleft.png"), tr("Text align left"),grp);
    actionAlignLeft->setCheckable(true);
    actionAlignRight = new QAction(QIcon(":/img/textright.png"), tr("Text align right"),grp);
    actionAlignRight->setCheckable(true);
    actionAlignCenter = new QAction(QIcon(":/img/textcenter.png"), tr("Text align center"),grp);
    actionAlignCenter->setCheckable(true);
    actionAlignJustify = new QAction(QIcon(":/img/textjustify.png"), tr("Text align Justify"),grp);
    actionAlignJustify->setCheckable(true);
		
		NewCharformat(textCursor());
		NewCharformat(textCursor());
		
		flyactions.append(actionUndo);
		flyactions.append(actionRedo);
		flyactions.append(actionBold);
		flyactions.append(actionItalic);
		flyactions.append(actionUnderline);
		flyactions.append(actionFonts);
		flyactions.append(actionBackColor);
		flyactions.append(actionTextColor);
		flyactions.append(actionBlockMargin);
		
		flyactions.append(actionAlignLeft);
		flyactions.append(actionAlignRight);
		flyactions.append(actionAlignCenter);
		flyactions.append(actionAlignJustify);
		
		
	  return flyactions;
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
			  QMenu *framefo = FrameMenu(inparent);
			
         menu->addAction(charsfo->menuAction()); 
         menu->addAction(blockfo->menuAction()); 
			   menu->addAction(tablesfo->menuAction()); 
			   menu->addAction(framefo->menuAction()); 
			   menu->addSeparator();
			  
				 NewCharformat(textCursor());
			
			
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
        a->setEnabled(Ram_Dat);
			  a->setIcon(QIcon(":/img/editpaste.png"));
        a = menu->addAction(tr("Delete"), this, SLOT(deleteSelected()));
        a->setEnabled(C_cursor.hasSelection());
			  a->setIcon(QIcon(":/img/delete.png"));
			
			  a = menu->addAction(tr("Insert image"), this, SLOT(InsertImageonCursor()));
			  a->setIcon(QIcon(":/img/thumbnail.png"));
			  
			
			  QTextImageFormat img = textCursor().charFormat().toImageFormat();
        if (img.isValid()) { 
				a = menu->addAction(tr("Image Info"), this, SLOT(ImageInfo()));
				a->setIcon(QIcon(":/img/thumbnail.png"));
				}
			
			  
			  
			
			
			  a = menu->addAction(tr("Layer margin"), this, SLOT(SetLayerMargin()));
			  a->setIcon(QIcon(":/img/view_top_bottom.png"));
			  
				
			
			
    }
		
		return menu;

}



void  TextWriter::ImageInfo()
{
	QTextImageFormat img = textCursor().charFormat().toImageFormat();
	if (img.isValid()) { 
		QVariant xx = img.property(_IMAGE_PICS_ITEM_); 
		if (!xx.isNull()) {
		       SPics pic = xx.value<SPics>();
			     const QString hrefadress = img.name();
           QPixmap rec = pic.pix();
			     QPixmap simgs  = rec.scaledToWidth( 50 );
			     QString msg = QString("Name=%3\nPoint=%1x%2\n").arg(rec.width()).arg(rec.height()).arg(hrefadress);
			     QString msg1 = QString("Description=%1\nSuffix=%2\n").arg(pic.info).arg(QString(pic.extension.data()));
			     QString msg2 = QString("Monitor DPI=%1\n").arg(rec.logicalDpiY());
			     QString msg3 = QString("Dimension= width=%1 height=%2 mm\n").arg(ToUnit(rec.width(),"mm")).arg(ToUnit(rec.height(),"mm"));
			     QString msg4 = QString("Dimension= width=%1 height=%2 cm\n").arg(ToUnit(rec.width(),"cm")).arg(ToUnit(rec.height(),"cm"));
			     QString msg5 = QString("Dimension= width=%1 height=%2 inch\n").arg(ToUnit(rec.width(),"inch")).arg(ToUnit(rec.height(),"inch"));
			     QString msg6 = QString("Dimension= width=%1 height=%2 em\n").arg(ToUnit(rec.width(),"em")).arg(ToUnit(rec.height(),"em"));
			     QString msg7 = QString("Dimension= width=%1 height=%2 px\n").arg(ToUnit(rec.width(),"px")).arg(ToUnit(rec.height(),"px"));
			     QString msg8 = QString("Dimension= width=%1 height=%2 dm\n").arg(ToUnit(rec.width(),"dm")).arg(ToUnit(rec.height(),"dm"));
			
			     msg.append(msg1);
			     msg.append(msg2);
			     msg.append(msg3);
			msg.append(msg4);
			msg.append(msg5);
			msg.append(msg6);
			msg.append(msg7);
			msg.append(msg8);
			
			     QMessageBox msgBox;
           msgBox.setStandardButtons(QMessageBox::Ok);
					 msgBox.setWindowTitle (tr("Image info %1").arg(hrefadress));
			     msgBox.setIconPixmap ( simgs );
			     msgBox.setText ( msg );
			     msgBox.exec();
			     /////msgBox.setDetailedText ( msg );
			     
		}
	}
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
	  SPics base;
	
    bool ok;
	  if (e.info == base.info) {
       QString txtinfo = QInputDialog::getText(0, tr("Image Description to blind people."),tr("Description:"), QLineEdit::Normal,e.name, &ok);
       if (txtinfo.size() > 0) {
       e.info = txtinfo.left(110);
       }
	  } else {
			/* having desc  !!!  */
		}
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





void TextWriter::insertPixmap( QPixmap p )
{
        QPixmap scaledsimage;
        QTextFrame  *Tframe = _d->rootFrame();
        QTextFrameFormat rootformat= Tframe->frameFormat();
				const int margininside = rootformat.leftMargin() + rootformat.rightMargin() + rootformat.padding();
        int limitwiimage = _MAXIEDITWI_;
    
				 if (limitwiimage > boundingRect().width()) {
					  limitwiimage = boundingRect().width() - margininside ;
				 }
         
         if (p.width() > limitwiimage) {
             /* question widht */
					   bool ok;
             int iw = QInputDialog::getInteger(0, tr("Image to width in layer dimension!"),
                                      tr("Point:"), limitwiimage, 20, limitwiimage, 1, &ok);
					if (iw > 0 && ok) {
						scaledsimage = p.scaledToWidth( iw );
					} else {
					return;
					}
          
          
        } else {
           scaledsimage = p; 
        }
        
         QByteArray bytes;
         QBuffer buffer(&bytes);
         buffer.open(QIODevice::WriteOnly);
         if (_DRAWMODUS_WEB_ == 1) {
				 scaledsimage.save(&buffer,"JPG",70);
				 } else {
         scaledsimage.save(&buffer,"PNG",100);
				 }
        
         QDateTime timer1( QDateTime::currentDateTime() );
         const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
				
         if (!scaledsimage.isNull()) {
         const QString nami = Imagename(TimestampsMs);
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
}

void TextWriter::in_image( int id )
{
	   QApplication::restoreOverrideCursor();
	   LoadGetImage *ht = qobject_cast<LoadGetImage *>(sender());
	   QPixmap  imagen = ht->pics();
	   insertPixmap(imagen);
}


void TextWriter::insertFromMime( const QMimeData * source )
{
	
	  if ( source->formats().contains("application/x-picslists") ) {
			 QByteArray dd = source->data("application/x-picslists"); 
			 QList<SPics> li = OpenImageGroup(QString(dd));
			      for (int i=0; i<li.size(); i++) {
                 SPics conni = li[i];
							   RegisterImage(conni,true);
             }
			return;
		}
	
    if ( source->hasImage() ) {
         QDateTime timer1( QDateTime::currentDateTime() );
         const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
         QPixmap aspixmape = qvariant_cast<QPixmap>(source->imageData());
                if (!aspixmape.isNull()) {
                insertPixmap(aspixmape);
                }
         return;
    }
    /* external html  */

    if ( source->formats().contains("text/html") ) {
         QString draghtml = source->html();
         QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(draghtml);
         textCursor().insertFragment(fragment);
         return;
    }
    /* external plain text incomming   */
      if ( source->hasText() )  {
        textCursor().insertText(source->text());
        return;
      }
      if ( source->hasUrls() )  {
          QList<QUrl> urls = source->urls();
          for ( int i = 0; i < urls.size(); ++i ) { 
          QUrl gettyurl(urls.at(i));
				 //////////// qDebug() << "### gettyurl " << gettyurl.toString();
              if (gettyurl.scheme() == "file") {
                  ImageonCursor(gettyurl.toLocalFile()); 
              } else if (gettyurl.scheme() == "http") {
                            Gloader *grephttp = new Gloader();
							              grephttp->Setting(this,i,gettyurl); 
							              grephttp->start(QThread::LowPriority);
                  
              }
              
          }
         
        return;          
      }
    
    
}











