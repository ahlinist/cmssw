<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:fn="http://www.w3.org/2005/02/xpath-functions">

<xsl:output method="xml" omit-xml-declaration="no" indent="yes"/>

<xsl:template match="/">
  <xsl:copy-of select="processing-instruction('xml-stylesheet')"/>
  <RUNS>
    <xsl:copy-of select="/RESULTS/@*"/>
    <xsl:apply-templates select="/RESULTS/ROW"/>
  </RUNS>
</xsl:template>

<xsl:template match="ROW">
  <RUN num="{NUM}">

    <NUMBER><xsl:value-of select="RUN_NUMBER"/></NUMBER>
    <START_TIME><xsl:value-of select="RUN_START_TIME"/></START_TIME>
    <END_TIME><xsl:value-of select="RUN_END_TIME"/></END_TIME>
    <EVENTS><xsl:value-of select="RUN_EVENTS"/></EVENTS>
    <GLOBAL_NAME><xsl:value-of select="RUN_GLOBALNAME"/></GLOBAL_NAME>
    <L1SOURCES><xsl:value-of select="RUN_L1SOURCES"/></L1SOURCES>
    <ONLINE_COMMENT><xsl:value-of select="RUN_ONLINE_COMMENT"/></ONLINE_COMMENT>
    <OFFLINE_COMMENT><xsl:value-of select="RUN_OFFLINE_COMMENT"/></OFFLINE_COMMENT>
    <STOP_REASON><xsl:value-of select="RUN_STOP_REASON"/></STOP_REASON>
    <TAG><xsl:value-of select="RUN_TAG"/></TAG>
    <COUNT_TAGS><xsl:value-of select="RUN_COUNT_TAGS"/></COUNT_TAGS>
    <RATE><xsl:value-of select="RUN_RATE"/></RATE>
    <BFIELD><xsl:value-of select="RUN_BFIELD"/></BFIELD>
    <BFIELD_COMMENT><xsl:value-of select="RUN_BFIELD_COMMENT"/></BFIELD_COMMENT>
    <STATUS><xsl:value-of select="RUN_STATUS"/></STATUS>
    <L1KEY><xsl:value-of select="RUN_L1KEY"/></L1KEY>
    <HLTKEY><xsl:value-of select="RUN_HLTKEY"/></HLTKEY>
    <ONLINE_SHIFTER><xsl:value-of select="RUN_ONLINE_SHIFTER"/></ONLINE_SHIFTER>
    <OFFLINE_SHIFTER><xsl:value-of select="RUN_OFFLINE_SHIFTER"/></OFFLINE_SHIFTER>
    <CREATE_USER><xsl:value-of select="RUN_CREATE_USER"/></CREATE_USER>
    <CREATE_TIME><xsl:value-of select="RUN_CREATE_TIME"/></CREATE_TIME>
    <L1SOURCES><xsl:value-of select="RUN_L1SOURCES"/></L1SOURCES>
    <SUBSYSTEMS>
    <xsl:for-each select="(on_CSC|on_DT|on_ECAL|on_HCAL|on_TRG|on_PIX|on_RPC|on_STRIP|on_SCAL|off_JMET|off_EGAM|off_MUON)[string-length(.)&gt;0 and .!='EXCL']">
      <xsl:if test="position()&gt;1"><xsl:text>, </xsl:text></xsl:if>
      <xsl:value-of select="substring-after(name(),'_')"/>
    </xsl:for-each>
    </SUBSYSTEMS>

    <xsl:for-each select="on_CSC|on_DT|on_ECAL|on_HCAL|on_TRG|on_PIX|on_RPC|on_STRIP|off_SCAL|off_CSC|off_DT|off_ECAL|off_HCAL|off_TRG|off_PIX|off_RPC|off_STRIP|off_SCAL|off_JMET|off_EGAM|off_MUON">
      <xsl:variable name="comment"><xsl:value-of select="name()"/><xsl:text>_comment</xsl:text></xsl:variable>
      <xsl:variable name="type">
        <xsl:choose>
          <xsl:when test="name()='off_JMET' or name()='off_EGAM' or name()='off_MUON'">OFFLINE</xsl:when>
          <xsl:otherwise>ONLINE</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="shifter">
        <xsl:choose>
          <xsl:when test="substring-before(name(),'_')='off'">OFFLINE</xsl:when>
          <xsl:otherwise>ONLINE</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:element name="SUBSYSTEM">
        <xsl:attribute name="name"><xsl:value-of select="substring-after(name(),'_')"/></xsl:attribute>
        <xsl:attribute name="value"><xsl:value-of select="text()"/></xsl:attribute>
        <xsl:attribute name="type"><xsl:value-of select="$type"/></xsl:attribute>
        <xsl:attribute name="shifter"><xsl:value-of select="$shifter"/></xsl:attribute>
        <xsl:value-of select="../*[name()=$comment]"/>
      </xsl:element>
    </xsl:for-each>

    <xsl:for-each select="l1s_CSC|l1s_DT|l1s_ECAL|l1s_HCAL|l1s_Random|l1s_RPC|l1s_Other">
      <xsl:variable name="comment"><xsl:value-of select="name()"/><xsl:text>_comment</xsl:text></xsl:variable>
      <xsl:variable name="name"><xsl:value-of select="substring(name(),5)"/></xsl:variable>
      <xsl:variable name="value">
        <xsl:choose>
          <xsl:when test="text()='IN'">IN</xsl:when>
          <xsl:otherwise>OUT</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:element name="L1SOURCE">
        <xsl:attribute name="name"><xsl:value-of select="$name"/></xsl:attribute>
        <xsl:attribute name="value"><xsl:value-of select="$value"/></xsl:attribute>
        <xsl:value-of select="../*[name()=$comment]"/>
      </xsl:element>
    </xsl:for-each>

  </RUN>
</xsl:template>

</xsl:stylesheet>
