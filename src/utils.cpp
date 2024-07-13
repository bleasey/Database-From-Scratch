#include <iostream>
#include "../include/db.h"
#include "../include/table.h"

void DB::print_prompt() { std::cout << "db > ";}

void Table::print_row(Row* row) {
  std::cout << row->id << " " <<  row->username << " "
            << row->email << std::endl;
}

void DB::read_input() {
  getline(std::cin, input_buffer->buffer);
  input_buffer->input_length = input_buffer->buffer.size();

  if (input_buffer->input_length <= 0) is_input_empty = true;
  else is_input_empty = false;
}

MetaCommandResult DB::do_meta_command() {
  if (input_buffer->buffer == ".exit") {
    delete this->input_buffer;
    delete this->table; // saves to disk, frees allocated memory
    exit(EXIT_SUCCESS);
  }
  else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}