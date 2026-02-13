#include "api/xml.h"
#include "api/xsd.h"
#include <stdio.h>
#include <stdlib.h>

static int read_file_to_buf(char *, const char *, size_t *);
static void teardown(void);

// public api be more explicit about ownership
void xml_destroy_xml(xml_t xml) {
  if (xml != NULL)
    xmlFreeDoc(xml);
}

// public api xmlDocPtr owned, must be freed with x;
xml_t xml_read_xml(const char *xml_path_to_file) {
  char xmlbuf[MAX_BUF] = {};
  size_t n;

  if (read_file_to_buf(xmlbuf, xml_path_to_file, &n) != 0) {
    exit(EXIT_FAILURE);
  }

  return xmlReadMemory(xmlbuf, n, NULL, NULL, 0);
}

static void teardown() {
  xmlSchemaCleanupTypes();
  xmlCleanupParser();
  xmlMemoryDump();
}

// provide a buf of MAX_BUF size
// provide a valid file path
static int read_file_to_buf(char *buf, const char *xmlfullpath, size_t *len) {
  if (buf == NULL || xmlfullpath == NULL) {
    perror("error no buf or path");
    return EXIT_FAILURE;
  }

  FILE *f = fopen(xmlfullpath, "rb");
  if (f == NULL) {
    perror("error failed to open file");
    return EXIT_FAILURE;
  }

  size_t n = fread(buf, 1, MAX_BUF, f);

  if (ferror(f)) {
    perror("error reading data");
    fclose(f);
    return EXIT_FAILURE;
  }

  if (fclose(f) == EOF) {
    perror("error closing file");
  }
  *len = n;
  return EXIT_SUCCESS;
}

// public api helper encapsulate workflow
int xml_run_workflow(char *xmlpath) {
  xml_t xml = xml_read_xml(xmlpath);

  if (xml == NULL) {
    perror("could not parse file\n");
    return EXIT_FAILURE;
  }

  schema_t schema = xsd_get_schema();

  if (schema == NULL) {
    perror("could allocate schema\n");
    return EXIT_FAILURE;
  }

  xsd_validate_xml(xml, schema);

  xsd_destroy_schema(schema);
  xml_destroy_xml(xml);
  teardown();
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage: <cmd> <path_to_xml>\n");
    exit(EXIT_FAILURE);
  }

  return xml_run_workflow(argv[1]);
}
