#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

/**
 * encapsulate QGraphicsView and compute scrollbar values for ruler(widget on left and top part mainview)
 */

class GraphicsView : public QGraphicsView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param scene 
	 */
	GraphicsView(QGraphicsScene * scene);
	/**
	 * destructor
	 */
	~GraphicsView();
	/**
	 * scale QGraphicsView
	 * @param sx 
	 * @param sy 
	 */
	void scale ( qreal sx, qreal sy );	
	/**
	 * return current scrollbar value according to scale
	 * @param scale 
	 * @return 
	 */
	int verticalScrollBarValue(qreal scale);
	/**
	 * return current scrollbar value according to scale
	 * @param scale 
	 * @return 
	 */
	int horizontalScrollBarValue(qreal scale);
};

#endif
