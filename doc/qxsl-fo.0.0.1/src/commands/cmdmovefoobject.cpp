//own include
#include "cmdmovefoobject.h"

//qt include
#include <QPoint>

//project includes
#include "objectfodoc.h"

CmdMoveFoObject::CmdMoveFoObject(QPoint oldPos,QPoint newPos,ObjectFoDoc *pObjectFoDoc)
 : QUndoCommand(),m_oldPos(oldPos),m_newPos(newPos),m_pObjectFoDoc(pObjectFoDoc)
{
	
}


CmdMoveFoObject::~CmdMoveFoObject()
{
	
}


void CmdMoveFoObject::undo()
{
	m_pObjectFoDoc->moveTo(m_oldPos);
}

void CmdMoveFoObject::redo()
{
	m_pObjectFoDoc->moveTo(m_newPos);
	QString objectFoType=QString(ObjectFoDoc::typeToString(m_pObjectFoDoc->type()));
	setText(QObject::tr("Item move [%1]").arg(objectFoType));
}
