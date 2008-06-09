/*******************************************************************************
 * main.cpp  Layer edit sample 
 On class class OS_application check if other instance of the same app is open
 on linux is possibel to send arguments on socked to actual open instance.
 Tested on Linux and Window
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#include "GraphicsView.h"
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QGraphicsView>
#include "singleapplication.h"


#if defined Q_WS_WIN
#include "windows.h"
#include "winerror.h"
#endif



int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
    a.setOrganizationName("CrossKern");
    a.setOrganizationDomain("crosskern.com");
    a.setApplicationName("Layer Handler Sample");
    const QString titles = QString("Layer edit GraphicsView Demo version (use ContextMenu)");
 
	 QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	 SingleApplication instance(titles);
 
 
	if(instance.isRunning())
	{
   #if defined Q_WS_WIN
   /* focus this QApplication instance from other QApplication*/
   HWND hWnd = FindWindowW( 0, (LPCWSTR) titles.utf16() );
   ShowWindow( hWnd, SW_RESTORE );
   SetForegroundWindow( hWnd );
   #endif
   instance.sendMessage(titles +" Application is already open!\n"
					"PID: " + QString::number(a.applicationPid()));
		return 0;
	}
  
  GraphicsView *panel = new GraphicsView;
  panel->setWindowTitle(titles); 
   
  
	 QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
       panel, SLOT(onOtherInstanceMessage(const QString&)));
  
  
	 panel->showMaximized();

	return a.exec();
}












