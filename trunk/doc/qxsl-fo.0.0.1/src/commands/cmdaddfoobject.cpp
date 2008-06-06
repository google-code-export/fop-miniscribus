//own include
#include "cmdaddfoobject.h"

//qt include
#include <QUndoCommand>

//project include
#include "objectfocontainerview.h"
#include "objectfodoc.h"


CmdAddFoObject::CmdAddFoObject()
: QUndoCommand()
{
}

CmdAddFoObject::CmdAddFoObject(ObjectFoDoc *pObjectFoDoc,ObjectFoContainerView *pFoContainerView)
: QUndoCommand()
{
	//child's foobject
	m_pObjectFoDoc=pObjectFoDoc;
	//parent's view
	m_pFoContainerView=pFoContainerView;
}

void CmdAddFoObject::undo()
{
	m_pFoContainerView->removeItemFromViewContainer(m_pObjectFoDoc);
}

void CmdAddFoObject::redo()
{
	m_pFoContainerView->slAddItemToViewContainer(m_pObjectFoDoc);
	QString objectFoType=QString(ObjectFoDoc::typeToString(m_pObjectFoDoc->type()));
	setText(QObject::tr("Item add [%1]").arg(objectFoType));
}

