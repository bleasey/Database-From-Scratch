#include <iostream>
#include "../include/db.h"
#include "../include/table.h"
#include "../include/btree.h"

void Table::print_row(Row* row) {
  std::cout << row->id << " " <<  row->username << " "
            << row->email << std::endl;
}

void Table::print_leaf(void* node) {
  std::cout << "-------------------------\n";
  uint32_t num_cells = *(pager->btree->leaf_num_cells(node));
  std::cout << "Number of cells  - " << num_cells << std::endl;
  
  for(uint32_t cell_num=0; cell_num<num_cells; cell_num++){
    std::cout << "Cell " << cell_num << ":  Key - " <<
    pager->btree->leaf_key(node, cell_num) << "; Value - " <<
    pager->btree->leaf_value(node, cell_num) << std::endl;
  }
  std::cout << "-------------------------\n";
}

void Table::print_btree() {
  uint32_t n_pages = pager->num_pages;
  for(uint32_t page_num=0; page_num<n_pages; page_num++){
    void* page = pager->get_page(page_num);
    print_leaf(page);
  }
}