#include "GraphicsView.h"
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QGraphicsView>

#if defined Q_WS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <sys/socket.h>
#include <errno.h>
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
  : QApplication(argc, argv),Apps_Open(false)
{
    windowTitle = QString ( "Layer edit sample" );
    const QString single_key = QString("ID 29385129512818123 by %1").arg(windowTitle);
    #if defined Q_WS_WIN
    m_mutex = (void *) CreateMutexW( 0, false, (LPCWSTR) single_key.utf16() );
	  m_prevInstance = ( GetLastError() == ERROR_ALREADY_EXISTS );
    if (m_prevInstance) {
    Apps_Open = true;
    }
    #endif
    #ifdef Q_OS_UNIX
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
	  sockaddr addr;
    memset(&addr, 0, sizeof(struct sockaddr));
	  addr.sa_family = AF_UNIX;
    strncpy(addr.sa_data,"/tmp/layeres.sock", sizeof(addr.sa_data) - 1);
	  int res = connect(s, &addr, sizeof(addr));
    qDebug() << "### sock connect res " << res;
    #endif
    /* QSetting setup to access */
    setOrganizationName("CrossKern");
    setOrganizationDomain("crosskern.com");
    setApplicationName("Layer Handler Sample");
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
  QFileInfo sockets("/tmp/layeres.sock");
  return sockets.exists();
  #endif
	return false;
}

~OS_application()
{
	if ( !m_prevInstance ) {
		CloseHandle( (HANDLE) m_mutex );
  }
}
/* start main window */
void MainStart()
{	
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
    GraphicsView *panel;
    QString pending_file;
    QString windowTitle;
    #if defined Q_WS_WIN
    void *m_mutex;
		bool m_prevInstance;
    #endif
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










