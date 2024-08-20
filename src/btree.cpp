#include <iostream>
#include <string.h>
#include <assert.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

uint32_t* BTree::leaf_num_cells(void* node) {
  return (uint32_t*)((uint8_t*)node + LEAF_NODE_NUM_CELLS_OFFSET);
}

void* BTree::leaf_cell(void* node, uint32_t cell_num) {
  assert (cell_num < LEAF_NODE_MAX_CELLS);
  return (uint8_t*)node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

uint32_t* BTree::leaf_key(void* node, uint32_t cell_num) {
  return (uint32_t*)leaf_cell(node, cell_num);
}

void* BTree::leaf_value(void* node, uint32_t cell_num) {
  return (uint8_t*)leaf_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

NodeType BTree::get_node_type(void* node) {
  uint8_t type = *((uint8_t*)node + NODE_TYPE_OFFSET);
  return (NodeType)type;
}

void BTree::set_node_type(void* node, NodeType type) {
  *((uint8_t*)node + NODE_TYPE_OFFSET) = (uint8_t)type;
}

void BTree::initialize_leaf_node(void* node) {
  set_node_type(node, NODE_LEAF);
  *leaf_num_cells(node) = 0;
}


/*
The following function will return either:
-- the position of the key,
-- the position of another key that we’ll need to move if we want to insert the new key, or
-- the position one past the last key
*/
Cursor* BTree::leaf_node_find(uint32_t page_num, uint32_t key, Table* table) {
  Pager* pager = table->pager;
  void* node = pager->get_page(page_num);
  uint32_t num_cells = *leaf_num_cells(node);

  Cursor* cursor = new Cursor(table);
  cursor->page_num = page_num;

  // Binary search to assign cell_num
  uint32_t min_index = 0;
  uint32_t one_past_max_index = num_cells;
  while (one_past_max_index != min_index) {
    uint32_t index = (min_index + one_past_max_index) / 2;
    uint32_t key_at_index = *leaf_key(node, index);
    if (key == key_at_index) {
      cursor->cell_num = index;
      return cursor;
    }
    else if (key < key_at_index) one_past_max_index = index;
    else min_index = index + 1;
  }

  cursor->cell_num = min_index;
  return cursor;
} 

void BTree::leaf_node_insert(Cursor* cursor, uint32_t key, Row* value) {
  Pager* pager = cursor->table->pager;
  void* node = pager->get_page(cursor->page_num);

  uint32_t num_cells = *(pager->btree->leaf_num_cells(node));
  if (num_cells >= LEAF_NODE_MAX_CELLS) {
    // Node full
    std::cout << "Error. Yet to implement splitting a leaf node.\n";
    exit(EXIT_FAILURE);
  }

  if (cursor->cell_num < num_cells) {
    // Shift all succeeding cells to make room for new cell
    for (uint32_t i = num_cells; i > cursor->cell_num; i--) {
      memcpy(leaf_cell(node, i), leaf_cell(node, i - 1), LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_num_cells(node)) += 1;
  *(leaf_key(node, cursor->cell_num)) = key;
  cursor->table->serialize_row(value, leaf_value(node, cursor->cell_num)); 
}