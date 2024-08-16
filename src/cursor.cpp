#include <string.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

Cursor::Cursor(Table* table, bool at_table_end) {  
  // Creates a cursor object at table end
  this->table = table;
  this->page_num = table->pager->root_page_num;

  void* root_node = table->pager->get_page(table->pager->root_page_num);
  uint32_t num_cells = *(table->pager->btree->leaf_num_cells(root_node));

  if (at_table_end){
    this->cell_num = num_cells;
    this->end_of_table = true;  
  }
  else {
    this->cell_num = 0;
    this->end_of_table = (num_cells == 0);
  }
}

void* Cursor::get_value() {
  void* page = table->pager->get_page(this->page_num);
  return table->pager->btree->leaf_value(page, this->cell_num);
}

void Cursor::advance() {
  Pager* pager = this->table->pager;
  void* page = pager->get_page(this->page_num);
  this->cell_num += 1;

  if (this->cell_num >= (*(pager->btree->leaf_num_cells(page)))) {
    this->end_of_table = true;
  }
}