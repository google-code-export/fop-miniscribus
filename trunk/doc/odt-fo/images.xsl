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

	<!-- Apply Positions -->
	<xsl:template name="create-image">
		<xsl:param name="component" />
		<xsl:param name="automatic-styles" />
		<xsl:param name="parent-width" />
		
		<!--parent-width:<xsl:value-of select="$parent-width"/>:parent-width-->
		<xsl:variable name="style-name" select="@draw:style-name" />
		<xsl:variable name="vertical-pos" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@style:vertical-pos" />

		<xsl:variable name="padding" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding" />
		<xsl:variable name="padding-left" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-left" />
		<xsl:variable name="padding-right" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-right" />
		<xsl:variable name="padding-top" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-top" />
		<xsl:variable name="padding-bottom" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-bottom" />
				
				
		<xsl:choose>
			<xsl:when test="($component/@text:anchor-type = 'page')">
				<fo:block-container>
					<xsl:variable name="draw-style-name" select="$component/@draw:style-name" />
					<xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties" />			
					<!--draw-style-name:<xsl:value-of select="$draw-style-name"/>:draw-style-name-->
					
					<!--xsl:for-each select="$draw-style/@fo:*">
						<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
					</xsl:for-each-->			
		
					<xsl:call-template name="apply-position">
						<xsl:with-param name="component" select="."/>
					</xsl:call-template>
					
					<fo:block>
						<fo:external-graphic>
							<xsl:if test="$padding">
								<xsl:attribute name="padding"><xsl:value-of select="$padding" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-top">
								<xsl:attribute name="padding-top"><xsl:value-of select="$padding-top" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-bottom">
								<xsl:attribute name="padding-bottom"><xsl:value-of select="$padding-bottom" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-left">
								<xsl:attribute name="padding-left"><xsl:value-of select="$padding-left" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-right">
								<xsl:attribute name="padding-right"><xsl:value-of select="$padding-right" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="@xlink:href">
								<xsl:attribute name="src"><xsl:value-of select="concat('url(', $component/@xlink:href, ')')" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="office:binary-data">
								<xsl:if test="@svg:width">
									<xsl:attribute name="content-width"><xsl:value-of select="substring-before($component/@svg:width - $padding, 'cm')" />cm</xsl:attribute>
								</xsl:if>
								<xsl:if test="@svg:height">
									<xsl:attribute name="content-height"><xsl:value-of select="substring-before($component/@svg:height - $padding, 'cm')" />cm</xsl:attribute>
								</xsl:if>
								<xsl:attribute name="src">url('data:image;base64,<xsl:value-of select="$component/office:binary-data" />')</xsl:attribute>
							</xsl:if>
							<xsl:call-template name="apply-border">
								<xsl:with-param name="component" select="."/>
								<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
							</xsl:call-template>
						</fo:external-graphic>
					</fo:block>		
				</fo:block-container>
			</xsl:when>
			<xsl:when test="($component/@text:anchor-type = 'paragraph')">
				<xsl:variable name="draw-style-name" select="$component/@draw:style-name" />
				<xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties" />			
				<!--horz-pos:<xsl:value-of select="$horz-pos"/>:horz-pos-->
				
				<fo:block-container>
					<xsl:variable name="horz-pos" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties/@style:horizontal-pos" />
					<!--draw-style-name:<xsl:value-of select="$draw-style-name"/>:draw-style-name-->
					
					<!--xsl:for-each select="$draw-style/@fo:*">
						<xsl:attribute name="{local-name()}"><xsl:value-of select="." /></xsl:attribute>
					</xsl:for-each-->			
		
					<xsl:call-template name="apply-position">
						<xsl:with-param name="component" select="."/>
					</xsl:call-template>
					
					<xsl:attribute name="left">.06cm</xsl:attribute>
					<xsl:if test="$horz-pos = 'right'">
						<xsl:variable name="image-width" select="substring-before($component/@svg:width, 'cm')" />
						<!--image-width:<xsl:value-of select="$image-width"/>:image-width
						parent-width:<xsl:value-of select="$parent-width"/>:parent-width
						left:<xsl:value-of select="($parent-width - $image-width)"/>:left-->
						<xsl:attribute name="left"><xsl:value-of select="($parent-width - $image-width - .06)" />cm</xsl:attribute>
					</xsl:if>
					
					<fo:block>
						<fo:external-graphic>
							<xsl:if test="$padding">
								<xsl:attribute name="padding"><xsl:value-of select="$padding" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-top">
								<xsl:attribute name="padding-top"><xsl:value-of select="$padding-top" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-bottom">
								<xsl:attribute name="padding-bottom"><xsl:value-of select="$padding-bottom" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-left">
								<xsl:attribute name="padding-left"><xsl:value-of select="$padding-left" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="$padding-right">
								<xsl:attribute name="padding-right"><xsl:value-of select="$padding-right" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="@xlink:href">
								<xsl:attribute name="src"><xsl:value-of select="concat('url(', $component/@xlink:href, ')')" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="office:binary-data">
								<xsl:if test="@svg:width">
									<xsl:attribute name="content-width"><xsl:value-of select="substring-before($component/@svg:width - $padding, 'cm')" />cm</xsl:attribute>
								</xsl:if>
								<xsl:if test="@svg:height">
									<xsl:attribute name="content-height"><xsl:value-of select="substring-before($component/@svg:height - $padding, 'cm')" />cm</xsl:attribute>
								</xsl:if>
								<xsl:attribute name="src">url('data:image;base64,<xsl:value-of select="$component/office:binary-data" />')</xsl:attribute>
							</xsl:if>
							<xsl:call-template name="apply-border">
								<xsl:with-param name="component" select="."/>
								<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
							</xsl:call-template>
						</fo:external-graphic>
					</fo:block>		
				</fo:block-container>
			</xsl:when>
			<xsl:otherwise>
				<fo:inline>
					<xsl:choose>
						<xsl:when test="$vertical-pos='middle'">
							<xsl:attribute name="alignment-baseline">after-edge</xsl:attribute>
						</xsl:when>
						<xsl:when test="$vertical-pos='bottom'">
							<xsl:attribute name="alignment-baseline">after-edge</xsl:attribute>
						</xsl:when>
					</xsl:choose>
					<fo:external-graphic>
						<xsl:choose>
							<xsl:when test="$vertical-pos='middle'">
								<xsl:attribute name="alignment-baseline">after-edge</xsl:attribute>
							</xsl:when>
							<xsl:when test="$vertical-pos='bottom'">
								<xsl:attribute name="baseline-shift">sub</xsl:attribute>
							</xsl:when>
						</xsl:choose>
						<xsl:if test="@xlink:href">
							<xsl:attribute name="src"><xsl:value-of select="concat('url(', $component/@xlink:href, ')')" /></xsl:attribute>
						</xsl:if>
						<xsl:if test="office:binary-data">
							<xsl:if test="@svg:width">
								<xsl:attribute name="content-width"><xsl:value-of select="$component/@svg:width" /></xsl:attribute>
							</xsl:if>
							<xsl:if test="@svg:height">
								<xsl:attribute name="content-height"><xsl:value-of select="$component/@svg:height" /></xsl:attribute>
							</xsl:if>
							<xsl:attribute name="src">url('data:image;base64,<xsl:value-of select="$component/office:binary-data" />')</xsl:attribute>
						</xsl:if>
						<xsl:call-template name="apply-border">
							<xsl:with-param name="component" select="."/>
							<xsl:with-param name="automatic-styles" select="$automatic-styles"/>
						</xsl:call-template>
					</fo:external-graphic>
				</fo:inline>		
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>
</xsl:stylesheet>