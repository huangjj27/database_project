//#include <iostream>
#include <fstream>
#include <string>
#include "catalog.h"
#include "serial.h"
using namespace std;

void InsertByFilename() {
  string str;
  cout << "please input the data file name: ";
  cin >> str;
  BuildCatalogFile(str);
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

void MatchKeyAWithValueB(string key, string value) {
  get_bin();
  struct catalog c = read_catalog();
  find_A_equals_B(key, value, &c);
}
int main(void) {
  std::string operation;
  std::string key, value;
  cout << "please input \"insert\"" << endl;
  cin >> operation;
  if (operation == "insert") InsertByFilename();
  cout << "please give the command(insert check find)" << endl;
  while (std::cin >> operation) {
    if (operation == "insert") InsertByFilename();
    else if (operation == "check") CheckCatalog();
    else if (operation == "find") {
      //cout << "please input A and B" << endl;
      //cin >> key >> value;
      //MatchKeyAWithValueB(key, value);
      get_bin();
    } else return 0;
  }
  return 0;
}
