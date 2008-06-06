#include <QDomDocument>

#include "focreator.h"

FoCreator::FoCreator()
{
}

FoCreator::~FoCreator()
{
}

QDomElement FoCreator::xslValueOf(QString selectPath)
{
	//create
        //<xsl:value-of select="cis_predm"/>

	QDomDocument doc;
	//create element block
	QDomElement xslTempl = doc.createElement( "xsl:value-of" );
	//create and set default atributes for block
	xslTempl.setAttribute( "select", selectPath );
	return xslTempl;
}

QDomElement FoCreator::xslApplyTemplates(QString selectPath)
{
	//create xsl:apply-teplates
	//<xsl:apply-templates select="$selectPath"/>

	QDomDocument doc;
	//create element block
	QDomElement xslTempl = doc.createElement( "xsl:apply-templates" );
	//create and set default atributes for block
	xslTempl.setAttribute( "select", selectPath );
	return xslTempl;
}

QDomElement FoCreator::xslTemplateMatch(QString matchPath)
{
	//create xsl:template match
	//<xsl:template-match match="$selectPath"/>

	QDomDocument doc;
	//create element block
	QDomElement xslTempl = doc.createElement( "xsl:template match" );
	//create and set default atributes for block
	xslTempl.setAttribute( "match", matchPath );
	return xslTempl;
}

QDomElement FoCreator::foBlockContainerElement()
{
	// <fo:block-container absolute-position="fixed" top="2cm" left="10cm" width="6em">
	// </fo:block-container>

	QDomDocument doc;
	//create element block
	QDomElement root = doc.createElement( "fo:block-container" );
	//create and set default atributes for block
	root.setAttribute( "absolute-position", "fixed" );
	root.setAttribute( "top", "" );
	root.setAttribute( "left", "" );
	root.setAttribute( "width", "60mm" );
	return root;
}

QDomElement FoCreator::foBlockElement()
{
	//create fo:block:
	//<fo:block text-align="left" font-size="8pt" font-family="serif" top="" left="" width="">
	//     fo-block text
	//</fo:block>
	
	QDomDocument doc;
	//create element block
	QDomElement root = doc.createElement( "fo:block" );
	//create and set default atributes for block

	root.setAttribute( "text-align", "left" );
	root.setAttribute( "font-size", "10pt" );
	root.setAttribute( "font-family", "serif" );
	QDomText domText=doc.createTextNode("fo block");
	root.appendChild(domText);

	return root;
}

QDomElement FoCreator::foCellElement()
{
	//create fo:table-cell:
	//<fo:table-cell>
	//	<fo:block>
	//		cell text
	//	</fo:block>
	//<fo:table-cell>

	QDomDocument doc;
	QDomElement rootCell = doc.createElement( "fo:table-cell" );
	
	QDomElement foblock = doc.createElement( "fo:block" );
	rootCell.appendChild(foblock);
	
	QDomText cellText=doc.createTextNode("cell text");
	foblock.appendChild(cellText);
	
	return rootCell;
}

QDomElement FoCreator::foRowElement()
{
	//create row with two cells
	QDomDocument doc;
	QDomElement rootRow = doc.createElement( "fo:table-row" );
	
	QDomElement cell_1=foCellElement();
	rootRow.appendChild(cell_1);
	
	QDomElement cell_2=foCellElement();
	rootRow.appendChild(cell_2);
	
	return rootRow;
}

QDomElement FoCreator::foTableElement()
{
	//create 2x2 table
	QDomDocument doc;
	
	//create element block
	QDomElement rootTable = doc.createElement( "fo:table" );
	
	QDomElement body = doc.createElement( "fo:table-body" );
	rootTable.appendChild(body);
	
	QDomElement row_1=foRowElement();
	body.appendChild(row_1);
	
	QDomElement row_2=foRowElement();
	body.appendChild(row_2);
	
	return rootTable;
}
