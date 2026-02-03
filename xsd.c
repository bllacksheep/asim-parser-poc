#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "api/xsd.h"

#define MAX_BUF 8 * 1024

static size_t getdata(char *buf, const char *path) {
    if (buf == NULL || path == NULL) {
        perror("error no buf or path");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("error failed to open file");
        exit(EXIT_FAILURE);
    }

    size_t n;
    n = fread(buf, 1, MAX_BUF, fp);

    if (ferror(fp)) {
        perror("error reading data");
        if (fclose(fp) == EOF)
            perror("error closing file");
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

    xsdbuf_n = getdata(xsdbuf, "stubs/test.xsd");
	ctxt = xmlSchemaNewMemParserCtxt(xsdbuf, xsdbuf_n);

    xmlbuf_n = getdata(xmlbuf, "stubs/test.xml");

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
