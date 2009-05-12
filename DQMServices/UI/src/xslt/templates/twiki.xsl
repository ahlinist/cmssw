<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>

<xsl:template match="/">

  <xsl:text>| Run# | </xsl:text>
  <xsl:text>Events | </xsl:text>
  <xsl:text>Start Time | </xsl:text>
  <xsl:text>End Time | </xsl:text>
  <xsl:text>L1 Sources | </xsl:text>
  <xsl:text>Subsystems |</xsl:text>
  <xsl:text>&#10;</xsl:text>

  <xsl:apply-templates select="/RUNS/RUN">
  </xsl:apply-templates>

</xsl:template>

<xsl:template match="RUN">
  <xsl:text>| </xsl:text>
  <xsl:value-of select="NUMBER"/>
  <xsl:text> | </xsl:text>
  <xsl:value-of select="EVENTS"/>
  <xsl:text> | </xsl:text>
  <xsl:value-of select="START_TIME"/>
  <xsl:text> | </xsl:text>
  <xsl:value-of select="END_TIME"/>
  <xsl:text> | </xsl:text>
  <xsl:value-of select="L1SOURCES"/>
  <xsl:text> | </xsl:text>
  <xsl:value-of select="SUBSYSTEMS"/>
  <xsl:text> |</xsl:text>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

</xsl:stylesheet>
