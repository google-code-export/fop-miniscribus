#include "helpers.h"

WorkPanel::WorkPanel( QWidget * parent )
 : QWorkspace(parent)
{
    
}

void WorkPanel::paintEvent(QPaintEvent *e) 
{
		QPainter painter(this);
		painter.setPen ( Qt::NoPen );
    painter.setBrush ( QBrush(Qt::white));
		painter.drawRect(rect());
}
