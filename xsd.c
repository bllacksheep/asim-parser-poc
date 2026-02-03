#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "api/xsd.h"

#define MAX_BUF 8 * 1024

// ensure your own buffers not NULL
// check your own data after read
void getdata(char *buf, const char *path) {
    FILE *fp = fopen(path, "r");
    fread(buf, 1, MAX_BUF, fp);
    if (fclose(fp) == EOF)
        perror("error closing file");
}

int main(int argc, char *argv[])
{
	int ret;
	xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;

    char xmlbuf[MAX_BUF];
    char xsdbuf[MAX_BUF];
    getdata(xsdbuf, "stubs/test.xsd");
    getdata(xmlbuf, "stubs/test.xml");

	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewMemParserCtxt(xsdbuf, strlen(xsdbuf));

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadMemory(xmlbuf, strlen(xmlbuf), NULL, NULL, 0);

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

	return 0;
}

