#ifndef FOPAGEVIEWMANAGER_H
#define FOPAGEVIEWMANAGER_H

#include <QObject>
#include <vector>

class PageView;

/**
 * Manager of view part of pages 
 */
class PageViewManager : public QObject
{
Q_OBJECT
public:
	PageViewManager(QObject *parent = 0);
	~PageViewManager();
	/**
	 * add page to manager
	 * @param pageView 
	 */
	void AddPageView(PageView *pageView);
private:
	/**
	 * add page to manager
	 * @param pageView 
	 */
	std::vector<PageView *>m_vecPageView;
};

#endif

///todo:curently we don't need this class 
