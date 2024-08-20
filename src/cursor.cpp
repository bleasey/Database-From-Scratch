#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

// Creates a cursor object at the table start
Cursor::Cursor(Table* table) {
  this->table = table;
  this->page_num = table->pager->root_page_num;
  this->cell_num = 0;

  void* root_node = table->pager->get_page(this->page_num);
  uint32_t num_cells = *(table->pager->btree->leaf_num_cells(root_node));

  this->end_of_table = (num_cells == 0);
}

// Creates a cursor object at the root node with a given key
Cursor::Cursor(Table* table, uint32_t key) {  
  this->table = table;
  this->page_num = table->pager->root_page_num;
  this->cell_num = table->find_key(key)->cell_num;

  void* root_node = table->pager->get_page(this->page_num);
  uint32_t num_cells = *(table->pager->btree->leaf_num_cells(root_node));

  this->end_of_table = (num_cells == this->cell_num);  
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