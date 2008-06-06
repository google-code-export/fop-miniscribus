
//own include
#include "cmdresizefoobject.h"

//qt includes

//project include
#include "objectfodoc.h"

CmdResizeFoObject::CmdResizeFoObject(QPoint oldSize,QPoint newSize,ObjectFoDoc *pObjectFoDoc)
 : QUndoCommand(),m_newSize(newSize),m_oldSize(oldSize),m_pObjectFoDoc(pObjectFoDoc)
{
	
}

CmdResizeFoObject::~CmdResizeFoObject()
{
	
}

void CmdResizeFoObject::undo()
{
	m_pObjectFoDoc->resize(m_oldSize);
}

void CmdResizeFoObject::redo()
{
	m_pObjectFoDoc->resize(m_newSize);
	QString objectFoType=QString(ObjectFoDoc::typeToString(m_pObjectFoDoc->type()));
	setText(QObject::tr("Item resize [%1]").arg(objectFoType));
}
