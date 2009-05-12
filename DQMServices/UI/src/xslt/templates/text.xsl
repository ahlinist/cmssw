<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:strip-space elements="*"/>

<xsl:template match="RUN">
  <xsl:param name="sep"/>
  <xsl:param name="quote"/>

  <xsl:variable name="dq">"</xsl:variable>

  <xsl:if test="position()=1">
    <xsl:for-each select="*">
      <xsl:choose>
        <xsl:when test="name()='SUBSYSTEM'">
          <xsl:value-of select="@name"/>
          <xsl:text>_</xsl:text>
          <xsl:value-of select="@shifter"/>
          <xsl:value-of select="$sep"/>
          <xsl:value-of select="@name"/>
          <xsl:text>_</xsl:text>
          <xsl:value-of select="@shifter"/>
          <xsl:text>_comment</xsl:text>
        </xsl:when>
        <xsl:when test="name()='L1SOURCE'">
          <xsl:text>L1_</xsl:text><xsl:value-of select="@name"/>
          <xsl:value-of select="$sep"/>
          <xsl:text>L1_</xsl:text><xsl:value-of select="@name"/><xsl:text>_comment</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="name()"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:value-of select="$sep"/>
    </xsl:for-each>
    <xsl:text>&#10;</xsl:text>
  </xsl:if>

  <xsl:for-each select="*">

    <xsl:choose>
      <xsl:when test="name()='SUBSYSTEM'">
        <xsl:value-of select="@value"/>
        <xsl:value-of select="$sep"/>
      </xsl:when>
      <xsl:when test="name()='L1SOURCE'">
        <xsl:value-of select="@value"/>
        <xsl:value-of select="$sep"/>
      </xsl:when>
    </xsl:choose>

    <xsl:choose>
      <xsl:when test="$quote=0 or number(normalize-space(text()))=normalize-space(text())">
        <xsl:value-of select="normalize-space(text())"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$dq"/>
        <xsl:value-of select="translate(normalize-space(text()), $dq, $dq + $dq)"/>
        <xsl:value-of select="$dq"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:value-of select="$sep"/>
  </xsl:for-each>
  <xsl:text>&#10;</xsl:text>

</xsl:template>

</xsl:stylesheet>
