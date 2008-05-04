#include "GraphicsView.h"
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QGraphicsView>
/* Unix SOCKET to comunicate on parent apps friend and send file or other msg! */
#define _UNIX_SOCKET_ "/tmp/layeres.sock"


#if defined Q_WS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int  make_local_app_socket()
{
    struct sockaddr_un name;
    int sock;
    size_t size;
    sock = socket (PF_LOCAL, SOCK_DGRAM, 0);
       if (sock < 0)
         {
           return 0;
         }
    name.sun_family = AF_LOCAL;
    strncpy (name.sun_path, _UNIX_SOCKET_, sizeof (name.sun_path));
    name.sun_path[sizeof (name.sun_path) - 1] = '\0';
    size = (offsetof (struct sockaddr_un, sun_path) + strlen (name.sun_path) + 1);

       if (bind (sock, (struct sockaddr *) &name, size) < 0)
         {
           return 0;
         }
    return sock;
}
#endif


/* catch event from QCoreApplication if haveit more */
#define osApplication \
    (static_cast<OS_application*>(QCoreApplication::instance()))
class OS_application : public QApplication
{
    Q_OBJECT
//
public:
 OS_application( int &argc, char **argv )
  : QApplication(argc, argv),unix_sock(0)
{
    windowTitle = QString ( "Layer edit sample" );
    Apps_Open = false;
    const QString single_key = QString("ID 29385129512818123 by %1").arg(windowTitle);
    #if defined Q_WS_WIN
    m_mutex = (void *) CreateMutexW( 0, false, (LPCWSTR) single_key.utf16() );
	  m_prevInstance = ( GetLastError() == ERROR_ALREADY_EXISTS );
    if (m_prevInstance) {
    Apps_Open = true;
    }
    #endif
    #ifdef Q_OS_UNIX
    unix_sock = make_local_app_socket();
    Apps_Open = unix_sock == 0 ?  true :  false;
    qDebug() << "### sock connect res " << Apps_Open << unix_sock;
    #endif
    if (!Apps_Open) {
    MainStart(); 
    } else {
      /* focus parent */ 
      beep();
      beep();
      FocusInstance();
    }  
}

bool focus()
{
  return Apps_Open;
}

/* focus preview process ! */
bool FocusInstance() const
{
  #if defined Q_WS_WIN
	HWND hWnd = FindWindowW( 0, (LPCWSTR) windowTitle.utf16() );
	if( hWnd )
	{
		long id = GetWindowThreadProcessId( hWnd, 0 );
		AttachThreadInput( id, 0, true );
		if ( IsIconic( hWnd ) )
    ShowWindow( hWnd, SW_RESTORE );
		SetForegroundWindow( hWnd );
		AttachThreadInput( id, 0, false );
		return true;
	}
  #endif
  #ifdef Q_OS_UNIX
  QFileInfo sockets(QString(_UNIX_SOCKET_));
  qDebug() << "### having " << sockets.exists();
        if (unix_sock == 0) {
        qDebug() << "### send file to socket parent here... ";
        return true;
        }
  #endif
	return false;
}

~OS_application()
{
  #if defined Q_WS_WIN
	if ( !m_prevInstance ) {
		CloseHandle( (HANDLE) m_mutex );
  }
  #endif
  #ifdef Q_OS_UNIX
  if (unix_sock != 0) {  /* close SOCKET if run ! */
  QFileInfo sockets(QString(_UNIX_SOCKET_));
  QFile fsocket(sockets.absoluteFilePath());
  fsocket.remove();
  }
  #endif
}
/* start main window */
void MainStart()
{	
    /* QSetting setup to access */
    setOrganizationName("CrossKern");
    setOrganizationDomain("crosskern.com");
    setApplicationName("Layer Handler Sample");
    
    panel = new GraphicsView;
    panel->setWindowTitle(windowTitle); 
    panel->showMaximized();
}
protected:
bool event(QEvent *ev)
{
              
    bool eaten;
    switch (ev->type()) {
    case QEvent::FileOpen:
        pending_file = static_cast<QFileOpenEvent *>(ev)->file();
        eaten = true;
        break;
    default:
        eaten = QApplication::event(ev);
        break;
    }
    return eaten;
}
private:
    #if defined Q_WS_WIN
    void *m_mutex;
		bool m_prevInstance;
    #endif
    GraphicsView *panel;
    QString pending_file;
    QString windowTitle;
    int unix_sock;
    bool Apps_Open;
    
};


int main(int argc, char *argv[])
{
    OS_application a(argc, argv);
    if (a.focus()) {
    return 0;
    }
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

#include "main.moc"










