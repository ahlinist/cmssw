<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>

<xsl:include href="text.xsl"/>

<xsl:template match="/">
  <xsl:apply-templates select="/RUNS/RUN">
    <xsl:with-param name="sep">
      <xsl:text>&#9;</xsl:text>
    </xsl:with-param>
    <xsl:with-param name="quote">
      <xsl:text>0</xsl:text>
    </xsl:with-param>
  </xsl:apply-templates>
</xsl:template>

</xsl:stylesheet>
