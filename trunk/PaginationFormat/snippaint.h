painter->drawRect(lrect);
    painter->save();
    painter->restore();
    
    painter->setBrush(QColor(Qt::red));
    painter->setPen(QPen(Qt::black,2));
    painter->drawRect(pageprint);
    QLineF diagonal(pageprint.topLeft(),pageprint.bottomRight());
    painter->drawLine(diagonal);
    
    
    painter->save();
    