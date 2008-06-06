#ifndef CMDMOVEFOOBJECT_H
#define CMDMOVEFOOBJECT_H

#include <QUndoCommand>
#include <QPoint>

class QPoint;
class ObjectFoDoc;

/**
 *  Class of QUndoCommand pattern, provide move formating object on workplace
 */
class CmdMoveFoObject : public QUndoCommand
{
public:
	CmdMoveFoObject(QPoint oldPos,QPoint newPos,ObjectFoDoc *pObjectFoDoc);
	~CmdMoveFoObject();
	void undo();
	void redo();
private:
	QPoint m_oldPos;
	QPoint m_newPos;
	ObjectFoDoc *m_pObjectFoDoc;
};

#endif

///todo: move from one object to another (e.g. from one page to second page) so we need handle oldparent and newparent in the future
