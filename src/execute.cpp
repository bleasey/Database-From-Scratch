#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"

ExecuteResult DB::execute_insert() {
  if (table->pager->num_rows >= Table::TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }

  Cursor* cursor = new Cursor(table, -1);
  table->serialize_row(table->row_to_insert, table->get_row(cursor));
  table->pager->num_rows += 1;

  delete cursor;
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_select() {
  Cursor* cursor = new Cursor(table, 0);
  Row row;

  while (!(cursor->end_of_table)){
    table->deserialize_row(table->get_row(cursor), &row);
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