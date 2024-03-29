#ifndef DB_H
#define DB_H

#include <string>

typedef struct InputBuffer{
  std::string buffer;
  ssize_t input_length;

  InputBuffer() {input_length = 0;}
} InputBuffer;


class DB {
public:
  // SECTION
  DB();
  //!SECTION

public:
  // SECTION - Utility
  void print_prompt();
  void read_input();
  void run();
  //!SECTION


private:
  InputBuffer* input_buffer;

};

#endif