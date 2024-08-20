#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <assert.h>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

Pager::Pager(const char* filename) {
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    this->pages[i] = NULL;
  }

  int fd = open(filename,
                O_RDWR |      // Read/Write mode
                O_CREAT,      // Create file if it does not exist
                S_IWUSR |     // User write permission
                S_IRUSR       // User read permission
                );

  if (fd == -1) {
    std::cout << "Unable to open file\n";
    exit(EXIT_FAILURE);
  }

  off_t flength = lseek(fd, 0, SEEK_END);

  this->file_descriptor = fd;
  this->file_length = flength;
  this->num_pages = this->file_length / Table::PAGE_SIZE;

  // File length assertion
  if (file_length % Table::PAGE_SIZE != 0) {
    std::cout << "Corrupt file. mydb file is not a whole number of pages.\n";
    exit(EXIT_FAILURE);
  }
}

Pager::~Pager() {
  // Free allocated memory
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    if (this->pages[i] == NULL) {
      // This page wasnt fetched, hence no memory was allocated
      continue;
    }
    free(this->pages[i]);
    this->pages[i] = NULL;
  }
  
  // close file
  int result = close(this->file_descriptor);
  if (result == -1) {
    std::cout << "Error closing db file.\n";
    exit(EXIT_FAILURE);
  }
}


void* Pager::get_page(uint32_t page_num) {
  // Checking if page requested is within limits
  assert (page_num <= this->num_pages);

  // Adding a page when previous is full
  // Else one of the existing pages is requested
  if (page_num == this->num_pages) {
    this->pages[page_num] = malloc(Table::PAGE_SIZE);
    this->btree->initialize_leaf_node(this->pages[page_num]);
    this->num_pages += 1;
    return this->pages[page_num]; 
  }

  // Fetching page from file if not in cache
  if (this->pages[page_num] == NULL) {
    void* page = malloc(Table::PAGE_SIZE);
    lseek(this->file_descriptor, page_num * Table::PAGE_SIZE, SEEK_SET);
    ssize_t bytes_read = read(this->file_descriptor, page, Table::PAGE_SIZE);
    
    if (bytes_read == -1) {
      std::cout << "Error reading file: " << errno << std::endl;
      exit(EXIT_FAILURE);
    }

    this->pages[page_num] = page;
  }

  return this->pages[page_num];
}


void Pager::flush_to_disk(uint32_t page_num) {
  if (this->pages[page_num] == NULL) {
    std::cout << "Tried to flush null page having index " << page_num
    << std::endl;
    exit(EXIT_FAILURE);
  }

  uint32_t num_cells = *(btree->leaf_num_cells(this->pages[page_num]));
  if (num_cells == 0) return;

  off_t offset = lseek(this->file_descriptor, page_num * Table::PAGE_SIZE, SEEK_SET);
  if (offset == -1) {
    std::cout << "Error seeking: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(this->file_descriptor, this->pages[page_num], Table::PAGE_SIZE);
  if (bytes_written == -1) {
    std::cout << "Error writing: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}