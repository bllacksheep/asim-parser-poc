#include "api/xsd.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage: command <path>\n");
    exit(1);
  }

  schema_t schema = xsd_get_schema();
  xsd_validate_xml(argv[1], schema);
  xsd_destroy_schema(schema);

  return 0;
}
