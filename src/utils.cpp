#include <iostream>
#include "../include/db.h"

void DB::print_prompt() { std::cout << "db > ";}

void DB::read_input() {
  std::cin >> input_buffer->buffer;
  input_buffer->input_length = input_buffer->buffer.size();

  if (input_buffer->input_length <= 0) {
    std::cout << "Error reading input\n";
    exit(EXIT_FAILURE);
  }
}