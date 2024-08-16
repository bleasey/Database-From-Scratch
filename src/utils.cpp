#include <iostream>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

void DB::print_prompt() { std::cout << "db > ";}

void DB::read_input() {
  getline(std::cin, input_buffer->buffer);
  input_buffer->input_length = input_buffer->buffer.size();

  if (input_buffer->input_length <= 0) is_input_empty = true;
  else is_input_empty = false;
}

void DB::print_info() {
  std::cout << "----- DATABASE INFO -----\n";
  std::cout << "Page/Leaf Size   - " << Table::PAGE_SIZE << std::endl;
  std::cout << "Row Size         - " << Table::ROW_SIZE << std::endl;
  std::cout << "Num Pages        - " << table->pager->num_pages << std::endl;
  std::cout << "Leaf Header Size - " << table->pager->btree->LEAF_NODE_HEADER_SIZE << std::endl;
  std::cout << "Leaf Key, Value  - " << table->pager->btree->LEAF_NODE_KEY_SIZE << " "
                                     << table->pager->btree->LEAF_NODE_VALUE_SIZE << std::endl;
  std::cout << "Leaf Max Cells   - " << table->pager->btree->LEAF_NODE_MAX_CELLS << std::endl;
  std::cout << "-------------------------\n";
}

MetaCommandResult DB::do_meta_command() {
  if (input_buffer->buffer == ".exit") {
    delete this->input_buffer;
    delete this->table; // saves to disk, frees allocated memory
    exit(EXIT_SUCCESS);
  }
  else if (input_buffer->buffer == ".dbinfo") {
    DB::print_info();
    return META_COMMAND_SUCCESS;
  }
  else if (input_buffer->buffer == ".btree") {
    table->print_btree();
    return META_COMMAND_SUCCESS;
  }
  else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}