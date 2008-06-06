<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format"  version="1.0">

<xsl:template match="report">

<fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">

    <!-- defines page layout -->
    <fo:layout-master-set>

	<fo:simple-page-master master-name="only" page-height="297mm" page-width="210mm" margin="10mm" margin-top="10mm" margin-bottom="10mm" margin-left="10mm" margin-right="10mm">
	    <fo:region-body margin-top="8mm" margin-bottom="5mm" margin-left="20mm" margin-right="25mm"/>
	    <fo:region-before precedence="false" extent="10mm"/>
	    <fo:region-after precedence="true" extent="16mm"/>
	    <fo:region-start extent="20mm"/>
	    <fo:region-end extent="25mm"/>
	</fo:simple-page-master>

    </fo:layout-master-set>
    <!-- end: defines page layout -->

  <fo:page-sequence master-reference="only" initial-page-number="1">

    <!-- top -->
    <fo:static-content flow-name="xsl-region-before">
	<fo:block font-size="10pt" font-family="sans-serif" background-color="blue" text-align="left">
	    topb
	</fo:block>
    </fo:static-content> 

    <!-- left -->
    <fo:static-content flow-name="xsl-region-start">
	<fo:block font-size="10pt" font-family="sans-serif"  background-color="blue"  text-align="left">
	    lefts
	</fo:block>
    </fo:static-content> 

    <!-- right  -->
    <fo:static-content flow-name="xsl-region-end">
	<fo:block font-size="10pt" font-family="sans-serif" background-color="blue"  text-align="left">
	    righte
	</fo:block>
    </fo:static-content> 

    <!-- bottom  -->
    <fo:static-content flow-name="xsl-region-after">
	<fo:block font-size="10pt" font-family="sans-serif" background-color="yellow"  text-align="left">
	    bottoma
	</fo:block>
    </fo:static-content> 

    <fo:flow flow-name="xsl-region-body" background-color="blue">
	<!-- block container -->
 	<fo:block-container position="absolute" top="10mm" left="60mm" width="60mm">
	    <fo:block font-size="10pt" font-family="sans-serif" background-color="blue" text-align="left">
		fo:block inside of block-container fo:block inside of block-container
	    </fo:block>
 	</fo:block-container>

      <!-- Velice jednoduchá tabulka -->
      <fo:table>

        <fo:table-body>

          <fo:table-row>
            <fo:table-cell>
              <fo:block font-size="10pt">A</fo:block>
            </fo:table-cell>
            <fo:table-cell>
              <fo:block font-size="10pt">B</fo:block>
            </fo:table-cell>
          </fo:table-row>

          <fo:table-row>
            <fo:table-cell>
              <fo:block font-size="10pt">C</fo:block>
            </fo:table-cell>
            <fo:table-cell>
              <fo:block font-size="10pt">D</fo:block>
            </fo:table-cell>
          </fo:table-row>

        </fo:table-body>

      </fo:table>


	<!-- first picture  -->
	<fo:block font-size="10pt" font-family="sans-serif" text-align="left">
	    aa
	    <fo:external-graphic src="url(./pict.jpg)" content-width="20mm" />
	    bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp rr ss tt uu vv xx yy zz
	</fo:block>
	<!-- first block  -->
	<fo:block font-size="10pt" font-family="sans-serif" background-color="red" text-align="left">
    	    This document specifies a syntax created by subsetting an existing, widely used international text processing standard
    	    (Standard Generalized Markup Language, ISO 8879:1986(E) as amended and corrected) for use on the World Wide Web. It
    	    is a product of the W3C XML Activity, details of which can be found at http://www.w3.org/XML. A list of current W3C
    	    Recommendations and other technical documents can be found at http://www.w3.org/TR.	
	</fo:block>
	<!-- second block  -->
	<fo:block font-size="10pt" font-family="sans-serif" background-color="red" text-align="left">
	    Second block
	</fo:block>
	<!-- second picture  -->
	<fo:block font-size="10pt" font-family="sans-serif" text-align="left">
	    This document specifies a syntax created by subsetting an existing, widely used international text processing standard
	    block bifore picture
	    fo:external-graphic src="url(./pict.jpg)" content-width="20mm" />position="absolute"
	    block after picture
	</fo:block>

	<!-- third block  -->
	<fo:block font-size="10pt" font-family="sans-serif" background-color="yellow" text-align="left">
    	    This document specifies a syntax created by subsetting an existing, widely used international text processing standard
    	    (Standard Generalized Markup Language, ISO 8879:1986(E) as amended and corrected) for use on the World Wide Web. It
    	    is a product of the W3C XML Activity, details of which can be found at http://www.w3.org/XML. A list of current W3C
    	    Recommendations and other technical documents can be found at http://www.w3.org/TR.	
	</fo:block>

    </fo:flow>
    
    <!-- 
    	    This document specifies a syntax created by subsetting an existing, widely used international text processing standard
    	    (Standard Generalized Markup Language, ISO 8879:1986(E) as amended and corrected) for use on the World Wide Web. It
    	    is a product of the W3C XML Activity, details of which can be found at http://www.w3.org/XML. A list of current W3C
    	    Recommendations and other technical documents can be found at http://www.w3.org/TR.	
    -->

  </fo:page-sequence>
  
</fo:root>

</xsl:template>

</xsl:stylesheet>
