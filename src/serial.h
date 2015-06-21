#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<sstream>

using namespace std;

struct catalog {
  int num;
  string key_name[100000];
  string key_type[100000];
  int count[100000];
  void operator=(const catalog& a) {
    num = a.num;
    for (int i = 1; i < num; i++) {
      key_name[i - 1] = a.key_name[i - 1];
      key_type[i - 1] = a.key_type[i - 1];
      count[i - 1] = a.count[i - 1];
    }
  }
};

struct serial {
  int aid[50];
  int offset[50];
  int len[50];
  string data;
  int count;
  int sum;
};

int strtonum(string s);

void get_bin();

string sertojson(serial ser, catalog* scanlog1);

void find_A_equals_B(string key, string value, catalog* catalog);

catalog read_catalog();
