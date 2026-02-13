#include "api/xsd.h"
#include "api/schema.h"
#include <libxml/parser.h>
#include <libxml/xmlschemastypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_file_to_buf(char *, const char *, size_t *);

// public api get the schema object that's built into the binary
schema_t xsd_get_schema() {
  char *s = malloc(stubs_test_xsd_len);
  memcpy(s, stubs_test_xsd, stubs_test_xsd_len);
  return s;
}

// pubic api destroy allocated schema after use
void xsd_destroy_schema(schema_t s) {
  if (s != NULL)
    free((void *)s);
}

// provide a buf of MAX_BUF size
// provide a valid file path
static int read_file_to_buf(char *buf, const char *path, size_t *len) {
  if (buf == NULL || path == NULL) {
    perror("error no buf or path");
    return -1;
  }

  FILE *f = fopen(path, "rb");
  if (f == NULL) {
    perror("error failed to open file");
    return -1;
  }

  size_t n = fread(buf, 1, MAX_BUF, f);

  if (ferror(f)) {
    perror("error reading data");
    fclose(f);
    return -1;
  }

  if (fclose(f) == EOF) {
    perror("error closing file");
  }
  *len = n;
  return 0;
}

void xsd_validate_xml(const char *xml_path, schema_t xsd_schema) {
  int ret;
  xmlDocPtr doc;
  xmlSchemaPtr schema = NULL;
  xmlSchemaParserCtxtPtr ctxt;

  char xmlbuf[MAX_BUF] = {};
  size_t xmlbuf_len;

  if (read_file_to_buf(xmlbuf, xml_path, &xmlbuf_len) != 0) {
    exit(EXIT_FAILURE);
  }

  ctxt = xmlSchemaNewMemParserCtxt(xsd_schema, stubs_test_xsd_len);

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
