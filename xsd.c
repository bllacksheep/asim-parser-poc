#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "api/xsd.h"

#define MAX_BUF 8 * 1024

const char xsdx[] = {
	#embed "stubs/test.xsd"
	,"\0"
};

const char *xsd =
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
"<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n"
"<xs:element name=\"shiporder\">\n"
"  <xs:complexType>\n"
"    <xs:sequence>\n"
"      <xs:element name=\"orderperson\" type=\"xs:string\"/>\n"
"      <xs:element name=\"shipto\">\n"
"        <xs:complexType>\n"
"          <xs:sequence>\n"
"            <xs:element name=\"name\" type=\"xs:string\"/>\n"
"            <xs:element name=\"address\" type=\"xs:string\"/>\n"
"            <xs:element name=\"city\" type=\"xs:string\"/>\n"
"            <xs:element name=\"country\" type=\"xs:string\"/>\n"
"          </xs:sequence>\n"
"        </xs:complexType>\n"
"      </xs:element>\n"
"      <xs:element name=\"item\" maxOccurs=\"unbounded\">\n"
"        <xs:complexType>\n"
"          <xs:sequence>\n"
"            <xs:element name=\"title\" type=\"xs:string\"/>\n"
"            <xs:element name=\"note\" type=\"xs:string\" minOccurs=\"0\"/>\n"
"            <xs:element name=\"quantity\" type=\"xs:positiveInteger\"/>\n"
"            <xs:element name=\"price\" type=\"xs:decimal\"/>\n"
"          </xs:sequence>\n"
"        </xs:complexType>\n"
"      </xs:element>\n"
"    </xs:sequence>\n"
"    <xs:attribute name=\"orderid\" type=\"xs:string\" use=\"required\"/>\n"
"  </xs:complexType>\n"
"</xs:element>\n"
"</xs:schema>\n";

// provide a buf of MAX_BUF size
// provide a valid file path
static size_t read_file_to_buf(char *buf, const char *path) {
    if (buf == NULL || path == NULL) {
        perror("error no buf or path");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("error failed to open file");
        exit(EXIT_FAILURE);
    }

    size_t n = fread(buf, 1, MAX_BUF, fp);

    if (ferror(fp)) {
        perror("error reading data");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp) == EOF)
        perror("error closing file");
    return n;
}

void x()
{
	int ret;
    size_t xmlbuf_n, xsdbuf_n;
	xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;

    char xmlbuf[MAX_BUF];
    char xsdbuf[MAX_BUF];

    xsdbuf_n = read_file_to_buf(xsdbuf, "stubs/test.xsd");
	// experiment direct from bytes
	ctxt = xmlSchemaNewMemParserCtxt(xsd, strlen(xsd));

    xmlbuf_n = read_file_to_buf(xmlbuf, "stubs/test.xml");

	xmlLineNumbersDefault(1);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadMemory(xmlbuf, xmlbuf_n, NULL, NULL, 0);

	if (doc == NULL){
		fprintf(stderr, "Could not parse file\n");
	}
	else{
		xmlSchemaValidCtxtPtr ctxt;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		
		if (ret == 0){
			printf("validates!\n");
		}
		else if (ret > 0){
			printf("no validate fails to validate\n");
		}
		else{
			printf("validation generated an internal error\n");
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}


	if(schema != NULL)
		xmlSchemaFree(schema);

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();
}

int main(int argc, char *argv[]) {
   x();
   return 0;
}
