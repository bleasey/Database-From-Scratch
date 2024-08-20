#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

ExecuteResult DB::execute_insert() {
  void* node = table->pager->get_page(table->pager->root_page_num);
  BTree* btree = table->pager->btree;

  uint32_t num_cells = *(btree->leaf_num_cells(node));
  if (num_cells >= btree->LEAF_NODE_MAX_CELLS) {
    return EXECUTE_TABLE_FULL;
  }

  Cursor* cursor = new Cursor(table, table->row_to_insert->id); // cursor initialized with given key
  
  // Checking if duplicate key exists
  if (cursor->cell_num < num_cells) {
    uint32_t key_at_index = *(btree->leaf_key(node, cursor->cell_num));
    if (table->row_to_insert->id == key_at_index) {
      delete cursor;
      return EXECUTE_DUPLICATE_KEY;
    }
  }

  btree->leaf_node_insert(cursor, table->row_to_insert->id, table->row_to_insert);

  delete cursor;
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_select() {
  Cursor* cursor = new Cursor(table);
  Row row;

  while (!(cursor->end_of_table)){
    table->deserialize_row(cursor->get_value(), &row);
    table->print_row(&row);
    cursor->advance();
  }

  delete cursor;
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_statement() {
  switch (statement_type) {
    case (STATEMENT_INSERT):
      return execute_insert();
    case (STATEMENT_SELECT):
      return execute_select();
    default:
      return EXECUTE_ERROR; // to escape "control reaches end of non-void func" warning
  }
}