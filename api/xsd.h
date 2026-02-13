#ifndef XSD_H
#define XSD_H 1

#include "xml.h"
#include <libxml/xmlschemastypes.h>

#define MAX_BUF 8 * 1024

typedef const char *const schema_t;

schema_t xsd_get_schema(void);
void xsd_validate_xml(xml_t, schema_t);
void xsd_destroy_schema(schema_t);

#endif
