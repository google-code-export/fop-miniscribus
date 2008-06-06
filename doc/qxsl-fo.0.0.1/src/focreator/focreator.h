#ifndef FOCREATOR_H
#define FOCREATOR_H

#include "singleton.h"

/**
 * singleton class which generate base FoObject constructions for inserting to QDomDocument
 */
class FoCreator : public Singleton <FoCreator>
{
public:
	/**
	 * constructor
	 */
	FoCreator();
	/**
	 * destructor
	 */
	~FoCreator();
	/**
	 * return fo:block:
	 *<fo:block text-align="left" font-size="8pt" font-family="serif" top="" left="" width="">
	 *     fo-block text
	 *</fo:block>
	 * @return 
	 */
	QDomElement foBlockElement();
	/**
	 * return fo:bloc-container
	 *<fo:block-container absolute-position="fixed" top="" left="" width="60mm">
	 *</fo:block-container>
	 * @return 
	 */
	QDomElement foBlockContainerElement();
	/**
	 * create and return fo:table-cell:
	 * <fo:table-cell>
	 *	<fo:block>
	 *		cell text
	 *	</fo:block>
	 * <fo:table-cell>
	 * @return 
	 */
	QDomElement foCellElement();
	/**
	 * create row with two cells
	 * @return 
	 */
	QDomElement foRowElement();
	/**
	 * construct and return 2x2 table
	 * @return 
	 */
	QDomElement foTableElement();
	/**
	 * create xsl:template match
	 * <xsl:template-match match="$selectPath"/>
	 * @param matchPath 
	 * @return 
	 */
	QDomElement xslTemplateMatch(QString matchPath);
	/**
	 * create xsl:apply-teplates
	 * <xsl:apply-templates select="$selectPath"/>
	 * @param selectPath 
	 * @return 
	 */
	QDomElement xslApplyTemplates(QString selectPath);
	/**
	 * create
         * <xsl:value-of select="cis_predm"/>
	 * @param selectPath 
	 * @return 
	 */
	QDomElement xslValueOf(QString selectPath);
};

#endif
