#include "include/db.h"
#include "include/table.h"

int main() {
  DB db;

  while(true) {
    db.run_once();
  }
}