#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <string>
using namespace std;

// define the node of binary search tree
struct Node {
  int count;
  int key_id;
  string key_and_type;
  string key;
  string type;
  Node *left;
  Node *right;
};

// function to search the corresponding node
// @root: the root of the BST
// @id: used as a key to select the node
Node* search(Node* &root, const int id);

// insert new node with params
// @root: the root of the BST
// @key: the key name of the inserting node
// @type: the type of param @key. Can be text, bool, int, json, array.
void insert(Node* &root, const string key, const string type);

// output the tree to outfile.list by ascending order.
// @root: the root of the BST
// @outfile: the file to store the catalog
void midorder(Node* &root, ofstream &outfile);

#endif
