#include <iostream>
#include <fstream>
#include <string>
#include "catalog.h"
#include "serial.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdlib.h>

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
  struct timeval tpstart,tpend;
  float timeuse;
  clock_t start, finish;
  std::string operation;
  std::string key, value;
  std::string file_name;
  cout << "please input \"insert\"" << endl;
  cin >> operation;
  if (operation == "insert") {
    cout << "please input the data file name: " << endl;
    cin >> file_name; 
    start = clock();
    InsertByFilename(file_name); 
    finish = clock();
    timeuse = (double)(finish-start)/CLOCKS_PER_SEC;
    cout << "Total time:" << timeuse << " seconds" <<endl;
    
  }
  cout << "please give the command(insert check find)" << endl;
  while (std::cin >> operation) {
    if (operation == "insert") {
      cout << "please input the data file name: " << endl;
      cin >> file_name;
      start = clock();
      InsertByFilename(file_name); 
      finish = clock();
      timeuse = (double)(finish-start)/CLOCKS_PER_SEC;
      cout << "Total time:" << timeuse << " seconds" <<endl;

    }
    else if (operation == "check") {
      start = clock();
      CheckCatalog();
      finish = clock();
      timeuse = (double)(finish-start)/CLOCKS_PER_SEC;
      cout << "Total time:" << timeuse << " seconds" <<endl;
    }
    else if (operation == "find") {
      cout << "please input A and B"<<endl;
      cin >> key >> value;
      start = clock();
      MatchKeyAWithValueB(key, value, file_name);
      finish = clock();
      timeuse = (double)(finish-start)/CLOCKS_PER_SEC;
      cout << "Total time:" << timeuse << " seconds" <<endl;
    }
    else return 0;
    cout << "please give the command(insert check find)" << endl;
  }
  return 0;
}
