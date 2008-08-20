#include "qtextpanelmime.h"
#include "qtextpanel.h"
#include "qtextpanelimage.h"

QIcon createColorToolButtonIcon(const QString &imageFile,QColor color)
{
	QPixmap pixmap(50, 80);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPixmap image(imageFile);
	QRect target(0, 0, 50, 60);
	QRect source(0, 0, 42, 42);
	painter.fillRect(QRect(0, 60, 50, 80), color);
	painter.drawPixmap(target, image, source);
	return QIcon(pixmap);
}

QIcon createColorIcon(QColor color)
{
	QPixmap pixmap(50, 50);
	pixmap.fill(color);
	return QIcon(pixmap);
}

QPixmap bruschChess(qreal chesswidth)
{
	QPixmap m_tile = QPixmap((int)chesswidth,(int)chesswidth);
	const qreal piece = chesswidth / 2;
	m_tile.fill(Qt::white);
	QPainter pt(&m_tile);
	QColor color(230, 230, 230);
	pt.fillRect(QRectF(0.,0.,piece,piece),color);
	pt.fillRect(QRectF(piece,piece,piece,piece),color);

	return m_tile;
}

PanelPageSize::PanelPageSize()
{
	landscape = false;
	coolspace = 0;
	modus = 0;
	name = "A4 (210 x 297 mm, 8.26 x 11.7 inches)";  ////////  G_regt.size()
	P_rect = QPrinter::A4;
	G_regt = QRectF(0,0,MM_TO_POINT(210),MM_TO_POINT(297));
	RealSize = G_regt.size();
	const qreal mr = MM_TO_POINT(10);
	P_margin = QRectF(mr,mr,mr,MM_TO_POINT(30));
	//////P_margin = QRectF(0,0,0,0);  ////// css like QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
	AllowHeaderFooter = true;
}

PanelPageSize& PanelPageSize::operator=(const PanelPageSize& d)
{
	name = d.name;
	landscape = d.landscape;
	G_regt = d.G_regt;
	modus = d.modus;
	RealSize = d.RealSize;
	coolspace = d.coolspace;
	P_rect = d.P_rect;
	P_margin = d.P_margin;
	return *this;
}

qreal PanelPageSize::faktor()
{
	return qMax(G_regt.width(),G_regt.height()) / qMin(G_regt.width(),G_regt.height());
}

void PanelPageSize::Register(QString n , QPrinter::PageSize pp , bool La)
{
	G_regt = M_PagesizeMake(P_rect,La);
	//////////////qDebug() << "### Register L." <<  La << "  name." << n << " G_regt ->" << G_regt;
	RealSize = G_regt.size();
	name = n;
	landscape = La;
	P_rect = pp;
}

QString PanelPageSize::HName()
{
	QString sep(" /");
	QString reg = name;
	if (landscape)
	{
		reg.append(sep+QT_TR_NOOP("Landscape"));
	}
	else
	{
		reg.append(sep+QT_TR_NOOP("Portrait"));
	}
	return reg;
}

QRectF PanelPageSize::BoundingPageSumme(const int summe)
{
	int summetotal = qBound(1,summe,MaximumPages);
	const qreal alto = summetotal * G_regt.height();
	return QRectF(0,0,G_regt.width(),alto);
}

QTextDocument *PanelPageSize::nulldoc(QString htm)
{
	QTextDocument *doc = new QTextDocument;
	if (htm.size() > 2)
	{
		doc->setHtml(htm);
	}
	HandlePrint(doc);
	return doc->clone();
}

/* form qtexdocument to this margin an papersize */
void PanelPageSize::HandlePrint(QTextDocument *doc)
{
	const qreal RightMargin = P_margin.y();
	const qreal LeftMargin = P_margin.height();
	const qreal LargeDoc = G_regt.width() - RightMargin  - LeftMargin;
	doc->setPageSize(G_regt.size());
	QTextFrame  *Tframe = doc->rootFrame();
	QTextFrameFormat Ftf = Tframe->frameFormat();
	Ftf.setLeftMargin(P_margin.height());
	Ftf.setBottomMargin(P_margin.width());
	Ftf.setTopMargin(P_margin.x());
	Ftf.setBackground(QBrush(Qt::transparent));
	Ftf.setRightMargin(P_margin.y());
	Ftf.setPadding(0);
	Tframe->setFrameFormat(Ftf);
	doc->setPageSize(G_regt.size());
}

int PanelPageSize::FillFopAttributes(QDomElement e)
{
	const qreal TopMargin = pointTo(P_margin.x(),"mm");
	const qreal RightMargin = pointTo(P_margin.y(),"mm");
	const qreal BottomMargin = pointTo(P_margin.width(),"mm");
	const qreal LeftMargin = pointTo(P_margin.height(),"mm");
	e.setAttribute("margin-top",QString("%1mm").arg(TopMargin));
	e.setAttribute("margin-bottom",QString("%1mm").arg(BottomMargin));
	e.setAttribute("margin-left",QString("%1mm").arg(LeftMargin));
	e.setAttribute("margin-right",QString("%1mm").arg(RightMargin));
	e.setAttribute("page-width",QString("%1mm").arg(pointTo(G_regt.width(),"mm")));
	e.setAttribute("page-height",QString("%1mm").arg(pointTo(G_regt.height(),"mm")));
	return 1;
}

/* allowed char on file name image to save */
QString imageName(const QString txt)
{
	QString touri = txt;
	touri = touri.replace(" ","_");
	QStringList piece = touri.split("");
	QString alloweduri = "abcdefghijklmnopqrstuvwxyz1234567890_-ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	QStringList alist = alloweduri.split("");

	for (int i = 0; i < piece.size(); ++i)
	{
		QString pin = piece.at(i);
		if (!alist.contains(pin))
		{
			touri.replace(pin,"");
		}
	}
	return touri.trimmed();
}

qreal FopInt(const QString datain)
{
	QString ctmp = datain;
	const QString data = ctmp.replace(" ","").trimmed();
	//////////qDebug() << "### request unit data->" << datain << " size->" << datain.size();
	qreal points = 0;
	if (data.size() < 1)
	{
		return points;
	}

	if (data.endsWith("%"))
	{
		return 12.5;
	}

	if (datain == "0")
	{
		return points;
	}

	if (data.endsWith("pt") ||  data.endsWith("px"))
	{
		points = data.left(data.length() - 2).toDouble();
		return points;
	}
	else if (data.endsWith("cm"))
	{
		double value = data.left(data.length() - 2).toDouble();
		points = CM_TO_POINT(value);
	}
	else if (data.endsWith("em"))
	{
		points = data.left(data.length() - 2).toDouble();
	}
	else if (data.endsWith("mm"))
	{
		double value = data.left(data.length() - 2).toDouble();
		points = MM_TO_POINT(value);
	}
	else if (data.endsWith("dm"))
	{
		double value = data.left(data.length() - 2).toDouble();
		points = DM_TO_POINT(value);
	}
	else if (data.endsWith("in"))
	{
		double value = data.left(data.length() - 2).toDouble();
		points = INCH_TO_POINT(value);
	}
	else if (data.endsWith("inch"))
	{
		double value = data.left(data.length() - 4).toDouble();
		points = INCH_TO_POINT(value);
	}
	else if (data.endsWith("pi"))
	{
		double value = data.left(data.length() - 4).toDouble();
		points = PI_TO_POINT(value);
	}
	else if (data.endsWith("dd"))
	{
		double value = data.left(data.length() - 4).toDouble();
		points = DD_TO_POINT(value);
	}
	else if (data.endsWith("cc"))
	{
		double value = data.left(data.length() - 4).toDouble();
		points = CC_TO_POINT(value);
	}
	else
	{
		points = 0;
	}

	return points;
}


qreal pointTo(qreal unit , const QString unita)
{
	qreal ri = 0;

	if (unita == "cm")
	{
		ri = POINT_TO_CM(unit);
	}
	else if (unita == "pt" || unita == "px")
	{
		ri = unit;
		return ri;
	}
	else if (unita == "mm")
	{
		ri = POINT_TO_MM(unit);
	}
	else if (unita == "dm")
	{
		ri = POINT_TO_DM(unit);
	}
	else if (unita == "inch")
	{
		ri = POINT_TO_INCH(unit);
	}
	else if (unita == "pi")
	{
		ri = POINT_TO_PI(unit);
	}
	else if (unita == "dd")
	{
		ri = POINT_TO_DD(unit);
	}
	else if (unita == "cc")
	{
		ri = POINT_TO_CC(unit);
	}
	else
	{
		ri = 10;
	}

	return ri;
}

qreal toUnit(qreal unit , const QString unita)
{
	return pointTo(unit,unita);
}

qreal toPoint(qreal unit , const QString unita)
{
	qreal ri = 0;

	if (unita == "cm")
	{
		ri = CM_TO_POINT(unit);
	}
	else if (unita == "pt" || unita == "px")
	{
		ri = unit;
		return ri;
	}
	else if (unita == "mm")
	{
		ri = MM_TO_POINT(unit);
	}
	else if (unita == "dm")
	{
		ri = DM_TO_POINT(unit);
	}
	else if (unita == "inch")
	{
		ri = INCH_TO_POINT(unit);
	}
	else if (unita == "pi")
	{
		ri = PI_TO_POINT(unit);
	}
	else if (unita == "dd")
	{
		ri = DD_TO_POINT(unit);
	}
	else if (unita == "cc")
	{
		ri = CC_TO_POINT(unit);
	}
	else
	{
		ri = 10;
	}

	return ri;
}

/* encode to name */
QString encodeBase64(QString xml)
{
	QByteArray text;
	text.append(xml);
	return text.toBase64();
}

/* decode to name */
QString decodeBase64(QString xml)
{
	QByteArray xcode("");;
	xcode.append(xml);
	QByteArray precode(QByteArray::fromBase64(xcode));
	QString notetxt = precode.data();
	return notetxt;
}

/* read the contenet of a local file as qstring */
QString ReadFile(const QString fullFileName)
{
	QString inside = "";
	QFile file(fullFileName);
	if (file.exists())
	{
		if (file.open(QFile::ReadOnly | QFile::Text))
		{
			inside = QString::fromUtf8(file.readAll());
			file.close();
		}
	}

	return inside;
}

/* read the contenet of a local file as QByteArray*/
QByteArray StreamFromFile(const QString fullFileName)
{
	QByteArray inside;
	QFile file(fullFileName);
	if (file.exists())
	{
		if (file.open(QFile::ReadOnly | QFile::Text))
		{
			inside = file.readAll();
			file.close();
		}
	}

	return inside;
}

void OpenDeskBrowser(QUrl loc)
{
#if defined Q_WS_MAC
	QProcess *m = new QProcess();
	QStringList macs;
	macs << loc.toString();  /* oeffnet der default browser */
	m->startDetached(QString("open") , macs);
	///////std::cout << "OpenDesktop [" << qPrintable(macs.join(" ")) << "]" << std::endl;
	return;
#endif

#if defined Q_WS_WIN

	QString fullFileName = loc.toString();

	if (fullFileName.startsWith("http://", Qt::CaseInsensitive) ||
	      fullFileName.startsWith("https://", Qt::CaseInsensitive) ||
	      fullFileName.startsWith("ftp://", Qt::CaseInsensitive) ||
	      fullFileName.startsWith("news://", Qt::CaseInsensitive) ||
	      fullFileName.startsWith("mailto:", Qt::CaseInsensitive) ||
	      fullFileName.startsWith("webdav://", Qt::CaseInsensitive))
	{
		/* all ok */
	}
	else
	{
		fullFileName.prepend("file:///");
	}

	bool wr = QDesktopServices::openUrl(QUrl(fullFileName));

	if (!wr)
	{
		QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Window Unable to open action file or dir  %1").arg(loc.toString()));
	}

	return;
#endif

	/* linux */

	bool r = QDesktopServices::openUrl(loc);
	if (!r)
	{
		QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Linux Unable to open action file or dir  %1").arg(loc.toString()));
	}

}

/* simple mkdir */
bool Cache(const QString dirpath)
{
	QDir dira(dirpath);
	if (dira.mkpath(dirpath))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* os path from sistem */
QString convertPath(QString path)
{
	return QDir::toNativeSeparators(path);
}

/* write a file to utf-8 format */
bool fwriteUtf8(const QString file ,QString xml)
{
	if (file.contains("/", Qt::CaseInsensitive))
	{
		QString ultimacartellaaperta = file.left(file.lastIndexOf("/"))+"/";
		QDir dira(ultimacartellaaperta);
		if (dira.mkpath(ultimacartellaaperta)) { }
		else
		{
			return false;
		}
	}

	QTextCodec *codecx;
	codecx = QTextCodec::codecForMib(106);
	QFile f(file);
	if (f.open(QFile::WriteOnly | QFile::Text))
	{
		QTextStream sw(&f);
		sw.setCodec(codecx);
		sw << xml;
		f.close();
		return true;
	}
	return false;
}

QString getGSLinuxPath(QString apps)
{
	QStringList potential_paths;
	potential_paths.append("/usr/local/bin");
	potential_paths.append("/sw/bin");   /* to use on mac as same */
	potential_paths.append("/opt/bin");
	QProcess *process = new QProcess(NULL);
	process->setReadChannelMode(QProcess::MergedChannels);
	QStringList env = process->systemEnvironment();
	env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=\\1;"+potential_paths.join(";"));
	process->setEnvironment(env);

	process->start(QString("which") ,  QStringList() << apps , QIODevice::ReadOnly);
	if (!process->waitForFinished())
	{
		return QString();
	}
	else
	{
		QString finder = process->readAll().trimmed();
		if (finder.endsWith(apps,Qt::CaseInsensitive))
		{
			///////////// qDebug() << "### finder " <<  finder;
			return finder;
		}
		else
		{
			return QString();
		}
	}
}

/* find gpl GhostScript path or exe */
QString getGSDefaultExeName()
{
	QString gsName;
	QString gVersion;
#if defined Q_WS_WIN
	QFileInfo Pinfo;
	// Try to locate GhostScript thanks to the qsetting
	gsName = "gswin32c.exe";
	QSettings softs("HKEY_LOCAL_MACHINE\\Software",QSettings::NativeFormat);
	QStringList allsoftware = softs.childGroups();
	QStringList gsonly = allsoftware.filter(QRegExp("Ghostscript"));
	//////////qDebug() << "### gsonly " <<  gsonly;
	for (int i = 0; i < gsonly.size(); ++i)
	{
		const QString RealName = gsonly.at(i);  /* realpath */
		if (RealName.contains("Ghostscript"))
		{
			//////////qDebug() << "### soft " <<  RealName;
			for (int e=1;e<99;e++)
			{
				/* check version 8 ++ 99 down */
				gVersion = QString("8.%1").arg(100 - e);
				if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6)
				{
					Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
					return gsName.prepend(Pinfo.absolutePath()+"/");
				}
				/* check version 7 ++ 99 down */
				gVersion = QString("7.%1").arg(100 - e);
				if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6)
				{
					Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
					return gsName.prepend(Pinfo.absolutePath()+"/");
				}
			}
		}
	}

	/* win not having GPL Ghostscript ! */
	gsName = "";
	return gsName;
#endif
#if defined Q_WS_MAC

	QString pathfinder = getGSLinuxPath(QString("gs"));  /* which  simple append systems path mac + linux */
	if (pathfinder.size() > 2 && pathfinder.endsWith("gs",Qt::CaseInsensitive))
	{
		return pathfinder;
	}

	QStringList paths;
	paths << "/usr/local/bin/gs" << "/usr/bin/gs" << "/bin/gs" << "/sw/bin/gs" << "/opt/bin/gs";
	QFileInfo gsp;
	for (int i = 0; i < paths.size(); ++i)
	{
		gsp.setFile(paths.at(i));
		if (gsp.exists())
		{
			return paths.at(i);
		}
	}
	gsName = "gs";
	return gsName;
#endif
#if defined Q_WS_X11
	return getGSLinuxPath();
#endif
	/* forum http://www.qtcentre.org/forum/f-qt-programming-2/t-qsettings-read-only-avaiable-10254.html */
}

/* find gpl GhostScript version  */
double getGSVersion()
{
	QProcess process;
	QString Stversion;
	bool ok;
	double GS_version = 0.00;
	QRegExp regExp1(" (\\d+)\\.(\\d+).*");
	process.setReadChannelMode(QProcess::MergedChannels);
	process.start(getGSDefaultExeName() ,  QStringList() << "-v");
	if (!process.waitForFinished())
	{
		return GS_version;
	}
	else
	{
		QString pu = process.readAll();
		QStringList line1 = pu.split("\n");
		int pos1 = regExp1.indexIn(line1.at(0));  /* only first line */
		if (pos1 > 0)
		{
			Stversion = QString("%1.%2").arg(regExp1.cap(1).toInt()).arg(regExp1.cap(2).toInt());
			double GS_version = Stversion.toDouble(&ok);
			if (ok && GS_version > 5)
			{
				return GS_version;
			}
		}
		else
		{
			return GS_version;
		}
	}
	return GS_version;
}

double JavaVersion()
{
	QProcess process;
	QString Stversion;
	bool ok;
	double js_version = 0.00;
	QRegExp regExp1(" (\\d+)\\.(\\d+).*");
	process.setReadChannelMode(QProcess::MergedChannels);
	process.start("java" ,  QStringList() << "-version");
	if (!process.waitForFinished())
	{
		return js_version;
	}
	else
	{
		QString pu = process.readAll();
		/////QStringList line1 = pu.split("\n");
		////////qDebug() << "### jsversion " << pu;
		int pos1 = regExp1.indexIn(pu);  /* only first line */
		if (pos1 > 0)
		{
			Stversion = QString("%1.%2").arg(regExp1.cap(1).toInt()).arg(regExp1.cap(2).toInt());
			double js_version = Stversion.toDouble(&ok);
			if (ok && js_version > 1)
			{
				return js_version;
			}
		}
		else
		{
			return js_version;
		}
	}
	return js_version;
}

/*
#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())
 remove dir after works
*/

int callGS(const QStringList args)
{
	const QString startnow = QDir::currentPath();
	const QString GhostScriptPath = getGSDefaultExeName();
	QDir::setCurrent(_GSCACHE_);
	QString  cmd1 =  GhostScriptPath + " ";
	cmd1 += args.join(" ");
	int fax = -1;
#if defined Q_WS_MAC
	fax = system(cmd1.toLocal8Bit());
	QDir::setCurrent(startnow);
	return fax;
#endif
	QProcess *process = new QProcess(NULL);
	process->setReadChannelMode(QProcess::MergedChannels);
	process->start(GhostScriptPath ,  args , QIODevice::ReadOnly);
	if (!process->waitForFinished())
	{
		fax = -1;
	}
	else
	{
		QString ghostcomment = process->readAll().trimmed();
		//////qDebug() << "ghostcomment-> " << ghostcomment;
		fax = 0;
	}

	QDir::setCurrent(startnow);
	return fax;
}


/*
extern inline int Funzionaaaaaaa_______lGS( const QStringList args )
{
   const QString startnow = QDir::currentPath();
   QDir::setCurrent(_GSCACHE_);
   QString  cmd1 = getGSDefaultExeName() + " ";
   cmd1 += args.join(" ");
   int fax = -1;
   fax = system(cmd1.toLocal8Bit());
   QDir::setCurrent(startnow);
   return fax;
}
*/


QPixmap LoadPS(QString fn , const QString arguments_append)
{
	const QString pdfFile = convertPath(fn);
	QChar letter('A' + (qrand() % 26));
	QDateTime timer1(QDateTime::currentDateTime());
	const QString qttmpFile = _GSCACHE_+QString("%2_%1.png").arg(timer1.toString("yyyy-MM-dd-HH-mm-ss-zzz")).arg(letter);
	QFileInfo fitmp(qttmpFile);
	const int VersionGS = getGSVersion();

	QFile lastaction(fitmp.absoluteFilePath());
	lastaction.remove();

	QPixmap pm;
	int ret = -1;
	QStringList args;

	if (arguments_append.size() > 3)
	{
		args.append(arguments_append);
	}

	if (VersionGS >=8)
	{
		args.append("-sDEVICE=png16m");
		args.append("-dGraphicsAlphaBits=4");
		args.append("-r72");
		args.append("-o");
		args.append(fitmp.fileName());
		args.append(pdfFile);
	}
	else
	{
		args.append("-sDEVICE=pnggray");
		args.append("-r72x72");
		args.append("-sOutputFile="+fitmp.fileName());
		args.append("-q");
		args.append(pdfFile);
	}
	ret = callGS(args);
	//////////qDebug() << "### ret " << ret << " VersionGS->" << VersionGS;
	if (ret == 0)
	{
		QPixmap tmpimage(fitmp.absoluteFilePath());
		lastaction.remove();
		return tmpimage;
	}
	return pm;
}




/*
version < 8
args.append("-r72x72");
args.append("-sOutputFile="+imageiosFileCache);
args.append("-q");
args.append(pdfFile);
*/

QPixmap LoadPDF(QString fn, int Page, int w)
{
	QString tmp, cmd1, cmd2;
	const QString pdfFile = convertPath(fn);
	const QString tmpFile = convertPath(QDir::homePath()+"/sctodaytmps.png");
	const QString qttmpFile = QDir::homePath()+"/sctodaytmps.png";
	QPixmap pm;
	tmp.setNum(Page);
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
	args.append("-sDEVICE=png16m");
	args.append("-r72");
	args.append("-dGraphicsAlphaBits=4");
	args.append("-o");
	args.append(tmpFile);
	args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
	args.append(pdfFile);
	ret = callGS(args);
	////////qDebug() << "### ret " << ret;
	if (ret == 0)
	{
		QPixmap tmpimage(qttmpFile);
		QPixmap penna = tmpimage.scaledToWidth(w);
		tmpimage.detach();
		QFile lastaction(qttmpFile);
		lastaction.remove();
		QPainter p;
		p.begin(&penna);
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
		p.drawRect(0, 0, penna.width(), penna.height());
		p.end();
		return penna;
	}
	return pm;
}

QRectF M_PagesizeMake(QPrinter::PageSize psize , bool landscape)
{
	QPrinter *print = new QPrinter(QPrinter::HighResolution);
	print->setFullPage(true);
	print->setPageSize(psize);
	if (!landscape)
	{
		print->setOrientation(QPrinter::Landscape);
	}
	else
	{
		print->setOrientation(QPrinter::Portrait);
	}

	const QRectF pa = print->pageRect(QPrinter::Point);
	const qreal faktor = qMax(pa.width(),pa.height()) / qMin(pa.width(),pa.height());

	const qreal smaller = qMin(pa.width(),pa.height());
	const qreal biger = smaller * faktor;
	delete print;

	if (landscape)
	{
		return QRectF(0,0,biger,smaller);
	}
	else
	{
		return QRectF(0,0,smaller,biger);
	}
}

QStringList QTextEditMimeData::formats() const
{
	if (!fragment.isEmpty())
		return QStringList() << QString::fromLatin1("text/plain") << QString::fromLatin1("text/html");
	else
		return QMimeData::formats();
}

QVariant QTextEditMimeData::retrieveData(const QString &mimeType, QVariant::Type type) const
{
	if (!fragment.isEmpty())
		setup();
	return QMimeData::retrieveData(mimeType, type);
}

void QTextEditMimeData::setup() const
{
	QTextEditMimeData *that = const_cast<QTextEditMimeData *>(this);
	that->setData(QLatin1String("text/html"), fragment.toHtml("utf-8").toUtf8());
	that->setText(fragment.toPlainText());
	fragment = QTextDocumentFragment();
}

QRectF CenterRectSlaveFromMaster(const QRectF Master ,
                                 QRectF Slave)
{
	QRectF SlaveOnline = Slave.translated(Master.center());
	const qreal wi = Slave.width() / 2;
	const qreal hi = Slave.height() / 2;
	SlaveOnline.translate(0 - wi , 0 - hi);
	return SlaveOnline;
}

GraphicsScene::GraphicsScene(QObject * parent)
		: QGraphicsScene(parent),bridge(0)
{
	QApplication::restoreOverrideCursor();
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

GraphicsScene::GraphicsScene(const QRectF & sceneRect, QObject * parent)
		: QGraphicsScene(sceneRect,  parent),bridge(0)
{
	setSceneRect(sceneRect);
	QApplication::restoreOverrideCursor();
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void GraphicsScene::SetVisibleArea(const QRectF area)
{
	emit MakeVisible(area);
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent * e)
{
	return QGraphicsScene::dragMoveEvent(e);
}

void GraphicsScene::clear()
{
	clearSelection();
	QGraphicsScene::clear();
	storno();
}

void GraphicsScene::clearSelection()
{
	QGraphicsScene::clearSelection();
	update(sceneRect());
}

void GraphicsScene::storno()
{
	bridge = 0;
	emit SelectOn(bridge);
}

qreal GraphicsScene::zmax()
{
	qreal mValue = 0.00;
	const QList<QGraphicsItem*> items =  QGraphicsScene::items();
	QList<QGraphicsItem*> subLevelItems;
	/* loop all elemenst */
	foreach(QGraphicsItem* item, items)
	{
		mValue = qMax(item->zValue(),mValue);
		if (item->parentItem())
		{
			QList<QGraphicsItem *> des = item->childItems();
			foreach(QGraphicsItem* de,des)
			{
				subLevelItems << de;
			}
		}
	}
	foreach(QGraphicsItem* it, subLevelItems)
	{
		mValue = qMax(it->zValue(),mValue);
	}
	/* loop all elemenst */
	qDebug() << "### sc maxi" << mValue;

	return mValue;
}

qreal GraphicsScene::zmin()
{
	qreal mValue = zmax();
	const QList<QGraphicsItem*> items =  QGraphicsScene::items();
	QList<QGraphicsItem*> subLevelItems;
	/* loop all elemenst */
	foreach(QGraphicsItem* item, items)
	{
		mValue = qMin(item->zValue(),mValue);
		if (item->parentItem())
		{
			QList<QGraphicsItem *> des = item->childItems();
			foreach(QGraphicsItem* de,des)
			{
				subLevelItems << de;
			}
		}
	}
	foreach(QGraphicsItem* it, subLevelItems)
	{
		mValue = qMin(it->zValue(),mValue);
	}
	/* loop all elemenst */
	qDebug() << "### sc minimum " << mValue;

	return mValue;
}

/* filter only item a top Zindex /  zValue */
bool GraphicsScene::WakeUp(const QPointF incomming)
{
	QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
	qreal thebest = 0;  /* zindex on front */
	for (int o=0;o<listing.size();o++)
	{
		listing[o]->setSelected(false);
		listing[o]->setFlag(QGraphicsItem::ItemIsSelectable,false);
		thebest = qMax(listing[o]->zValue(),thebest);
	}
	///////qDebug() << "### thebest->" << thebest;
	/* activate item at top z-index zValue  /  not the down not visible!!!! */
	for (int e=0;e<listing.size();e++)
	{
		if (listing[e]->zValue() == thebest)
		{
			listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
			listing[e]->setSelected(true);
			bridge = listing[e];
			emit SelectOn(listing[e]);
			return true;
		}
	}
	emit SelectOn(bridge);
	return false;
}

void GraphicsScene::keyPressEvent(QKeyEvent *e)
{
	QGraphicsScene::keyPressEvent(e);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
	{
		event->setAccepted(true) ;
		clearSelection();
	}
	else
	{
		storno();
	}
	QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
	{
		event->setAccepted(true) ;
		clearSelection();
	}
	else
	{
		storno();
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
	{
		event->setAccepted(true) ;
		clearSelection();
	}
	else
	{
		storno();
	}
	QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphicsScene::setSceneRect(const QRectF & rect)
{
	///////Area = rect;
	QGraphicsScene::setSceneRect(rect);
}

QPixmap ImagefromMime(const QMimeData *mime)
{
	QPixmap one;
	QStringList dli = mime->formats();

	if (dli.contains("application/x-picslists"))
	{
		QByteArray dd = mime->data("application/x-picslists");
		QList<TPics> li = OpenImageGroup(QString(dd.data()));
		if (li.size() > 0)
		{
			TPics primoi = li.first();
			return primoi.pix();
		}

	}


	if (dli.contains("text/html"))
	{
		QByteArray dd = mime->data("text/html");
		QTextDocument *picdoc = new QTextDocument();
		picdoc->setHtml(QString(dd.data()));
		/*
		           QMapIterator<QString,TPics> i(imglist());
		               while (i.hasNext()) {
		                   i.next();
		           TPics e  = i.value();
		             picdoc->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
		       }
		*/

		QTextFrame  *Tframe = picdoc->rootFrame();
		QTextFrameFormat rootformats = Tframe->frameFormat();
		rootformats.setBottomMargin(0);
		rootformats.setTopMargin(0);
		rootformats.setRightMargin(0);
		rootformats.setLeftMargin(0);
		rootformats.setPadding(10);
		rootformats.setBorder(1);
		rootformats.setBorderBrush(Qt::black);
		rootformats.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		Tframe->setFrameFormat(rootformats);
		picdoc->adjustSize();
		/* paint doc */
		QAbstractTextDocumentLayout *Layout = picdoc->documentLayout();
		QRectF docirec = Layout->frameBoundingRect(picdoc->rootFrame());
		QPixmap PicsDocument(docirec.size().toSize());
		PicsDocument.fill(Qt::white);
		QRectF clip(0, 0,PicsDocument.width(),PicsDocument.height());
		QPainter *p = new QPainter(&PicsDocument);
		p->setRenderHint(QPainter::Antialiasing, true);
		picdoc->drawContents(p,clip);
		p->end();
		return PicsDocument;
	}

	return one;
}

QPixmap RenderPixmapFromSvgByte(QByteArray streams)
{
	QSvgRenderer  svgRenderer(streams);
	QPixmap pix(svgRenderer.defaultSize());
	pix.fill(Qt::transparent);
	QPainter paint(&pix);
	svgRenderer.render(&paint);
	return pix;
}


/* image get remote */


LoadGetImage::LoadGetImage(int nr , QUrl url_send)
		: QHttp(url_send.host(),QHttp::ConnectionModeHttp ,80)
{
	url = url_send;
	cid = nr;
	setHost(url_send.host() , 80);
}

void LoadGetImage::Start()
{
	const QString METHOD =  "GET";
	const QString agent = QString("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
	header.setRequest(METHOD,url.path(),1,1);
	header.setValue("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");
	header.setValue("Host",url.host());
	header.setValue("User-Agent",agent);
	connect(this, SIGNAL(done(bool)), this , SLOT(ImageReady(bool)));
	Http_id = request(header,0,0);
}

void LoadGetImage::ImageReady(bool error)
{
	if (!error)
	{
		resultimage.loadFromData(readAll());
		if (!resultimage.isNull())
		{
			emit take(cid);
		}
	}
}

void Gloader::Setting(QObject *parent , int id , QUrl url_send)
{
	receiver = parent;
	cid = id;
	url = url_send;
	setTerminationEnabled(true);
}

void Gloader::run()
{
	Rhttp = new LoadGetImage(cid,url);
	connect(Rhttp, SIGNAL(take(int)), receiver , SLOT(in_image(int)));
	Rhttp->Start();
	exec();
}
