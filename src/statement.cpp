#include <iostream>
#include <string.h>
#include "../include/db.h"

PrepareResult DB::prepare_insert() {
  statement->type = STATEMENT_INSERT;

  char* tmp_buffer = strdup(input_buffer->buffer.c_str());

  // Parse: "Insert {id} {username} {email}"
  char* keyword = strtok(tmp_buffer, " ");
  char* id_string = strtok(NULL, " ");
  char* username = strtok(NULL, " ");
  char* email = strtok(NULL, " ");
  char* remaining_buffer = strtok(NULL, " ");

  // Parsing error if incorrect no. of elements present
  if (email == NULL || remaining_buffer != NULL) {
    free(tmp_buffer);
    return PREPARE_SYNTAX_ERROR;
  }

  int id = atoi(id_string);
  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    free(tmp_buffer);
    return PREPARE_STRING_TOO_LONG;
  }
  if (strlen(email) > COLUMN_EMAIL_SIZE) {
    free(tmp_buffer);
    return PREPARE_STRING_TOO_LONG;
  }

  statement->row_to_insert.id = id;
  strcpy(statement->row_to_insert.username, username);
  strcpy(statement->row_to_insert.email, email);
  free(tmp_buffer);

  return PREPARE_SUCCESS;
}

PrepareResult DB::prepare_statement() {
  if (input_buffer->buffer.substr(0, 6) == "insert") {
    return prepare_insert();
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