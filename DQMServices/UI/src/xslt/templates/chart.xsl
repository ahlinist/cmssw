<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>

<xsl:template match="/">
  <xsl:text>{ "byRunNumber": {</xsl:text>

      <xsl:text>"rate": </xsl:text>
        <xsl:text>{</xsl:text>
          <xsl:text>"label": "Rate, Hz",</xsl:text>
          <xsl:text>"data": [</xsl:text>
            <xsl:for-each select="/RUNS/RUN">
              <xsl:if test="position()&gt;1"><xsl:text>,</xsl:text></xsl:if>
              <xsl:apply-templates select=".">
                <xsl:with-param name="x"><xsl:value-of select="NUMBER"/></xsl:with-param>
                <xsl:with-param name="y"><xsl:value-of select="RATE"/></xsl:with-param>
              </xsl:apply-templates>
            </xsl:for-each>
          <xsl:text>],</xsl:text>
        <xsl:text>},</xsl:text>

      <xsl:text>"events": </xsl:text>
        <xsl:text>{</xsl:text>
          <xsl:text>"label": "Events",</xsl:text>
          <xsl:text>"data": [</xsl:text>
            <xsl:for-each select="/RUNS/RUN">
              <xsl:if test="position()&gt;1"><xsl:text>,</xsl:text></xsl:if>
              <xsl:apply-templates select=".">
                <xsl:with-param name="x"><xsl:value-of select="NUMBER"/></xsl:with-param>
                <xsl:with-param name="y"><xsl:value-of select="EVENTS"/></xsl:with-param>
              </xsl:apply-templates>
            </xsl:for-each>
          <xsl:text>]</xsl:text>
        <xsl:text>},</xsl:text>

      <xsl:text>"bfield": </xsl:text>
        <xsl:text>{</xsl:text>
          <xsl:text>"label": "B Field, T",</xsl:text>
          <xsl:text>"data": [</xsl:text>
            <xsl:for-each select="/RUNS/RUN">
              <xsl:if test="position()&gt;1"><xsl:text>,</xsl:text></xsl:if>
              <xsl:apply-templates select=".">
                <xsl:with-param name="x"><xsl:value-of select="NUMBER"/></xsl:with-param>
                <xsl:with-param name="y"><xsl:value-of select="BFIELD"/></xsl:with-param>
              </xsl:apply-templates>
            </xsl:for-each>
          <xsl:text>]</xsl:text>
        <xsl:text>},</xsl:text>

    <xsl:text>}</xsl:text>
  <xsl:text>}</xsl:text>
</xsl:template>

<xsl:template match="RUN">
  <xsl:param name="x"/>
  <xsl:param name="y"/>

  <xsl:text>[</xsl:text>
  <xsl:value-of select="$x"/>
  <xsl:text>,</xsl:text>
  <xsl:value-of select="$y"/>
  <xsl:text>]</xsl:text>
</xsl:template>

</xsl:stylesheet>
