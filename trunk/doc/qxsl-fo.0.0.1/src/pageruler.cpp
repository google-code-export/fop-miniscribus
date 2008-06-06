//own include
#include "pageruler.h"

//qt include
#include <QGraphicsRectItem>
#include <QLabel>

//project include
#include "pageview.h"
#include "mainview.h"

PageRuler::PageRuler(ERulerType position,MainView *pMainView)
 : QWidget(),m_pMainView(pMainView),m_position(position)
{
	setAutoFillBackground(true);

	QPalette pal = palette();
	pal.setColor(QPalette::Window,QColor(Qt::white));
	setPalette( pal );

	if (position==leftPosition)
	{
		//set width for left ruler
		setFixedWidth(10);
	}
	else if (position==topPostion)
	{
		//set height for top ruler
		setFixedHeight(10);
	}

	createCursors();
	
}

PageRuler::~PageRuler()
{
}

//this could be changes to variable count of cursors
void PageRuler::createCursors()
{
	for(int i=0;i<4;i++)
	{
		QLabel *tmp= new QLabel(this);
		m_pCursor[i]=tmp;
		m_pCursor[i]->setFrameStyle(QFrame::Panel | QFrame::Plain);
		m_pCursor[i]->setFixedHeight(10);
		m_pCursor[i]->setFixedWidth(10);
		m_pCursor[i]->show();
		m_pCursor[i]->move(-10,-10);
	}
}

void PageRuler::setRulerPosition(QGraphicsRectItem *pQGraphicsRect)
{
	QRectF rect=pQGraphicsRect->boundingRect();

	QPointF point = pQGraphicsRect->scenePos();

	QPointF scale = m_pMainView->scale();
	
	int verticalScrollBarValue=m_pMainView->verticalScrollBarValue(scale.y());
	int horizontalScrollBarValue=m_pMainView->horizontalScrollBarValue(scale.x());
	
	int xOffset=point.x()+rect.x();
	int yOffset=point.y()+rect.y();
	
	if (m_position==leftPosition)
	{
 		m_pCursor[0]->move(0,(yOffset)*scale.y() - verticalScrollBarValue);
 		m_pCursor[1]->move(0,(yOffset + (int)rect.height())* scale.y() - verticalScrollBarValue);
		
	}
	else if (m_position==topPostion)
	{
 		m_pCursor[2]->move(xOffset * scale.x() - horizontalScrollBarValue,0);
 		m_pCursor[3]->move( (xOffset+(int)rect.width())*scale.x()-horizontalScrollBarValue,0);
	}

}
