#include "codeview.h"

CodeView::CodeView()
 : QTextEdit()
{	
	//edit text feature is not supperted yet :)
	setReadOnly(true);
}


CodeView::~CodeView()
{
	
}

void CodeView::load(QString foData)
{
	clear();
	setPlainText(foData);
}
