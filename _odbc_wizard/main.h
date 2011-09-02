#ifndef MAIN_H
#define MAIN_H


#include "qxml.h"


#define _PROGRAM_NAME_ "ODBC Connection Wizard Browser by PPK-Screendesign/Webprogramm di Peter Hohl"
#define _PROGRAM_NAME_DOMAINE_ "ciz.ch"
#define _ORGANIZATION_NAME_ "PPK-Screendesign/Webprogramm"
#define _PROGRAM_VERSION_ "version 0.9.2"
#define _PROGRAM_OWNER_ "version 0.9.2"
#define _PROGRAM_TITLE_  _PROGRAM_NAME_" - "_PROGRAM_VERSION_
#define _USER_AGENT_CURL_APPS "Mozilla/5.0 (Windows; U; Windows NT 5.1; it-CH; rv:1.7.12) Gecko/20050919 Firefox/1.0.7"
#define Q_WORKS_PEND "open"
#define _PROGRAM_SHORT_NAME "odbc_wizard"

#define WORK_CACHEDIR \
              QString( "%1/.%2/" ).arg( QDir::homePath() ,_PROGRAM_SHORT_NAME)
#define FILE_CACHEDIR \
              QString( "%1_tmp.xml" ).arg( WORK_CACHEDIR )
              
              
#define EXPORT_FIELD_NAME \
              QString( "%1_field_reg.xml" ).arg( WORK_CACHEDIR )          
               
              
              
#define BUG_REPORT \
               QString( "%1/ticket.pdf" ).arg(QDir::currentPath())

#endif