#ifndef TABLE_H
#define TABLE_H

#include <string>

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define COLUMN_USERNAME_SIZE 31
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100


typedef struct Pager{
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
  Pager(const char* filename);
  ~Pager();
  void* get_page(uint32_t page_num);
  void flush_to_disk(uint32_t page_num, uint32_t size);
} Pager;

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1] = {'\0'};
  char email[COLUMN_EMAIL_SIZE + 1] = {'\0'};
} Row;

class Table {
public:
  Table();
  ~Table();

public:
  void serialize_row(Row* source, void* destination);
  void deserialize_row(void* source, Row* destination);
  void* get_row(uint32_t row_num);
  
public:
  void print_row(Row* row);

public:
  static const uint32_t ID_SIZE = size_of_attribute(Row, id);
  static const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
  static const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
  static const uint32_t ID_OFFSET = 0;
  static const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
  static const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
  static const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

  static const uint32_t PAGE_SIZE = 4096;
  static const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
  static const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

public:
  Pager* pager;
  Row* row_to_insert;

};

#endif