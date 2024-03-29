#include <iostream>
#include "../include/db.h"

DB::DB() {
  input_buffer = new InputBuffer();
}

void DB::run() {
  while (true) {
    print_prompt();
    read_input();

    if (input_buffer->buffer == ".exit") {
      delete input_buffer;
      exit(EXIT_SUCCESS);
    }
    else {
      std::cout << "Unrecognized command " << input_buffer->buffer << std::endl;
    }
  }
}