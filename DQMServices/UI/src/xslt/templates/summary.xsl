<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>

<xsl:template match="/">

  <xsl:text>{
</xsl:text>
  
  <xsl:for-each select="/RESULTS/ROW">
    <xsl:sort select="SUB_ABBR"/>
    <xsl:sort select="NAME"/>
    <xsl:sort select="FOLDER"/>

    <xsl:variable name="prev_position" select="position()-1"/>

    <xsl:variable name="prev_sub">
      <xsl:if test="$prev_position&gt;0">
        <xsl:for-each select="/RESULTS/ROW">
          <xsl:sort select="SUB_ABBR"/>
          <xsl:sort select="NAME"/>
          <xsl:sort select="FOLDER"/>
          <xsl:if test="$prev_position=position()">
            <xsl:value-of select="SUB_ABBR"/>
          </xsl:if> 
        </xsl:for-each>
      </xsl:if>
    </xsl:variable>

    <xsl:variable name="prev_name">
      <xsl:if test="$prev_position&gt;0">
        <xsl:for-each select="/RESULTS/ROW">
          <xsl:sort select="SUB_ABBR"/>
          <xsl:sort select="NAME"/>
          <xsl:sort select="FOLDER"/>
          <xsl:if test="$prev_position=position()">
            <xsl:value-of select="NAME"/>
          </xsl:if> 
        </xsl:for-each>
      </xsl:if>
    </xsl:variable>

    <xsl:if test="$prev_sub!=SUB_ABBR">
      <xsl:if test="position()&gt;1"><xsl:text>}},
</xsl:text></xsl:if>
      <xsl:text>"</xsl:text><xsl:value-of select="SUB_ABBR"/>
      <xsl:text>":{"</xsl:text>
      <xsl:value-of select="NAME"/>
      <xsl:text>":{</xsl:text>
    </xsl:if>

    <xsl:if test="$prev_sub=SUB_ABBR and $prev_name!=NAME">
      <xsl:text>},"</xsl:text>
      <xsl:value-of select="NAME"/>
      <xsl:text>":{</xsl:text>
    </xsl:if>

    <xsl:if test="$prev_sub=SUB_ABBR and $prev_name=NAME">
      <xsl:text>,</xsl:text>
    </xsl:if>

    <xsl:text>"</xsl:text>
    <xsl:value-of select="FOLDER"/>
    <xsl:text>":</xsl:text>
    <xsl:value-of select="VALUE"/>

  </xsl:for-each>

  <xsl:if test="count(/RESULTS/ROW)&gt;0">
    <xsl:text>}}</xsl:text>
  </xsl:if>

  <xsl:text>
}</xsl:text>

</xsl:template>

</xsl:stylesheet>
