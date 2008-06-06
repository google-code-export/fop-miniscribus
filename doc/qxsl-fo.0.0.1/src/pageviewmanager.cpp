//own include
#include "pageviewmanager.h"

//qt includes

//proj includes
#include "pageview.h"
#include "pagedoc.h"

PageViewManager::PageViewManager(QObject *parent)
 : QObject(parent)
{

}


PageViewManager::~PageViewManager()
{
	long size = m_vecPageView.size();
	for (long i = 0; i < size ;i++ )
	{
		if ( m_vecPageView[i] != NULL ) {
			delete m_vecPageView[i];
		}
	}
	m_vecPageView.clear();
}

void PageViewManager::AddPageView(PageView *pageView)
{
	m_vecPageView.push_back(pageView);
}

