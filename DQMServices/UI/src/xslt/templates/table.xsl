<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:strip-space elements="*"/>

<xsl:output method="html" omit-xml-declaration="yes" indent="no"/>

<xsl:template match="/">
  <html>
    <head>
      <style>
body, td, th {
  font-family: Arial, Helvetica, sans-serif;
  font-size: 12px;
}
table {
	border-spacing: 0px;
	border-collapse: separate;
        empty-cells: show;
}
td, th {
	border: thin solid #BCD884;
	padding: 1px 5px 1px 5px;
}
      </style>
    </head>
    <body>
      <table>
        <xsl:apply-templates select="/RUNS/RUN"/>
      </table>
    </body>
  </html>
</xsl:template>

<xsl:template match="RUN">

  <xsl:if test="position()=1">
    <tr>
    <xsl:for-each select="*">
      <xsl:choose>
        <xsl:when test="name()='SUBSYSTEM'">
          <th>
            <xsl:value-of select="@name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@shifter"/>
          </th>
          <th>
            <xsl:value-of select="@name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@shifter"/>
            <xsl:text>_comment</xsl:text>
          </th>
        </xsl:when>
        <xsl:when test="name()='L1SOURCE'">
          <th>
            <xsl:text>L1_</xsl:text><xsl:value-of select="@name"/>
          </th>
          <th>
            <xsl:text>L1_</xsl:text><xsl:value-of select="@name"/><xsl:text>_comment</xsl:text>
          </th>
        </xsl:when>
        <xsl:otherwise>
          <th>
            <xsl:value-of select="name()"/>
          </th>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
    </tr>
    <xsl:text>&#10;</xsl:text>
  </xsl:if>

  <tr>
  <xsl:for-each select="*">
    <xsl:choose>
      <xsl:when test="name()='SUBSYSTEM'">
        <td>
          <xsl:value-of select="@value"/>
        </td>
      </xsl:when>
      <xsl:when test="name()='L1SOURCE'">
        <td>
          <xsl:value-of select="@value"/>
        </td>
      </xsl:when>
    </xsl:choose>
    <td>
      <xsl:value-of select="normalize-space(text())"/>
    </td>
  </xsl:for-each>
  </tr>
  <xsl:text>&#10;</xsl:text>
  
</xsl:template>

</xsl:stylesheet>

