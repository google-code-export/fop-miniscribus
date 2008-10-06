#include "DocEdit.h"

static void paintWidgedDebug( QPainter *p , const QRectF rect )
{
    p->save();
    p->setBrush(Qt::green);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

/**/
EditArea::EditArea( QWidget *parent )
        : QAbstractScrollArea(0),page(0,0,0,0),lineTimer(0),
        workArea(0,0),scaleFaktor(1.3),portrait_mode(true),mesure(11.2),_doc(new PDocument),overwriteMode(false),
        position_selection_start(-1),cursorIsFocusIndicator(false),clipboard(QApplication::clipboard())
{
    mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,0.,0.);
    _doc->openFile("index.html");
    C_cursor = QTextCursor(_doc);
    cursorMovetoPosition(QPointF(0,0));
    setBlinkingCursorEnabled(true);
    adjustScrollbars();
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(verticalValue(int)));
    connect(clipboard, SIGNAL(dataChanged() ), this, SLOT(clipboard_new()));
    connect(_doc, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
    setAcceptDrops ( true );
    resize(700,400);
}

void EditArea::clipboard_new()
{
	qDebug() << "### clipboard fill  ";
}

void EditArea::cursorPosition( QTextCursor & cursor )
{
    LastCharFormat = cursor.charFormat();  
	if (cursor.isCopyOf(C_cursor)) {
      
	} else {
		 C_cursor.setPosition(cursor.position());
	}
    
    emit txtcursorChanged(C_cursor);
}

void EditArea::paintEvent( QPaintEvent *Event )
{
    lineTimer++;
    QTransform matrix;
    QTextFrameFormat docrootformat = _doc->rootFrame()->frameFormat();
    QPainter *p = new QPainter( viewport() );
    p->setRenderHint(QPainter::Antialiasing, true);
    p->setPen(Qt::NoPen);
    p->setBrush(_EDITORBGCOLOR_);
    p->drawRect(Event->rect());

    if (lineTimer != 1) {
        const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
        const int y = yOffset();
        const int x = xOffset();
        p->translate(-x,-y);
        p->setWorldTransform(mcurrent,true);  /*  zoom && center on adjustScrollbars() */
        p->setBrush(Qt::white);
        p->drawRect(page);

        for (int i = 0; i < dotChain.size(); ++i) {
            const QPointF nowPoint = dotChain.at(i);
            QRectF fire(nowPoint  - QPointF(8,8),QSizeF(16,16));
            p->setBrush(Qt::red);
            p->setPen(Qt::white);
            p->setOpacity(0.5);
            p->drawEllipse(fire);
            p->setOpacity(1.0);
        }

        paintEditPage(0,p,page);
        paintShadow(p,page);
        /* reset matrix as a null matrix */

        p->setWorldTransform(matrix);
        /* top left bars static to not move */
        paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,viewAreaRect.height()),mcurrent);
        paintWidged(p,QRectF(0,0,viewAreaRect.width(),SLIDERMARGIN_TICK_TOTAL),mcurrent);
        /* top left bars static to not move */
        /* horrizzontal  slider */
        p->translate(-x,0);
        slider_Horrizzontal_Top = QRectF(mcurrent.m31(),SLIDERSPACER,workArea.width(),SLIDERMARGIN_TICK);  /* click top area */
        paintScale(p,slider_Horrizzontal_Top,qMakePair(docrootformat.leftMargin(),docrootformat.rightMargin()),mcurrent);
        top_matrix = p->worldTransform();
        /* horrizzontal*/
        p->setWorldTransform(matrix); /* reset */
        p->translate(0,-y);
        /* vertical */
        slider_Vertical_Left = QRectF(SLIDERSPACER,mcurrent.m32(),SLIDERMARGIN_TICK,workArea.height());  /* click left area */
        paintScale(p,slider_Vertical_Left,qMakePair(docrootformat.topMargin(),docrootformat.bottomMargin()),mcurrent);
        /* vertical */
        left_matrix = p->worldTransform();
        p->setWorldTransform(matrix);
        paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,SLIDERMARGIN_TICK_TOTAL),mcurrent);
        ///////////////paintArea(p,pageMatrix().mapRect(CurrentBlockRect()),HightlightColor());

    }  else  {
        qDebug() << "### maybe first run " << lineTimer;
        adjustScrollbars();
        fitToLarge();
        QAbstractScrollArea::paintEvent(Event);
    }
    p->setWorldTransform(matrix);
    cursorRectSlider(docrootformat,p);


    p->end();
}

void EditArea::cursorRectSlider( const QTextFrameFormat docrootformat  , QPainter *p )
{
    sl_cursor[0] = QRectF(0,0,_doc->size().width(),PAPERSPACE + SLIDERMARGIN_TICK_TOTAL);
    sl_cursor[5] = QRectF(0,0,PAPERSPACE + SLIDERMARGIN_TICK_TOTAL,_doc->size().height());

    sl_cursor[1] = divideRect(sl_cursor[0],0);
    sl_cursor[2] = divideRect(sl_cursor[0],2);
    sl_cursor[3] = divideRect(sl_cursor[5],0);
    sl_cursor[4] = divideRect(sl_cursor[5],2);

    ///////////QTransform aa;
    /////paintWidged(p,sl_cursor[5],aa);
}

bool EditArea::clickSlider( const QPointF p )
{
    for (int i = 0; i < 6; ++i)  {
        if (sl_cursor[i].contains(p)) {
            return true;
        }
    }
    return false;
}

bool EditArea::HandleMoveSlider(  QPointF point , bool top )
{
    /*
    if (top) {
    qDebug() << "### HandleMoveSlider top  Xpos." << POINT_TO_CM(point.x()) << "  Ypos."<< point.y();
    } else {
    qDebug() << "### HandleMoveSlider left Ypos." << POINT_TO_CM(point.y()) << "  Ypos."<< point.x();
    }
    */

    QTextFrameFormat foframe = _doc->rootFrame()->frameFormat();
    bool moved = false;
    for (int i = 0; i < 6; ++i)  {
        if (sl_cursor[i].contains(point)) {
            if (i == 1 && top) {
                ///////qDebug() << "### wake top 1 " << i << point.y();
                foframe.setLeftMargin(point.x());
                moved = true;
            }
            if (i == 2 && top) {
                /////////qDebug() << "### wake top 2 " << i << point.y();
                foframe.setRightMargin(_doc->size().width() - point.x());
                moved = true;
            }
            if (i == 3 && !top) {
                ///////////qDebug() << "### wake left " << i << point.y();
                foframe.setTopMargin(point.y());
                moved = true;
            }
            if (i == 4 && !top) {
                const qreal bottmm = _doc->size().height() - point.y();
                ////////////////qDebug() << "### wake left " << i << bottmm;
                foframe.setBottomMargin(bottmm);
                moved = true;
            }
        }
    }

    if (moved) {
        _doc->rootFrame()->setFrameFormat(foframe);
        update(boundingRect());
        return true;
    } else {
        update(boundingRect());
        return false;
    }
}

QRectF EditArea::boundingRect() const
{
    return QRectF(0,0,viewport()->width(),viewport()->height());
}

QTransform EditArea::pageMatrix()
{
    QTransform runmatrix = mcurrent;
    runmatrix.translate(xOffset(),yOffset());
    return runmatrix;
}



void EditArea::update( const  QRectF rect )
{
    /* tanslate to doc if need */
    const QRectF maximum = boundingRect();
    if (maximum.contains(rect)) {
        /* smaller transform */
        QRectF realrect = pageMatrix().mapRect(rect);
        ////////qDebug() << "------repaint hi -" << realrect.height() << "-------";
        viewport()->update(realrect.toRect());
    } else {
        viewport()->update(maximum.toRect());
        /////////qDebug() << "------repaint full -";
    }
}


void EditArea::update()
{
    update(boundingRect());
}

bool EditArea::isOnSlider( const QPointF p )
{
    /* top area y0,x0 intersect slider top and slider left */
    QRectF totalForbitten(0,0,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL);
    QRectF sliderTop(totalForbitten.topRight(),QSizeF(boundingRect().width(),SLIDERMARGIN_TICK_TOTAL * 1.111));
    QRectF sliderLeft(totalForbitten.bottomLeft(),QSizeF(SLIDERMARGIN_TICK_TOTAL * 1.111,boundingRect().height()));
    if (totalForbitten.contains(p)) {
        return false;
    }
    int cc = 0;
    bool isaccept = false;
    if (sliderTop.contains(p)) {
        cc = 1;
        isaccept = HandleMoveSlider(slider_maps(p,true),true);
    } else if (sliderLeft.contains(p)) {
        cc = 2;
        isaccept = HandleMoveSlider(slider_maps(p,false),false);
    }

    if (isaccept) {
        if (cc == 1) {
            QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        } else if (cc == 2) {
            QApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        }
    } else {
        cursorCheck();
    }


    return isaccept;
}

void EditArea::mouseReleaseEvent( QMouseEvent *e )
{
    cursorCheck();
    ////////resetClickTimer();
    position_selection_start = -1;
}

void EditArea::startDragAction()
{
    
    if (!C_cursor.hasSelection()) {
	return;
	}
	const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
	if (selectionLength > 0) {
	RangeSelection = qMakePair(C_cursor.position(),C_cursor.anchor());
	}
    
    
    /* start drag */
    qDebug() << "------ start drag action --------------------------";
    QMimeData *data = createMimeDataFromSelection();
    if (data) {
            QApplication::clipboard()->setMimeData(data);
            QDrag *drag = new QDrag(this);
            drag->setMimeData(data); 
            drag->setHotSpot(QPoint(-25,-25));
            const QPixmap playdragicon = imagefromMime(data);
            if (!playdragicon.isNull()) {
            drag->setPixmap(playdragicon);
            }
            if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            resetClickTimer();
            }
    }
    resetClickTimer();
}

void EditArea::dragMoveEvent(QDragMoveEvent *e)
 {
     qDebug() << "------dragMoveEvent--------------------------";
     
     if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
         e->acceptProposedAction();
         cursorMovetoPosition( maps(e->pos()) );
         EnsureVisibleCursor();
     }
     update();
 }
 
void EditArea::dragEnterEvent(QDragEnterEvent *e)
{
     qDebug() << "------dragEnterEvent--------------------------";
     e->acceptProposedAction();
}

void EditArea::dropEvent(QDropEvent *e)
{
    if (isSelfPlacePaste()) {
     return;
    }
    qDebug() << "------dropEvent--------------------------";
    if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
         e->acceptProposedAction();
         cursorMovetoPosition( maps(e->pos()) );
         const QMimeData *dat = e->mimeData();
          if (dat) {
         insertMimeDataOnCursor(dat);
          }
         
     }
     
    RangeSelection = qMakePair(-1,-1);
    resetClickTimer();
    update();
}

bool EditArea::isSelfPlacePaste()
{
	const int Sx1 = RangeSelection.first - 2;
	const int Sx2 = RangeSelection.second + 2;
	bool goup = false;
	bool godown = false;
	const int foundits = C_cursor.position();
	if (foundits < Sx1 && foundits < Sx2) {
		 /* go up */
		 goup = true;
	}
	if (foundits > Sx1 && foundits > Sx2) {
		 /* go down */
		 godown = true;
	}
	if (goup || godown) {
		return false;
	} else {
		return true;
	}
}


void EditArea::mousePressEvent ( QMouseEvent *e )
{
    qDebug() << "------mousePressEvent--------------------------" << trippleClickTimer.isActive();
    
    if ( e->button() != Qt::LeftButton) {
        QApplication::restoreOverrideCursor();
        return;
    }
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
        if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
            /* text event to doc */
            if (cursorIsFocusIndicator && trippleClickTimer.isActive() && C_cursor.hasSelection()) {
            startDragAction();
            e->setAccepted(true);
            return;
            //////cursorMovetoPosition( maps(e->pos()) );
            } else {
            cursorMovetoPosition( maps(e->pos()) );
            const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
            if (selectionLength < 1) {
            position_selection_start = C_cursor.position();
            qDebug() << "------set selection start --" << position_selection_start;
            }
            
            }
        }
    }
    //////
    e->setAccepted(isaccept);
}

void EditArea::mouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() != Qt::LeftButton) {
        QApplication::restoreOverrideCursor();
        return;
    }
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
        if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
            /* text event to doc */
            textDoubleClickEvent(maps(e->pos()));
        }
    }
    //////QApplication::restoreOverrideCursor();
    e->setAccepted(isaccept);
}


void EditArea::mouseMoveEvent ( QMouseEvent *e )
{
    qDebug() << "------mouseMoveEvent--------------------------";
    
    
    if ( e->button() != Qt::LeftButton) {
        ////////return;
    }
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
        if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
            /* text event to doc */
            textMoveEvent(maps(e->pos()));
        }
    }
    //////QApplication::restoreOverrideCursor();
    e->setAccepted(isaccept);

}



void EditArea::resizeEvent(QResizeEvent *e)
{
    adjustScrollbars();
}

void EditArea::keyPressEvent ( QKeyEvent * e )
{
    controller_keyPressEvent(e);
}





void EditArea::wheelEvent (QWheelEvent * event)
{
    qreal docksscale = scaleFaktor;
    if (event->delta() > 1) {
        zoomOut();
    } else {
        zoomIn();
    }
}




void EditArea::contextMenuEvent(QContextMenuEvent *event)
{

    cursorCheck();

    if (isOnPage(_doc->boundingRect(),maps(event->pos()),scaleFaktor)) {

        QMenu *menu = new QMenu(this);
        menu->addAction(tr("Swap page format"),this, SLOT(triggerFormat()));
        menu->addAction(tr("Fit to Window"),this, SLOT(fitToLarge()));
        menu->addAction(tr("Zoom in CTRL++"),this, SLOT(zoomIn()));
        menu->addAction(tr("Zoom out CTRL+-"),this, SLOT(zoomOut()));
        menu->exec(event->globalPos());
        delete menu;

    }
}





void EditArea::adjustScrollbars()
{
    /* portrait - landscape */
    qreal lmax = qMax(_doc->pageSize().width(),_doc->pageSize().height());
    qreal lmin = qMin(_doc->pageSize().width(),_doc->pageSize().height());
    if (portrait_mode) {
        page = QRectF(0,0,lmin,lmax);
        _doc->setFormat(PDocument::PPortrait);
    } else {
        page = QRectF(0,0,lmax,lmin);
        _doc->setFormat(PDocument::PLandscape);
    }
    const qreal left_slide = SLIDERMARGIN_TICK_TOTAL;
    workArea = QSize(page.width() * scaleFaktor , page.height() * scaleFaktor);
    bool stayinwi =  workArea.width() < viewport()->width() ? true : false;
    bool stayinhi =  workArea.height() < viewport()->height() ? true : false;
    border_wi = left_slide;
    qreal border_hi = 0.1;
    if ( stayinwi ) {
        border_wi = qMax( left_slide ,qAbs((qreal)(workArea.width()  / 2) - (viewport()->width() / 2)));
    } else {
        border_hi = left_slide;
    }
    mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL + PAPERSPACE); ////// ultimo da sopra penultimo da sinistra
    QSize viewPanelSize = viewport()->size();
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    verticalScrollBar()->setPageStep(workArea.height());  /* zoom state */
    horizontalScrollBar()->setPageStep(workArea.width()); /* zoom state */
    verticalScrollBar()->setRange(0, ( page.height() * scaleFaktor) - viewPanelSize.height() + SLIDERMARGIN_TICK_TOTAL * 3);
    horizontalScrollBar()->setRange(0, ( page.width() * scaleFaktor) - viewPanelSize.width()  + SLIDERMARGIN_TICK_TOTAL * 3);
    /////////////cursorCheck();
}

void EditArea::verticalValue( const int index )
{
    update();
}

/* ####################################### Public slot  init ###########################################*/

void EditArea::triggerFormat()
{
    portrait_mode = portrait_mode == true ? false : true;
    adjustScrollbars();
    viewport()->update();
}


void EditArea::fitToLarge()
{
    adjustScrollbars();
    const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
    const qreal maxavaiable = viewAreaRect.width() - SLIDERMARGIN_TICK_TOTAL;
    setZoom(maxavaiable / page.width());
}



void EditArea::zoomIn()
{
    setZoom(mcurrent.m11() + SCALING_STEEP);
}

void EditArea::zoomOut()
{
    setZoom(mcurrent.m11() - SCALING_STEEP);
}

void EditArea::setZoom( const qreal value )
{

    if (value < 0.45 || value > 10) {
        return;
    } else {

        qDebug() << "------setZoom--" << value << "------------------------";

        ////qDebug() << "-------------------------------------------------------------------";
        ////qDebug() << "### new setZoom " << value;
        //////qDebug() << "-------------------------------------------------------------------";
        scaleFaktor = value;
        adjustScrollbars();
        viewport()->update();
    }
}


/* ####################################### Public slot  init ###########################################*/


/* ####################################### Translate mouse events init ###################################*/

void EditArea::cursorCheck()
{
    QCursor  *cur =  QApplication::overrideCursor();
    if (!cur) {
        return;
    }
    if (cur->shape() != Qt::ArrowCursor) {
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    }

}

QPointF EditArea::maps( QPointF p )
{
    QTransform transmouse = mcurrent.inverted();
    transmouse.translate (xOffset(),yOffset());  /* scroll bars x,y */
    return transmouse.map(p);
}


QPointF EditArea::slider_maps( QPointF p , bool top )
{
    bool hacks = false;
    QTransform transmouse = mcurrent.inverted();
    if (top) {
        transmouse.translate (xOffset(),SLIDERMARGIN_TICK_TOTAL + SLIDERSPACER);  /* scroll bars x,y */
    } else {
        if (xOffset() == 0) {
            hacks = true;
            transmouse.translate (0,yOffset());  /* scroll bars x,y */
        } else {
            transmouse.translate(SLIDERMARGIN_TICK_TOTAL + SLIDERSPACER ,yOffset());  /* scroll bars x,y */
        }
    }
    QPointF np = transmouse.map(p);
    if (hacks) {
        np.setX ( SLIDERMARGIN_TICK_TOTAL / 2 );
    }
    return np;
}

/* ####################################### Translate mouse events end ###################################*/


/* ####################################### Text api handler  ###################################*/

void EditArea::setBlinkingCursorEnabled( bool enable )
{
    if (enable && QApplication::cursorFlashTime() > 0) {
        cursorTimeLine.start( QApplication::cursorFlashTime() / 2,this);
    }  else {
        cursorTimeLine.stop();
    }
    update();
}

bool EditArea::editEnable()
{
    return cursorTimeLine.isActive();

}

QTextCursor EditArea::textCursor()
{
    return C_cursor;
}

void EditArea::cursorMovetoPosition( const QPointF &pos )
{
    const int cursorPosFozze = _doc->documentLayout()->hitTest(pos,Qt::FuzzyHit);
    if (cursorPosFozze != -1) {
        C_cursor = QTextCursor(_doc);
        C_cursor.setPosition(cursorPosFozze);
        cursortime = true; /* fast display */
        repaintCursor(false);
    }

}

void EditArea::timerEvent(QTimerEvent *event)
{
    if (!editEnable()) {
        return;
    }
    /* activate undo redo only at the first blink cursor */
    if (!_doc->isUndoRedoEnabled()) {
        _doc->setUndoRedoEnabled(true);
    }
    if (event->timerId() == cursorTimeLine.timerId()) {
    /////qDebug() << "------timer -" << event->timerId() << " xx";
    cursortime = cursortime == true ? false : true;
    repaintCursor(false);
    }
}

void EditArea::repaintCursor( bool allrect )
{
    if (allrect) {
        update();
        return;
    } else {

        if (C_cursor.hasComplexSelection() && C_cursor.currentTable()) {
            QTextTable *table = C_cursor.currentTable();
            QRectF tablerect = _doc->documentLayout()->frameBoundingRect(table);
            update(tablerect);
            return;
        }

        const  QRectF paragraphrect = CurrentBlockRect();
        
        if (cursorIsFocusIndicator) {
           update(); 
           return;            
        }
        

        if (paragraphrect.isValid()) {
            update(paragraphrect);
        } else {
            update();
        }

    }

}

QRectF EditArea::CurrentBlockRect()
{
    return _doc->documentLayout()->blockBoundingRect(textCursor().block());
}

void EditArea::paintEditPage( const int index  , QPainter * painter  , const QRectF viewarea )
{
    ////////const QPointF topleft = pageIndexTopLeft(index);
    QAbstractTextDocumentLayout::PaintContext CTX;
    CTX.palette.setColor(QPalette::Text, Qt::black);
    const QRectF body = QRectF(viewarea.topLeft().x(), viewarea.topLeft().y() ,_doc->size().width(),_doc->size().height()); /* on view */
    QRectF view(0, index * body.height(), body.width(), body.height() );   /* on doc */
    painter->save();
    painter->translate(body.topLeft());
    painter->setClipRect(view);
    CTX.clip = view;
    CTX.palette.setColor(QPalette::Text, Qt::black);
    CTX.palette.setColor(QPalette::Highlight,Qt::red);
    CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
    
    if (editEnable() ) {
        
        if (cursortime) {
        CTX.cursorPosition = textCursor().position();
        }
        
        if ( textCursor().hasSelection()) {
            cursorIsFocusIndicator  = true;
            QAbstractTextDocumentLayout::Selection Internal_selection;
            Internal_selection.cursor = textCursor();
            QPalette::ColorGroup cg = cursorIsFocusIndicator ? QPalette::Active : QPalette::Inactive;
            Internal_selection.format.setBackground(CTX.palette.brush(cg, QPalette::Highlight));
            Internal_selection.format.setForeground(CTX.palette.brush(cg, QPalette::HighlightedText));
            Internal_selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            CTX.selections.append(Internal_selection);
        } else {
           cursorIsFocusIndicator  = false;
        }
    } else {
    CTX.cursorPosition = -1;
    }
    _doc->documentLayout()->draw(painter,CTX);
    painter->restore();
}


QTextLine EditArea::currentTextLine(const QTextCursor &cursor)
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

QColor EditArea::HightlightColor() const
{
    QColor BackHightlight("#0072ab");
    BackHightlight.setAlpha(180);
    return BackHightlight;
}

/* swap Insert key */
void EditArea::swapOverwriteMode()
{
    overwriteMode = overwriteMode == false ? true : false;
    QApplication::beep();
}

QTextTableCell EditArea::cellOnPosition( const int posi )
{
    if (posi != -1) {
        QTextCursor tmpcursor(_doc);
        tmpcursor.setPosition(posi);
        if ( !tmpcursor.currentTable() ) {
            return QTextTableCell();
        }
        return tmpcursor.currentTable()->cellAt(tmpcursor);
    }  else {
        return QTextTableCell();
    }
}

void EditArea::gotoNextTableCell()
{
    QTextTable *table = textCursor().currentTable();
    if (!table) {
        return;
    }
    QTextTableCell cell = cellOnPosition(textCursor().position());

    int newColumn = cell.column() + cell.columnSpan();
    int newRow = cell.row();

    if (newColumn >= table->columns()) {
        newColumn = 0;
        ++newRow;
        if (newRow >= table->rows())
            table->insertRows(table->rows(), 1);
    }

    cell = table->cellAt(newRow, newColumn);
    const int moveto  = cell.firstCursorPosition().position();
    C_cursor.setPosition(moveto);
    cursortime = true; /* fast display */
}

void EditArea::gotoPreviousTableCell()
{
    QTextTable *table = textCursor().currentTable();
    if (!table) {
        return;
    }
    QTextTableCell cell = cellOnPosition(textCursor().position());
    int newColumn = cell.column() - 1;
    int newRow = cell.row();
    if (newColumn < 0) {
        newColumn = table->columns() - 1;
        --newRow;
        if (newRow < 0)
            return;
    }
    cell = table->cellAt(newRow, newColumn);
    const int moveto  = cell.firstCursorPosition().position();
    C_cursor.setPosition(moveto);
    cursortime = true; /* fast display */
}

void EditArea::selectAll()
{
    if (position_selection_start == C_cursor.anchor()) {
        return;
    }

    C_cursor.select(QTextCursor::Document);
    position_selection_start = C_cursor.anchor();
    update();
}

void EditArea::paste()
{
    if (!editEnable()) {
        return;
    }
    insertMimeDataOnCursor(clipboard->mimeData());
    update();
    resetClickTimer();
}

void EditArea::copy()
{
    if (!C_cursor.hasSelection()) {
        QApplication::beep();
        return;
    }
    QMimeData *data = createMimeDataFromSelection();
    clipboard->setMimeData(data);
    update();
}

void EditArea::cut()
{
    if (!editEnable()) {
        return;
    }
    if (!C_cursor.hasSelection()) {
        QApplication::beep();
        return;
    }
    copy();
    C_cursor.removeSelectedText();
    update();
}

void EditArea::undo()
{
    if (!editEnable()) {
        return;
    }

    _doc->undo();
    update();
}

void EditArea::redo()
{
    if (!editEnable()) {
        return;
    }
    _doc->redo();
    update();
}

void EditArea::EnsureVisibleCursor()
{
    const QRectF  blockrect = CurrentBlockRect();
    qDebug() << "### EnsureVisibleCursor  " << blockrect.topLeft().y();
}

void EditArea::resetClickTimer()
{
	 if (trippleClickTimer.isActive()) {
	 trippleClickTimer.stop();
    }
	
}

/* ####################################### Text api handler  ###################################*/

/* ####################################### Text api event handler  ###################################*/

void EditArea::textDoubleClickEvent( const  QPointF point )
{
    resetClickTimer();
    const QTextCursor oldSelection = C_cursor;
    const int cursorPosFozze = _doc->documentLayout()->hitTest(point,Qt::FuzzyHit);
    cursorMovetoPosition(point);
	position_selection_start = -1;
	C_cursor.clearSelection();
    QTextLine line = currentTextLine(C_cursor);
	bool doEmit = false;
	if (line.isValid() && line.textLength()) {
	C_cursor.select(QTextCursor::WordUnderCursor);
	doEmit = true;
	}
    trippleClickTimer.start(qApp->doubleClickInterval(),this);
	if (doEmit) {
	repaintCursor();
	}
}

void EditArea::textMoveEvent( const QPointF point  )  
{
    const int cursorPosFozze = _doc->documentLayout()->hitTest(point,Qt::FuzzyHit);
    const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
    if (cursorPosFozze >= INT_MAX || cursorPosFozze ==  -1) {
    return;
    }
    
    bool is_select_items = textCursor().hasSelection();
    
    qDebug() << "### textMoveEvent  ->" << cursorPosFozze << " sel-lengh -> "<< selectionLength << is_select_items;
    const int stopat = qMax(position_selection_start,cursorPosFozze); 
	const int startat = qMin(position_selection_start,cursorPosFozze);
    
    
    if (selectionLength > 0) {
    /* having selection */
	RangeSelection = qMakePair(C_cursor.position(),C_cursor.anchor());
	} else {
    /* no selection set a new one ?? must have one from press event */
    /////C_cursor.setPosition(cursorPosFozze);
    ////////position_selection_start = cursorPosFozze;
    }
    
    if (position_selection_start >= 0 && cursorPosFozze >= 0 && !C_cursor.currentTable()) {
        
        if (stopat == cursorPosFozze) {
			 /* move >>>>>>>>>>>>>>>>>>>>>>>>>  */
			             C_cursor.setPosition(startat);
						 for (int i = startat; i < cursorPosFozze; ++i) {
						 C_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
						 }
		 }  else {
			 
			              C_cursor.setPosition(stopat);
                          const int diffs = stopat - cursorPosFozze;
			              for (int i = 0; i < diffs; ++i) {
                          C_cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
                          }
		 }
        
         if (C_cursor.hasSelection()) {
         trippleClickTimer.start(qApp->doubleClickInterval(),this);
         }
         
         repaintCursor();
         return;
    }
    
    if ( position_selection_start >= 0 && C_cursor.currentTable() ) {
			QTextTable *table = C_cursor.currentTable();
			QTextTableCell firstcell = cellOnPosition(startat);
			QTextTableCell lastcell = cellOnPosition(stopat);
			if ( firstcell.isValid() && lastcell.isValid() ) {
			int fcellrow = firstcell.row();
			int fcellcool = firstcell.column();
			int numRows = qBound(1,lastcell.row() - fcellrow,table->rows());
			int numColumns = qBound(1,lastcell.column() - fcellcool,table->columns());
			C_cursor.selectedTableCells(&fcellrow,&numRows,&fcellcool,&numColumns);
			C_cursor.setPosition(firstcell.firstPosition());
			C_cursor.setPosition(lastcell.lastPosition(), QTextCursor::KeepAnchor);
			}
            
            if (C_cursor.hasSelection()) {
            trippleClickTimer.start(qApp->doubleClickInterval(),this);
            }
         
            repaintCursor(true);
            return;
    }
    
}


void EditArea::controller_keyPressEvent ( QKeyEvent * e )
{
    //////////qDebug() << "### Controller_keyPressEvent  " << e->text() << e->key();
    resetClickTimer();
    cursortime = false;

    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S) {
        return;
    }
    if ( e->key() == Qt::Key_Insert) {
        swapOverwriteMode();
        e->accept();
        return;
    }
    if ( e->key() == Qt::Key_ScrollLock ) {
        ////////SwapCursorMode();
        e->accept();
        return;
    }
    if (C_cursor.currentTable() && e->key() == Qt::Key_Tab || C_cursor.currentTable() && e->key() == Qt::Key_Right ) {
        gotoNextTableCell();
        e->accept();
        return;
    }
    if (C_cursor.currentTable() && e->key() == Qt::Key_Left ) {
        gotoPreviousTableCell();
        e->accept();
        return;
    }

    LastCharFormat = C_cursor.charFormat();

    /* fast access */

    if (e == QKeySequence::SelectAll || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_A) {
        e->accept();
        selectAll();
        return;
    } else if (e == QKeySequence::Copy || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C) {
        e->accept();
        copy();
        return;
    }  else if (e == QKeySequence::Paste || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_V) {
        e->accept();
        paste();
        return;
    } else if (e == QKeySequence::Cut || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C) {
        e->accept();
        cut();
        return;
    }


    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_B) {
        LastCharFormat.setFontWeight(!C_cursor.charFormat().font().bold()  ? QFont::Bold : QFont::Normal);
        C_cursor.setCharFormat(LastCharFormat);
        e->accept();
        return;
    }
    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_I) {
        LastCharFormat.setFontItalic(!LastCharFormat.font().italic() ? true : false );
        C_cursor.setCharFormat(LastCharFormat);
        e->accept();
        return;
    }

    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_J) {
        ////////InsertImageonCursor();
        e->accept();
        return;
    }

    if ((e->modifiers() & Qt::AltModifier) && e->key() == Qt::Key_S) {
        ////////// showhtml();
        return;
    }


    if ( e == QKeySequence::ZoomIn) {
        zoomIn();
    }
    if ( e == QKeySequence::ZoomOut) {
        zoomOut();
    }



    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_U) {
        LastCharFormat.setUnderlineStyle(!LastCharFormat.font().underline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline );
        C_cursor.setCharFormat(LastCharFormat);
        e->accept();
        return;
    }
    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_H) {
        C_cursor.insertFragment(QTextDocumentFragment::fromHtml("<hr>"));
        e->accept();
        return;
    }
    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Z || e == QKeySequence::Undo) {
        e->accept();
        undo();
        return;
    }
    if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Y || e == QKeySequence::Redo ) {
        e->accept();
        redo();
        return;
    }


    //////////////return;

    /* move action on cursor ? */
    if (cursorMoveKeyEvent(e)) {
        e->accept();
        repaintCursor();
        EnsureVisibleCursor();
        return;
    }

    /* fast access end  */

    if (e->key() == Qt::Key_Backspace && !(e->modifiers() & ~Qt::ShiftModifier)) {

        QTextBlockFormat blockFmt = C_cursor.blockFormat();

        if (blockFmt.bottomMargin() == 12) {
            blockFmt.setBottomMargin(0);
        }

        if (blockFmt.topMargin() == 12) {
            blockFmt.setTopMargin(0);
        }



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
    }  else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {

        QTextBlockFormat blockFmt = C_cursor.blockFormat();


        if (e->modifiers() & Qt::ControlModifier) {
            /*  no br !  */
            C_cursor.insertBlock();
        } else {
            C_cursor.insertBlock();   /* default format can take from setting */
        }
        /* leave default qt margin */
        if (blockFmt.bottomMargin() == 12) {
            blockFmt.setBottomMargin(0);
        }
        if (blockFmt.topMargin() == 12) {
            blockFmt.setTopMargin(0);
        }
        blockFmt.setIndent(blockFmt.indent() - 1);
        C_cursor.setBlockFormat(blockFmt);
        e->accept();
        goto accept;
    } else if (e == QKeySequence::Delete) {

        if (C_cursor.hasSelection()) {
            QString remtxt = C_cursor.selectedText();
            for (int i = 0; i < remtxt.size(); ++i) {
                C_cursor.deleteChar();
            }
            ////////////SwapSelectionsCursor();  /* clear */
        } else {
            C_cursor.deleteChar();
        }

        goto accept;
    } else if (e == QKeySequence::DeleteEndOfWord) {
        C_cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
        goto accept;
    } else if (e == QKeySequence::DeleteStartOfWord) {
        C_cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
        goto accept;
    }
    else if (e == QKeySequence::DeleteEndOfLine) {
        QTextBlock block = C_cursor.block();
        if (C_cursor.position() == block.position() + block.length() - 2) {
            C_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        } else {
            C_cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            C_cursor.deleteChar();
        }
        goto accept;
    }  else {
        /* go insert text ......................*/
        goto process;
    }



process:
    {
        QString text = e->text();
        if (!text.isEmpty() && (text.at(0).isPrint() || text.at(0) == QLatin1Char('\t'))) {

            if (getoverwriteMode() && !C_cursor.hasSelection() && !C_cursor.atBlockEnd()) {
                C_cursor.deleteChar();
                repaintCursor();
            }

            C_cursor.insertText(text);
            e->accept();


            return;

        } else if (!text.isEmpty() && C_cursor.hasSelection() ) {
            QString remove = C_cursor.selectedText();
            for (int i = 0; i < remove.size(); ++i) {
                C_cursor.deletePreviousChar();
            }

            C_cursor.insertText(text);
            

            return;

        }  else {
            e->ignore();
            QApplication::beep();
            return;
        }
        
        repaintCursor();

    }



    /* ########## section accept ################*/
accept:
    {
        repaintCursor();
        /* ########## section accept ################*/
    }


}






bool EditArea::cursorMoveKeyEvent(QKeyEvent *e)
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
    repaintCursor(true);
    const bool moved = C_cursor.movePosition(op,QTextCursor::MoveAnchor);
    if (moved) {
        if (C_cursor.position() != oldCursorPos) {
            repaintCursor();
        }

    }
    return true;
}


/* ####################################### Text api event handler  ###################################*/
void EditArea::insertImage( const  QImage image )
{
    if (image.width() > _doc->size().width()) {
     return;
    }
    if (image.isNull()) {
     return;
    }
    
    QDateTime timer1( QDateTime::currentDateTime() );
    const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
    _doc->addResource(QTextDocument::ImageResource,QUrl(TimestampsMs),image); 
    
        QTextImageFormat format;
        format.setName( TimestampsMs );
        format.setHeight ( image.height() );
        format.setWidth ( image.width() );
        textCursor().insertImage( format );
}

void EditArea::insertImage( const QPixmap image )
{
    if (image.width() > _doc->size().width()) {
     return;
    }
    if (image.isNull()) {
     return;
    }
    
    QDateTime timer1( QDateTime::currentDateTime() );
    const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
    _doc->addResource(QTextDocument::ImageResource,QUrl(TimestampsMs),image.toImage()); 
    
        QTextImageFormat format;
        format.setName( TimestampsMs );
        format.setHeight ( image.height() );
        format.setWidth ( image.width() );
        textCursor().insertImage( format );
}


void EditArea::insertMimeDataOnCursor( const QMimeData *md )
{
    
    if (isSelfPlacePaste()) {
     return;
    }
    
    
    
	QTextDocumentFragment fragment;
	resetClickTimer();
    if ( md->hasUrls() )  {
          QList<QUrl> urls = md->urls();
          for ( int i = 0; i < urls.size(); ++i ) { 
              QUrl gettyurl(urls.at(i));
              if (gettyurl.scheme() == "file") {
                  
                  QImage pic(gettyurl.toLocalFile());
                  if (!pic.isNull()) {
                  insertImage(pic); 
                  } else {
                    /* search if html file or document */
                      
                  }
                  
              } else if (gettyurl.scheme() == "http") {
                  /* fill net cache */
              }
              
          }		
        C_cursor.clearSelection();
        repaintCursor(true);
        return;          
    }
    
    if ( md->hasImage() ) {
    QImage image = qvariant_cast<QImage>(md->imageData());
        if (!image.isNull()) {
        insertImage(image);
        }
    C_cursor.clearSelection();
    repaintCursor(true);
    return;
    }
    
    QString cosa;
    bool hasData;
		
    if (md->hasFormat(QLatin1String("application/x-qrichtext")) ) {
        cosa = md->data(QLatin1String("application/x-qrichtext"));
        fragment = QTextDocumentFragment::fromHtml(QString::fromUtf8(md->data(QLatin1String("application/x-qrichtext"))));
        hasData = true;
    } else if (md->formats().contains("text/html")) {
        cosa = QString::fromUtf8(md->data(QLatin1String("text/html")));
        fragment = QTextDocumentFragment::fromHtml(md->html());
        C_cursor.insertFragment(fragment);
        C_cursor.clearSelection();
        repaintCursor(true);
        EnsureVisibleCursor();
        return;
    } else if (md->formats().contains("text/plain")) {
        cosa = QString::fromUtf8(md->data(QLatin1String("text/plain")));
        fragment = QTextDocumentFragment::fromPlainText(cosa);
        hasData = true;
    }
    
    if (hasData && cosa.size() > 0 ) {
        C_cursor.insertFragment(fragment);
        C_cursor.clearSelection();
        repaintCursor(true);
        EnsureVisibleCursor();
    } else {
			QApplication::beep();
    }
}

QMimeData *EditArea::createMimeDataFromSelection()
{
	 QTextCharFormat base = C_cursor.charFormat();
	 QString txt;
	
	 if (C_cursor.hasSelection()) {
		 txt = C_cursor.selectedText();
	 }
	 QTextImageFormat pico = base.toImageFormat(); 
	 if (pico.isValid()) {
         QImage image;
         const QString picname = pico.name();
         const QVariant data = _doc->resource(QTextDocument::ImageResource,QUrl::fromEncoded(picname.toUtf8()));
         if (data.type() == QVariant::Image) {
            image = qvariant_cast<QImage>(data);
         } else if (data.type() == QVariant::ByteArray) {
            image.loadFromData(data.toByteArray());
         }
         if (!image.isNull()) {
            QMimeData *xm = new QMimeData();
            xm->setImageData(image);
            return xm;
         }          
	 }
	 const QTextDocumentFragment fragment(C_cursor);
	 if ( fragment.isEmpty() && txt.size() > 0 ) {
		QMimeData *xm = new QMimeData();
		xm->setText(txt);
		return xm;
	 } else if (fragment.isEmpty() && txt.isEmpty() ) {
		QMimeData *xm = new QMimeData();
		xm->setText(QString("...Selection...Error..."));
		return xm;
	} else {
	return new QTextEditMimeData(fragment);
	}
}













