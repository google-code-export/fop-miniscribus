
2.9.2011 updated to build un ubuntu linux 
on news xslt & xml libs + curl



..................................worknote snip 

actualdbname = dbi->databaseName();
    
            QSqlQuery query(queri,dbi);
            /////query.exec(queri);
            QSqlRecord rec = query.record();
            int sumcol = rec.count();
            int sumrow = query.numRowsAffected();
    
        em = Qxml();
        em.SetstreamFile(FILE_CACHEDIR);
        QDomElement root = em.createElement("odbc_root");
        root.setAttribute ("build",UmanTimeFromUnix(QTime_Null()));
        root.setAttribute ("import_from",actualdbname);
        em.appendChild( root );
        QDomElement queril = em.createElement( "query" );
        queril.setAttribute ("name",queri);
        queril.setAttribute ("sumrow",QString::number(sumrow));
        queril.setAttribute ("sumcool",QString::number(sumcol));
        queril.setAttribute ("user",autor);
        root.appendChild(queril);
        /* ################################################################################### */
                    int recnum = 0 - 1;
                    while (query.next()) { 
                    QDomElement line = em.createElement( "line" );
                    recnum++;
                             for(int i=0;i<sumcol;i++){
                             QString value = query.value(i).toString();
                             QString fname = rec.fieldName(i);
                             qDebug() << "### fieldname "  << fname;
                             line.setAttribute ("fieldname",fname);
                             line.setAttribute ("value",value);
                             line.setAttribute ("id",recnum);
                             
                             }
                    queril.appendChild(line);
                    }  
            
    
    
       em.saveXML(FILE_CACHEDIR);
     QFile f(FILE_CACHEDIR);              
          if ( f.exists()  and f.size() > 0) {
             return true; 
          } else {
              return false;
          }








void  Make_Arianna()  
{
    
     
}






   Qxml em = Qxml();
        em.SetstreamFile(FILE_CACHEDIR);
        QDomElement root = em.createElement("odbc_root");
        root.setAttribute ("build",UmanTimeFromUnix(QTime_Null()));
        root.setAttribute ("export_from",QString(setter.value("ODBBC_Name").toString()));
        em.appendChild( root );
        QDomElement queril = em.createElement( "query" );
        queril.setAttribute ("name",qx);
        queril.setAttribute ("sumrow",QString::number(sumrow));
        queril.setAttribute ("sumcool",QString::number(sumcol));
        queril.setAttribute ("user",userd);
        root.appendChild(queril);
        
                    while (query.next()) { 
                    recnum++;
                    QDomElement lines = em.createElement( "line" );
                    lines.setAttribute ("id",recnum);
                    queril.appendChild(lines);
                    
                             for(int i=0;i<sumcol;i++){
                             QDomElement line = em.createElement( "row" );
                             QString value = query.value(i).toString();
                             QString fname = rec.fieldName(i);
                             ///////qDebug() << "### fieldname "  << fname;
                             line.setAttribute ("fieldname",fname);
                             line.setAttribute ("value",value);
                                 if (is_numeric(value)) {
                                    line.setAttribute ("digit","1"); 
                                 } else {
                                     line.setAttribute ("digit","0");
                                 }
                             line.setAttribute ("id",i);
                             //////qDebug() << "### recnum "  << recnum;
                             lines.appendChild(line);
                             }
                    
                    } 
                    
        ///////em.saveXML(FILE_CACHEDIR);    
        ///////em.Print();




















info@schweiz-reg.ch
