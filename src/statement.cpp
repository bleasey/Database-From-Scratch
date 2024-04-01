#include <iostream>
#include "../include/db.h"

PrepareResult DB::prepare_statement() {
  if (input_buffer->buffer.substr(0, 6) == "insert") {
    statement->type = STATEMENT_INSERT;

    // Need to fix input overflow bug
    int args_assigned = sscanf(
      (input_buffer->buffer).c_str(), "insert %d %s %s", &(statement->row_to_insert.id),
      statement->row_to_insert.username, statement->row_to_insert.email);
    
    if (args_assigned < 3) {
      return PREPARE_SYNTAX_ERROR;
    }

    return PREPARE_SUCCESS;
  }
  if (input_buffer->buffer.substr(0, 6) == "select") {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult DB::execute_insert() {
  if (table->num_rows >= TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }

  serialize_row(&(statement->row_to_insert), get_row(table, table->num_rows));
  table->num_rows += 1;
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_select() {
  Row row;
  for (uint32_t i = 0; i < table->num_rows; i++) {
    deserialize_row(get_row(table, i), &row);
    print_row(&row);
  }
  return EXECUTE_SUCCESS;
}

ExecuteResult DB::execute_statement() {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      return execute_insert();
    case (STATEMENT_SELECT):
      return execute_select();
    default:
      return EXECUTE_ERROR; // to escape "control reaches end of non-void func" warning
  }
}