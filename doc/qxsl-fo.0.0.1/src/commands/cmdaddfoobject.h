#ifndef CMDADDFOOBJECT_H
#define CMDADDFOOBJECT_H

#include <QUndoCommand>

class ObjectFoContainerView;
class ObjectFoDoc;

/**
 * Class of QUndoCommand pattern, provide add formating object to workplace
 */
class CmdAddFoObject : public QUndoCommand
{
public:
	CmdAddFoObject();
	CmdAddFoObject(ObjectFoDoc *pObjectFoDoc,ObjectFoContainerView *pFoObjectView);
	void undo();
	void redo();
private:
	ObjectFoDoc *m_pObjectFoDoc;
	ObjectFoContainerView *m_pFoContainerView;
};

#endif // COMMANDS_H

///todo: remove dependency on view
