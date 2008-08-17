#include "qtextpaintmashine.h"
#include "panel.h"

#include "qtextpanel.h"
#include "config.h"
#include <QPixmap>
#include <QImage>



ComposeDoc::ComposeDoc( QObject *creator , MountPage p )
  : QThread(0)
{
  page = p;  
  receiver = creator;
  setTerminationEnabled(true);
}


void ComposeDoc::run()
{
    
}












