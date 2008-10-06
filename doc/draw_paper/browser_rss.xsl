<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:param name="unixtime" select="'0000000'" />
<xsl:param name="ddate" select="'0000000'" />
<xsl:param name="ddformat" select="'0000000'" />



<xsl:template match="/">
 <html>
 <head>
 </head>
 <body>
   <h1>Date <xsl:value-of select="$ddate" /></h1>
  <h2>UnixTime <xsl:value-of select="$unixtime" /></h2>
   <br/>
   
   <p>Format path: <xsl:value-of select="$ddformat" /></p>
   
   <br/>

 <ul>
  <xsl:for-each select="rss/channel/item">
   <li><h2><xsl:value-of select="./title" /></h2><br/><i><xsl:value-of select="./pubDate" /></i><br/><xsl:value-of select="./link" /><br/>
   <xsl:value-of select="./description" />
   </li>
  </xsl:for-each>
 </ul>
 </body>
 </html>
</xsl:template>

</xsl:stylesheet>