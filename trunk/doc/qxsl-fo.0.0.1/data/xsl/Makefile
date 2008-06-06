all:glossary.pdf xml2pdf.pdf base_template.pdf base.pdf
base.pdf:base.xml base.xsl
	 fop -xml ./base.xml -xsl ./base.xsl -pdf base.pdf  
base_template.pdf:base_template.xsl base_template.xml
	 fop -xml ./base_template.xml -xsl ./base_template.xsl -pdf base_template.pdf  
glossary.pdf:glossary.xsl glossary.xml
	 fop -xml ./glossary.xml -xsl ./glossary.xsl -pdf glossary.pdf  
xml2pdf.pdf:xml2pdf.xsl xslfoRef.xml
	 fop -xml ./xslfoRef.xml -xsl ./xml2pdf.xsl -pdf xml2pdf.pdf