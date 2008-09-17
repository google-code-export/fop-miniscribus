<?xml version="1.0" encoding="UTF-8"?>
      
<!--
	===========================================
	Office2fO
	===========================================
      
	Project Info:  http://www.openedgesoft.com/office2fo
	(C) Copyright 2008, OpenEdge www.openedgesoft.com
	
	This file is part of Office2fO
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.
	
	You should have received a copy of the GNU General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301, USA
	 
	copyright 2008 OpenEdge.
	license   http://www.gnu.org/licenses/gpl.txt GNU General Public License v2
	link      http://www.openedgesoft.com/office2fo Office2fO
	link      http://www.openedgesoft.com OpenEdge
-->
   
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:office="http://openoffice.org/2000/office"
	xmlns:style="http://openoffice.org/2000/style"
	xmlns:text="http://openoffice.org/2000/text"
	xmlns:table="http://openoffice.org/2000/table"
	xmlns:draw="http://openoffice.org/2000/drawing"
	xmlns:fo="http://www.w3.org/1999/XSL/Format"
	xmlns:xlink="http://www.w3.org/1999/xlink"
	xmlns:number="http://openoffice.org/2000/datastyle"
	xmlns:svg="http://www.w3.org/2000/svg"
	xmlns:chart="http://openoffice.org/2000/chart"
	xmlns:dr3d="http://openoffice.org/2000/dr3d"
	xmlns:math="http://www.w3.org/1998/Math/MathML"
	xmlns:form="http://openoffice.org/2000/form"
	xmlns:script="http://openoffice.org/2000/script"
	xmlns:dc="http://purl.org/dc/elements/1.1/"
	xmlns:meta="http://openoffice.org/2000/meta"
	xmlns:config="http://openoffice.org/2001/config"
	xmlns:help="http://openoffice.org/2000/help"	
	exclude-result-prefixes="office style text table draw fo xlink number svg chart dr3d math form script dc meta config help"
	xmlns="http://www.w3.org/1999/xhtml">

	<xsl:include href="padding.xsl" />
	<xsl:include href="images.xsl" />
	<xsl:include href="borders.xsl" />

	<xsl:output method="xml" encoding="UTF-8"
		media-type="application/xhtml+xml" indent="yes"
		omit-xml-declaration="yes" />

	<!-- styles -->
	<!--xsl:variable name="automatic-styles"
		select="//office:document/office:automatic-styles" / -->

	<!-- dummy variable used in iterations -->
	<xsl:variable name="random-nodes" select="document('')//node()" />

	<xsl:variable name="page-master-properties"
		select="/office:document/office:automatic-styles/style:page-master/style:properties" />

	<xsl:variable name="page-width"
		select="$page-master-properties/@fo:page-width" />
	<xsl:variable name="page-height"
		select="$page-master-properties/@fo:page-height" />

	<xsl:variable name="page-margin-top"
		select="$page-master-properties/@fo:margin-top" />
	<xsl:variable name="page-margin-bottom"
		select="$page-master-properties/@fo:margin-bottom" />
	<xsl:variable name="page-margin-left"
		select="$page-master-properties/@fo:margin-left" />
	<xsl:variable name="page-margin-right"
		select="$page-master-properties/@fo:margin-right" />

	<!-- default styles -->		
	<xsl:variable name="default-style-paragraph"
		select="/office:document/office:styles/style:default-style[@style:family='paragraph']" />

	<xsl:variable name="default-style-table-row" 
		select="/office:document/office:styles/style:default-style[@style:family='table-row']" />
		
	<xsl:variable name="all-auto-styles" 
		select="/office:document/office:styles" />

	<!-- document -->
	<xsl:template match="/">

		<!-- header -->
		<fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns="http://www.w3.org/1999/xhtml">
			<fo:layout-master-set>
				<xsl:call-template name="pageTemplate">
					<xsl:with-param name="pagename">mainpage</xsl:with-param>
					<xsl:with-param name="regionname">main</xsl:with-param>
				</xsl:call-template>
				<xsl:call-template name="pageTemplate">
					<xsl:with-param name="pagename">firstpage</xsl:with-param>
					<xsl:with-param name="regionname">first</xsl:with-param>
				</xsl:call-template>
				<xsl:call-template name="pageTemplate">
					<xsl:with-param name="pagename">lastpage</xsl:with-param>
					<xsl:with-param name="regionname">last</xsl:with-param>
				</xsl:call-template>
				<fo:page-sequence-master master-name="main-sequence">
					<fo:repeatable-page-master-alternatives>
						<fo:conditional-page-master-reference master-reference="firstpage-layout" page-position="first"/>
						<fo:conditional-page-master-reference master-reference="lastpage-layout" page-position="last"/>
						<fo:conditional-page-master-reference master-reference="mainpage-layout" page-position="rest"/>
					</fo:repeatable-page-master-alternatives>
				</fo:page-sequence-master>
			</fo:layout-master-set>

			<fo:page-sequence master-reference="main-sequence">
				<fo:title>Sample Document</fo:title>
				<static-content flow-name="last-region">
					<block-container position="fixed" left="2.007cm" top="25.587cm" width="17.07cm" min-height="3.119cm">
						<block font-size="12pt" font-family="Arial" text-align="center" font-weight="bold">This is the end section</block>
					</block-container>
				</static-content>
				<fo:flow flow-name="frame-body">
					<xsl:apply-templates select="office:document/office:body/*">
						<xsl:with-param name="automatic-styles" select="office:document/*" />
					</xsl:apply-templates>
				</fo:flow>
			</fo:page-sequence>

		</fo:root>
	</xsl:template>

	<xsl:template match="office:forms">	
	</xsl:template>
	
	<xsl:template match="text:sequence-decls">		
	</xsl:template>
	
	<xsl:template match="office:script">
	</xsl:template>
		
	<xsl:template match="office:font-decls">
	</xsl:template>
	
	<!-- Included objects -->
	<xsl:template match="draw:object">

		<xsl:param name="automatic-styles" />
		<fo:block-container>
			<!-- position -->
			<xsl:attribute name="position">absolute</xsl:attribute>
			<xsl:attribute name="left"><xsl:value-of select="./@svg:x" /></xsl:attribute>
			<xsl:attribute name="top"><xsl:value-of select="./@svg:y" /></xsl:attribute>

			<xsl:attribute name="width"><xsl:value-of select="./@svg:width" /></xsl:attribute>
			<xsl:attribute name="height"><xsl:value-of select="./@svg:height" /></xsl:attribute>

			<xsl:apply-templates select="office:document/office:body/*">
				<xsl:with-param name="automatic-styles" select="office:document/office:automatic-styles" />
			</xsl:apply-templates>

		</fo:block-container>

	</xsl:template>

	<xsl:template match="text:footnote">
		<xsl:param name="automatic-styles"/>
		
		<fo:footnote>
			<fo:inline><xsl:value-of select="text:footnote-citation"/></fo:inline>
			<fo:footnote-body>
				<fo:block>
					<xsl:apply-templates select="text:footnote-body/*">
						<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
					</xsl:apply-templates>
				</fo:block>
			</fo:footnote-body>
		</fo:footnote>
				
	</xsl:template>
	
	<!-- Table -->
	<xsl:template match="table:table">
		<xsl:param name="automatic-styles" />
		<!-- table style -->
		<xsl:variable name="table-style-name"
			select="@table:style-name" />
		<xsl:variable name="table-style"
			select="$automatic-styles//style:style[@style:name=$table-style-name]/style:properties" />

		<fo:block>

			<fo:table>
				<xsl:attribute name="id"><xsl:value-of select="@table:name" /></xsl:attribute>
				<xsl:attribute name="width"><xsl:value-of select="$table-style/@style:width" /></xsl:attribute>
				<xsl:attribute name="margin-top"><xsl:value-of select="$table-style/@fo:margin-top" /></xsl:attribute>
				<xsl:attribute name="margin-bottom"><xsl:value-of select="$table-style/@fo:margin-bottom" /></xsl:attribute>
				<xsl:attribute name="margin-left"><xsl:value-of select="$table-style/@fo:margin-left" /></xsl:attribute>
				<xsl:attribute name="margin-right"><xsl:value-of select="$table-style/@fo:margin-right" /></xsl:attribute>

				<!-- build table columns -->
				<xsl:for-each select="table:table-column">

					<xsl:choose>
						<xsl:when
							test="@table:number-columns-repeated">
							<xsl:call-template name="iterateTableColumns">

								<xsl:with-param name="numIterations" select="@table:number-columns-repeated" />
								<xsl:with-param name="column" select="." />
								<xsl:with-param name="automatic-styles" select="$automatic-styles" />
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="iterateTableColumns">
								<xsl:with-param name="numIterations" select="'1'" />
								<xsl:with-param name="column" select="." />
								<xsl:with-param name="automatic-styles" select="$automatic-styles" />
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>

				</xsl:for-each>

				<xsl:apply-templates select="table:table-header-rows">
						<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
				</xsl:apply-templates>


				<fo:table-body>
					<xsl:apply-templates select="table:table-row">
						<xsl:with-param name="automatic-styles" select="$automatic-styles" />
					</xsl:apply-templates>
				</fo:table-body>

			</fo:table>
		</fo:block>

	</xsl:template>
 
	<!-- Table Headers -->
	<xsl:template match="table:table-header-rows">
		<xsl:param name="automatic-styles"/>
		<fo:table-header>
			<xsl:apply-templates>
				<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
			</xsl:apply-templates>
		</fo:table-header>
	</xsl:template>

	<!-- Table Rows -->
	<xsl:template match="table:table-row">

		<xsl:param name="automatic-styles" />
		
		<xsl:variable name="row-style-name" select="@table:style-name"/>
		<xsl:variable name="row-style" select="$automatic-styles//style:style[@style:name=$row-style-name]/style:properties" />
		
		<fo:table-row>
		
			<!-- apply default values -->
			<xsl:for-each select="$default-style-table-row/style:properties/@fo:*">
				<xsl:attribute name="{local-name()}"><xsl:value-of select="."/></xsl:attribute>
			</xsl:for-each>
		
			<xsl:if test="$row-style/@style:row-height">
				<xsl:attribute name="height"><xsl:value-of select="$row-style/@style:row-height"/></xsl:attribute>								
			</xsl:if>
		
			<xsl:choose>
				<xsl:when test="$row-style/@style:row-height">
					<xsl:apply-templates select="table:table-cell">
						<xsl:with-param name="automatic-styles" select="$automatic-styles" />
						<xsl:with-param name="cell-height" select="$row-style/@style:row-height"/>	
					</xsl:apply-templates>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates select="table:table-cell">
						<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
					</xsl:apply-templates>		
				</xsl:otherwise>
			</xsl:choose>
			
		</fo:table-row>
	</xsl:template>

	<!-- Table Cells -->

	<xsl:template match="table:table-cell">
		<xsl:param name="automatic-styles" />
		<xsl:param name="cell-height"/>
		
		<xsl:variable name="cell-style-name" select="@table:style-name" />
		<xsl:variable name="cell-style" select="$automatic-styles//style:style[@style:name=$cell-style-name]/style:properties" />

		<xsl:variable name="pos" select="position()" />
		<xsl:variable name="col-style" select="../../table:table-column[position()=$pos]/@table:style-name" />
		<!--parent:<xsl:value-of select="$col-style"/>:parent-->
		<xsl:variable name="col-style" select="$automatic-styles//style:style[@style:name=$col-style]/style:properties" />
		<xsl:variable name="col-width" select="$col-style/@style:column-width" />
		<xsl:variable name="col-width-value" select='substring-before($col-width, "cm")' />
		
		<fo:table-cell>

			<!-- copy the cell properties and change the sizes -->

			<xsl:attribute name="border-bottom">0.0001cm solid</xsl:attribute>						
			
			<xsl:for-each select="$cell-style/@fo:*">
				<xsl:attribute name="{local-name()}"><xsl:value-of select="."/></xsl:attribute>						
			</xsl:for-each>
						
			<xsl:for-each select="$cell-style/@fo:*">

				<xsl:variable name="attrib-name" select="local-name()"/>
				
				<xsl:if test="starts-with($attrib-name, 'border')">
				
					<xsl:variable name="propB" select='substring-before(., "cm")' />
					<xsl:choose>
						<xsl:when test="$propB">
							<xsl:variable name="propA" select='substring-after(., "cm")' />
							<xsl:variable name="propBNew" select='number($propB) * number("1")' />
							<xsl:variable name="prop" select='concat($propBNew, "cm ", $propA)' />													
							<xsl:attribute name="{local-name()}"><xsl:value-of select="$prop" /></xsl:attribute>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
			
				</xsl:if>
				
			</xsl:for-each>			
						
			<!-- background image -->
			<xsl:if test="$cell-style/style:background-image">
				<xsl:attribute name="background-image">
					<xsl:if test="$cell-style/style:background-image/@xlink:href">
						<xsl:value-of select="$cell-style/style:background-image/@xlink:href"/>
					</xsl:if>
					<xsl:if test="$cell-style/style:background-image/office:binary-data">data:image;base64,<xsl:value-of select="$cell-style/style:background-image/office:binary-data" /></xsl:if>
				</xsl:attribute>				
				<xsl:attribute name="background-repeat">
					<xsl:value-of select="$cell-style/style:background-image/@style:repeat"/>
				</xsl:attribute>				
			</xsl:if>

			<xsl:choose>
 
				<xsl:when test="$cell-height">
					<fo:block-container>
						<xsl:attribute name="height"><xsl:value-of select="$cell-height"/></xsl:attribute>
						<xsl:attribute name="overflow">hidden</xsl:attribute>
						<xsl:apply-templates>
							<xsl:with-param name="automatic-styles" select="$automatic-styles" />
							<xsl:with-param name="parent-width" select="$col-width-value" />
						</xsl:apply-templates>
					</fo:block-container>
				</xsl:when>

				<xsl:otherwise>
						<xsl:apply-templates>
							<xsl:with-param name="automatic-styles" select="$automatic-styles" />			
							<xsl:with-param name="parent-width" select="$col-width-value" />
						</xsl:apply-templates>										
				</xsl:otherwise>
			
			</xsl:choose>			

		</fo:table-cell>
	</xsl:template>

	<!-- Image -->
	<xsl:template match="draw:image">		
		<xsl:param name="automatic-styles" />
		<xsl:param name="parent-width" />
		<!--parent-width:<xsl:value-of select="$parent-width"/>:parent-width-->
 		
		<xsl:call-template name="create-image">
			<xsl:with-param name="component" select="."/>
			<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
			<xsl:with-param name="parent-width" select="$parent-width"/>
		</xsl:call-template>
		
	</xsl:template>
          
	<!-- SVG line -->
	<xsl:template match="draw:line">
		<xsl:param name="automatic-styles" />
		<fo:block-container position="absolute">
			<fo:block>
				<fo:instream-foreign-object>
					<svg:svg>
						<xsl:attribute name="viewBox">0 0 0 0</xsl:attribute>
						<xsl:attribute name="width"><xsl:value-of select="$page-width" /></xsl:attribute>
						<xsl:attribute name="height"><xsl:value-of select="$page-height" /></xsl:attribute>
						<svg:line>
							<xsl:for-each select="./@svg:*">
								<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
							</xsl:for-each>

							<!-- style -->
							<xsl:variable name="draw-style-name" select="@draw:style-name" />
							<xsl:variable name="draw-style"
								select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties" />
							<xsl:attribute name="style">
								<xsl:value-of select="concat('stroke-width:', $draw-style/@svg:stroke-width, ';stroke:', $draw-style/@svg:stroke-color)" />
							</xsl:attribute>
						</svg:line>
					</svg:svg>
				</fo:instream-foreign-object>
			</fo:block>
		</fo:block-container>
	</xsl:template>

	<!-- block container -->
  
	<xsl:template match="draw:text-box">
		<xsl:param name="automatic-styles" />
		<xsl:param name="parent-width" />
		<fo:block-container>
		
			<!-- borders -->
			<xsl:variable name="draw-style-name" select="@draw:style-name" />
			<xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties" />

			<xsl:call-template name="apply-border">
				<xsl:with-param name="component" select="."/>
				<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
			</xsl:call-template>

			<!--- paddings -->
			<xsl:if test="$draw-style/@fo:padding-top">
				<xsl:attribute name="padding-top"><xsl:value-of select="$draw-style/@fo:padding-top" /></xsl:attribute>
			</xsl:if>
			<xsl:if test="$draw-style/@fo:padding-bottom">
				<xsl:attribute name="padding-bottom"><xsl:value-of select="$draw-style/@fo:padding-bottom" /></xsl:attribute>
			</xsl:if>
			<xsl:if test="$draw-style/@fo:padding-left">
				<xsl:attribute name="padding-left"><xsl:value-of select="$draw-style/@fo:padding-left" /> </xsl:attribute>
			</xsl:if>
			<xsl:if test="$draw-style/@fo:padding-right">
				<xsl:attribute name="padding-right"><xsl:value-of select="$draw-style/@fo:padding-right" /></xsl:attribute>
			</xsl:if>

			<!-- FO properties -->
			<xsl:for-each select="$draw-style/@fo:*">
				<xsl:if test="local-name() != 'min-width'">
					<xsl:if test="(local-name() != 'min-height') and (local-name() != 'margin-left') and (local-name() != 'margin-right')">
						<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
					</xsl:if>
				</xsl:if>
			</xsl:for-each>

			<!-- Basic attributes -->
			<!-- Anchor type -->			
			<xsl:choose>
				<xsl:when test="@text:anchor-type = 'paragraph'">
					<xsl:attribute name="position">absolute</xsl:attribute>
				</xsl:when>
				<xsl:when test="@text:anchor-type = 'page'">
					<xsl:attribute name="position">fixed</xsl:attribute>
				</xsl:when>				
				<xsl:when test="@text:anchor-type = 'as-char'">
					<xsl:attribute name="position">relative</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="position">absolute</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>			
						
			<xsl:if test="@svg:x">
				<xsl:attribute name="left"><xsl:value-of select="./@svg:x" /></xsl:attribute>
			</xsl:if>

			<xsl:if test="@svg:y">
				<xsl:attribute name="top"><xsl:value-of select="./@svg:y" /></xsl:attribute>
			</xsl:if>
			
			<xsl:choose>
				<xsl:when test="@svg:width">
					<xsl:attribute name="width"><xsl:value-of select="@svg:width"/></xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:if test="@fo:min-width">
						<xsl:attribute name="min-width"><xsl:value-of select="@fo:min-width"/></xsl:attribute>
					</xsl:if>
				</xsl:otherwise>
			</xsl:choose>

			<xsl:choose>
				<xsl:when test="@svg:height">
					<xsl:attribute name="height"><xsl:value-of select="@svg:height"/></xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:if test="@fo:min-height">
						<xsl:attribute name="height"><xsl:value-of select="@fo:min-height"/></xsl:attribute>
					</xsl:if>
				</xsl:otherwise>
			</xsl:choose>			
    
			<xsl:apply-templates>
				<xsl:with-param name="automatic-styles" select="$automatic-styles" />
				<xsl:with-param name="test-param">paramvalue</xsl:with-param>
				<xsl:with-param name="parent-width" select="$parent-width"/>
			</xsl:apply-templates>
			<!--xsl:call-template name="apply-position">  
				<xsl:with-param name="component" select="."/>
			</xsl:call-template-->
		</fo:block-container>
	</xsl:template>

	<xsl:template match="text:tab-stop">
		<xsl:text> &#160;  &#160;</xsl:text>
	</xsl:template>

	<!-- block -->
	<xsl:template match="text:p">
		<xsl:param name="automatic-styles" />
		<xsl:param name="parent-width" />
		<fo:block>
			<!-- FO attributes -->
			<xsl:variable name="text-style-name"
				select="@text:style-name" />

			<xsl:if test="$default-style-paragraph/style:properties/@fo:font-size">
				<xsl:attribute name="font-size"><xsl:value-of select="$default-style-paragraph/style:properties/@fo:font-size" /></xsl:attribute>
			</xsl:if>

			<xsl:if test="$default-style-paragraph/style:properties/@fo:font-family">
				<xsl:attribute name="font-family"><xsl:value-of select="$default-style-paragraph/style:properties/@fo:font-family" /></xsl:attribute>
			</xsl:if>

			<xsl:if test="$default-style-paragraph/style:properties/@style:font-name">
					<xsl:variable name="font-name" select="$default-style-paragraph/style:properties/@style:font-name"/>					
					<!-- xsl:variable name="font-family_tmp" select='substring-before($font-name, "+")'/>
					<xsl:variable name="font-family" select='substring-before($font-family_tmp, "-Bold")'/ -->
					<xsl:attribute name="font-family"><xsl:value-of select="$font-name"/></xsl:attribute>				
			</xsl:if>

			<xsl:variable name="block-style" select="$automatic-styles//style:style[@style:name=$text-style-name]" />

			<xsl:if test="$block-style/style:properties/@style:font-name">
				<xsl:variable name="font-name" select="$block-style/style:properties/@style:font-name"/>
				<!-- xsl:variable name="font-family_tmp" select='substring-before($font-name, "+")'/>
				<xsl:variable name="font-family" select='substring-before($font-family_tmp, "-Bold")'/ -->
				<xsl:attribute name="font-family"><xsl:value-of select="$font-name"/></xsl:attribute>				
			</xsl:if>

			<xsl:for-each select="$block-style/style:properties/@fo:*">
				<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
			</xsl:for-each>

			<xsl:choose>
				<xsl:when test="string-length(current()) = 0">
					<xsl:text> &#160;</xsl:text>
					<xsl:apply-templates>
						<xsl:with-param name="automatic-styles" select="$automatic-styles" />
						<xsl:with-param name="parent-width" select="$parent-width"/>
					</xsl:apply-templates>
				</xsl:when>

				<xsl:otherwise>
					<xsl:apply-templates>
						<xsl:with-param name="automatic-styles" select="$automatic-styles" />
						<xsl:with-param name="parent-width" select="$parent-width"/>
					</xsl:apply-templates>
				</xsl:otherwise>
			</xsl:choose>

		</fo:block>
	</xsl:template>

	<!-- Hyperlink -->
	<xsl:template match="text:a">		
		<fo:basic-link>
			<xsl:attribute name="color">blue</xsl:attribute>
			<xsl:attribute name="text-decoration">underline</xsl:attribute>		
			<xsl:attribute name="external-destination"><xsl:value-of select="concat('url(', @xlink:href, ')')"/></xsl:attribute>
			<xsl:value-of select="."/>
		</fo:basic-link>

	</xsl:template>
	
	<!-- Inline -->
	<xsl:template match="text:span">
		<xsl:param name="automatic-styles" />
		<fo:inline>

			<!-- FO attributes -->
			<xsl:variable name="text-style-name" select="@text:style-name" />
			<xsl:variable name="text-style" select="$automatic-styles//style:style[@style:name=$text-style-name]/style:properties" />

			<!-- text position attribute -->
			<xsl:if test="$text-style/@style:text-position">
				<xsl:variable name="text-position" select="$text-style/@style:text-position" />
				<xsl:variable name="baseline-shift" select='substring-before($text-position, " ")' />
				<xsl:variable name="font-size" select='substring-after($text-position, " ")' />

				<xsl:attribute name="baseline-shift"><xsl:value-of select="$baseline-shift" /></xsl:attribute>
				<xsl:attribute name="font-size"><xsl:value-of select="$font-size" /> </xsl:attribute>
			</xsl:if>

			<!-- Underline -->
			<xsl:if test="$text-style/@style:text-underline = 'single'">
				<xsl:attribute name="text-decoration">underline</xsl:attribute>
			</xsl:if>

			<!-- Line through -->

			<xsl:if test="$text-style/@style:text-crossing-out = 'single-line'">
				<xsl:attribute name="text-decoration">line-through</xsl:attribute>
			</xsl:if>

			<!-- Copy FO attributes -->
			<xsl:for-each select="$text-style/@fo:*">
				<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
			</xsl:for-each>

			<xsl:value-of select="." />
		</fo:inline>
	</xsl:template>

	<!-- Page Template Setup -->
	<xsl:template name="pageTemplate">
		<xsl:param name="pagename" />
		<xsl:param name="regionname" />
		<fo:simple-page-master>
			<xsl:attribute name="margin-top"><xsl:value-of select="$page-margin-top" /></xsl:attribute>
			<xsl:attribute name="margin-bottom"><xsl:value-of select="$page-margin-bottom" /></xsl:attribute>
			<xsl:attribute name="margin-left"><xsl:value-of select="$page-margin-left" /></xsl:attribute>
			<xsl:attribute name="margin-right"><xsl:value-of select="$page-margin-right" /></xsl:attribute>
	
			<xsl:attribute name="page-width"><xsl:value-of select="$page-width" /></xsl:attribute>
			<xsl:attribute name="page-height"><xsl:value-of select="$page-height" /></xsl:attribute>
			<xsl:attribute name="master-name"><xsl:value-of select="$pagename" />-layout</xsl:attribute>
	
			<fo:region-body region-name="frame-body" margin-bottom="0mm" margin-top="0mm" margin-left="0mm" margin-right="0mm">
	
				<xsl:if test="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image/office:binary-data">
					<xsl:attribute name="background-image">
						<xsl:if test="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image/office:binary-data ">data:image;base64,<xsl:value-of select="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image/office:binary-data" /></xsl:if>
					</xsl:attribute>				
					<xsl:attribute name="background-repeat">
						<xsl:value-of select="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image/@style:repeat"/>
					</xsl:attribute>
	
					<xsl:choose>
						<xsl:when test="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image@style:position = 'top center'">
							<xsl:attribute name="background-position-horizontal">center</xsl:attribute>
							<xsl:attribute name="background-position-vertical">top</xsl:attribute>
						</xsl:when>
						<xsl:when test="office:document/office:automatic-styles/style:page-master/style:properties/style:background-image@style:position = 'top left'">
							<xsl:attribute name="background-position-horizontal">left</xsl:attribute>
							<xsl:attribute name="background-position-vertical">top</xsl:attribute>
						</xsl:when>
					</xsl:choose>							
				</xsl:if>
			</fo:region-body>
			<region-before extent="10mm">				
				<xsl:attribute name="region-name"><xsl:value-of select="$regionname" />-region</xsl:attribute>
			</region-before>
		</fo:simple-page-master>
	
	</xsl:template>
	
	<!-- Table column iterations -->
	<xsl:template name="iterateTableColumns">
		<xsl:param name="numIterations" />
		<xsl:param name="column" />
		<xsl:param name="automatic-styles" />

		<xsl:variable name="column-style-name" select="@table:style-name" />
		<xsl:variable name="column-style" select="$automatic-styles//style:style[@style:name=$column-style-name]/style:properties" />

		<xsl:for-each select="$random-nodes[position() &lt;= $numIterations]">
			<fo:table-column>
				<xsl:attribute name="column-width">
                      <xsl:value-of select="$column-style/@style:column-width" />
				</xsl:attribute>
			</fo:table-column>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet>