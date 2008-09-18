<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:param name="unixtime" select="'0000000'" />

<xsl:template match="/">
 <html>
 <head>
 </head>
 <body>

  <h1>UnixTime <xsl:value-of select="$unixtime" /></h1>


 <table border="1">
  <xsl:for-each select="test/konfig">
   <tr>
    <td><xsl:value-of select="@name" /></td>
    <td><xsl:value-of select="@wert" /></td>
    <td><xsl:value-of select="@typ" /></td>
   </tr>
  </xsl:for-each>
 </table>
 </body>
 </html>
</xsl:template>

</xsl:stylesheet>