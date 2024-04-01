#include <iostream>
#include "../include/db.h"

DB::DB() {
  input_buffer = new InputBuffer();
  statement = new Statement();
  table = new Table();
}

void DB::run_once() {
  print_prompt();
  read_input();
  if (is_input_empty) return;

  // Handle meta/special commands
  if (input_buffer->buffer[0] == '.') {
    switch (do_meta_command()) {
      case (META_COMMAND_SUCCESS):
        return;
      case (META_COMMAND_UNRECOGNIZED_COMMAND):
        std::cout << "Unrecognized command " << input_buffer->buffer << std::endl;
        return;
    }
  }

  // Analyse/Parse statement
  switch (prepare_statement()) {
    case (PREPARE_SUCCESS):
      break;
    case (PREPARE_STRING_TOO_LONG):
      std::cout << "Error. Input string too long.\n";
      return;
    case (PREPARE_SYNTAX_ERROR):
      std::cout << "Syntax error. Could not parse statement.\n"; 
      return;
    case (PREPARE_UNRECOGNIZED_STATEMENT):
      std::cout << "Unrecognized keyword at start of " << input_buffer->buffer << std::endl;
      return;
  }

  // Execute statement
  switch (execute_statement()) {
    case (EXECUTE_SUCCESS):
      std::cout << "Executed.\n";
      break;
    case (EXECUTE_TABLE_FULL):
      std::cout << "Error: Table full.\n";
      break;
  }
}