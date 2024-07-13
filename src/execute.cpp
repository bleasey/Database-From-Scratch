#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"

ExecuteResult DB::execute_insert() {
  if (table->pager->num_rows >= Table::TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }

  table->serialize_row(table->row_to_insert, table->get_row(table->pager->num_rows));
  table->pager->num_rows += 1;
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_select() {
  Row row;
  for (uint32_t i = 0; i < table->pager->num_rows; i++) {
    table->deserialize_row(table->get_row(i), &row);
    table->print_row(&row);
  }
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