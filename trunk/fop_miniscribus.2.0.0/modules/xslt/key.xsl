<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:key name="user-only" match="konfig" use="@typ"/>

<xsl:template match="/">
 <html>
 <head>
 </head>
 <body>
 <table border="1">
  <xsl:for-each select="key('user-only','user')">
   <tr>
    <td><xsl:value-of select="@name" /></td>
    <td><xsl:value-of select="@wert" /></td>
   </tr>
  </xsl:for-each>
 </table>
 </body>
 </html>
</xsl:template>

</xsl:stylesheet>