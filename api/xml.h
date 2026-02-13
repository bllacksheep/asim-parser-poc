#ifndef XML_H
#define XML_H 1
#include <libxml/xmlschemastypes.h>
typedef const xmlDocPtr xml_t;

xml_t xml_read_xml(const char *);
void xml_free_xml(xml_t xml);

#endif
