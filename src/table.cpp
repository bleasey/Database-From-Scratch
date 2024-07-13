#include <string.h>
#include "../include/db.h"

Table::Table() {  
  this->pager = new Pager("table.mydb");
  this->num_rows = pager->file_length / DB::ROW_SIZE;
}

Table::~Table() {
  Pager* pager = this->pager;
  uint32_t num_full_pages = this->num_rows / DB::ROWS_PER_PAGE;

  // Writing pages to disk
  for (uint32_t i = 0; i < num_full_pages; i++) {
    if (pager->pages[i] == NULL) {
      // This page wasnt fetched, hence not updated
      continue;
    }
    pager->flush_to_disk(i, DB::PAGE_SIZE);
  }

  // There may be a partial page to write to the end of the file
  // This should not be needed after we switch to a B-tree
  uint32_t num_additional_rows = this->num_rows % DB::ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pager->pages[page_num] != NULL) {
      pager->flush_to_disk(page_num, num_additional_rows * DB::ROW_SIZE);
    }
  }

  delete this->pager; // closes file and frees page memory
}