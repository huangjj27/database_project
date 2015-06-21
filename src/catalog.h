#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include "binary_tree.h"
using namespace std;

// trim the nested objects or array
// there is only 1 layer nestd, so only trim once is enough.
string SkipObjArr(string str);

// sort the key and its type as a BST
void BuildCatalogTree(Node* &root, string str);

// get the source file, sort it into a BST and ouput it to a catalog
void BuildCatalogFile(string infile_name);

#endif
