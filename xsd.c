#include "api/xsd.h"
#include "api/schema.h"
#include "api/xml.h"
#include <libxml/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// public api get the schema object that's built into the binary
schema_t xsd_get_schema() {
  char *s = malloc(stubs_test_xsd_len);
  memcpy(s, stubs_test_xsd, stubs_test_xsd_len);
  return s;
}

// pubic api ownership destroy allocated schema after use
void xsd_destroy_schema(schema_t s) {
  if (s != NULL)
    free((void *)s);
}

// public api validate an xml blob against a schema
void xsd_validate_xml(xml_t xml, schema_t xsd_schema) {
  int ret;
  xmlSchemaPtr schema = NULL;
  xmlSchemaParserCtxtPtr schema_ctx;

  schema_ctx = xmlSchemaNewMemParserCtxt(xsd_schema, stubs_test_xsd_len);

  xmlLineNumbersDefault(1);

  xmlSchemaSetParserErrors(schema_ctx, (xmlSchemaValidityErrorFunc)fprintf,
                           (xmlSchemaValidityWarningFunc)fprintf, stderr);
  schema = xmlSchemaParse(schema_ctx);
  xmlSchemaFreeParserCtxt(schema_ctx);

  xmlSchemaValidCtxtPtr schema_valid_ctx;

  schema_valid_ctx = xmlSchemaNewValidCtxt(schema);
  xmlSchemaSetValidErrors(schema_valid_ctx, (xmlSchemaValidityErrorFunc)fprintf,
                          (xmlSchemaValidityWarningFunc)fprintf, stderr);

  ret = xmlSchemaValidateDoc(schema_valid_ctx, xml);

  if (ret == 0) {
    printf("validates!\n");
  } else if (ret > 0) {
    printf("no validate fails to validate\n");
  } else {
    printf("validation generated an internal error\n");
  }

  if (schema_valid_ctx != NULL)
    xmlSchemaFreeValidCtxt(schema_valid_ctx);

  if (schema != NULL)
    xmlSchemaFree(schema);
}
