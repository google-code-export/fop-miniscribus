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
	<xsl:template name="apply-position">

		<xsl:param name="component" />
		<!-- Anchor type -->
		<xsl:if test="@svg:x">
			<xsl:attribute name="left"><xsl:value-of select="$component/@svg:x" /></xsl:attribute>			
		</xsl:if>
		<xsl:if test="@svg:y">
			<xsl:attribute name="top"><xsl:value-of  select="$component/@svg:y" /></xsl:attribute>
		</xsl:if>
		<xsl:if test="@svg:width">
			<xsl:attribute name="width"><xsl:value-of select="$component/@svg:width" /></xsl:attribute>
		</xsl:if>
		<xsl:if test="@svg:height">
			<xsl:attribute name="height"><xsl:value-of select="$component/@svg:height" /></xsl:attribute>
		</xsl:if>
	
		<xsl:variable name="style-name" select="../@draw:style-name" />

		<xsl:variable name="parent-border" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:border" />
		<xsl:variable name="parent-border-value">
			<xsl:choose>
				<xsl:when test="$parent-border = 'none'">0</xsl:when>
				<xsl:when test="$parent-border"><xsl:value-of select='substring-before($parent-border, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-border-left" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:border-left" />
		<xsl:variable name="parent-border-left-value">
			<xsl:choose>
				<xsl:when test="$parent-border-left = 'none'">0</xsl:when>
				<xsl:when test="$parent-border-left"><xsl:value-of select='substring-before($parent-border-left, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-border-right" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:border-right" />
		<xsl:variable name="parent-border-right-value">
			<xsl:choose>
				<xsl:when test="$parent-border-right = 'none'">0</xsl:when>
				<xsl:when test="$parent-border-right"><xsl:value-of select='substring-before($parent-border-right, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-border-top" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:border-top" />
		<xsl:variable name="parent-border-top-value">
			<xsl:choose>
				<xsl:when test="$parent-border-top = 'none'">0</xsl:when>
				<xsl:when test="$parent-border-top"><xsl:value-of select='substring-before($parent-border-top, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-border-bottom" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:border-bottom" />
		<xsl:variable name="parent-border-bottom-value">
			<xsl:choose>
				<xsl:when test="$parent-border-bottom = 'none'">0</xsl:when>
				<xsl:when test="$parent-border-bottom"><xsl:value-of select='substring-before($parent-border-bottom, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-padding" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding" />
		<xsl:variable name="parent-padding-value">
			<xsl:choose>
				<xsl:when test="$parent-padding"><xsl:value-of select='substring-before($parent-padding, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-padding-left" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-left" />
		<xsl:variable name="parent-padding-left-value">
			<xsl:choose>
				<xsl:when test="$parent-padding-left"><xsl:value-of select='substring-before($parent-padding-left, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-padding-right" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-right" />
		<xsl:variable name="parent-padding-right-value">
			<xsl:choose>
				<xsl:when test="$parent-padding-right"><xsl:value-of select='substring-before($parent-padding-right, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-padding-top" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-top" />
		<xsl:variable name="parent-padding-top-value">
			<xsl:choose>
				<xsl:when test="$parent-padding-top"><xsl:value-of select='substring-before($parent-padding-top, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="parent-padding-bottom" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:padding-bottom" />
		<xsl:variable name="parent-padding-bottom-value">
			<xsl:choose>
				<xsl:when test="$parent-padding-bottom"><xsl:value-of select='substring-before($parent-padding-bottom, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<xsl:variable name="parent-margin-left" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:margin-left" />
		<xsl:variable name="parent-margin-left-value">
			<xsl:choose>
				<xsl:when test="$parent-margin-left = 'none'">0</xsl:when>
				<xsl:when test="$parent-margin-left"><xsl:value-of select='substring-before($parent-margin-left, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<xsl:variable name="parent-margin-right" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:margin-right" />
		<xsl:variable name="parent-margin-right-value">
			<xsl:choose>
				<xsl:when test="$parent-margin-right = 'none'">0</xsl:when>
				<xsl:when test="$parent-margin-right"><xsl:value-of select='substring-before($parent-margin-right, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<xsl:variable name="parent-margin-top" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:margin-top" />
		<xsl:variable name="parent-margin-top-value">
			<xsl:choose>
				<xsl:when test="$parent-margin-top = 'none'">0</xsl:when>
				<xsl:when test="$parent-margin-top"><xsl:value-of select='substring-before($parent-margin-top, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<xsl:variable name="parent-margin-bottom" select="//office:automatic-styles/style:style[@style:name=$style-name]/style:properties/@fo:margin-bottom" />
		<xsl:variable name="parent-margin-bottom-value">
			<xsl:choose>
				<xsl:when test="$parent-margin-bottom = 'none'">0</xsl:when>
				<xsl:when test="$parent-margin-bottom"><xsl:value-of select='substring-before($parent-margin-bottom, "cm")' /></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!--xsl:variable name="parent-border-value" select='substring-before($parent-border, "cm")'/-->
		<!--parent-padding:<xsl:value-of select="$parent-padding"/>:parent-padding
		parent-padding-value:<xsl:value-of select="$parent-padding-value"/>:parent-padding-value-->
		
		<!--style-name:<xsl:value-of select="$style-name"/>:style-name
		parent-border-value:<xsl:value-of select="$parent-border"/>:parent-border-value
		here-->
		<!--:<xsl:value-of select="$horz-pos"/>:-->
		
		<xsl:choose>
			<!--xsl:when test="$component/@text:anchor-type = 'paragraph'">
				<xsl:attribute name="text-align">right</xsl:attribute>
			</xsl:when-->
			<xsl:when test="$component/@text:anchor-type = 'page'">
				<xsl:attribute name="position">fixed</xsl:attribute>
				<!--xsl:attribute name="left">0cm</xsl:attribute>
				<xsl:attribute name="top">0cm</xsl:attribute-->
			</xsl:when>				
			<xsl:when test="($component/@text:anchor-type = 'frame') or ($component/@text:anchor-type = 'paragraph')">
				<xsl:variable name="image-style-name" select="$component/@draw:style-name" />
	
				<xsl:variable name="image-width" select="$component/@svg:width" />
				<xsl:variable name="image-width-value" select='substring-before($image-width, "cm")' />
				
				<xsl:variable name="parent-width" select="../@svg:width" />
				<xsl:variable name="parent-width-value" select='substring-before($parent-width, "cm")' />
				
				<xsl:variable name="parent-height" select="../@svg:width" />
				<xsl:variable name="parent-height-value" select='substring-before($parent-height, "cm")' />
				
				<xsl:variable name="horz-pos" select="//office:automatic-styles/style:style[@style:name=$image-style-name]/style:properties/@style:horizontal-pos" />
				<xsl:attribute name="position">absolute</xsl:attribute>
	
				<xsl:choose>
					<xsl:when test="$horz-pos = 'right'">
						<xsl:variable name="x-pos" select="$parent-width-value - $image-width-value" />
						<xsl:variable name="x-pos" select="$x-pos + $parent-padding-value + $parent-border-value" />
						<xsl:variable name="x-pos" select="$x-pos + $parent-border-right-value" />
						<!--xsl:variable name="x-pos" select="$x-pos - $parent-margin-left-value - $parent-margin-left-value" /-->

						<xsl:variable name="y-pos" select="-0.062" />
						<xsl:variable name="y-pos" select="$y-pos - $parent-padding-value - $parent-border-value" />
						<xsl:variable name="y-pos" select="$y-pos - $parent-padding-top-value - $parent-border-top-value" />
						<!--xsl:variable name="y-pos" select="$y-pos - $parent-margin-top-value - $parent-margin-top-value" /-->
						
						<xsl:attribute name="left"><xsl:value-of select="$x-pos"/>cm</xsl:attribute>
						<xsl:attribute name="top"><xsl:value-of select="$y-pos"/>cm</xsl:attribute>
					</xsl:when>
					<xsl:when test="$horz-pos = 'center'">
						<xsl:variable name="overall-width" select="$parent-width-value - $image-width-value - $parent-padding-value - $parent-border-value + 0.002" />
						<xsl:attribute name="left"><xsl:value-of select="$overall-width div 2"/>cm</xsl:attribute>
						<xsl:attribute name="top"><xsl:value-of select="-$parent-padding-value - $parent-border-value -$parent-padding-top-value - $parent-border-top-value -0.06"/>cm</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>

						<xsl:variable name="x-pos" select="0.000" />
						<xsl:variable name="x-pos" select="$x-pos - $parent-padding-value - $parent-border-value" />
						<xsl:variable name="x-pos" select="$x-pos - $parent-padding-left-value - $parent-border-left-value" />
						<!--xsl:variable name="x-pos" select="$x-pos - $parent-margin-left-value - $parent-margin-left-value" /-->

						<xsl:variable name="y-pos" select="-0.062" />
						<xsl:variable name="y-pos" select="$y-pos - $parent-padding-value - $parent-border-value" />
						<xsl:variable name="y-pos" select="$y-pos - $parent-padding-top-value - $parent-border-top-value" />
						<!--xsl:variable name="y-pos" select="$y-pos - $parent-margin-top-value - $parent-margin-top-value" /-->

						<!--$parent-padding-value:<xsl:value-of select="$parent-padding-value"/>:$parent-padding-value
						$parent-border-value:<xsl:value-of select="$parent-border-value"/>:$parent-border-value
						$parent-padding-left-value:<xsl:value-of select="$parent-border-left-value"/>:$parent-border-left-value
						$parent-padding-top-value:<xsl:value-of select="$parent-border-top-value"/>:$parent-border-top-value
						$x-pos:<xsl:value-of select="$x-pos"/>:$x-pos-->

						<xsl:attribute name="left"><xsl:value-of select="$x-pos"/>cm</xsl:attribute>
						<xsl:attribute name="top"><xsl:value-of select="$y-pos"/>cm</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
				
			</xsl:when>				
			<xsl:otherwise>
				<xsl:attribute name="position">absolute</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>