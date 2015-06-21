#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

#include "binary_tree.h"
#include "catalog.h"
using namespace std;

string SkipObjArr(string str) {
  string s;
  if (str[str.length() - 1] == ',') {
    for (int i = 1; i < str.length() - 2; i++) {
      s += str[i];
      if (str[i] == '{') {
        while (str[i] != '}') i++;
        s += str[i];
      }
      if (str[i] == '[') {
        while (str[i] != ']') i++;
        s += str[i];
      }
    }
  }

  if (str[str.length() - 1] == '}') {
    for (int i = 1; i < str.length() - 1; i++) {
      s += str[i];
      if (str[i] == '{') {
        while (str[i] != '}') i++;
        s += str[i];
      }
      if (str[i] == '[') {
        while (str[i] != ']') i++;
        s += str[i];
      }
    }
  }
  s += ',';     // used to figer out the key-value pairs.
  return s;
}

void BuildCatalogTree(Node* &root, string str) {
  if (str[0] == '{') {
    string s;
    s = SkipObjArr(str);  // trim the brakets
    for (int j = 0; j < s.length(); j++) {

      // get a pair of key-value pair.
      string ss;
      while (j < s.length()) {
        ss += s[j];
        if (s[j] == ',') {
          break;
        }
        j++;
      }


      string attr;
      string type;
      // get know what the value type of the key is
      for (int i = ss.find_first_of('"') + 1; i < ss.find(':') - 1; i++) {
        attr += ss[i];
      }

      switch (ss[ss.find(':') + 2]) {
        case '"':
          type = "text";
          break;
        case '{':
          type = "json";
          break;
        case '[':
          type = "array";
          break;
        case 't': case 'f':
          type = "bool";
          break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          type = "int";
          break;
        default:
          break;
      }
      insert(root, attr, type);
    }
  }
}

void BuildCatalogFile(string infile_name) {
  ifstream infile;
  infile.open(infile_name, ios::in);
  ofstream outfile;
  outfile.open("catalog.data", ios::ate | ios::out);
  string str;
  Node* root = NULL;
  outfile << "_id\tKey_name\tKey_type\tcount" << endl;
  while (getline(infile, str)) {
    BuildCatalogTree(root, str);
  }
  midorder(root, outfile);
  infile.close();
  outfile.close();
}
