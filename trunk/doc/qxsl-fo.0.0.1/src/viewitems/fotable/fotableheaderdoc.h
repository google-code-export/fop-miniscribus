#ifndef FOTABLEHEADERDOC_H
#define FOTABLEHEADERDOC_H

#include "nodefo.h"

/**
 * represent fo formating object fo:tableheader
 * not implement yet
 */
class FoTableHeaderDoc : public NodeFo
{
public:
	FoTableHeaderDoc(QDomElement pElement);
	~FoTableHeaderDoc();
};

#endif
