<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" id="polafunctor" version="1.0">

  <xsl:template name="ptype">
	  <xsl:variable name="type_id" select="@type"/>
	  <xsl:for-each select="../Typedef[@id=$type_id]">
            <xsl:value-of select="@name"/>
          </xsl:for-each>
          <xsl:for-each select="../Class[@id=$type_id]">
            <xsl:value-of select="@name"/>
          </xsl:for-each>
          <xsl:for-each select="../FundamentalType[@id=$type_id]">
            <xsl:value-of select="@name"/>
          </xsl:for-each>
	  <xsl:for-each select="../CvQualifiedType[@id=$type_id]">
	     <xsl:value-of select="'CvQualifiedType'"/>
	  </xsl:for-each>
	  <xsl:for-each select="../ReferenceType[@id=$type_id]">
	    <xsl:call-template name="ptype"/>
            <xsl:value-of select="' &amp;'"/>
          </xsl:for-each>
          <xsl:for-each select="../PointerType[@id=$type_id]">
             <xsl:call-template name="ptype"/>
             <xsl:value-of select="' *'"/>
          </xsl:for-each>
          <xsl:for-each select="../Enumeration[@id=$type_id]">
             <xsl:value-of select="@name"/>
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
            <xsl:variable name="type_id" select="@returns"/>
	    <xsl:for-each select="../Typedef[@id=$type_id]">
               <xsl:value-of select="@name"/>
	    </xsl:for-each>
	    <xsl:for-each select="../Class[@id=$type_id]">
               <xsl:value-of select="@name"/>
	    </xsl:for-each>
	    <xsl:for-each select="../FundamentalType[@id=$type_id]">
               <xsl:value-of select="@name"/>
            </xsl:for-each>
            <xsl:for-each select="../CvQualifiedType[@id=$type_id]">
		    <xsl:value-of select="'CvQualifiedType'"/>
	    </xsl:for-each>
	    <xsl:for-each select="../ReferenceType[@id=$type_id]">
		    <xsl:call-template name="ptype"/>
                    <xsl:value-of select="' &amp;'"/>
	    </xsl:for-each>
	    <xsl:for-each select="../PointerType[@id=$type_id]">
                    <xsl:call-template name="ptype"/>
                    <xsl:value-of select="' *'"/>
	    </xsl:for-each>
	    <xsl:for-each select="../Enumeration[@id=$type_id]">
		    <xsl:value-of select="@name"/>
	    </xsl:for-each>
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
