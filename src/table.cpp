#include <string.h>
#include "../include/db.h"

// Table struct constructors & destructors
Table::Table() {
  num_rows = 0;
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    pages[i] = NULL;
  }
}

Table::~Table() {
  for (uint32_t i = 0; pages[i]; i++) {
    free(pages[i]);
  }
}