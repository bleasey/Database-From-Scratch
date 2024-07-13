#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"

Table::Table() {  
  this->pager = new Pager("table.mydb");
  this->pager->num_rows = pager->file_length / Table::ROW_SIZE;
  this->row_to_insert = new Row();
}

Table::~Table() {
  Pager* pager = this->pager;
  uint32_t num_full_pages = this->pager->num_rows / Table::ROWS_PER_PAGE;

  // Writing pages to disk
  for (uint32_t i = 0; i < num_full_pages; i++) {
    if (pager->pages[i] == NULL) {
      // This page wasnt fetched, hence not updated
      continue;
    }
    pager->flush_to_disk(i, Table::PAGE_SIZE);
  }

  // There may be a partial page to write to the end of the file
  // This should not be needed after we switch to a B-tree
  uint32_t num_additional_rows = this->pager->num_rows % Table::ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pager->pages[page_num] != NULL) {
      pager->flush_to_disk(page_num, num_additional_rows * Table::ROW_SIZE);
    }
  }

  delete this->pager; // closes file and frees page memory
}


void Table::serialize_row(Row* source, void* destination) {
  // The typecast to (uint8_t*) is done as void pointer arithmetic throws warnings.
  // (uint8_t*) indicates that the data elements are 1 byte long
  memcpy((uint8_t*)destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy((uint8_t*)destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy((uint8_t*)destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void Table::deserialize_row(void* source, Row* destination) {
  memcpy(&(destination->id), (uint8_t*)source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), (uint8_t*)source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), (uint8_t*)source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* Table::get_row(Cursor* cursor) {
  uint32_t page_num = cursor->row_num / ROWS_PER_PAGE;
  void* page = this->pager->get_page(page_num);
  uint32_t row_offset = cursor->row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return (uint8_t*)page + byte_offset;
}