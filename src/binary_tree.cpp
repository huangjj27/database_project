#include <iostream>
#include <fstream>
#include <string>
#include "binary_tree.h"

using namespace std;

Node* search(Node* &root, const int id) {
  if (root == NULL) return NULL;
  if (root->key_id == id) return root;
  Node* search_left_tree = search(root->left, id);
  Node* search_right_tree = search(root->right, id);
  if (search_left_tree != NULL) return search_left_tree;
  if (search_right_tree != NULL) return search_right_tree;
  return NULL;
}

void insert(Node* &root, const string key, const string type) {
  string key_and_type = key + type;
  if (root == NULL) {
    root = new Node;
    root->key = key;
    root->type = type;
    root->key_and_type = key_and_type;
    root->count = 1;
    root->left = root->right = NULL;
    return;
  } else if (root->key_and_type == key_and_type) {
    root->count++;
    return;
  } else if (key_and_type < root->key_and_type) {
    insert(root->left, key, type);
  } else {
    insert(root->right, key, type);
  }
}

int id = 0;
void midorder(Node* &root, ofstream &outfile) {
  if (root != NULL) {
    midorder(root->left, outfile);
    id++;
    root->key_id = id;
    outfile << root->key_id << '\t' << root->key << '\t'
      << root->type << '\t' << root->count << endl;
    midorder(root->right, outfile);
  }
}
