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
	<xsl:template name="apply-border">
		<xsl:param name="component" />
		<xsl:param name="automatic-styles" />

		<!-- borders -->
		<xsl:variable name="draw-style-name" select="$component/@draw:style-name" />
		<xsl:variable name="draw-style" select="$automatic-styles//style:style[@style:name=$draw-style-name]/style:properties" />
		<!--draw-style-name:<xsl:value-of select="$draw-style-name"/>:draw-style-name-->

		<xsl:variable name="border" select="//office:automatic-styles/style:style[@style:name=$draw-style-name]/style:properties/@fo:border" />
		<xsl:variable name="border-value">
			<xsl:choose>
				<xsl:when test="$border = 'none'">0</xsl:when>
				<xsl:when test="$border"><xsl:value-of select='substring-before($border, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="border-left" select="//office:automatic-styles/style:style[@style:name=$draw-style-name]/style:properties/@fo:border-left" />
		<xsl:variable name="border-left-value">
			<xsl:choose>
				<xsl:when test="$border-left = 'none'">0</xsl:when>
				<xsl:when test="$border-left"><xsl:value-of select='substring-before($border-left, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="border-right" select="//office:automatic-styles/style:style[@style:name=$draw-style-name]/style:properties/@fo:border-right" />
		<xsl:variable name="border-right-value">
			<xsl:choose>
				<xsl:when test="$border-right = 'none'">0</xsl:when>
				<xsl:when test="$border-right"><xsl:value-of select='substring-before($border-right, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="border-top" select="//office:automatic-styles/style:style[@style:name=$draw-style-name]/style:properties/@fo:border-top" />
		<xsl:variable name="border-top-value">
			<xsl:choose>
				<xsl:when test="$border-top = 'none'">0</xsl:when>
				<xsl:when test="$border-top"><xsl:value-of select='substring-before($border-top, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="border-bottom" select="//office:automatic-styles/style:style[@style:name=$draw-style-name]/style:properties/@fo:border-bottom" />
		<xsl:variable name="border-bottom-value">
			<xsl:choose>
				<xsl:when test="$border-bottom = 'none'">0</xsl:when>
				<xsl:when test="$border-bottom"><xsl:value-of select='substring-before($border-bottom, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!--border-bottom-style="solid" border-top-style="solid" border-left-style="solid" border-right-style="solid" 
		border-left-color="#000000" border-right-color="#000000" border-top-color="#000000" border-bottom-color="#000000" -->
		<xsl:if test="$border">
			<xsl:attribute name="border-bottom-style">solid</xsl:attribute>
			<xsl:attribute name="border-top-style">solid</xsl:attribute>
			<xsl:attribute name="border-left-style">solid</xsl:attribute>
			<xsl:attribute name="border-right-style">solid</xsl:attribute>
			<xsl:variable name="border-colour" select='substring-after($border, "#")' />
			<xsl:attribute name="border-top-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-bottom-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-left-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-right-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-left-width"><xsl:value-of select="$border-value" />cm</xsl:attribute>
			<xsl:attribute name="border-right-width"><xsl:value-of select="$border-value" />cm</xsl:attribute>
			<xsl:attribute name="border-top-width"><xsl:value-of select="$border-value" />cm</xsl:attribute>
			<xsl:attribute name="border-bottom-width"><xsl:value-of select="$border-value" />cm</xsl:attribute>
		</xsl:if>

		<xsl:if test="$border-top">
			<xsl:attribute name="border-top-style">solid</xsl:attribute>
			<xsl:variable name="border-colour" select='substring-after($border-top, "#")' />
			<xsl:attribute name="border-top-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-top-width"><xsl:value-of select="$border-top-value" />cm</xsl:attribute>
		</xsl:if>
				
		<xsl:if test="$border-bottom">
			<xsl:attribute name="border-bottom-style">solid</xsl:attribute>
			<xsl:variable name="border-colour" select='substring-after($border-bottom, "#")' />
			<xsl:attribute name="border-bottom-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-bottom-width"><xsl:value-of select="$border-bottom-value" />cm</xsl:attribute>
		</xsl:if>
				
		<xsl:if test="$border-left">
			<xsl:attribute name="border-left-style">solid</xsl:attribute>
			<xsl:variable name="border-colour" select='substring-after($border-left, "#")' />
			<xsl:attribute name="border-left-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-left-width"><xsl:value-of select="$border-left-value" />cm</xsl:attribute>
		</xsl:if>
		
		<xsl:if test="$border-right">
			<xsl:attribute name="border-right-style">solid</xsl:attribute>
			<xsl:variable name="border-colour" select='substring-after($border-right, "#")' />
			<xsl:attribute name="border-right-color"><xsl:value-of select="concat('#', $border-colour)" /></xsl:attribute>
			<xsl:attribute name="border-right-width"><xsl:value-of select="$border-right-value" />cm</xsl:attribute>
		</xsl:if>
		
		<!-- border style -->
		<xsl:if test="$draw-style/@draw:stroke='solid'">
			<xsl:attribute name="border-bottom-style">solid</xsl:attribute>
			<xsl:attribute name="border-top-style">solid</xsl:attribute>
			<xsl:attribute name="border-left-style">solid</xsl:attribute>
			<xsl:attribute name="border-right-style">solid</xsl:attribute>
		</xsl:if>

		<xsl:if test="$draw-style/@svg:stroke-width">
			<xsl:variable name="stroke-width" select="$draw-style/@svg:stroke-width" />
			<xsl:attribute name="border-left-width"><xsl:value-of select="$stroke-width" /></xsl:attribute>
			<xsl:attribute name="border-right-width"><xsl:value-of select="$stroke-width" /></xsl:attribute>
			<xsl:attribute name="border-top-width"><xsl:value-of select="$stroke-width" /> </xsl:attribute>
			<xsl:attribute name="border-bottom-width"><xsl:value-of select="$stroke-width" /></xsl:attribute>
		</xsl:if>

		<xsl:if test="$draw-style/@svg:stroke-color">
			<xsl:variable name="stroke-color" select="$draw-style/@svg:stroke-color" />
			<xsl:attribute name="border-left-color"><xsl:value-of select="$stroke-color" /></xsl:attribute>
			<xsl:attribute name="border-right-color"><xsl:value-of select="$stroke-color" /></xsl:attribute>
			<xsl:attribute name="border-top-color"><xsl:value-of select="$stroke-color" /></xsl:attribute>
			<xsl:attribute name="border-bottom-color"><xsl:value-of select="$stroke-color" /></xsl:attribute>
		</xsl:if>

	</xsl:template>

</xsl:stylesheet>