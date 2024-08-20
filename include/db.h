#ifndef DB_H
#define DB_H

#include <string>

class Table;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_DUPLICATE_KEY,
  EXECUTE_TABLE_FULL,
  EXECUTE_ERROR
} ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_STRING_TOO_LONG,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} StatementType;

typedef struct InputBuffer{
  std::string buffer;
  ssize_t input_length;
  InputBuffer() {input_length = 0;}
} InputBuffer;


class DB {
public:
  DB();
  void run_once();

public:
  void print_prompt();
  void read_input();
  MetaCommandResult do_meta_command();
  void print_info();

public:
  PrepareResult prepare_insert();
  PrepareResult prepare_statement();
  
public:
  ExecuteResult execute_insert();
  ExecuteResult execute_select();
  ExecuteResult execute_statement();

public:
  bool is_input_empty;
  InputBuffer* input_buffer;
  StatementType statement_type;
  Table* table;

};

#endif