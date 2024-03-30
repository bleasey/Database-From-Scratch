#include <iostream>
#include "../include/db.h"

void DB::print_prompt() { std::cout << "db > ";}

void DB::read_input() {
  getline(std::cin, input_buffer->buffer);
  input_buffer->input_length = input_buffer->buffer.size();

  if (input_buffer->input_length <= 0) {
    std::cout << "Error reading input\n";
    exit(EXIT_FAILURE);
  }
}

MetaCommandResult DB::do_meta_command() {
  if (input_buffer->buffer == ".exit") {
    delete input_buffer;
    exit(EXIT_SUCCESS);
  }
  else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult DB::prepare_statement() {
  if (input_buffer->buffer.substr(0, 6) == "insert") {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (input_buffer->buffer.substr(0, 6) == "select") {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void DB::execute_statement() {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}