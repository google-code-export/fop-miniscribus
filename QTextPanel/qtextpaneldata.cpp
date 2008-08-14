#include "qtextpaneldata.h"

QTextPanelData* QTextPanelData::_instance = NULL;

QTextPanelData* QTextPanelData::instance()
{
	if (_instance == NULL)
	{
		_instance = new QTextPanelData();
	}

	return _instance;
}

QTextPanelData::QTextPanelData()
		: transfile(0),pdev(new QPrinter(QPrinter::ScreenResolution))
{
	/////history_pics.clear();
	FF_db = QFontDatabase();
	SessionID = qrand() % 1000;
	PanelPageSize A4;
	current_Page_Format = A4;
	history_page_norms.clear();

	/* qsetting init to open */
	QCoreApplication::setOrganizationName("CrossKern");
	QCoreApplication::setOrganizationDomain("fop.ciz.ch");
	QCoreApplication::setApplicationName("QTextPanel");

	AppendPaper(A4);    /* append default */

	PanelPageSize CDLabel;
	CDLabel.name = "CD-Label (128 x 128mm)";
	CDLabel.G_regt = QRectF(0,0,MM_TO_POINT(128),MM_TO_POINT(128));
	CDLabel.P_rect = QPrinter::Custom;
	CDLabel.RealSize = CDLabel.G_regt.size();
	const qreal margindd = MM_TO_POINT(8);
	/* qtext document minimum size bug not break line at 128mm */
	CDLabel.G_regt = QRectF(0,0,MM_TO_POINT(200),MM_TO_POINT(200));

	CDLabel.P_margin = QRectF(margindd,margindd,margindd,margindd);

	AppendPaper(CDLabel);    /* append default */

	FormatRegister(QT_TR_NOOP("A4 (210 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
	FormatRegister(QT_TR_NOOP("A4 (211 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
	FormatRegister(QT_TR_NOOP("A0 (841 x 1189 mm)"), QPrinter::A0);
	FormatRegister(QT_TR_NOOP("A1 (594 x 841 mm)"), QPrinter::A1);
	FormatRegister(QT_TR_NOOP("A2 (420 x 594 mm)"), QPrinter::A2);
	FormatRegister(QT_TR_NOOP("A3 (297 x 420 mm)"), QPrinter::A3);
	FormatRegister(QT_TR_NOOP("A5 (148 x 210 mm)"), QPrinter::A5);
	FormatRegister(QT_TR_NOOP("A6 (105 x 148 mm)"), QPrinter::A6);
	FormatRegister(QT_TR_NOOP("A7 (74 x 105 mm)"), QPrinter::A7);
	FormatRegister(QT_TR_NOOP("A8 (52 x 74 mm)"), QPrinter::A8);
	FormatRegister(QT_TR_NOOP("A9 (37 x 52 mm)"), QPrinter::A9);
	FormatRegister(QT_TR_NOOP("B0 (1000 x 1414 mm)"), QPrinter::B0);
	FormatRegister(QT_TR_NOOP("B1 (707 x 1000 mm)"), QPrinter::B1);
	FormatRegister(QT_TR_NOOP("B2 (500 x 707 mm)"), QPrinter::B2);
	FormatRegister(QT_TR_NOOP("B3 (353 x 500 mm)"), QPrinter::B3);
	FormatRegister(QT_TR_NOOP("B4 (250 x 353 mm)"), QPrinter::B4);
	FormatRegister(QT_TR_NOOP("B5 (176 x 250 mm, 6.93 x 9.84 inches)"), QPrinter::B5);
	FormatRegister(QT_TR_NOOP("B6 (125 x 176 mm)"), QPrinter::B6);
	FormatRegister(QT_TR_NOOP("B7 (88 x 125 mm)"), QPrinter::B7);
	FormatRegister(QT_TR_NOOP("B8 (62 x 88 mm)"), QPrinter::B8);
	FormatRegister(QT_TR_NOOP("B9 (44 x 62 mm)"), QPrinter::B9);
	FormatRegister(QT_TR_NOOP("B10 (31 x 44 mm)"), QPrinter::B10);
	FormatRegister(QT_TR_NOOP("C5E (163 x 229 mm)"), QPrinter::C5E);
	FormatRegister(QT_TR_NOOP("DLE (110 x 220 mm)"), QPrinter::DLE);
	FormatRegister(QT_TR_NOOP("Executive (7.5 x 10 inches, 191 x 254 mm)"), QPrinter::Executive);
	FormatRegister(QT_TR_NOOP("Folio (210 x 330 mm)"), QPrinter::Folio);
	FormatRegister(QT_TR_NOOP("Ledger (432 x 279 mm)"), QPrinter::Ledger);
	FormatRegister(QT_TR_NOOP("Legal (8.5 x 14 inches, 216 x 356 mm)"), QPrinter::Legal);
	FormatRegister(QT_TR_NOOP("Letter (8.5 x 11 inches, 216 x 279 mm)"), QPrinter::Letter);
	FormatRegister(QT_TR_NOOP("Tabloid (279 x 432 mm)"), QPrinter::Tabloid);
	FormatRegister(QT_TR_NOOP("US Common #10 Envelope (105 x 241 mm)"), QPrinter::Comm10E);

   /* other mesure size go to file and append here on load */

	//////LoadFontDir(QDir::currentPath());
}

QTextPanelData::~QTextPanelData()
{

}

void QTextPanelData::FormatRegister(const QString txt , QPrinter::PageSize pp)
{
	PanelPageSize Lxx;
	Lxx.Register(txt,pp,false);
	history_page_norms.insert(history_page_norms.size() + 1,Lxx);

	PanelPageSize Pxx;
	Pxx.Register(txt,pp,true);
	history_page_norms.insert(history_page_norms.size() + 1,Pxx);
}

void QTextPanelData::AppendPaper(PanelPageSize cur)
{
	history_page_norms.insert(history_page_norms.size() + 1,cur);
}


void QTextPanelData::LoadFontDir(const QString path)
{
	////  QFontDatabase


	QDir dir(path);
	if (dir.exists())
	{
		const QFileInfoList list = dir.entryInfoList();
		QFileInfo fi;
		for (int l = 0; l < list.size(); l++)
		{
			fi = list.at(l);
			if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
			{

			}
			else if (fi.isFile())
			{
				QString surr = fi.completeSuffix().toLower();
				if (surr == "ttf")
				{
					QByteArray fakefont = StreamFromFile(fi.absoluteFilePath());
					int loadet = FF_db.addApplicationFontFromData(fakefont);
					/////qDebug() << "### load " << fi.fileName() << " loadet ->" << loadet;
				}
				////////////qDebug() << "### read  " << fi.fileName() << " extension ->" << surr;
			}

		}
	}
}

PanelPageSize QTextPanelData::FindPagePsize(const QRect paper)
{
	QMapIterator<int,PanelPageSize> i(mpages());
	while (i.hasNext())
	{
		i.next();
		PanelPageSize xx = i.value();
		if (xx.G_regt.toRect() == paper)
		{
			return xx;
		}
	}
	return history_page_norms[1];
}

/* save temporary current clipboard same as clone !!!! */
void QTextPanelData::SaveMimeTmp()
{
	const QMimeData *mime = QApplication::clipboard()->mimeData();
	QMimeData *tmpcopy = new QMimeData();
	QStringList items = mime->formats();
	for (int i = 0; i < items.size(); ++i)
	{
		const QString typer = items.at(i);
		const QByteArray chunkdat = mime->data(typer);
		tmpcopy->setData(typer,chunkdat);
		/////////////qDebug() << "### session register mime type->" << typer << " size->" << chunkdat.size();
	}
	if (items.size() > 0)
	{
		mime_story.insert(mime_story.size(),tmpcopy);
	}
}

void QTextPanelData::Set_Translate_File(const QString file)
{
	transfile = file;
	PPtr.load(file);
	qApp->installTranslator(&PPtr);
}
