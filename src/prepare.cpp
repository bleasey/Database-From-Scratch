#include <iostream>
#include <string.h>
#include "../include/db.h"
#include "../include/table.h"

PrepareResult DB::prepare_insert() {
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

  table->row_to_insert->id = id;
  strcpy(table->row_to_insert->username, username);
  strcpy(table->row_to_insert->email, email);
  free(tmp_buffer);

  return PREPARE_SUCCESS;
}

PrepareResult DB::prepare_statement() {
  if (input_buffer->buffer.substr(0, 6) == "insert") {
    statement_type = STATEMENT_INSERT;
    return prepare_insert();
  }
  if (input_buffer->buffer.substr(0, 6) == "select") {
    statement_type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}