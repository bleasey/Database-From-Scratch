#ifndef DB_H
#define DB_H

#include <string>

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100


typedef struct InputBuffer{
  std::string buffer;
  ssize_t input_length;
  InputBuffer() {input_length = 0;}
} InputBuffer;

typedef enum {
  EXECUTE_SUCCESS,
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
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} StatementType;

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE] = {'\0'};
  char email[COLUMN_EMAIL_SIZE] = {'\0'};
} Row;

typedef struct Table{
  uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
  Table();
  ~Table();
} Table;

typedef struct {
  StatementType type;
  Row row_to_insert;  // only used by insert statement
} Statement;


class DB {
public:
  DB();
  void run_once();

public:
  void print_prompt();
  void print_row(Row* row);
  void read_input();
  MetaCommandResult do_meta_command();

public:
  PrepareResult prepare_statement();
  ExecuteResult execute_insert();
  ExecuteResult execute_select();
  ExecuteResult execute_statement();

public:
  void serialize_row(Row* source, void* destination);
  void deserialize_row(void* source, Row* destination);
  void* get_row(Table* table, uint32_t row_num);

private:
  const uint32_t ID_SIZE = size_of_attribute(Row, id);
  const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
  const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
  const uint32_t ID_OFFSET = 0;
  const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
  const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
  const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

  const uint32_t PAGE_SIZE = 4096;
  const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
  const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

private:
  bool is_input_empty;
  InputBuffer* input_buffer;
  Statement* statement;
  Table* table;

};

#endif