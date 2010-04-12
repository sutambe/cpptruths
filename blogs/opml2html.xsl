<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes" />

  <xsl:template match="opml/head" >
    <head><title>Sumant's <xsl:value-of select="title"/>
    </title></head>
     <h2><font color="red"><xsl:value-of select="title"/></font></h2> 
     Date created: <xsl:value-of select="dateCreated"/>
  </xsl:template>

  <xsl:template match="opml/body">
    <ul>
      <xsl:apply-templates select="outline">
        <xsl:sort select="@text" />
      </xsl:apply-templates>
    </ul>
  </xsl:template>

  <xsl:template match="outline">
    <xsl:choose>
      <xsl:when test="@htmlUrl">
        <xsl:variable name="url" select="@htmlUrl" />
        <xsl:variable name="xml" select="@xmlUrl" />
        <li>
          <a href="{$url}" >
            <xsl:value-of select="@text" />
          </a>
          (<a href="{$xml}"><img border="0" src="xml_small.jpg" alt="xml" /></a>)
        </li>
      </xsl:when>
      <xsl:otherwise>
        <li>
          <b><xsl:value-of select="@text" /></b>
        </li>
        <ul>
          <xsl:apply-templates select="outline">
            <xsl:sort select="@title" />
          </xsl:apply-templates>
        </ul>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
