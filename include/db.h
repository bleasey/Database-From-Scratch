#ifndef DB_H
#define DB_H

#include <string>

typedef struct InputBuffer{
  std::string buffer;
  ssize_t input_length;

  InputBuffer() {input_length = 0;}
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

class DB {
public:
  DB();

public:
  // SECTION
  MetaCommandResult do_meta_command();
  PrepareResult prepare_statement();
  void execute_statement();
  //!SECTION

public:
  // SECTION - Utility
  void print_prompt();
  void read_input();
  void run_once();
  //!SECTION


private:
  InputBuffer* input_buffer;
  Statement* statement;

};

#endif