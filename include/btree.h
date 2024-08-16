#ifndef BTREE_H
#define BTREE_H

#include <string>


typedef enum {
  NODE_INTERNAL,
  NODE_LEAF
} NodeType;


class BTree {
// public:
//   BTree();
//   ~BTree();

public:
  uint32_t* leaf_num_cells(void* node);
  void* leaf_cell(void* node, uint32_t cell_num);
  uint32_t* leaf_key(void* node, uint32_t cell_num);
  void* leaf_value(void* node, uint32_t cell_num);

public:
  void initialize_leaf_node(void* node);
  void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);

public:
  // Temporary redefinition
  static const uint32_t ROW_SIZE = 292;
  static const uint32_t PAGE_SIZE = 4096;

public:
  // Common Node Header Layout
  // NODE_TYPE, IS_ROOT, PARENT_POINTER
  static const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
  static const uint32_t NODE_TYPE_OFFSET = 0;
  static const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
  static const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
  static const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
  static const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
  static const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

public:
  // Leaf Node Header Layout
  // COMMON_HEADER, NUM_CELLS
  static const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
  static const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
  static const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

public:
  // Leaf Node Body Layout
  // LEAF_HEADER, KEY, VALUE
  static const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
  static const uint32_t LEAF_NODE_KEY_OFFSET = 0;
  static const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
  static const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
  static const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
  static const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
  static const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

};

#endif