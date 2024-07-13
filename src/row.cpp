#include <string.h>
#include "../include/db.h"


void DB::serialize_row(Row* source, void* destination) {
  // The typecast to (uint8_t*) is done as void pointer arithmetic throws warnings.
  // (uint8_t*) indicates that the data elements are 1 byte long
  memcpy((uint8_t*)destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy((uint8_t*)destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy((uint8_t*)destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void DB::deserialize_row(void* source, Row* destination) {
  memcpy(&(destination->id), (uint8_t*)source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), (uint8_t*)source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), (uint8_t*)source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* DB::get_row(Table* table, uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void* page = table->pager->get_page(page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return (uint8_t*)page + byte_offset;
}