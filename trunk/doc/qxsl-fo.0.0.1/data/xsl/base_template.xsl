<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format"  version="1.0">

<xsl:template match="report">

<fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">

    <!-- defines page layout -->
    <fo:layout-master-set>

	<fo:simple-page-master master-name="only" page-height="297mm" page-width="210mm" margin="10mm" margin-top="10mm" margin-bottom="10mm" margin-left="10mm" margin-right="10mm">
	    <fo:region-body margin-top="10mm" margin-bottom="5mm" margin-left="20mm" margin-right="25mm"/>
	    <fo:region-before precedence="false" extent="15mm"/>
	    <fo:region-after precedence="true" extent="10mm"/>
	    <fo:region-start extent="30mm"/>
	    <fo:region-end extent="20mm"/>
	</fo:simple-page-master>

    </fo:layout-master-set>
    <!-- end: defines page layout -->

  <fo:page-sequence master-reference="only" initial-page-number="1">

    <fo:static-content flow-name="xsl-region-before">
	<fo:block font-size="12pt" font-family="sans-serif" line-height="15pt" space-after.optimum="3pt" text-align="left">
	** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
	** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
	</fo:block>
    </fo:static-content> 

    <fo:static-content flow-name="xsl-region-start">
	<fo:block font-size="12pt" font-family="sans-serif" line-height="15pt" space-after.optimum="3pt" text-align="left">
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	</fo:block>
    </fo:static-content> 

    <fo:static-content flow-name="xsl-region-end">
	<fo:block font-size="12pt" font-family="sans-serif" line-height="15pt" space-after.optimum="3pt" text-align="left">
        aa bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp rr ss tt uu vv
	</fo:block>
    </fo:static-content> 

    <fo:static-content flow-name="xsl-region-after">
	<fo:block font-size="12pt" font-family="sans-serif" line-height="15pt" space-after.optimum="3pt" text-align="left">
        aa bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp rr ss tt uu vv
	</fo:block>
    </fo:static-content> 

    <fo:flow flow-name="xsl-region-body">
	
        <fo:table>
    	    <fo:table-body>

    		<fo:table-row>	
	
    		    <fo:table-cell>
        		<fo:block>cis_predm aa bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp rr ss tt uu vv xx yy aa bb cc dd ee ff gg hh ii jj kk ll mm nn</fo:block>
    		    </fo:table-cell>
		    
    		    <fo:table-cell>
        		<fo:block>nazov</fo:block>
    		    </fo:table-cell>
	    
    		</fo:table-row>

		<xsl:apply-templates select="telo/skrok/predmet"/>	    	
	    
		
    	    </fo:table-body>
	</fo:table>


    	<fo:block font-size="10pt" font-family="sans-serif" text-align="left">
    	    This document specifies a syntax created by subsetting an existing, widely used international text processing standard
    	    (Standard Generalized Markup Language, ISO 8879:1986(E) as amended and corrected) for use on the World Wide Web. It
    	    is a product of the W3C XML Activity, details of which can be found at http://www.w3.org/XML. A list of current W3C
    	    Recommendations and other technical documents can be found at http://www.w3.org/TR.
	</fo:block> 
	
    </fo:flow>

  </fo:page-sequence>
  
</fo:root>

</xsl:template>


<xsl:template match="telo/skrok/predmet">
    	<fo:table-row>	
	
    	    <fo:table-cell>
        	<fo:block><xsl:value-of select="cis_predm"/></fo:block>
    	    </fo:table-cell>
		    
    	    <fo:table-cell>
        	<fo:block><xsl:value-of select="nazov"/></fo:block>
    	    </fo:table-cell>
	    
    	</fo:table-row>
<!--<fo:block font-size="12pt" font-family="sans-serif" line-height="15pt" space-after.optimum="3pt" text-align="left">
	iii
	
	aaa
    </fo:block>
-->
</xsl:template>

</xsl:stylesheet>
