#ifndef XSD_H
#define XSD_H 1

#define MAX_BUF 8 * 1024

typedef char *schema_t;

schema_t xsd_get_schema(void);
void xsd_validate_xml(const char *, const schema_t);
void *xsd_destroy_schema(schema_t);

#endif
