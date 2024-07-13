#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include "../include/db.h"
#include "../include/table.h"

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
  uint32_t num_pages = this->file_length / Table::PAGE_SIZE;
  uint32_t page_offset = this->file_length % Table::PAGE_SIZE;

  // Adding a page when previous is full
  if (page_num == num_pages && page_offset == 0) {
    this->pages[page_num] = malloc(Table::PAGE_SIZE);
    return this->pages[page_num]; 
  }

  // Else one of the existing pages is requested

  if (page_num >= num_pages) {
    std::cout << "Invalid call made to page: " << page_num 
    << "; pages present: " << num_pages << " with offset: "
    << page_offset << std::endl;
    exit(EXIT_FAILURE);
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


void Pager::flush_to_disk(uint32_t page_num, uint32_t size) {
  if (this->pages[page_num] == NULL) {
    std::cout << "Tried to flush null page having index " << page_num
    << std::endl;
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(this->file_descriptor, page_num * Table::PAGE_SIZE, SEEK_SET);

  if (offset == -1) {
    std::cout << "Error seeking: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(this->file_descriptor, this->pages[page_num], size);

  if (bytes_written == -1) {
    std::cout << "Error writing: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}