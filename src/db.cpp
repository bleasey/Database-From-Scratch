#include <iostream>
#include "../include/db.h"

DB::DB() {
  input_buffer = new InputBuffer();
  statement = new Statement();
}

void DB::run_once() {
  print_prompt();
  read_input();

  if (input_buffer->buffer[0] == '.') {
    switch (do_meta_command()) {
      case (META_COMMAND_SUCCESS):
        return;
      case (META_COMMAND_UNRECOGNIZED_COMMAND):
        std::cout << "Unrecognized command " << input_buffer->buffer << std::endl;
        return;
    }
    }

  switch (prepare_statement()) {
    case (PREPARE_SUCCESS):
      break;
    case (PREPARE_UNRECOGNIZED_STATEMENT):
      std::cout << "Unrecognized keyword at start of " << input_buffer->buffer << std::endl;
      return;
  }

  execute_statement();
  std::cout << "Executed.\n";
}