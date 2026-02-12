#include "api/xsd.h"
// #include "api/schema.h"
#include <libxml/parser.h>
#include <libxml/xmlschemastypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 8 * 1024
static char *xsd_get_schema(void);
static void xsd_validate_schema(const char *, const char *);
static void *xsd_destroy_schema(char *s);
static size_t read_file_to_buf(char *, const char *);

static char *xsd_get_schema() {
  char *s = malloc(stubs_test_xsd_len);
  memcpy(s, stubs_test_xsd, stubs_test_xsd_len);
  return s;
}

static void *destroy_xsd_schema(char *s) {
  if (s != NULL)
    free(s);
}

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

void xsd_validate_schema(const char *xml_path, const char *xsd_schema) {
  int ret;
  size_t xmlbuf_len;
  xmlDocPtr doc;
  xmlSchemaPtr schema = NULL;
  xmlSchemaParserCtxtPtr ctxt;

  char xmlbuf[MAX_BUF];
  char xsdbuf[MAX_BUF];

  // xsdbuf_n = read_file_to_buf(xsdbuf, xml_path);
  //  experiment direct from bytes
  ctxt = xmlSchemaNewMemParserCtxt(xsd_schema, stubs_test_xsd_len);

  xmlbuf_len = read_file_to_buf(xmlbuf, xml_path);

  xmlLineNumbersDefault(1);

  xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf,
                           (xmlSchemaValidityWarningFunc)fprintf, stderr);
  schema = xmlSchemaParse(ctxt);
  xmlSchemaFreeParserCtxt(ctxt);

  doc = xmlReadMemory(xmlbuf, xmlbuf_len, NULL, NULL, 0);

  if (doc == NULL) {
    fprintf(stderr, "Could not parse file\n");
  } else {
    xmlSchemaValidCtxtPtr ctxt;

    ctxt = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf,
                            (xmlSchemaValidityWarningFunc)fprintf, stderr);

    ret = xmlSchemaValidateDoc(ctxt, doc);

    if (ret == 0) {
      printf("validates!\n");
    } else if (ret > 0) {
      printf("no validate fails to validate\n");
    } else {
      printf("validation generated an internal error\n");
    }
    xmlSchemaFreeValidCtxt(ctxt);
    xmlFreeDoc(doc);
  }

  if (schema != NULL)
    xmlSchemaFree(schema);

  xmlSchemaCleanupTypes();
  xmlCleanupParser();
  xmlMemoryDump();
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage: command <path>\n");
    exit(1);
  }

  char *schema = xsd_get_schema();
  xsd_validate_schema(argv[1], schema);
  destroy_xsd_schema(schema);

  return 0;
}
