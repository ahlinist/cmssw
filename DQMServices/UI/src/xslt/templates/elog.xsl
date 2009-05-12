<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>

<xsl:template match="/">

  <xsl:text>&#10;</xsl:text>

  <xsl:apply-templates select="/RUNS/RUN"/>

</xsl:template>

<xsl:template match="RUN">

  <xsl:if test="position()&gt;1">
    <xsl:text>&#10;</xsl:text>
    <xsl:text>----------------------------------------------------------------------------------------------------------------</xsl:text>
    <xsl:text>&#10;</xsl:text>
    <xsl:text>&#10;</xsl:text>
  </xsl:if>

  <xsl:text>Run Number:&#09;</xsl:text>
  <xsl:value-of select="NUMBER"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>Start Time:&#09;</xsl:text>
  <xsl:value-of select="START_TIME"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>Stop Time:&#09;</xsl:text>
  <xsl:value-of select="END_TIME"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text># Triggers:&#09;</xsl:text>
  <xsl:value-of select="EVENTS"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>Rate:&#09;&#09;</xsl:text>
  <xsl:value-of select="RATE"/>
  <xsl:text> Hz&#10;</xsl:text>

  <xsl:text>HLT Key:&#09;</xsl:text>
  <xsl:value-of select="HLTKEY"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>L1 Key: &#09;</xsl:text>
  <xsl:value-of select="L1KEY"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>B Field:&#09;</xsl:text>
  <xsl:value-of select="BFIELD"/>
  <xsl:text> Tesla</xsl:text>
  <xsl:if test="string-length(BFIELD_COMMENT)&gt;0">
    <xsl:text> (</xsl:text>
    <xsl:value-of select="BFIELD_COMMENT"/>
    <xsl:text>)</xsl:text>
  </xsl:if>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>L1 Sources:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:apply-templates select="L1SOURCE[@value='IN']"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>DPG Subsystems:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:text>  Online Shifter:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:apply-templates select="SUBSYSTEM[@type='ONLINE' and @shifter='ONLINE']"/>
  <xsl:text>  Offline Shifter:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:apply-templates select="SUBSYSTEM[@type='ONLINE' and @shifter='OFFLINE']"/>
  <xsl:text>&#10;</xsl:text>
  <xsl:text>POG Subsystems:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:apply-templates select="SUBSYSTEM[@type='OFFLINE']"/>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>Stop Reason:&#09;</xsl:text>
  <xsl:value-of select="STOP_REASON"/>
  <xsl:text>&#10;</xsl:text>
  <xsl:text>&#10;</xsl:text>

  <xsl:text>Online Comment:&#09;</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:value-of select="ONLINE_COMMENT"/>
  <xsl:text>&#10;</xsl:text>
  
</xsl:template>

<xsl:template match="L1SOURCE">
  <xsl:text>  </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text>:&#09;</xsl:text>
  <xsl:value-of select="."/>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="SUBSYSTEM">
  <xsl:text>    </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text> (</xsl:text>
  <xsl:value-of select="@value"/>
  <xsl:text>):&#09;</xsl:text>
  <xsl:value-of select="."/>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

</xsl:stylesheet>
