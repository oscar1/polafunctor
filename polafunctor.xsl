<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" id="polafunctor" version="1.0">

    <xsl:template name="typecontext">
        <xsl:param name="mycontext" />
	<xsl:for-each select="/GCC_XML/Class[@id=$mycontext]">
		<xsl:value-of select="@demangled"/><xsl:value-of select="'::'"/>
	</xsl:for-each>
	<xsl:for-each select="/GCC_XML/Namespace[@id=$mycontext]">
		<xsl:if test="@demangled !='::'">
		    <xsl:value-of select="@demangled"/><xsl:value-of select="'::'"/>
		</xsl:if>
	</xsl:for-each>
    </xsl:template>

    <xsl:template name="ptype">
	  <xsl:param name="mytype" />
	  <xsl:for-each select="/GCC_XML/Typedef[@id=$mytype]">
	    <xsl:call-template name="typecontext">
              <xsl:with-param name="mycontext"><xsl:value-of select="@context"/></xsl:with-param>
            </xsl:call-template>
            <xsl:value-of select="@name"/>
          </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/Class[@id=$mytype]">
            <xsl:value-of select="@demangled"/>
          </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/FundamentalType[@id=$mytype]">
            <xsl:value-of select="@name"/>
          </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/Enumeration[@id=$mytype]">
             <xsl:call-template name="typecontext">
		<xsl:with-param name="mycontext"><xsl:value-of select="@context"/></xsl:with-param>
	     </xsl:call-template>
	     <xsl:value-of select="@name"/>
	  </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/CvQualifiedType[@id=$mytype]">
	     <xsl:variable name="reftype" select="@type"/>
             <xsl:call-template name="ptype">
		     <xsl:with-param name="mytype"><xsl:value-of select="$reftype"/></xsl:with-param>
	     </xsl:call-template>
             <xsl:if test="@const = '1'"><xsl:value-of select="' const'"/></xsl:if>
	     <xsl:if test="@volatile = '1'"><xsl:value-of select="' volatile'"/></xsl:if>
	     <xsl:if test="@restrict = '1'"><xsl:value-of select="' restrict'"/></xsl:if>
	  </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/ReferenceType[@id=$mytype]">
            <xsl:variable name="reftype" select="@type"/>
	    <xsl:call-template name="ptype">
		    <xsl:with-param name="mytype"><xsl:value-of select="$reftype"/></xsl:with-param>
	    </xsl:call-template>
            <xsl:value-of select="' &amp;'"/>
          </xsl:for-each>
	  <xsl:for-each select="/GCC_XML/PointerType[@id=$mytype]">
	     <xsl:variable name="reftype" select="@type"/>
	     <xsl:call-template name="ptype">
		     <xsl:with-param name="mytype"><xsl:value-of select="$reftype"/></xsl:with-param>
	     </xsl:call-template>
             <xsl:value-of select="' *'"/>
          </xsl:for-each>
  </xsl:template>

  <xsl:template name="pmethod">
      <method>
	 <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="const">
             <xsl:value-of select="@const"/>
         </xsl:attribute>
	 <returntype>
	    <xsl:variable name="typeid" select="@returns"/>
            <xsl:call-template name="ptype">
		    <xsl:with-param name="mytype"><xsl:value-of select="$typeid"/></xsl:with-param>
            </xsl:call-template>
	 </returntype>
      </method>
  </xsl:template>

  <xsl:template name="rmethods">
	  <xsl:variable name="class_id" select="@id"/>
	  <xsl:for-each select="../Method[@context=$class_id and @overrides='' and @access='public']" >
	      <xsl:sort select="@name" />
	      <xsl:call-template name="pmethod"/>
          </xsl:for-each>
	  <xsl:for-each select="Base">
	     <xsl:variable name="base_id" select="@type"/>
	     <xsl:for-each select="../../Class[@id=$base_id]">
			<xsl:call-template name="rmethods"/>
	     </xsl:for-each>
          </xsl:for-each>
  </xsl:template>

  <xsl:template match="GCC_XML"> 
    <Files>
	<xsl:for-each select="File">
          <xsl:sort select="@name" />
	  <xsl:variable name="file_id" select="@id"/>
	  <xsl:if test="count(../Class[@file=$file_id and @abstract='1'])>0">
	    <File>
		 <xsl:attribute name="name">
			  <xsl:value-of select="@name"/>
		 </xsl:attribute>
		 <xsl:for-each select="../Class[@file=$file_id and @abstract='1']">
			 <xsl:sort select="@demangled" />
			 <xsl:variable name="class_id" select="@id"/>
			 <class>
                            <xsl:attribute name="name">
				    <xsl:value-of select="@demangled"/>
				    <xsl:for-each select="../Method">
					    <xsl:sort select="@name" />
					    <method>
                                                <xsl:attribute name="name">
							<xsl:value-of select="@name"/>
						</xsl:attribute>
					    </method>
				    </xsl:for-each>
			    </xsl:attribute>
			    <xsl:call-template name="rmethods"/>
			 </class>
		 </xsl:for-each>
	    </File>
	 </xsl:if> 
	</xsl:for-each>           
    </Files>      
  </xsl:template>  <!-- Match evidence -->
</xsl:stylesheet>
