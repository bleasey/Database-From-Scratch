#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

Table::Table() {  
  this->pager = new Pager("table.mydb");
  this->row_to_insert = new Row();

  this->pager->root_page_num = 0;

  if (pager->num_pages == 0) {
    // New database file. Initialize page 0 as leaf node.
    void* root_node = this->pager->get_page(0);
    this->pager->btree->initialize_leaf_node(root_node);
  }
}

Table::~Table() {
  Pager* pager = this->pager;

  // Writing pages to disk
  for (uint32_t i = 0; i < this->pager->num_pages; i++) {
    if (pager->pages[i] == NULL) {
      // This page wasnt fetched, hence not updated
      continue;
    }
    pager->flush_to_disk(i);
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


// Return the position of the given key.
// If the key is not present, return the position
// where it should be inserted
Cursor* Table::find_key(uint32_t key) {
  void* root_node = pager->get_page(pager->root_page_num);

  if (pager->btree->get_node_type(root_node) == NODE_LEAF) {
    return pager->btree->leaf_node_find(pager->root_page_num, key, this);
  }
  else {
    std::cout << "Need to implement searching an internal node\n";
    exit(EXIT_FAILURE);
  }
}