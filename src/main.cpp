#include <iostream>
#include <fstream>
#include <string>
#include "catalog.h"
#include "serial.h"
using namespace std;

void InsertByFilename(string file_name) {
  BuildCatalogFile(file_name);
  cout << "catalog built!" << endl;
}

void CheckCatalog() {
  ifstream infile;
  string str;
  infile.open("catalog.data", ios::in);
  while (getline(infile, str)) {
    cout << str << endl;
  }
}

void MatchKeyAWithValueB(string key, string value, string file_name) {
  get_bin(file_name);
  struct catalog c = read_catalog();
  find_A_equals_B(key, value, &c);
}
int main(void) {
  std::string operation;
  std::string key, value;
  std::string file_name;
  cout << "please input \"insert\"" << endl;
  cin >> operation;
  if (operation == "insert") {
    cout << "please input the data file name: " << endl;
    cin >> file_name;
    InsertByFilename(file_name);
  }
  cout << "please give the command(insert check find)" << endl;
  while (std::cin >> operation) {
    if (operation == "insert") {
      cout << "please input the data file name: " << endl;
      cin >> file_name;
      InsertByFilename(file_name);
    }
    else if (operation == "check") CheckCatalog();
    else if (operation == "find") {
    cout << "please input A and B"<<endl;
    cin >> key >> value;
    MatchKeyAWithValueB(key, value, file_name);
                //get_bin();
  }
    else return 0;
  }
  return 0;
}
