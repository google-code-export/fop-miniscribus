#include "Fo_Reader.h"

#include "Config.h"
#include "BasicFoConfig.h"
#include "Fo_Format.h"



    ////TAG_NOT_KNOW = 404,
    ////PAGE_SETUP, /* fo:simple-page-master   */
    ////BLOCK_TAG,    /* fo:block  */
    ////TABLE_TAG,  /* fo:table */
    ///TABLE_FOOTER, /* table-footer  */
    /////TABLE_HEADER, /* table-header */
    //////TABLE_BODY, /* fo:table-body */
    //TABLE_COL, /* fo:table-column */
    ////TABLE_ROW,   /* fo:table-row */
    ///TABLE_CELL, /* fo:table-cell */
    ////BLOCK_CONTAINER,  /* fo:block-container   floating layer */
    /////INLINE_STYLE,  /* fo:inline */
    /////INLINE_CONTAINER,  /* fo:inline-container */
    /////LINK_DOC,  /* fo:basic-link */
    /////IMAGE_SRC, /* fo:external-graphic   all supported qt4.4 image + tif */
    //////IMAGE_INLINE,  /* fo:instream-foreign-object  svg inline */
    /////LIST_BLOCK,   /* fo:list-block  ul/ol */
    /////LIST_ITEM,   /* fo:list-item  li */
    //////LIST_BODY,   /* fo:list-item-body */
    ///////LIST_LABEL,   /* fo:list-item-label  */
    /////FIRST_LAST_TAG,      /* fo:root */
    ///////INLINE_BR,     /* fo:br provisional line breack*/
    ///////FOCHAR,      /* fo:character  */
    /////FOLEADER,      /* fo:leader  inline space  */
    //////FOOTNOTEBOTTOM,            /* fo:footnote - fo:footnote-body */
    //////FOPAGENRCITATION,        /*  fo:page-number-citation*/
    /////////FLOATBLOCK   /* fo:float */









using namespace ApacheFop;



Fo_Reader::~Fo_Reader()
{
    ApiSession *session = ApiSession::instance();
    session->current_Page_Format = PSize();
}

Fo_Reader::Fo_Reader(  const QString readfile , QObject *parent  )
        : Fo_Format( parent ),device( new StreamFop()),
         file(0),Qdoc(new QTextDocument()),LayerCount(0),readerFopVersionDocument(2.0),
         Current_Block_Tree_Level(0),oldMiniScribusFormat(false)
{
    #if QT_VERSION >= 0x040500
    //////netswap = new NetCacheSwap();
    #endif
    
    Remote_Image_List.clear();
    
    doc_cur = 0;
    QFont userfont( QApplication::font() );
    userfont.setPointSize(_DEFAULT_FONT_POINT_SIZE_);
    Qdoc->setDefaultFont ( userfont );
    Tcursor = QTextCursor(Qdoc);
    finfo = QFileInfo(readfile);
    file = finfo.absoluteFilePath();
    ImageCount = 0;
    table_cur = 0;
    LoadFopFile(file);
}

void Fo_Reader::LoadFopFile( const QString readfile )
{
     if (device->LoadFile(readfile))
    {
        if (device->isValid())
        {
            finfo = QFileInfo(readfile);
            const QString ext = finfo.completeSuffix().toLower();
            oldMiniScribusFormat = ext == "fop" ? true : false;
            file = finfo.absoluteFilePath();
            read_dir = QDir(finfo.absolutePath());
            Xdoc = device->Dom();
            const QString foxml = Xdoc.toString (5);
            const QString signature = oldMiniscribusSignature(  foxml );
            const QString builddoc = documentBuild( foxml  );
            if (signature.contains("FOP converter to")) {
            oldMiniScribusFormat = true;
            readerFopVersionDocument = 1.3;
            } else {
            oldMiniScribusFormat = false;
            }
            ////qDebug() << "### signaturesignaturesignaturesignature " << signature;
            /////qDebug() << "### signaturesignaturesignaturesignature " << builddoc;
            /////////////qDebug() << "### signaturesignaturesignaturesignature " << readerFopVersionDocument;
            delete device;
            device = new StreamFop();
            read();
        }
    }
}


void Fo_Reader::read()
{

    QDomElement root = Xdoc.documentElement();
    if (root.tagName() !="fo:root")
    {
        return;
    }
    ApiSession *session = ApiSession::instance();
    session->clearKey();
    Tcursor = QTextCursor(Qdoc);
    
    QDomElement layout_master = root.firstChildElement("fo:layout-master-set");
    QDomElement layout = layout_master.firstChildElement("fo:simple-page-master");
    qreal pwi = Unit(layout.attribute ("page-width",QString("210mm")));
    qreal phi = Unit(layout.attribute ("page-height",QString("297mm")));
    PageSize = session->FindPagePsize(QRect(0,0,pwi,phi));
    QString yourname = layout.attribute("master-name");
    const qreal largefront = qMax (pwi,phi);
    
    FoRegion tmp = regionFormat(layout);
    FoRegion tmpbody = regionFormat(layout.firstChildElement("fo:region-body"));
    FoRegion masterbod;
             masterbod.fomax(tmp,tmpbody);
    
    PageSize.body = masterbod;
    ////FoRegion region_before() 
    PageSize.area[0] = regionFormat(layout.firstChildElement("fo:region-before"));
    /////FoRegion region_after() const { return 
    PageSize.area[1] = regionFormat(layout.firstChildElement("fo:region-after"));
    //////FoRegion region_start() const  { return 
    PageSize.area[2] = regionFormat(layout.firstChildElement("fo:region-start"));
    ///////FoRegion region_end()  const { return 
    PageSize.area[3] = regionFormat(layout.firstChildElement("fo:region-end"));
    


    
    /////return;
    
    
   
    
    Docwidth = pwi - PageSize.width();
    session->CurrentDocWidth = Docwidth;
    ///////Qdoc->setPageSize ( QSizeF( Docwidth , phi - MarginPage.x() - MarginPage.width() ) );
    PageSize.HandlePrint(Qdoc);
    QDomElement master = root.firstChildElement("fo:page-sequence");
    
    QString  tite = master.firstChildElement("fo:title").text();
    if (tite.size() > 4) {
       PageSize.doctitle =   tite;
    }
    
    
    
    Tcursor.setPosition(0);
    /* header & footer here ! */
    QDomElement page = master.firstChildElement("fo:flow");
    /////////qDebug() << "### Docwidth  " << Docwidth;
    ///////////qDebug() << "### START READ DOCUMENT ################################################################";
    RootFramePaint(page);
    /*
    QString debugtext = session->DebugAttributes();
    Tcursor.setPosition(doc_cur);
    Tcursor.beginEditBlock();
    Tcursor.setCharFormat(DefaultCharFormats());
    Tcursor.insertText(debugtext);
    Tcursor.endEditBlock();
    Tcursor.atBlockEnd();
    */
    
    
    session->current_Page_Format = PageSize;
    session->AppendPaper(PageSize);
    PageSize.HandlePrint(Qdoc);
    QTextFrame  *RootFrame = Qdoc->rootFrame();
    QTextFrameFormat rf = RootFrame->frameFormat(); 
    const QRectF allrect = Qdoc->documentLayout()->frameBoundingRect(RootFrame);
    const int blocksumme = Qdoc->blockCount();
    
    const qreal altouno = PageSize.height();
    if (readerFopVersionDocument < 1.5 && blocksumme > 0 ) {
      /* error not possible to having block this version !!!!!  */
        qWarning() << "This doc version not having block normal! " << blocksumme << "Doc Version :" << readerFopVersionDocument;
        
        if (blocksumme < 3) {
            int pagefill = 1;
            MoveEndDocNow();
            QPointF lastpoint(200.,layerYCurrentRead);
            for (int x = 0; x < MaximumPages; ++x)  {
                QRectF caser = PageSize.PageExternal(x);
                if (caser.contains(lastpoint)) {
                  pagefill = x;  
                }
            }
            
            
            for (int i = 0; i < pagefill; ++i)  {
               appendPageParaBreak(Tcursor);
            }
            
            /* small piece not fill a page */
            if (pagefill == 0) {
               appendPageParaBreak(Tcursor,false);  
            }
            
        }
        
        
    }
    
    doc_cur = Tcursor.position();
    ////////qDebug() << "### layerYCurrentRead  " << layerYCurrentRead << "--------------------------------------------------------";
    /////////qDebug() << "### blockcount  " << Qdoc->blockCount() << "--------------------------------------------------------";
    ///////////qDebug() << "### blockcount  " << allrect.toRect() << "--------------------------------------------------------";
     
}


void Fo_Reader::appendPageParaBreak( QTextCursor Cinline , bool fullsize )
{
    Cinline.movePosition(QTextCursor::End);
    Cinline.insertBlock();
                Cinline.beginEditBlock();
                Cinline.insertText(" \t");
                if (fullsize) {
                Cinline.setBlockFormat(PageBreackBlock());
                }
                Cinline.endEditBlock();
   //////////// qDebug() << "### appendPageParaBreak  action -------------------------------------------------------";
}



bool Fo_Reader::placeNewAbsoluteLayer( const QDomElement e )
{
    QStringList attri = attributeList(e);
    const int rotateD = e.attribute("reference-orientation","0").toInt();
    qreal LargeWi = Unit(e.attribute("width","-1"));
    qreal lleft = Unit(e.attribute("left","-1"));
    qreal ltop = Unit(e.attribute("top","-1"));
    qreal LargeHi = Unit(e.attribute("height","-1"));

    LayerCount++;
    QMap<QString,SPics> list;
    
    attri.append(QString("docversion:%1").arg(readerFopVersionDocument));
    
    
    
    
    
    QTextDocument *ldoc = new QTextDocument();
    QTextCursor layercursor(ldoc);
    FrameDomIterator(e.firstChild(),layercursor);
    
    QTextFrame  *RootFrame = ldoc->rootFrame();
    QTextFrameFormat rf = RootFrame->frameFormat(); 
    const QRectF rect = ldoc->documentLayout()->frameBoundingRect(RootFrame);
    const qreal lastLayerFromtop = layerYCurrentRead;
    
    if ( readerFopVersionDocument > 1.5) {
    
        /* not first layer */
        if (LayerCount !=1 && e.attribute("top").isEmpty() ) {
        layerYCurrentRead = qMax(lastLayerFromtop + rect.height(),layerYCurrentRead);
        layerYCurrentRead = qMax(lastLayerFromtop + LargeHi,layerYCurrentRead);
        }
        
        if (LargeHi < 1) {
        attri.append(QString("height:%1pt").arg(rect.height()));
        attri.append(QString("display-align:auto"));  
        }
        if (ltop < 1 && e.attribute("top").isEmpty()) {
        attri.append(QString("top:%1pt").arg(layerYCurrentRead));
        attri.append(QString("display-align:auto")); 
        }
        if (lleft < 1 && e.attribute("left").isEmpty() ) {
        attri.append(QString("left:%1pt").arg(PSize().body.margin_left));
        attri.append(QString("display-align:auto")); 
        }
    
    } else {
        
        layerYCurrentRead = qMax(ltop + LargeHi,layerYCurrentRead);
        
    }
    
    /* unique attributes */
    const QString style = attri.join(";");
    //////qDebug() << "### inite an absolute  " << style;
    
    
    
    RichDoc xlayer;
    xlayer.Register(ldoc,list,style);
    layerList.insert(LayerCount,xlayer);
    return true;
}

void Fo_Reader::RootFramePaint( const QDomElement e )
{
    if (e.tagName() !="fo:flow")
    {
        return;
    }
    Tcursor = QTextCursor(Qdoc);
    Qdoc->clear();
    DocRootFrameDefault();
    MoveEndDocNow();
    ////////////qDebug() << "### root move cursor " << doc_cur;
    doc_cur = Tcursor.position();
    QDomNode child = e.firstChild();
    QDomElement LastElementParent = e;
    int tagposition = -1;

    while ( !child.isNull() )
    {
        if ( child.isElement() )
        {
            const QDomElement el = child.toElement();
            tagposition++;

            if ( tagposition == 0 )
            {

            }



            ////////////qDebug() << "### FRAME/Root  loop Iterator  " << el.tagName() << " pos." << tagposition;

            if ( FoTag(el) == INLINE_BR )
            {
                ///////////qDebug() << "### root line breack............................... ";
                Tcursor.insertText(QString(QChar::LineSeparator));
                MoveEndDocNow();
            }
            else if (FoTag(el) == FOLEADER)
            {
                FoLeaderPaint(el,Tcursor);
                MoveEndDocNow();
            }
            else if (FoTag(el) == BLOCK_TAG)
            {
                FoBlockTagPaint(el,Tcursor);  /* root blocks */
                MoveEndDocNow();
                ////////////qDebug() << "### paragraph  " << tagposition << " ------------------------------------------------------------------------------";
            }
            else if ( FoTag(el) == FOOTNOTEBOTTOM  )
            {
                //////FootNoteSave(el,Tcursor);
                ///////MoveEndDocNow();
            }
            else if (FoTag(el) == BLOCK_CONTAINER)
            {
                FoBlockContainerPaint(el,Tcursor);
                MoveEndDocNow();
            }
            else if (FoTag(el) == TABLE_TAG)
            {
                FoTableTagPaint(el,Tcursor);
                MoveEndDocNow();
            }
            else if (FoTag(el) == LIST_BLOCK)
            {
                FoListUlTagPaint(el);
                MoveEndDocNow();
            }
            else if ( FoTag(el) == FLOATBLOCK  )
            {
                /* root frame conflict insert para null*/
                Tcursor.beginEditBlock();
                Tcursor.setBlockFormat(DefaultMargin());
                doc_cur = Tcursor.position();
                bool Bx =  FoFloatingContainerPaint(el,LastElementParent);
                Tcursor.endEditBlock();
                MoveEndDocNow();
            }
            else
            {
                /* not relax tag */
                if (FoTag(el) == TAG_NOT_KNOW )
                {
                    DidplayUnknowTag(el,Tcursor);
                }



                MoveEndDocNow();
            }

            LastElementParent = el;
        }
        child = child.nextSibling();
    }

    doc_cur = Tcursor.position();
    MoveEndDocNow();

    ///////////qDebug() << "### END READ DOCUMENT ################################################################";
}
/* only root block */

bool Fo_Reader::RunningFirstFrame( QTextCursor Cursor )
{
    QTextFrame *rootframe = Qdoc->rootFrame();
    int CursorFirstFrame = rootframe->firstPosition();
    return CursorFirstFrame == Cursor.position() ? true : false;
}



bool Fo_Reader::FoBlockTagPaint( const QDomElement e , QTextCursor Cursor )
{
    Current_Block_Tree_Level = 0;
    if (FoTag(e) != BLOCK_TAG)
    {
        return false;
    }
    QTextBlockFormat dd;
    QTextCharFormat cc;
    ///////Cursor.setBlockFormat(dd);  /* try to reset !!!! */
    Cursor.setCharFormat(cc);  /* try to reset !!!! */
    
    const QTextBlockFormat blf = TextBlockFormFromDom(e, DefaultMargin()); 
    
    if (FoTag(e) == INLINE_BR)
    {
        return false;
    }
    if (RunningFirstFrame(Cursor))
    {
        ///////////////qDebug() << "#### First root block " << __LINE__;
    }
    else
    {
        Cursor.insertBlock();
        Cursor.setBlockFormat(blf);
    }
    return InlineBlockLoop(e,Cursor);
}


bool Fo_Reader::FoLeaderPaint( const QDomElement e  , QTextCursor Cursor )
{
    if (FoTag(e) != FOLEADER)
    {
        return false;
    }
    /*   check if is a self writteln fo:leader to emulate a breack line or empity paragraph !!!!  */  
    if ( e.attribute("leader-pattern") == "space" && 
        e.attribute("leader-length") == RecoveryBreackLineParagraph() && 
        AllowtoBreack(e)) {
      /* breack line br check line hight ???? */
       Cursor.setBlockFormat(DefaultMargin());
       Cursor.insertText(QString(QChar::Nbsp),DefaultCharFormats());
       Cursor.setCharFormat ( DefaultCharFormats() );
       Cursor.endEditBlock();
       Cursor.atBlockEnd();
      return true;            
    }

    /* draw space */
    FopLeader space;
    space.read(e,Docwidth);
    const QString namespaceimg = space.Name();
    /* dispay */
    QPixmap spaceimg = space.DummyImage();
    QUrl recresourcein(namespaceimg);
    Qdoc->addResource( QTextDocument::ImageResource,recresourcein,spaceimg);

    SPics ximg;
    ximg.name = namespaceimg;
    ximg.set_pics(spaceimg);
    /* insert pixel 1 x1 */
    QTextImageFormat format;
    format.setName( ximg.name );
    format.setHeight( spaceimg.height() );
    format.setWidth ( spaceimg.width() );
    format.setProperty(LeaderNummer,space);
    Cursor.insertImage( format );
    if (space.leaderpattern == 3)
    {
        /////leader-pattern =   space | rule | dots | use-content | inherit
        FrameDomIterator(e.firstChild(),Cursor);
        return false;
    }

    return false;
}

void Fo_Reader::DidplayUnknowTag( const QDomElement e  , QTextCursor Cursor )
{
    const QString tagname = e.tagName().toLower();
    QDomDocument wrax = DomelenentToString(e,QString("Unknow Tag %1: please send a report to author if this tag is important! try  to Fop file to ispect this tag.").arg(tagname));
    QString pretag = Qt::escape(wrax.toString(1));
    pretag.prepend("<pre>");
    pretag.append("</pre>");
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(pretag);
    Cursor.setCharFormat ( PreFormatChar() );
    Cursor.insertFragment(fragment);
    Cursor.setCharFormat ( PreFormatChar() );
}


bool Fo_Reader::InlineSpanning( const QDomElement e , const QDomElement parentup  , QTextCursor Cinline , bool HandleSpace )  /* default false */
{

    ////////////qDebug() << "### InlineSpanning................................  ";
    
    if (FoTag(e) != INLINE_STYLE)
    {
        return false;
    }

    if (FoTag(e) == INLINE_BR)
    {
        return false;
    }
    Current_Block_Tree_Level++;

    QTextBlockFormat Paraformat = TextBlockFormFromDom(parentup,DefaultMargin());
    QTextCharFormat Charformat = GlobalCharFormat(parentup,DefaultCharFormats());
    const QTextCharFormat InlineStyle = GlobalCharFormat(e,Charformat);

    Cinline.setCharFormat(InlineStyle);
    QStringList texter = e.text().split("\n");
    for (int i = 0; i < texter.size(); ++i)
    {
        if (HandleSpace)
        {
            Cinline.insertText(foptrimmed(texter.at(i),e.attribute("text-transform")));
        }
        else
        {
            Cinline.insertText(texter.at(i));
        }
        if (i > 0)
        {
            Cinline.insertText(QString(QChar::Nbsp)); /////
        }
    }

    doc_cur = Cinline.position();

    QDomNode child = e.firstChild();
    QDomElement LastElementParent = e;
    while ( !child.isNull() )
    {

        if ( child.isElement() )
        {
            const QDomElement childElement = child.toElement();
            const QTextCharFormat InlineStyle = GlobalCharFormat(childElement,Charformat);
            if ( FoTag(childElement) == INLINE_BR )
            {
                Cinline.insertText(QString(QChar::LineSeparator));
            }
            else if ( FoTag(childElement) == INLINE_STYLE)
            {
                InlineSpanning(childElement,parentup,Cinline,HandleSpace);
                Cinline.setCharFormat(Charformat);
            }
            else if ( FoTag(childElement) == LINK_DOC )
            {
                Cinline.setCharFormat(InlineStyle);
                Cinline.insertText(foptrimmed(childElement.text(),childElement.attribute("text-transform")));
                Cinline.setCharFormat(Charformat);
            }

        }
        doc_cur = Cinline.position();
        child = child.nextSibling();

    }


    doc_cur = Cinline.position();
    return true;
}

void Fo_Reader::FootNoteSave( const QDomElement e  , QTextCursor Cursor )
{
    /* register tag to resave  */
    return;
}

bool Fo_Reader::InlineBlockLoop( const QDomElement e , QTextCursor Cinline , bool skipborder )  /* default false */
{

    if (FoTag(e) != BLOCK_TAG)
    {
        return false;
    }

    if (FoTag(e) == INLINE_BR)
    {
        return false;
    }
    ApiSession *sx = ApiSession::instance();
    Current_Block_Tree_Level++;
    const int OriginalPos = Cinline.position();
    if (RunningFirstFrame(Cinline))
    {
        Cinline.beginEditBlock();
        Cinline.setBlockFormat(DefaultMargin());
    }
    QTextBlock bf = Cinline.block();
    Cinline.beginEditBlock();
    QTextBlockFormat bbformat = Cinline.block().blockFormat();
    const QTextBlockFormat blf = TextBlockFormFromDom(e,bbformat); 
    
    bbformat.setAlignment(  TagAlignElement(e) ); /////TagAlignElement(e)
    QTextCharFormat  Charformat;
    Charformat = GlobalCharFormat(e,DefaultCharFormats());
    /* HandleSpace default forever true */
    bool HandleSpace = e.attribute("white-space-collapse","true") == "true" ? true : false;
    bool NoWrapLine = e.attribute("wrap-option","wrap") == "wrap" ? false : true;

    if (!HandleSpace)
    {
        bbformat.setProperty(QTextFormat::BlockNonBreakableLines,1);
        bbformat.setNonBreakableLines(true);
    }

    if (NoWrapLine)
    {
        bbformat.setNonBreakableLines(true);
        HandleSpace = false;
    }
    int BlockNummer = Cinline.block().position();
    ///////////qDebug() << "### InlineBlockLoop top margin  " << bbformat.topMargin() << "BB nr.->" << BlockNummer << "BB tree level->" << Current_Block_Tree_Level;
    Cinline.setBlockFormat(blf);
    Cinline.setCharFormat(Charformat);

    QDomNode child = e.firstChild();
    QDomElement LastElementParent = e;
    int tagposition = -1;
    while ( !child.isNull() )
    {
        tagposition++;

        if ( child.isElement() )
        {
            const QDomElement childElement = child.toElement();
            const QTextCharFormat InlineStyle = GlobalCharFormat(childElement,Charformat);
            
           //////////// qDebug() << "### BLock loop Iterator  " << childElement.tagName() << " pos." << tagposition;

            if ( !childElement.hasAttributes()  && !childElement.hasChildNodes() )
            {
                ///////////////Cinline.insertText(QString(QChar::LineSeparator));   /* null tag !!!!!!! only name */
            } 
            
            if ( FoTag(childElement) == INLINE_STYLE)
            {
                InlineSpanning(childElement,e,Tcursor,HandleSpace);
                Cinline.setCharFormat(Charformat);
            }
            else if ( FoTag(childElement) == INLINE_BR )
            {
                Tcursor.insertText(QString(QChar::LineSeparator));
            }
            else if ( FoTag(childElement) == FOPAGENRCITATION )
            {
                Tcursor.insertText(".");
            }
            else if ( FoTag(childElement) == FOLEADER )
            {
                bool brline = FoLeaderPaint(childElement,Tcursor);
                if (brline) {
                 return true;
                }
            }
            else if ( FoTag(childElement) == FOOTNOTEBOTTOM  )
            {

                /* register tag to resave  */
                FopLeader footnote;
                footnote.read(childElement,Docwidth);
                bbformat.setProperty(FootNoteNummer,footnote);
                Cinline.setBlockFormat(bbformat);

                //////qDebug() << "### FootNoteSave on block  " << footnote.ELoriginal;
                /////FootNoteSave(childElement,Tcursor);
            }
            else if ( FoTag(childElement) == LINK_DOC )
            {
                Tcursor.setCharFormat(InlineStyle);
                Tcursor.insertText(foptrimmed(childElement.text(),childElement.attribute("text-transform")));
                Tcursor.setCharFormat(Charformat);
            }
            else if ( FoTag(childElement) == FOPAGENRPRINT )
            {
                Tcursor.setCharFormat(InlineStyle);       ////////// FOPAGENRPRINT
                Tcursor.insertText(_PAGE_NUMERATION_);
                Tcursor.setCharFormat(Charformat);
            }
            else if ( FoTag(childElement) == FOCHAR  )
            {
                /* One letter format make frame float left right?  fo:page-number*/
                Cinline.setCharFormat(InlineStyle);
                Cinline.insertText(childElement.attribute ("character"));
                Cinline.setCharFormat(Charformat);
            }
            else if ( FoTag(childElement) == BLOCK_CONTAINER)
            {
                /* inline frame elements */
                FoBlockContainerPaint(childElement,Cinline);
                
            } else if ( FoTag(childElement) == BLOCK_TAG)
            {
                /* inline frame elements */
                InlineBlockLoop(childElement,Cinline);
                
            } else if ( FoTag(childElement) == TABLE_TAG  )
            {
                FoTableTagPaint( childElement , Cinline );
            }
            else if ( FoTag(childElement) == IMAGE_INLINE  )
            {
                FoDrawSvgInline( childElement , Cinline );
                /////////qDebug() << "##   svg image paint    ";
            }
            else if ( FoTag(childElement) == IMAGE_SRC  )
            {
               //////////// qDebug() << "##   normal image paint1   ";
                
                FoDrawImage( childElement , Cinline );
            }
            else if ( FoTag(childElement) == FLOATBLOCK  )
            {
                FoFloatingContainerPaint(childElement,LastElementParent);
            }
            else
            {
                /* not relax tag */
                if (FoTag(childElement) == TAG_NOT_KNOW )
                {
                    DidplayUnknowTag(childElement,Cinline);
                }

            }

            /////FoDrawImage(  const QDomElement e )
            LastElementParent = childElement;

        }
        else if (child.isCDATASection())
        {
            const QDomCDATASection ChunkPRE = child.toCDATASection();
            QString pretag = Qt::escape(child.nodeValue());
            pretag.prepend("<pre>");
            pretag.append("</pre>");
            QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(pretag);
            
            Cinline.setCharFormat ( PreFormatChar() );
            Cinline.insertFragment(fragment);
            Cinline.setCharFormat ( PreFormatChar() );
            
            
            bbformat.setProperty(QTextFormat::BlockNonBreakableLines,1);
            bbformat.setNonBreakableLines(true);
        }
        else if (child.isText())
        {
            /* plain text normal */
            ///////const QDomText txtNode = child.toText()()
            QString paratext = child.nodeValue();
            QStringList texter = paratext.split("\n");
            if (texter.size() > 0)
            {

                for (int i = 0; i < texter.size(); ++i)
                {
                    if (HandleSpace)
                    {
                        Cinline.insertText(foptrimmed(texter.at(i),e.attribute("text-transform")));
                    }
                    else
                    {
                        Cinline.insertText(texter.at(i));
                    }
                    ///////qDebug() << "### block text  " << texter.at(i).simplified();

                    if (i > 0)
                    {
                        Cinline.insertText(QString(QChar::Nbsp)); /////
                    }
                }

            }
            else
            {
                Cinline.insertText("ERROR DOM......");
            }


        }

        child = child.nextSibling();
    }


    
    if (!e.attribute("id").isEmpty()) {
    QString uniquechecki = sx->validateUnique(e.attribute("id"));
    bbformat.setProperty(BookMarkInternalID,uniquechecki);
    }
    Cinline.setBlockFormat(bbformat);
    Cinline.endEditBlock();
    Cinline.atBlockEnd();
    return true;
}











bool Fo_Reader::FoBlockContainerPaint( const QDomElement e , QTextCursor Cinline  )
{
    if (FoTag(e) != BLOCK_CONTAINER)     /* block in block make frame */
    {
        return false;
    }
    if (FoTag(e) == INLINE_BR)
    {
        return false;
    }
    FopLeader xx;
    xx.read(e,Docwidth);
    QColor bgcolor = QColor(Qt::white);
    ///////////qDebug() << "### fo:block-containerPaint a " << e.tagName();
    ///////if (IsAbsoluteLayer(e))
    const int rotateD = e.attribute("reference-orientation","0").toInt();
    qreal LargeWi = Unit(e.attribute("width","0"));
    qreal lleft = Unit(e.attribute("left","0"));
    qreal ltop = Unit(e.attribute("top","0"));
    QTextLength wide = BlockMesure(e);
    ///////////////qDebug() << "### fo:block-containerPaint c " << wide.rawValue();
    
    /* check if absolute */
    if (wide.rawValue() > 9 && !e.attribute("left").isEmpty() && !e.attribute("top").isEmpty()) {
    return placeNewAbsoluteLayer(e);
    }
    
    
    
    QTextFrameFormat frame = xx.format;
    frame.setWidth(wide);
    QTextFrameFormat FrameFormat = PaintFrameFormat(e,frame);


    if (!e.attribute("background-color").isEmpty())
    {
        bgcolor = ColorFromFoString( e.attribute("background-color") );
    }
    QDomElement  firstPara = e.firstChildElement("fo:block");
    if (!firstPara.isNull())
    {
        if (!firstPara.attribute("background-color").isEmpty())
        {
            bgcolor = ColorFromFoString(firstPara.attribute("background-color") );
        }
    }

    FrameFormat.setBackground(bgcolor);
    QTextFrame *Divframe = Cinline.insertFrame(FrameFormat);
    doc_cur = Divframe->firstPosition();
    Cinline.setPosition(doc_cur);
    FrameDomIterator(e.firstChild(),Cinline);
    return true;
}


void Fo_Reader::PaintMessage(   QTextCursor Curs , const QString msg )
{
    Curs.insertBlock();
    Curs.setBlockFormat(DefaultMargin());
    Curs.insertText(QString("Error Message:\n%1").arg(msg),AlertCharFormats());
    Curs.endEditBlock();
    Curs.atBlockEnd();
    doc_cur = Curs.position();

}



bool Fo_Reader::FoListUlTagPaint( const QDomElement e  )
{
    ////////////qDebug() << "### FoListUlTagPaint aaaa " << e.tagName();
    if (FoTag(e) != LIST_BLOCK)
    {
        return false;
    }
    bool fattoblocco = false;
    int lisumm = 0;

    Tcursor.setPosition(doc_cur);
    QTextBlock bf = Tcursor.block();
    QTextBlockFormat format = TextBlockFormFromDom(e,bf.blockFormat());
    Tcursor.setBlockFormat(format);


    QTextListFormat  formatul = TextListFromFromDom(e);
    formatul.setStyle ( QTextListFormat::ListDisc );
    formatul.setIndent(1);
    QDomElement litem = e.firstChildElement("fo:list-item");

    QTextList *Uls =  Tcursor.insertList( formatul );

    while ( !litem.isNull() )
    {
        QDomElement libody = litem.firstChildElement("fo:list-item-body");
        QDomElement sublock = libody.firstChildElement("fo:block");
        if (!sublock.isNull())
        {
            lisumm++;
            if (lisumm > 0 )
            {
                Tcursor.insertBlock();
            }
            fattoblocco = InlineBlockLoop(sublock,Tcursor);
            Uls->add( Tcursor.block() );
           ///////////////// qDebug() << "### li  " << fattoblocco << " nr.->" << lisumm;
        }
        litem = litem.nextSiblingElement("fo:list-item");
    }
    Uls->removeItem(0);
    //////////qDebug() << "### doc_cur at list  " << doc_cur;
    return true;
}
















bool Fo_Reader::FoDrawImage(  const QDomElement e , QTextCursor Cursor )
{
    ImageCount++;
    const QString LastPathToRestore = QDir::currentPath();    /* reset current dir to last */
    
    qDebug() << "##   current path image sssssssss   " << LastPathToRestore;
    
    
    bool RunningFromFile = read_dir.exists();
    QDateTime timer1( QDateTime::currentDateTime() );
    const QString TimestampsMs = QString("%1-%2-file").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
    qreal wi = qMax ( Unit( e.attribute( "width" , "0" ),IMAGE) , Unit( e.attribute( "content-width" , "0" ),IMAGE));
    qreal hi =  qMax ( Unit( e.attribute( "height" , "0"),IMAGE) , Unit( e.attribute( "content-height" , "0" ),IMAGE));
    qreal havingnummer = qMax(wi,hi);
    QSize imageSize;
    imageSize = QSize(wi,hi);
    if (!wi > 0 && !hi > 0)
    {
        imageSize = QSize(havingnummer,havingnummer);
    }
    if (havingnummer < 1)
    {
        imageSize = QSize(20,20);
    }


    QString scaling = e.attribute("scaling","0");

    /* dont scale if not having set nothing width / height */
    if (havingnummer < 1)
    {
        scaling = "0";
    }
    QString FileAbsolute = 0;
    QString FileSuffix = "PNG";
    QString FileBaseName = "NotFound";

    ApiSession *session = ApiSession::instance();

    QDir::setCurrent(read_dir.absolutePath());

    QByteArray              derangedata;
    QPixmap                 resultimage;
    QPixmap                 scaledsimage;
    QString                 resourceName;

    SPics ximg;

    const QString hrefimageplace = ImagesrcUrl(e);
    QUrl imageurl(hrefimageplace );
    //////////////qDebug() << "##   dir readdddddddddddddddddddddddddddddddddd   " << read_dir.absolutePath();
    //////////////////qDebug() << "##   image url   " << imageurl;




    if (hrefimageplace.startsWith("http://", Qt::CaseInsensitive) ||
            hrefimageplace.startsWith("https://", Qt::CaseInsensitive) ||
            hrefimageplace.startsWith("ftp://", Qt::CaseInsensitive))
    {
        resourceName = hrefimageplace;
        Remote_Image_List.append(resourceName);
        /* grab */
        resultimage = QPixmap(":/img/File_down.png");
        ximg.set_pics( resultimage );
        derangedata = ximg.streams();
        RunningFromFile = false;
        #if QT_VERSION >= 0x040500
        ///////netswap->start_Get(imageurl);
        #endif
        
        
    }

    if (RunningFromFile)
    {

        QFileInfo imageFix(hrefimageplace);
        if (imageFix.exists())
        {
            FileAbsolute = imageFix.absoluteFilePath();

            //////////qDebug() << "##   open file   " << FileAbsolute;

            FileSuffix = imageFix.completeSuffix().toLower();

            /////////////////qDebug() << "##   FileSuffix   " << FileSuffix;

            FileBaseName = imageFix.baseName();
            if (FileSuffix.endsWith(".gz"))
            {
                derangedata = OpenGzipOneFileByte( imageFix.absoluteFilePath() );
            }
            else
            {
                /* normal image file */
                QFile f(imageFix.absoluteFilePath());
                if (f.open(QIODevice::ReadOnly))
                {
                    derangedata = f.readAll();
                    f.close();
                }
            }




           /////////////// qDebug() << "##   derangedata   " << derangedata.size();

            if (FileSuffix.contains("sv"))
            {
                resourceName = QString("/svg/%1/%2/file/%3").arg(TimestampsMs).arg(ImageCount).arg(imageFix.absoluteFilePath());
                resultimage = RenderPixmapFromSvgByte(  derangedata );
                session->SvgList.insert(resourceName,derangedata);
            }
            else
            {
                resourceName = QString("/png/%1/%2/file/%3").arg(TimestampsMs).arg(ImageCount).arg(imageFix.absoluteFilePath());
                resultimage.loadFromData( derangedata );
            }
        }
        QDir::setCurrent(LastPathToRestore);
    }

    /* image is here or null */
    if (resultimage.isNull())
    {
        ///////imageSize
        QPixmap notfound(":/img/image-file-48x48.png");
        resultimage = notfound.scaled(imageSize,Qt::IgnoreAspectRatio);
        wi = resultimage.width();
        hi = resultimage.height();
        scaling = "0";
    }

    if (wi !=0 && wi !=resultimage.width() && scaling=="0")
    {
        scaling = "uniform";
    }
    if (hi !=0 && hi !=resultimage.height() && scaling=="0")
    {
        scaling = "uniform";
    }
    /* scaling image size ?? */
    if (scaling == "uniform")
    {
        if (wi !=0)
        {
            scaledsimage = resultimage.scaledToWidth(wi);
        }
        else if (hi !=0)
        {
            scaledsimage = resultimage.scaledToHeight(hi);
        }
    }
    else if (scaling == "non-uniform" && wi!=0 && hi!=0)
    {
        scaledsimage = resultimage.scaled(wi,hi,Qt::IgnoreAspectRatio);
    }
    else
    {
        scaledsimage = resultimage;
    }

    QByteArray extensionCurrent;
    extensionCurrent.append(FileSuffix.toUpper());
    ximg.set_pics(scaledsimage);
    ximg.name = FileBaseName;
    ximg.extension = extensionCurrent;
    QUrl recresourcein(resourceName);
    
    
    
    
    
    
    
    session->ImagePageList.insert(resourceName,ximg);
    
    QTextDocument *currentdoc = const_cast<QTextDocument *>(Cursor.block().document());
    currentdoc->addResource( QTextDocument::ImageResource,recresourcein,scaledsimage);
    
    
    QTextImageFormat format;
    format.setName( resourceName );
    format.setHeight ( scaledsimage.height() );
    format.setWidth ( scaledsimage.width() );
    format.setToolTip(ximg.info);
    format.setProperty(_IMAGE_PICS_ITEM_,ximg);
    Cursor.insertImage( format );
    QTextBlock bf = Cursor.block();
    QTextBlockFormat bbff = bf.blockFormat();
    if ( TagAlignElement(e) != Qt::AlignLeft )
    {
        bbff.setAlignment( TagAlignElement(e) );
        Cursor.setBlockFormat(bbff);
    }
    QDir::setCurrent(LastPathToRestore);
    /////////////qDebug() << "##   dir restore dirrrrrrrrrrrrrrrrrrrr   " << LastPathToRestore;
    return true;
}


bool Fo_Reader::FoDrawSvgInline( const QDomElement e  , QTextCursor Cursor  )
{
    if (FoTag(e) !=IMAGE_INLINE)
    {
        return false;
    }
    ImageCount++;
    ImageCount++;
    ImageCount++;

    QDateTime timer1( QDateTime::currentDateTime() );
    const QString TimestampsMs = QString("%1-%2-stream").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
    QString j_op( QString::number( qrand() % 1000 ) );
    const QString resourceName = QString("/svg/%1/%2/%3").arg(TimestampsMs).arg(ImageCount).arg(j_op);
    SPics ximg;


    qreal wi = qMax ( Unit( e.attribute( "width" , "0" ),IMAGE) , Unit( e.attribute( "content-width" , "0" ),IMAGE));
    qreal hi =  qMax ( Unit( e.attribute( "height" , "0"),IMAGE) , Unit( e.attribute( "content-height" , "0" ),IMAGE));

    QDomElement domObject = e.firstChildElement();
    if ( domObject.tagName().toLower() == "svg:svg" || domObject.tagName().toLower() == "svg" )
    {



        QPixmap paintsvg = RenderSvg(e,resourceName);   /* go in session image resource here on svg format */
        QPixmap scaler;
        if (wi > 0)
        {
            scaler = paintsvg.scaledToWidth(wi);
        }
        else if (hi > 0)
        {
            scaler = paintsvg.scaledToHeight(hi);
        }
        else
        {
            scaler = paintsvg;
        }

        ApiSession *session = ApiSession::instance();  /* back up image resource as normal not svg to write on save ... */

        ximg.set_pics(scaler);
        ximg.name = Imagename( resourceName );
        ximg.info = "Inline svg image";

        session->ImagePageList.insert(resourceName,ximg);   /* need only if inline original svg stream can not repopulate on save */
        
        QTextDocument *currentdoc = const_cast<QTextDocument *>(Cursor.block().document());
        currentdoc->addResource( QTextDocument::ImageResource,QUrl(resourceName),scaler);
        
        
        QTextImageFormat format;
        format.setName( resourceName );
        format.setWidth ( scaler.width() );
        format.setHeight ( scaler.height() );

        format.setToolTip(ximg.info);
        format.setProperty(_IMAGE_PICS_ITEM_,ximg);

        Cursor.insertImage( format );  /* cursor insert image QPixmap */

        QTextBlock bf = Tcursor.block();
        QTextBlockFormat bbff = bf.blockFormat();

        if ( TagAlignElement(e) != Qt::AlignLeft )
        {
            bbff.setAlignment( TagAlignElement(e) );
            Cursor.setBlockFormat(bbff);
        }

        return true;
    }
    else
    {
        return false;
    }
}



void Fo_Reader::MoveEndDocNow()
{
    Tcursor.movePosition(QTextCursor::End);
    doc_cur = Tcursor.position();
////qDebug() << "### MoveEndDocNow  " << doc_cur;
}

void Fo_Reader::DocRootFrameDefault()
{
    QTextFrame  *Tframe = Qdoc->rootFrame();
    QTextFrameFormat Ftf = Tframe->frameFormat();
    Ftf.setLeftMargin(0);
    Ftf.setBottomMargin(0);
    Ftf.setTopMargin(0);
    Ftf.setRightMargin(0);
    Ftf.setPadding ( 0);
    Tframe->setFrameFormat(Ftf);
}



bool Fo_Reader::FoChildBlockPaint( const QDomElement e , bool recursive )
{
    if (FoTag(e) !=BLOCK_TAG)     /* block in block make frame */
    {
        return false;
    }
    QDomElement page = e.firstChildElement("fo:block");
    if (page.isNull())
    {
        return false;
    }
    if (FoTag(page) == INLINE_BR)
    {
        return false;
    }
    Tcursor.setPosition(doc_cur);
    QDomNode child = e.firstChild();
    if (child.isText() )
    {
        Tcursor.insertBlock();
        bool firstBl = InlineBlockLoop(e,Tcursor,true);
        /////////////qDebug() << "### first parent before frame block block   " << firstBl;
    }
    doc_cur = Tcursor.position();
    return FoFloatingContainerPaint(page,e);   /* go page while down format from parent up e */
}

bool Fo_Reader::FoFloatingContainerPaint( const QDomElement e , const QDomElement parentup )
{
    ////////////qDebug() << "###floatfloatfloat..a." << e.tagName().toLower();
    ////////////////qDebug() << "###floatfloatfloat..b." << parentup.tagName().toLower();

    bool take = false;
    bool isBlockBlock = (FoTag(e) == BLOCK_TAG && FoTag(parentup) == BLOCK_TAG) == true ? true : false;
    if (FoTag(e) == FLOATBLOCK)     /* floating block   parentup must fo:block-container or fo:block  !fo:float */
    {
        take = true;
    }
    if (FoTag(e) == BLOCK_TAG)
    {
        take = true;
    }
    if (!take)
    {
        return false;
    }
    QDomElement FirstPara;
    QTextLength mesure = BlockMesure(e);
    if (isBlockBlock)
    {
        mesure = BlockMesure(parentup);
    }
    ///////////////qDebug() << "############ Draw_Width  " << QVariant(mesure).toInt();
    ////////////////qDebug() << "############ isBlockBlock  " << isBlockBlock;
    /* new container */
    QTextFrameFormat FrameFormat;
    FrameFormat.setWidth(mesure);
    QTextCharFormat  Charformat;
    QTextBlockFormat Paraformat = TextBlockFormFromDom(e,DefaultMargin());
    if (!isBlockBlock)
    {
        FrameFormat = PaintFrameFormat(e,FrameFormat);  /* float */
        FirstPara = e.firstChildElement();
        if (FirstPara.isNull())
        {
            return false;
        }

        Charformat = GlobalCharFormat(parentup,DefaultCharFormats());
        Charformat = GlobalCharFormat(FirstPara,Charformat);
    }
    else
    {
        FirstPara = e;
        FrameFormat = PaintFrameFormat(parentup,FrameFormat); /* block blcok */
        Charformat = GlobalCharFormat(e,DefaultCharFormats());
        Charformat = GlobalCharFormat(parentup,Charformat);
        Paraformat = TextBlockFormFromDom(e,Paraformat);
        FrameFormat = PaintFrameFormat(e,FrameFormat);
    }
    FrameFormat.setWidth(mesure);  /* if is loast ! */

    QColor bgcolor = QColor(Qt::transparent);

    if (!parentup.attribute("background-color").isEmpty())
    {
        bgcolor = ColorFromFoString( parentup.attribute("background-color") );
    }

    if (!e.attribute("background-color").isEmpty())
    {
        bgcolor = ColorFromFoString( e.attribute("background-color") );
    }

    qreal borderrforce = isBorder(e);
    if (borderrforce < 0.1)
    {
        /* editinting border leave by print */
        FrameFormat.setBorderBrush ( bgcolor );
        FrameFormat.setBorder ( 0.4 );
        FrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_DotDotDash);
    }



    ///////FrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted); /* debug */
    QTextFrame *Divframe = Tcursor.insertFrame(FrameFormat);
    doc_cur = Divframe->firstPosition();
    Tcursor.setPosition(doc_cur);

    Paraformat = TextBlockFormFromDom(parentup,DefaultMargin());
    Tcursor.setCharFormat(Charformat);
    QDomElement blocks;


    doc_cur = Tcursor.position();
    int helpline = -1;

    if (isBlockBlock)
    {

        if (InlineBlockLoop(e,Tcursor,true))
        {
            doc_cur = Tcursor.position();
           /////////// qDebug() << "### radice  ";
            helpline = 1;
        }

        blocks = e.firstChildElement("fo:block");
        if (blocks.isNull())
        {
           /////////// qDebug() << "### secondo false  ";
            doc_cur = Tcursor.position();
            return true;
        }
        else
        {
            ////////////////qDebug() << "### secondo vero  ";
        }



    }
    else
    {
        blocks = e.firstChildElement("fo:block");
    }



    while (!blocks.isNull())
    {
        helpline++;
        ////////////qDebug() << "### loop float block block  " << helpline << "|" << blocks.text();
        if (helpline > 0)
        {
            Tcursor.insertBlock();
        }
        bool firstBl = InlineBlockLoop(blocks,Tcursor,true);
        doc_cur = Tcursor.position();
       ///////////// qDebug() << "### block block float loop " << firstBl;
        blocks = blocks.nextSiblingElement("fo:block");
    }




    doc_cur = Tcursor.position();
    return true;
}


bool Fo_Reader::FoTableTagPaint( const QDomElement e  , QTextCursor Cursor )
{

    if (FoTag(e) != TABLE_TAG)
    {
        return false;
    }
    
    QDomElement tabi = e.firstChildElement("fo:table-body");
    
    bool colum_WI_command = false;
    QTextTableCell cell;
    QDomElement bodytable = e.firstChildElement("fo:table-body");
    int columnCounter = 0;
    int rowCounter = 0;
    int MultipleBodyTableRowLine = 0;
    QVector<QTextLength> constraints;
    
    
    QDomElement column = e.firstChildElement("fo:table-column");
    /* column count and sett wi distance */
    if (!column.isNull())
    {
        while (!column.isNull())
        {
            QTextLength cool_wi = BlockMesure(column);
            const int MiTx = cool_wi.type();
            if (cool_wi.rawValue() !=100 &&  MiTx != 2 )
            {
                colum_WI_command = true;  /* width from table-column not table */
                constraints.insert(columnCounter,cool_wi);
            }
            columnCounter++;
            column = column.nextSiblingElement("fo:table-column");
        }
    }
    if (columnCounter == 0) {
        columnCounter = 1;
        constraints.insert(0,QTextLength(QTextLength::PercentageLength,100)); 
    }
    
    trlastelemnt = bodytable.firstChildElement("fo:table-row");
    while (!trlastelemnt.isNull())
    {
        rowCounter++;
        trlastelemnt = trlastelemnt.nextSiblingElement("fo:table-row");
    }
    
        /* check for multiple body */
    QDomNode bchild = e.firstChild();
    
    
    while ( !bchild.isNull() )
    {
        if ( bchild.isElement() )
        {
            QDomElement tde = bchild.toElement();
            //////////////qDebug() << "### parse table count name ....  " << tde.tagName();
            
            if (FoTag(tde) == TABLE_BODY || FoTag(tde) == TABLE_FOOTER || FoTag(tde) == TABLE_HEADER )
            {
                QDomElement multirows = tde.firstChildElement("fo:table-row");
                while (!multirows.isNull())
                {
                    MultipleBodyTableRowLine++;
                    multirows = multirows.nextSiblingElement("fo:table-row");
                }

            }
        }
        bchild = bchild.nextSibling();
    }
    /* check for multiple body */

    ///////////qDebug() << "### MultipleBodyTableRowLine" << MultipleBodyTableRowLine;
    ///////////qDebug() << "### columnCounter" << columnCounter;
    //////////qDebug() << "### constraints.count() " << constraints.count();
    /////////////qDebug() << "### rowCounter" << rowCounter;
    
    if (rowCounter !=0 && columnCounter!=0)  {
        /* ok table having ...*/
    }  else  {
        return false;
    }
    QTextTable *qtable = Cursor.insertTable(qMax (rowCounter,MultipleBodyTableRowLine),columnCounter);
    
    QTextTableFormat tableFormat = qtable->format();
    tableFormat.setBorder(0);
    tableFormat.setCellSpacing(2.2);
    tableFormat.setCellPadding(2.2);
    if (constraints.count()  ==  columnCounter)
    {
        tableFormat.setColumnWidthConstraints(constraints);
    }
    int gorow = -1;
    int gocool = -1;
    
    QDomNode tablechi = e.firstChild();
    
    while ( !tablechi.isNull() )
    {
        if ( tablechi.isElement() )
        {
            
            const QDomElement tde = tablechi.toElement();
            /////////qDebug() << "### parse table name " << tde.tagName();
            
            if (FoTag(tde) == TABLE_BODY || FoTag(tde) == TABLE_FOOTER || FoTag(tde) == TABLE_HEADER )
            {
                QDomElement multirows = tde.firstChildElement("fo:table-row");
                while (!multirows.isNull())
                {
                    gorow++;
                    gocool = -1;
                    trlastelemnt = multirows;
                    ////////qDebug() << "### parse gorow " << gorow;
                    
                        QDomElement domcell = multirows.firstChildElement("fo:table-cell");
                         while (!domcell.isNull())  {
                             const int is_spancol = domcell.attribute( "number-columns-spanned","0").simplified().toInt();
                             gocool++;
                             
                               if (is_spancol > 1)  {
                                    qtable->mergeCells(gorow,gocool,1,is_spancol);  /* last zero no merge */
                                    cell = qtable->cellAt(gorow,gocool);
                                } else {
                                    cell = qtable->cellAt(gorow,gocool);
                                }
                                
                                if (!FoTableCellLoop(domcell,cell,Cursor)) {
                                   ///////////qWarning() << "Error on parse table cell  row:" << gorow << " column:" << gocool; 
                                }
                                
                             domcell = domcell.nextSiblingElement("fo:table-cell");
                         }
                    
                    
                    
                    
                    
                    
                    
                    multirows = multirows.nextSiblingElement("fo:table-row");
                }

            }
        }
        tablechi = tablechi.nextSibling();
    }

    tableFormat.setWidth ( QTextLength(QTextLength::PercentageLength,100) );
    qtable->setFormat( tableFormat );
    
    
    
    return true;
}


bool Fo_Reader::FoTableCellLoop( const QDomElement e  , QTextTableCell  cell  ,  QTextCursor Cursor  , qreal maxhight )
{
    const QString fotag = e.tagName().toLower();
    if ( fotag != "fo:table-cell")
    {
        return false;
    }
    if ( !cell.isValid() )
    {
        return false;
    }
    
    QColor bgcolor(255,255,255,0);  /* to init if table having color ! */
    
   

    if (!trlastelemnt.isNull()) {
        if (!trlastelemnt.attribute("background-color").isEmpty())  {
        bgcolor = ColorFromFoString( trlastelemnt.attribute("background-color") );
        }
    }
    
    QDomElement trcat = e.previousSiblingElement("fo:table-row");
    if (!trcat.isNull()) {
        if (!trcat.attribute("background-color").isEmpty())  {
        bgcolor = ColorFromFoString(trcat.attribute("background-color") );
        }
        ////////////////////qDebug() << "### beccato il sopra trrrrr ---------------------------------------------";
    }
    
    
    QTextLength mesure;  /* need to frame if border */
    bool makeframe = false;
    qreal FutureFrametopPadding = 0;
    QDomElement  firstPara = e.firstChildElement("fo:block");



    int is_spancol = e.attribute( "number-columns-spanned" ).trimmed().toInt();
    


    QTextTableCellFormat td_format = cell.format().toTableCellFormat();
    if (!e.attribute("background-color").isEmpty())
    {
        bgcolor = ColorFromFoString( e.attribute("background-color") );
    }

    QTextFrameFormat CellfindBorder = PaintFrameFormat(e,QTextFrameFormat());
    QTextFrameFormat BlockfindBorder = CellfindBorder;
    if (!firstPara.isNull())
    {
        BlockfindBorder	= PaintFrameFormat(firstPara,CellfindBorder);
        if (!firstPara.attribute("background-color").isEmpty())
        {
            bgcolor = ColorFromFoString( firstPara.attribute("background-color") );
        }
    }

    ApiSession *session = ApiSession::instance();
    bool no_option = false;
    if (no_option)
    {
        if (BlockfindBorder.border() > 0)
        {
            makeframe = true;
            //////qDebug() << "### cell border " << CellfindBorder.border();
        }
        else if (CellfindBorder.border() > 0 && !firstPara.isNull())
        {
            makeframe = true;
            /////qDebug() << "### block border " << BlockfindBorder.border();
        }
        else
        {
            //////qDebug() << "### border none ......... ";
        }
    }

    qreal actualWiiii = ColumnLarge(LastTableCON,cell.column());
    if (actualWiiii > 0)
    {
        mesure = QTextLength(QTextLength::FixedLength,actualWiiii);
    }
    else
    {
        mesure = QTextLength(QTextLength::PercentageLength,100);
    }

    BlockfindBorder.setWidth(mesure);
    if (bgcolor.isValid () )
    {
        BlockfindBorder.setBackground(bgcolor);
    }

    Qt::Alignment cellalign = TagAlignElement(e); /* fake */
    if ( cellalign == Qt::AlignVCenter && maxhight > 0.5 && !firstPara.isNull())
    {

        QRect fiBBrect = BlockRect(firstPara,actualWiiii);
        qreal ccenter = maxhight / 2;
        qreal fromtopPadding = ccenter - (fiBBrect.height() / 2);
        BlockfindBorder.setHeight(fiBBrect.height() + BlockfindBorder.border());

        if (fromtopPadding > 0)
        {
            if (makeframe)
            {
                FutureFrametopPadding = fromtopPadding;
                td_format.setPadding(0);
            }
            else
            {
                td_format.setTopPadding(fromtopPadding);
            }
        }
    }
    else
    {
        td_format.setPadding(0);
    }

    BlockfindBorder.setTopMargin ( FutureFrametopPadding );
    QTextCursor tmpcur  = cell.firstCursorPosition();
    Cursor.setPosition(tmpcur.position());
    /* reset only if not miniscribus signature */

/*
    if (makeframe)
    {
        QTextFrame *Divframe = Cellcursor.insertFrame(BlockfindBorder);
        int poscuuuri = Divframe->firstPosition();
        Cellcursor.setPosition(poscuuuri);
        td_format.setVerticalAlignment ( QTextCharFormat::AlignTop );
    }
*/
    FrameDomIterator(e.firstChild(),Cursor);
    td_format.setBackground(bgcolor);
    cell.setFormat(td_format);
    int ramnottoclassPos = Cursor.position();
    return true;
}

void Fo_Reader::FrameDomIterator(  QDomNode node ,  QTextCursor Cinline  )
{
    int LastCellCurorrisPos = Cinline.position();
    QDomElement LastElementParent;
    int loop = -1;
    while ( !node.isNull() )
    {
         loop++;
        
        if ( node.isElement() )
        {
            const QDomElement el = node.toElement();
           //////////// qDebug() << "### FrameDomIterator " << el.tagName() << " cursor->" << LastCellCurorrisPos << "-" << loop;
            if ( FoTag(el) == INLINE_BR )
            {
                Cinline.insertText(QString(QChar::LineSeparator));
            }
            else if (FoTag(el) == FOLEADER)
            {
                FoLeaderPaint(el,Cinline);
            }
            else if (FoTag(el) == BLOCK_TAG)
            {
                if (loop !=0) {
                Cinline.insertBlock();
                Cinline.setBlockFormat(DefaultMargin());
                }
                
                InlineBlockLoop(el,Cinline,true);
            }
            else if ( FoTag(el) == BLOCK_CONTAINER)
            {
                /* inline frame elements */
                FoBlockContainerPaint(el,Cinline);
            }
            else if ( FoTag(el) == IMAGE_INLINE  )
            {
                FoDrawSvgInline(el,Cinline);
            }
            else if ( FoTag(el) == FOOTNOTEBOTTOM  )
            {
                ////////FootNoteSave(el,Cinline);
            }
            else if ( FoTag(el) == IMAGE_SRC  )
            {
               //////////// qDebug() << "##   normal image paint2    ";
                FoDrawImage(el,Cinline);
            }
            else
            {
                if (FoTag(el) == TAG_NOT_KNOW )
                {
                    DidplayUnknowTag(el,Cinline);
                }
            }
            ////////  FOOTNOTEBOTTOM
            LastElementParent = el;
            LastCellCurorrisPos = Cinline.position();
        }
        node = node.nextSibling();
    }

    ////////////////LastCellCurorrisPos = Cinline.position();
}


#ifdef _HAVING_NEW_TEXTDOCUMENT_



#else
/*   source  xslt open office document  */

OO_Xslt::OO_Xslt( const QString filedoc , OOTYPE doc )
 : havingdoc(false)
{
    qdoc = new QTextDocument();
    qdoc->clear(); /* default size margin */
	OpenFile(filedoc,doc);
}


void OO_Xslt::OpenFile( const QString filedoc , OOTYPE doc )
{
      QFileInfo fi(filedoc);
      if (!fi.exists()) {
       return;
      }
    
	  ooFile.clear();
      ooimage.clear();
	  QuaZip zip(filedoc);
      filelist.clear();
    
      zip.open(QuaZip::mdUnzip);
      QuaZipFile file(&zip);
	  for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
          
			     if (file.open(QIODevice::ReadOnly) ) {
                 const QString name = zip.getCurrentFileName(); 
                     if (!name.endsWith("/")) {
					  filelist.append(name);
                      const QByteArray fean = file.readAll();
                      ooFile.insert(name,fean);
                      ////////////qDebug() << "### name  " << name;
                     }
			     file.close();
                 }
          
		}
		zip.close();
		if (ooFile.size() > 0) {
			havingdoc = true;
            convertXslt(doc);
		}
}

void OO_Xslt::convertXslt( OOTYPE doc )  {
    
    bool dirok = Cache(_GSCACHE_);  /* mkdir */
    
    qt_unlink(XT_STREAMFILE);
    qt_unlink(XT_XMLFILE);
    qt_unlink(XT_STYLEFILE);
    
    ooimage.clear();
    
    
     if (!filelist.contains("content.xml")) {
		qWarning() << "Unable to find file :content.xml";
		return;
	 }
     QByteArray streamPage = ooFile["content.xml"];
     ////////qDebug() << "### streamPage " << streamPage.size();
     
      QFile f(XT_XMLFILE);
      if (f.open(QFile::WriteOnly)) {
        f.write(streamPage);
        f.close();
      }
      
      StreamFop *buf = new StreamFop();
      if (doc == DOC_1_VERSION) {
      buf->LoadFile(_OOOV1FILE_);
      } else {
      buf->LoadFile(_OOOV2FILE_);  
      }
      buf->PutOnFile(XT_STYLEFILE);
      /* convert and delete file */
      
      Xslt_Convert *xmake = new Xslt_Convert(XT_XMLFILE,XT_STYLEFILE,XT_STREAMFILE);
      const QByteArray chunkhtml = xmake->stream();
      
      QString html = QString::fromUtf8(chunkhtml.constData());
      html = html.replace("#Pictures/","");
      /* replace #Pictures/ to  ""  */
      qdoc = new QTextDocument();
      qdoc->clear(); /* default size margin */
      qdoc->setHtml(html);
      QString chtm = qdoc->toHtml("utf-8");
      /////////////qDebug() << "### html co  .... " << chtm;
      
      qt_unlink(XT_STREAMFILE);
      qt_unlink(XT_XMLFILE);
      qt_unlink(XT_STYLEFILE);
      
      QMapIterator<QString, QByteArray> it( ooFile );
      while ( it.hasNext() ) {
             it.next();
                   if (it.key().contains( "Pictures" )) {
                       QString naimg = it.key();
                       naimg = naimg.replace("Pictures/","");
                       qdoc->addResource( QTextDocument::ImageResource, QUrl(naimg), QImage::fromData( it.value() ) );
                       //////////qDebug() << "### naimg .... " << naimg;
                       ooimage.insert(naimg,it.value());
                   }
          }
    
}



  
  








#endif





































