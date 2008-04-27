#include "gmain.h"
//
/*  Save file as gmain.cpp  */
/*  incomming class name Gmain */
//
#include <QCloseEvent>



Gmain::Gmain( QWidget * parent )
	: QMainWindow( parent )
{
	setupUi( this );
	/////tableWidget->
	///////////highlight = new XmlHighlighter(textEdit->document());
	////////highlight_0 = new XmlHighlighter(textEdit_3->document());
	BaseInit();
	//////////qDebug() << "###  xml size " << textEdit->toPlainText().size();
	opendFile(QString::fromUtf8(":/img/toresource.dshttp"));
	
}



void Gmain::on_StreamLocalFile_clicked()
{
	
	QMessageBox::information(0, tr("StreamLocalFile"),tr("Not implemented ... line 27 gmain.cpp\n only send stream file to communicator..."));
	
}


void Gmain::on_SendRequest_clicked()
{
	QString pathrequest,setcontenttype,methodsender;
	HeaderSend.clear();
	QUrl url;
	
	
	qDebug() << "###  new on_SendRequest_clicked " << textEdit->toPlainText().size();
	qDebug() << "###################################################################";
	
	
	const QAbstractItemModel  *model = tableWidget->model();
	     const int summline = model->rowCount();
	        for (int e = 0; e < summline; ++e) { 
						QModelIndex index_0 = model->index(e,0);
						QModelIndex index_1 = model->index(e,1);
						const QString name = model->data(index_0,Qt::DisplayRole).toString();
						const QString value = model->data(index_1,Qt::DisplayRole).toString();
						if (name.size() > 1 && value.size() > 1) {
							HeaderSend.insert(name,value);
						}
					}
					
					if (!usesoapaction->isChecked() && lineEdit_2->text().size() > 1) {
						HeaderSend.insert("SOAPAction",lineEdit_2->text());
					}
					
					
          QUrl gotourl(movetourl->text(),QUrl::StrictMode);
					if (!gotourl.isValid()) {
						QMessageBox::information(0, tr("URL message"),tr("Url not valid!"));
						movetourl->setText("http://"+movetourl->text());
						movetourl->setFocus();  
					return;
					} else {
						
						if (gotourl.host().size() < 1) {
							QMessageBox::information(0, tr("URL message"),tr("Url not valid!"));
              movetourl->setText("http://"+movetourl->text());
							movetourl->setFocus();
					    return;
						}
						url = QUrl(movetourl->text());
						HeaderSend.insert("Host",gotourl.host());
						pathrequest = gotourl.path();
						setcontenttype = comboBox_2->currentText().trimmed();
						
						if (setcontenttype.size() > 2) {
							HeaderSend.insert("Content-Type",setcontenttype);
						}
						
						methodsender = comboBox->currentText().trimmed();
					}
					
					QMapIterator <QString,QString> d(HeaderSend);
                 while (d.hasNext()) {
                       d.next();
									     ////////qDebug() << "### r " << d.key() << "|" << d.value();
                 }
								 
					if (url.port() < 1) {
              url.setPort(80);
					}
					qDebug() << "###  url.port() " << url.port();
					qDebug() << "###  pathrequest " << pathrequest;
	        qDebug() << "###  setcontenttype " << setcontenttype;
	        qDebug() << "###  methodsender " << methodsender;
					
					
					QString errorStr;
					int errorLine, errorColumn;
					QDomDocument doc;
					
					if (!checkBox_2->isChecked() && textEdit->toPlainText().size() > 0) {
						
						   	  if (!doc.setContent(textEdit->toPlainText(),false, &errorStr, &errorLine, &errorColumn)) {
							     //////return doc.toString(5);
						      QMessageBox::information(0, tr("Found xml error"),tr("Check line %1 column %2 on string \"%3\"!")
						                     .arg(errorLine - 1)
						                     .arg(errorColumn - 1)
						                     .arg(errorStr));
						        return;
					         }
						        
                    QByteArray *bites = new QByteArray();
                                bites->append(doc.toByteArray(5)); 
						        buffer_send = new QBuffer(bites,this);
										if (!buffer_send->open(QIODevice::ReadWrite)) {
											QMessageBox::information(0, tr("QBuffer"),tr("unable to create a QBuffer!"));
											return;
										}
										buffer_incomming = new QBuffer(this);
										if (!buffer_incomming->open(QIODevice::ReadWrite)) {
											QMessageBox::information(0, tr("QBuffer"),tr("unable to create a QBuffer!"));
											return;
										}
										qDebug() << "###  buffer data " << buffer_send->data();
										buffer_send->close();
										buffer_incomming->close();
										http_instance = new Communicator(HeaderSend,url,setcontenttype,methodsender);
										http_instance->Start(buffer_send,buffer_incomming);
										connect(http_instance, SIGNAL(LastDone(int)), this, SLOT(ReadyStream(int)));
										return;
										
					} else {
						
						        buffer_incomming = new QBuffer(this);
										if (!buffer_incomming->open(QIODevice::ReadWrite)) {
											QMessageBox::information(0, tr("QBuffer"),tr("unable to create a QBuffer!"));
											return;
										}
										buffer_incomming->close();
										http_instance = new Communicator(HeaderSend,url,setcontenttype,methodsender);
										http_instance->Start(0,buffer_incomming);
										connect(http_instance, SIGNAL(LastDone(int)), this, SLOT(ReadyStream(int)));
										return;
						
						
					}
	
}





void Gmain::on_SaveAsFileResult_triggered()
{
	QString errorStr;
	int errorLine, errorColumn;
	QDomDocument doc;
	QByteArray bitesave;
	
	     if (!checkBox_2->isChecked() && textEdit->toPlainText().size() > 0) {
				  
				         if (!doc.setContent(textEdit->toPlainText(),false, &errorStr, &errorLine, &errorColumn)) {
							     //////return doc.toString(5);
						      QMessageBox::information(0, tr("Found xml error"),tr("Check line %1 column %2 on string \"%3\"!")
						                     .arg(errorLine - 1)
						                     .arg(errorColumn - 1)
						                     .arg(errorStr));
						        return;
					         } else {
										 bitesave.append(doc.toByteArray(5)); 
									 }
				 
			 }  else {
				  /* file location */
				  bitesave.append(lineEdit_3->text());
			 }
			 
			 
			 QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                            QCoreApplication::applicationDirPath(),
                            tr("Http dev file (*.dshttp)"));
	
	   	if (!fileName.isEmpty() ) {
		    if (!fileName.endsWith(".dshttp")) {
            fileName.append(".dshttp");
        } 
				
				QFile f( fileName );
        if ( f.open( QIODevice::ReadWrite ) )
        {
					
					  QDataStream ds(&f);
						/* place header */
					  quint32 op,op2;
					  if (usesoapaction->isChecked()) {
							op = 1;
						} else {
							op = 0;
						}
						if (checkBox_2->isChecked()) {
							op2 = 1;
						} else {
							op2 = 0;
						}
						QStringList params;
						QString url,soap,methode,tipologia;
						url = movetourl->text();
						soap = lineEdit_2->text();
						methode = comboBox->currentText();
						tipologia = comboBox_2->currentText();
						params << url << soap << methode << tipologia; 
						ds.setVersion(QDataStream::Qt_4_2);
						ds << (quint32)MAGICNUMMERDATA;
						ds << (quint32)MMVERSION;
					  ds << bitesave;  //// stream xml 
					  ds << params; /* params */
					  ds << (quint32)op;  /* soap 1 - 0 */
					  ds << (quint32)op2;  /* xml 1 - 0 */
						/*
						              ds >> biter;  //// stream xml 
										      ds >> url;
										      ds << soap;
										      ds >> methode;
										      ds >> tipologia;
										      ds >> op;
										      ds >> op2;
				    */
			 
					 //// url movetourl->text()
					 //// soap lineEdit_2->text()
					 //// method comboBox->currentText().trimmed();
					 //// tipe comboBox_2->currentText().trimmed();
					 ////////QList<QPair<int, double> > list;
					 ///////list.append(qMakePair(66, 3.14159)); 
			 
							 const QAbstractItemModel  *model = tableWidget->model();
							 const int summline = model->rowCount();
									for (int e = 0; e < summline; ++e) { 
										QModelIndex index_0 = model->index(e,0);
										QModelIndex index_1 = model->index(e,1);
										const QString name = model->data(index_0,Qt::DisplayRole).toString();
										const QString value = model->data(index_1,Qt::DisplayRole).toString();
										if (name.size() > 1 && value.size() > 1) {
											QPair<QString,QString>  item = qMakePair(name,value);
											ds << item;
										}
									}
									
					       f.close();
						}
				}
}

void Gmain::on_actionOpen_file_triggered()
{
	QString fixfile = QFileDialog::getOpenFileName(this, tr("Open file"),
	                          QCoreApplication::applicationDirPath(),
	                                         "Http dev file (*.dshttp)" );
	opendFile(fixfile);
	
}

void Gmain::on_DemoRun_triggered()
{
	opendFile(QString::fromUtf8(":/img/toresource.dshttp"));
	
}


void Gmain::opendFile(  const QString fixfile )
{
	quint32 magic, version , op, op2;
	 
    if (!fixfile.isEmpty()) {
			  QString inside;
			  QFile file(fixfile); 
				//////if (file.exists()) {
						if (file.open(QIODevice::ReadOnly)) {
																
										QDataStream ds(&file);
										/* place header */
										ds.setVersion(QDataStream::Qt_4_2);
										ds >> magic;
										if ( (quint32)MAGICNUMMERDATA != magic ) {
											qDebug() << "######## MAGICNUMBER not ok  ";
											file.close();
											return;
										}
										ds >> version;
										if ( (quint32)MMVERSION != version ) {
											qDebug() << "######## VERSION not ok  ";
											file.close();
											return;
										}		
										QByteArray biter;
										QString url,soap,methode,tipologia;
										QStringList params;
										bool useopt,checker;
                          ds >> biter;  //// stream xml 
										      ds >> params;
										      ds >> op;
										      ds >> op2;
										
										      qDebug() << "###  params " << params;
										      qDebug() << "###  checker " << op;
										      qDebug() << "###  useopt " << op2;
										
										      if ((int)op == 1) {
                          usesoapaction->setChecked ( true ); 
													} else {
														usesoapaction->setChecked ( false );
													}
													if ((int)op2 == 1) {
                          checkBox_2->setChecked ( true ); 
													} else {
													checkBox_2->setChecked ( false );
													}
										      textEdit->setPlainText(QString(biter));
                          movetourl->setText(params.at(0)); 
										      lineEdit_2->setText(params.at(1)); 
                          comboBox->setCurrentIndex(comboBox->findText(params.at(2)));
													comboBox_2->setCurrentIndex(comboBox_2->findText(params.at(3)));
													//////ds << movetourl->text();  /* url */
													//////ds << lineEdit_2->text();  /* soap action */
													//////ds << comboBox->currentText(); /* method */
													///////ds << comboBox_2->currentText();  /* contenet type */
													/////ds << usesoapaction->isChecked();  /* soap 1 - 0 */
													///////ds << checkBox_2->isChecked(); 	
													QList<QPair<QString,QString> > listVV;
													int stone = -1;
													 tableWidget->clear ();
																while (!ds.atEnd()) {
																			QPair<QString,QString>  item;
																	    stone++;
																	    ds >> item;
																	    listVV.append(item);
																	    QTableWidgetItem *x1 = new QTableWidgetItem(item.first);
																	    QTableWidgetItem *x2 = new QTableWidgetItem(item.second);
																	    //////qDebug() << "###  item " << item;
																	    tableWidget->setItem(stone, 0,x1);
																	    tableWidget->setItem(stone, 1,x2);
																	    
																}
																
													
												  
                       	file.close();									
															
															
						}
				/////}
	
		}
}




void Gmain::ReadyStream( int headercode )
{
	qDebug() << "###########ReadyStream########################################################";
	qDebug() << "### headercode " << headercode;
	QChar letter('A' + (qrand() % 26));
    /* if gzip go file  */
	const QString tmpfiler = QString("%1/http_%2.gz").arg( QDir::homePath ()).arg(letter);
	////////////  search content-encoding=gzip
	QMap<QString,QString> hhin = http_instance->inmap();
	 QFile f( tmpfiler );
	
      
	
	             QMapIterator <QString,QString> d(hhin);
                 while (d.hasNext()) {
                       d.next();
									     if (d.key() == "content-encoding") {
												   if (d.value().contains("gzip",Qt::CaseInsensitive) || 
														     d.value().contains("deflate",Qt::CaseInsensitive)) {
														 if ( f.open( QIODevice::WriteOnly ) ) {
																f.write(buffer_incomming->data());
																f.close();
															} else {
															return;
															}
															
															/* uncompress stream */
															 gzFile file;
																file = gzopen (tmpfiler.toUtf8().data(),"rb");
																if(!file) {
																	std::cout << "### Unable to work on cache dir!. "  << std::endl;
																	return;
																}
																buffer_incomming = new QBuffer(this);
														    if (!buffer_incomming->open(QIODevice::ReadWrite)) {
																	return;
																}
																QByteArray input;
																char buffer[1024];
																QByteArray inputData;
																while(int readBytes =  gzread (file, buffer, 1024))
																 {
																	input.append(QByteArray(buffer, readBytes));
																  }
																gzclose(file);
																f.remove();
                                buffer_incomming->write(input);
														 
													 } else if (d.value().contains("qcompress",Qt::CaseInsensitive)) {
														 
                                QByteArray dummi = qUncompress ( buffer_incomming->data() );
														    buffer_incomming = new QBuffer(this);
														    if (!buffer_incomming->open(QIODevice::ReadWrite)) {
																	return;
																}
																buffer_incomming->write(dummi);
													 }
											 }
									     ////d.key() << "|" << d.value();
                 }
	
	QString pasres = QString(buffer_incomming->data());
	///////qDebug() << "### incomming " << buffer_incomming->data();
  QString  sender="----------------------send request------------------------------------\n";
	QString errorStr;
	int errorLine, errorColumn;
	QDomDocument doc;
	if (!doc.setContent(buffer_incomming->data(),false, &errorStr, &errorLine, &errorColumn)) {
		 /* handle binary file save as ...*/
		  textEdit_3->setPlainText("Not xml response! size() ="+QString("%1").arg(buffer_incomming->data().size())+" \n"+pasres);
	}  else {
		 textEdit_3->setPlainText(doc.toString(5));
		 
	}
	
	QMapIterator <QString,QString> de(HeaderSend);
                 while (de.hasNext()) {
                       de.next();
									     ////////qDebug() << "### r " << d.key() << "|" << d.value();
									     sender.append(QString("ValueName %1 = %2  \n").arg(de.key()).arg(de.value()));
                 }
								 
				sender.append("----------------------send end------------------------------------\n");
				sender.append("----------------------incomming header----------------------------\n");
				sender.append(QString("Header responds = %1\n").arg(headercode));
				sender.append(QString("%1\n").arg(http_instance->headerin()));
				sender.append("----------------------incomming end-------------------------------\n");
	
	
	textEdit_2->setPlainText(sender);
	
	
	
	buffer_send->close();
	buffer_incomming->close();
	tabWidget->setCurrentIndex(1);
	
}


void Gmain::on_CheckXmlValid_clicked()
{
	if (textEdit->toPlainText().size() > 0 ) {
		
		QString errorStr;
    int errorLine, errorColumn;
    QDomDocument doc;
					if (!doc.setContent(textEdit->toPlainText(),false, &errorStr, &errorLine, &errorColumn)) {
							//////return doc.toString(5);
						 QMessageBox::information(0, tr("Found xml error"),tr("Check line %1 column %2 on string \"%3\"!")
						                     .arg(errorLine - 1)
						                     .arg(errorColumn - 1)
						                     .arg(errorStr));
					} else {
						
						 QMessageBox::information(0, tr("XML valid."),tr("All tag are valid size %1.").arg(textEdit->toPlainText().size()));
						 textEdit->setPlainText(doc.toString(5));
					}
		
		
	} else {
		QMessageBox::information(0, tr("XML not found!"),tr("Null size xml document!"));
	}
	
}





void Gmain::BaseInit()
{
	
QString fields ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" "
            "xmlns:tns=\"http://www.ignyte.com/whatsshowing\" xmlns:xs=\"http://www.w3.org"
            "/2001/XMLSchema\">\n"
            "<soap:Body>\n"
            "<tns:GetTheatersAndMovies>\n"
            "<tns:zipCode>2342</tns:zipCode>\n"
            "<tns:radius>2352</tns:radius>\n"
            "</tns:GetTheatersAndMovies>\n"
            "</soap:Body>\n"
            "</soap:Envelope>\n");
//////textEdit->setPlainText(fields);

}






void Gmain::closeEvent( QCloseEvent* e )
{
	//////qApp->exit(0);
	tableWidget->deleteLater();
	delete tableWidget;
  //////deleteLater(); 	
	e->accept();
}






void Gmain::on_actionQT_info_triggered()
{
	OpenDeskBrowser( QUrl("http://trolltech.com/"));
}

void Gmain::on_actionAbout_app_triggered()
{
	OpenDeskBrowser( QUrl("http://www.crosskern.com/"));
}

void Gmain::on_WebdavSpecification_triggered()
{
	OpenDeskBrowser( QUrl("http://www.webdav.org/specs/rfc2518.html"));
}

void Gmain::on_LinkGeneratorSoap_triggered()
{
	OpenDeskBrowser( QUrl("http://www.soapclient.com/soapclient.com/soaptest.html"));
}
















