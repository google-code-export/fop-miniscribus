<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:office="http://openoffice.org/2000/office" xmlns:style="http://openoffice.org/2000/style" xmlns:text="http://openoffice.org/2000/text" xmlns:table="http://openoffice.org/2000/table" xmlns:draw="http://openoffice.org/2000/drawing" xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:number="http://openoffice.org/2000/datastyle" xmlns:svg="http://www.w3.org/2000/svg" xmlns:chart="http://openoffice.org/2000/chart" xmlns:dr3d="http://openoffice.org/2000/dr3d" xmlns:math="http://www.w3.org/1998/Math/MathML" xmlns:form="http://openoffice.org/2000/form" xmlns:script="http://openoffice.org/2000/script" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:meta="http://openoffice.org/2000/meta" xmlns:config="http://openoffice.org/2001/config" xmlns:help="http://openoffice.org/2000/help" exclude-result-prefixes="office style text table draw fo xlink number svg chart dr3d math form script dc meta config help" xmlns="http://www.w3.org/1999/xhtml">


       <xsl:output method="xml" encoding="UTF-8" media-type="application/xhtml+xml" indent="yes" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" omit-xml-declaration="yes"/>

       <!-- styles -->
       <!--xsl:variable name="automatic-styles"
               select="//office:document/office:automatic-styles" / -->

       <!-- dummy variable used in iterations -->
       <xsl:variable name="random-nodes" select="document('')//node()"/>
       <xsl:variable name="page-master-properties" select="/office:document/office:automatic-styles/style:page-master/style:properties"/>
       <xsl:variable name="page-width" select="$page-master-properties/@fo:page-width"/>
       <xsl:variable name="page-height" select="$page-master-properties/@fo:page-height"/>

       <!-- document -->
       <xsl:template match="/">

               <!-- header -->
               <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns="http://www.w3.org/1999/xhtml">
                       <fo:layout-master-set>
                               <fo:simple-page-master>

                                       <xsl:attribute name="margin-top">0cm</xsl:attribute>
                                       <xsl:attribute name="margin-bottom">0cm</xsl:attribute>
                                       <xsl:attribute name="margin-left">0cm</xsl:attribute>
                                       <xsl:attribute name="margin-right">0cm</xsl:attribute>

                                       <xsl:attribute name="page-width"><xsl:value-of select="$page-width"/>
                                       </xsl:attribute>
                                       <xsl:attribute name="page-height"><xsl:value-of select="$page-height"/>
                                       </xsl:attribute>
                                       <xsl:attribute name="master-name">bookpage</xsl:attribute>

                                       <fo:region-body region-name="bookpage-body" margin-bottom="5mm" margin-top="5mm"/>
                               </fo:simple-page-master>
                       </fo:layout-master-set>

                       <xsl:apply-templates select="office:document/office:body/*">
                               <xsl:with-param name="automatic-styles" select="office:document/office:automatic-styles"/>
                       </xsl:apply-templates>

               </fo:root>
       </xsl:template>

       <!-- page -->
       <xsl:template match="draw:page">
               <xsl:param name="automatic-styles"/>
               <fo:page-sequence master-reference="bookpage">
                       <fo:title>Sample Document</fo:title>
                       <fo:flow flow-name="bookpage-body">
                               <xsl:apply-templates>
                                       <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                               </xsl:apply-templates>
                       </fo:flow>
               </fo:page-sequence>
       </xsl:template>

       <!-- Included objects -->
       <xsl:template match="draw:object">
               <xsl:param name="automatic-styles"/>
               <fo:block-container>
                       <!-- position -->
                       <xsl:attribute name="position">absolute</xsl:attribute>
                       <xsl:attribute name="left"><xsl:value-of select="./@svg:x"/>
                       </xsl:attribute>
                       <xsl:attribute name="top"><xsl:value-of select="./@svg:y"/>
                       </xsl:attribute>
                       <xsl:attribute name="width"><xsl:value-of select="./@svg:width"/>
                       </xsl:attribute>
                       <xsl:attribute name="height"><xsl:value-of select="./@svg:height"/>
                       </xsl:attribute>

                       <xsl:apply-templates select="office:document/office:body/*">
                               <xsl:with-param name="automatic-styles" select="office:document/office:automatic-styles"/>
                       </xsl:apply-templates>

               </fo:block-container>
       </xsl:template>

       <!-- Table -->
       <xsl:template match="table:table">
               <xsl:param name="automatic-styles"/>
               <!-- table style -->
               <xsl:variable name="table-style-name" select="@table:style-name"/>
               <xsl:variable name="table-style" select="$automatic-styles//style:style[@style:name=$table-style-name]/style:properties"/>

               <fo:block>
                       <fo:table>
                               <xsl:attribute name="width"><xsl:value-of select="$table-style/@style:width"/>
                               </xsl:attribute>

                               <!-- build table columns -->
                               <xsl:for-each select="table:table-column">

                                       <xsl:choose>
                                               <xsl:when test="@table:number-columns-repeated">
                                                       <xsl:call-template name="iterateTableColumns">
                                                               <xsl:with-param name="numIterations" select="@table:number-columns-repeated"/>
                                                               <xsl:with-param name="column" select="."/>
                                                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                                                       </xsl:call-template>
                                               </xsl:when>
                                               <xsl:otherwise>
                                                       <xsl:call-template name="iterateTableColumns">
                                                               <xsl:with-param name="numIterations" select="'1'"/>
                                                               <xsl:with-param name="column" select="."/>
                                                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                                                       </xsl:call-template>
                                               </xsl:otherwise>
                                       </xsl:choose>

                               </xsl:for-each>

                               <fo:table-body>
                                       <xsl:apply-templates select="table:table-row">
                                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                                       </xsl:apply-templates>
                               </fo:table-body>

                       </fo:table>
               </fo:block>

       </xsl:template>

       <!-- Table Rows -->
       <!-- some styles should be used here? -->
       <xsl:template match="table:table-row">
               <xsl:param name="automatic-styles"/>
               <fo:table-row>
                       <xsl:apply-templates select="table:table-cell">
                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                       </xsl:apply-templates>
               </fo:table-row>
       </xsl:template>

       <!-- Table Cells -->
       <xsl:template match="table:table-cell">
               <xsl:param name="automatic-styles"/>

               <xsl:variable name="cell-style-name" select="@table:style-name"/>
               <xsl:variable name="cell-style" select="$automatic-styles//style:style[@style:name=$cell-style-name]/style:properties"/>

               <fo:table-cell>

                       <!-- copy the cell properties and change the sizes -->
                       <xsl:for-each select="$cell-style/@fo:*">
                               <xsl:variable name="propB" select="substring-before(., &quot;cm&quot;)"/>
                               <xsl:choose>
                                       <xsl:when test="$propB">
                                               <xsl:variable name="propA" select="substring-after(., &quot;cm&quot;)"/>
                                               <xsl:variable name="propBNew" select="number($propB) * number(&quot;6.6&quot;)"/>
                                               <xsl:variable name="prop" select="concat($propBNew, &quot;cm &quot;, $propA)"/>
                                               <xsl:attribute name="{local-name()}"><xsl:value-of select="$prop"/></xsl:attribute>
                                       </xsl:when>
                                       <xsl:otherwise>
                                               <xsl:attribute name="{local-name()}"><xsl:value-of select="."/></xsl:attribute>
                                       </xsl:otherwise>
                               </xsl:choose>
                       </xsl:for-each>

                       <xsl:apply-templates>
                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                       </xsl:apply-templates>
               </fo:table-cell>
       </xsl:template>

       <!-- Image -->
       <xsl:template match="draw:image">
               <xsl:param name="automatic-styles"/>
               <fo:block-container>
                       <!-- position -->
                       <xsl:attribute name="position">absolute</xsl:attribute>
                       <xsl:attribute name="left"><xsl:value-of select="./@svg:x"/>
                       </xsl:attribute>
                       <xsl:attribute name="top"><xsl:value-of select="./@svg:y"/>
                       </xsl:attribute>
                       <xsl:attribute name="width"><xsl:value-of select="./@svg:width"/>
                       </xsl:attribute>
                       <xsl:attribute name="height"><xsl:value-of select="./@svg:height"/>
                       </xsl:attribute>
                       <!-- image url -->
                       <fo:block>
                               <fo:external-graphic>
                                       <xsl:attribute name="src"><xsl:value-of select="concat('url(', @xlink:href, ')')"/>
                                       </xsl:attribute>
                               </fo:external-graphic>
                       </fo:block>

               </fo:block-container>
       </xsl:template>

       <!-- SVG line -->
       <xsl:template match="draw:line">
               <xsl:param name="automatic-styles"/>
               <fo:block-container position="absolute">
                       <fo:block>
                               <fo:instream-foreign-object>
                                       <svg:svg>
                                               <xsl:attribute name="viewBox">0 0 0 0</xsl:attribute>
                                               <xsl:attribute name="width"><xsl:value-of select="$page-width"/>
                                               </xsl:attribute>
                                               <xsl:attribute name="height"><xsl:value-of select="$page-height"/>
                                               </xsl:attribute>
                                               <svg:line>
                                                       <xsl:for-each select="./@svg:*">
                                                               <xsl:attribute name="{local-name()}"><xsl:value-of select="."/>
                                                               </xsl:attribute>
                                                       </xsl:for-each>

                                                       <!-- style -->
                                                       <xsl:variable name="draw-style-name" select="@draw:style-name"/>
                                                       <xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties"/>
                                                       <xsl:attribute name="style">
                                                                       <xsl:value-of select="concat('stroke-width:', $draw-style/@svg:stroke-width, ';stroke:', $draw-style/@svg:stroke-color)"/>
                                                               </xsl:attribute>

                                               </svg:line>
                                       </svg:svg>
                               </fo:instream-foreign-object>
                       </fo:block>
               </fo:block-container>
       </xsl:template>

       <!-- block container -->
       <xsl:template match="draw:text-box">
               <xsl:param name="automatic-styles"/>
               <fo:block-container>

                       <!-- borders -->
                       <xsl:variable name="draw-style-name" select="@draw:style-name"/>
                       <xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties"/>

                       <!-- border style -->
                       <xsl:if test="$draw-style/@draw:stroke='solid'">
                               <xsl:attribute name="border-bottom-style">solid</xsl:attribute>
                               <xsl:attribute name="border-top-style">solid</xsl:attribute>
                               <xsl:attribute name="border-left-style">solid</xsl:attribute>
                               <xsl:attribute name="border-right-style">solid</xsl:attribute>
                       </xsl:if>

                       <!-- border width -->
                       <xsl:if test="$draw-style/@svg:stroke-width">
                               <xsl:variable name="stroke-width" select="$draw-style/@svg:stroke-width"/>
                               <xsl:attribute name="border-left-width"><xsl:value-of select="$stroke-width"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-right-width"><xsl:value-of select="$stroke-width"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-top-width"><xsl:value-of select="$stroke-width"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-bottom-width"><xsl:value-of select="$stroke-width"/>
                               </xsl:attribute>
                       </xsl:if>

                       <!-- border color -->
                       <xsl:if test="$draw-style/@svg:stroke-color">
                               <xsl:variable name="stroke-color" select="$draw-style/@svg:stroke-color"/>
                               <xsl:attribute name="border-left-color"><xsl:value-of select="$stroke-color"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-right-color"><xsl:value-of select="$stroke-color"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-top-color"><xsl:value-of select="$stroke-color"/>
                               </xsl:attribute>
                               <xsl:attribute name="border-bottom-color"><xsl:value-of select="$stroke-color"/>
                               </xsl:attribute>
                       </xsl:if>

                       <!--- paddings -->
                       <xsl:if test="$draw-style/@fo:padding-top">
                               <xsl:attribute name="padding-top"><xsl:value-of select="$draw-style/@fo:padding-top"/>
                               </xsl:attribute>
                       </xsl:if>
                       <xsl:if test="$draw-style/@fo:padding-bottom">
                               <xsl:attribute name="padding-bottom"><xsl:value-of select="$draw-style/@fo:padding-bottom"/>
                               </xsl:attribute>
                       </xsl:if>
                       <xsl:if test="$draw-style/@fo:padding-left">
                               <xsl:attribute name="padding-left"><xsl:value-of select="$draw-style/@fo:padding-left"/>
                               </xsl:attribute>
                       </xsl:if>
                       <xsl:if test="$draw-style/@fo:padding-right">
                               <xsl:attribute name="padding-right"><xsl:value-of select="$draw-style/@fo:padding-right"/>
                               </xsl:attribute>
                       </xsl:if>

                       <!-- Basic attributes -->
                       <xsl:attribute name="position">absolute</xsl:attribute>
                       <xsl:attribute name="left"><xsl:value-of select="./@svg:x"/>
                       </xsl:attribute>
                       <xsl:attribute name="top"><xsl:value-of select="./@svg:y"/>
                       </xsl:attribute>
                       <xsl:attribute name="width"><xsl:value-of select="./@svg:width"/>
                       </xsl:attribute>
                       <xsl:attribute name="height"><xsl:value-of select="./@svg:height"/>
                       </xsl:attribute>

                       <xsl:apply-templates>
                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                       </xsl:apply-templates>
               </fo:block-container>
       </xsl:template>

       <!-- block -->
       <xsl:template match="text:p">
               <xsl:param name="automatic-styles"/>
               <fo:block>
                       <!-- fo attributes -->
                       <xsl:variable name="text-style-name" select="@text:style-name"/>
                       <xsl:for-each select="$automatic-styles//style:style[@style:name=$text-style-name]/style:properties/@fo:*">
                               <xsl:attribute name="{local-name()}"><xsl:value-of select="."/>
                               </xsl:attribute>
                       </xsl:for-each>
                       <xsl:apply-templates>
                               <xsl:with-param name="automatic-styles" select="$automatic-styles"/>
                       </xsl:apply-templates>
               </fo:block>
       </xsl:template>

       <!-- Inline -->
       <xsl:template match="text:span">
               <xsl:param name="automatic-styles"/>
               <fo:inline>
                       <!-- FO attributes -->
                       <xsl:variable name="text-style-name" select="@text:style-name"/>
                       <xsl:for-each select="$automatic-styles//style:style[@style:name=$text-style-name]/style:properties/@fo:*">
                               <xsl:attribute name="{local-name()}"><xsl:value-of select="."/>
                               </xsl:attribute>
                       </xsl:for-each>
                       <xsl:value-of select="."/>
               </fo:inline>
       </xsl:template>

       <!-- Table column iteratiosn -->
       <xsl:template name="iterateTableColumns">
               <xsl:param name="numIterations"/>
               <xsl:param name="column"/>
               <xsl:param name="automatic-styles"/>

               <xsl:variable name="column-style-name" select="@table:style-name"/>
               <xsl:variable name="column-style" select="$automatic-styles//style:style[@style:name=$column-style-name]/style:properties"/>

               <xsl:for-each select="$random-nodes[position() &lt;= $numIterations]">
                               <fo:table-column>
                                               <xsl:attribute name="column-width">
                                                       <xsl:value-of select="$column-style/@style:column-width"/>
                                               </xsl:attribute>
                               </fo:table-column>
               </xsl:for-each>

       </xsl:template>

</xsl:stylesheet>