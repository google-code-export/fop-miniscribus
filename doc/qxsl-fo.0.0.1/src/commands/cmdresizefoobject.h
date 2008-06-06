#ifndef CMDRESIZEFOOBJECT_H
#define CMDRESIZEFOOBJECT_H

#include <QUndoCommand>
#include <QPoint>

class ObjectFoDoc;

/**
 *  Class of QUndoCommand pattern, provide resize of formating object
 */
class CmdResizeFoObject : public QUndoCommand
{
public:
	CmdResizeFoObject(QPoint oldSize,QPoint newSize,ObjectFoDoc *pObjectFoDoc);
	~CmdResizeFoObject();
	void undo();
	void redo();
private:
	QPoint m_newSize;
	QPoint m_oldSize;
	ObjectFoDoc *m_pObjectFoDoc;
};

#endif
