#include <iostream>
#include "config.h"
#include "structs.h"
#include "structs_methods.h"
#include "solvers.h"
#include "checkers.h"

using namespace std;

int main()
{ 
  cout << "Show work of LinkedList" << endl;
  LinkedList* head = insert_after();
  LinkedList* current = head;
  for (int i = 1; i < 10; i++){
    current = insert_after(current, i);
  }
  current = head;
  while (current != nullptr){
    cout << current->data << endl;
    current = current->next;
  }
  cout << "Reverse list" << endl;
  head = reverse_rec(head);
  current = head;
  while (current != nullptr){
    cout << current->data << endl;
    current = current->next;
  }
  erase(head);

  cout << "IO_FLAV test" << endl;
  compare_io_flav();
  
  cout << "Dynamic arr test" << endl;
  compare_dynamic_arr();

  return 0;
}