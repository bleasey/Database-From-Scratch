#include <string.h>
#include "../include/db.h"
#include "../include/table.h"

Cursor::Cursor(Table* table, uint32_t row_num) {  
  // Creates a cursor object at given position
  this->table = table;
  this->row_num = row_num;

  // if row_num is -1, cursor points to table end
  if (row_num == -1){
    this->row_num = table->pager->num_rows;
  }

  this->end_of_table = (this->row_num == table->pager->num_rows);
}

void Cursor::advance() {
  this->row_num += 1;
  if (this->row_num >= this->table->pager->num_rows){
    this->end_of_table = true;
  }
}