#include"serial.h"
#define PAGE_SIZE 8192
int strtonum(string s);
string numtostr(int n);

union intandchar
{
  int a;
  char b[4];
};

int strtonum(string s) {
  int num;
  stringstream ss(s);
  ss>>num;
  return num;
}

string numtostr(int n) {
  string str;
  stringstream ss;
  ss << n;
  ss >> str;
  return str;
}

void get_bin() {
  struct catalog c;
  string str = "";
  string str1 = "";
  string str2 = "";
  string str3 = "";
  string strf = "";
  c.num = 0;
  string data = "";

  c = read_catalog();

  FILE* out = fopen("create.data", "wb+");
  string tmp = "";
  freopen("data2.json","r",stdin);
  //getline(cin,str);
  //ofstream out("create.data");
  while(getline(cin,str)) {
    if (str == "[") continue;     //文件开头标志
    if (str == "]") break;        //文件结束标志
    int sum = 0;
    int count = 0;
    int aid[50];
    int off[50];
    string str3[50];

    str3[0] = "";

    int i = 2;          //忽略开头的{“
    int len = str.size();
    while(i < len - 1) {
      int l = 0;        //key_name的偏移亮
      while(str[i] != '"') {
        str1 += str[i];
        i++;
      }
      i += 3;         //忽略”：和空格
      if (str[i] == '{') {      //key_type的偏移量
        str2 = "json";
        while(str[i] != '}') {
          str3[count] += str[i];
          i++;
          l++;
        }
        str3[count] += '}';
        l++;
      } else if (str[i] == '[') {
        str2 = "array";
        while(str[i] != ']') {
          str3[count] += str[i];
          i++;
          l++;
        }
        str3[count] += ']';
        l++;
      } else if (str[i] == '"') {
        str2 = "text";
        i++;
        while(str[i] != '"') {
          str3[count] += str[i];
          i++;
          l++;
        }
      } else if (str[i] == 'f' || str[i] == 't') {
        str2 = "bool";
        while(str[i] != ',' && str[i] != '}') {
          str3[count] += str[i];
          i++;
          l++;
        }
         //                       if (str3[count] == "true")str3[count] = "t";
         //                       else str3[count] = "f";
      } else {
        str2 = "int";
        string t = "";
        while(str[i] != ',' && str[i] != '}') {
          t += str[i];
          //str3[count] += str[i];
          i++;
          //l++;
        }
        l += 4;
        intandchar int_;
        int_.a = strtonum(t);
        str3[count] += int_.b[0] + int_.b[1] + int_.b[2] + int_.b[3];
      }
      while(str[i] != ',' && i < len - 1) i++;
      if (i != len - 1) i += 3;
      off[count] = l;
      for (int j = 0; j < c.num; j++) {
        if(str1 == c.key_name[j] && str2 == c.key_type[j]) {
          aid[count] = j+1;
          break;
        }
      }
      str2 = "";
      str1 = "";
      count++;
      str3[count] = "";
    }

    int ss = 0;
    //out << count << ' ';
    char* a, b[4];
    a = (char*)&count;
    b[0]=*((char *)a+0);
        b[1]=*((char *)a+1);
        b[2]=*((char *)a+2);
        b[3]=*((char *)a+3);
    tmp += b[0] + b[1] + b[2] + b[3];
    //tmp += ' ';
    for (int j = 0; j < count; j++) {
      //out << aid[j] << ' ';
      a = (char*)&(aid[j]);
        b[0]=*((char *)a+0);
            b[1]=*((char *)a+1);
            b[2]=*((char *)a+2);
            b[3]=*((char *)a+3);
        tmp += b[0] + b[1] + b[2] + b[3];
      //tmp += ' ';
    }
    for (int j = 0; j < count; j++) {
      //out << ss << ' ';
      a = (char*)&ss;
      b[0]=*((char *)a+0);
            b[1]=*((char *)a+1);
            b[2]=*((char *)a+2);
            b[3]=*((char *)a+3);
        tmp += b[0] + b[1] + b[2] + b[3];
      //tmp += ' ';
      ss += off[j];
      sum += off[j];
    }
    //out << sum << ' ';
    a = (char*)&sum;
    b[0]=*((char *)a+0);
        b[1]=*((char *)a+1);
        b[2]=*((char *)a+2);
        b[3]=*((char *)a+3);
    tmp += b[0] + b[1] + b[2] + b[3];
    //tmp += ' ';
    for (int j = 0; j < count; j++) {
      //for (unsigned int i = 0; i < str3[j].length(); i++) out << str3[j][i];
      //out << str3[j] << ' ';
      tmp += str3[j];
      //tmp += ' ';
    }
    //out << endl;
    tmp += "\n";

    if (tmp.size()/PAGE_SIZE) {
      fwrite(tmp.substr(0, PAGE_SIZE).data(), PAGE_SIZE, 1, out);
      tmp = tmp.substr(PAGE_SIZE, tmp.size()%PAGE_SIZE);
    }
  }
  fwrite(tmp.data(), 1, tmp.length(), out);


  fclose(out);
  /*out = fopen("create.data", "rb+");
  char* text ;
  fread(text, 32, PAGE_SIZE/32, out);
  cout << text << endl;
  fclose(out);*/
  out = NULL;

  fclose(stdin);
}


string sertojson(serial ser, catalog* scanlog1) {
  string ans = "", flag[5] = {"json", "array", "text", "int", "bool"};
    ans += "{";

      for (int i = 0; i < ser.count; i++) {
        catalog* scanlog = scanlog1;
        int id = ser.aid[i];

        ans += "\"";
            ans += scanlog->key_name[id-1];
          ans += "\"";
          ans += ": ";
      if ((scanlog->key_type[id-1]) == flag[0]) {
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              ans += ser.data[j];
          }
      }
      if ((scanlog->key_type[id-1]) == flag[1]) {
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              ans += ser.data[j];
        }
      }

        if ((scanlog->key_type[id-1]) == flag[2]) {
          ans += "\"";
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              ans += ser.data[j];
          }
          ans += "\"";
      }

      if ((scanlog->key_type[id-1]) == flag[3]) {
        intandchar int_;
        int j_ = 0;
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              //ans += ser.data[j];
          int_.b[j_] = ser.data[j];
          //ans += numtostr(tmp);
          }
        ans += numtostr(int_.a);
      }
        if ((scanlog->key_type[id-1]) == flag[4]) {
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              if (ser.data[j] == 't') ans += "true";
              if (ser.data[j] == 'f') ans += "false";
          }
      }
          if (i != ser.count - 1) ans += ", ";
      }
    ans += "}";
    return ans;
}

void find_A_equals_B(string key, string value, catalog* catalog) {

  if (catalog->num == 0) {
    cout << "NONE\n";
    return;
  }

  intandchar int_;
  string value_ = "";
    string type = "", flag[5] = {"json", "array", "text", "int", "bool"};
  if (value[0] == '[' && value[value.size() - 1] == ']') type = flag[1];
  else if (value[0] == '{' && value[value.size() - 1] == '}') type = flag[0];
  else if (value == "true" || value == "false") type = flag[4];
  else {
    for (int j = 0; j < value.size(); j++) {
      if (value[j] > '9' || value[j] < '0') {
        type = flag[2];
        break;
      }
    }
    if (type == "") type = flag[3];
  }
  if (type == flag[3]) {
    int_.a = strtonum(value);
    value_ += int_.b[0] + int_.b[1] + int_.b[2] + int_.b[3];
  } else {
    value_ = value;
  }

  int aid[1000000];
  int id_num = 0;
  for (int j = 1; j <= catalog->num; j++) {
    if (key == catalog->key_name[j-1] && type == catalog->key_type[j-1]) {
            aid[id_num++] = j;
        }
    }
  if (id_num == 0) {
    cout << "NONE\n";
    return;
  }

  bool is_find = false;
  char read_buffer[PAGE_SIZE];
  string str = "", str1 = "";
  FILE* input = fopen("create.data", "rb+");
  if (input == NULL) {
    cout << "no create.data\n";
    return;
  }

  for (int i_ = 0; i_ < id_num; i_++) { //

  int count = catalog->count[aid[i_]-1];

  bool is_find_key_in_this_one;

  serial ser;
  int found_id_[50] = {0};
  int found_num = 0;
  int i = 0;
  while(1) {
    //if (str.size()%(PAGE_SIZE/32)) is_read_more_data = false;
    if (i == str.size()) {
      if (fread(read_buffer, PAGE_SIZE, 1, input) == 0) break;
      str += read_buffer;
      i = 0;
    }
    if (count <= 0) break;
    is_find_key_in_this_one = false;
    found_num = 0;
    //get ser count (attributes number)

    for (int j = 0; j < 4; j++) {
      if (i >= str.size()) {
          fread(read_buffer, PAGE_SIZE, 1, input);
          str = "";
          str += read_buffer;
          i = 0;
        }
      int_.b[j] = str[i++];
    }
    ser.count = int_.a;
    cout << "get ser count (attributes number)\n";
    //get ser aid
    for (int j_ = 0; j_ < ser.count; j_++) {
      for (int j = 0; j < 4; j++) {
          if (i >= str.size()) {
              fread(read_buffer, PAGE_SIZE, 1, input);
              str = "";
              str += read_buffer;
              i = 0;
            }
          int_.b[j] = str[i++];
        }
        ser.aid[j_]= int_.a;
    }
    cout << "get ser aid\n";
    //judge whether the keyA is in this json
    for (int j = 0; j < ser.count; j++) {
      if (aid[i_] == ser.aid[j]) {
        is_find_key_in_this_one = true;
        found_id_[found_num++] = j;
        count--;
      }
    }
    cout << "judge whether the keyA is in this json\n" << count << "\n";
    if (is_find_key_in_this_one) {
      for (int j_ = 0; j_ < ser.count; j_++) {
          for (int j = 0; j < 4; j++) {
              if (i >= str.size()) {
                fread(read_buffer, PAGE_SIZE, 1, input);
                str = "";
                str += read_buffer;
                i = 0;
              }
          int_.b[j] = str[i++];
            }
        ser.offset[j_] = int_.a;
        }
      //get ser sum (the length of data)
      for (int j = 0; j < 4; j++) {
          if (i >= str.size()) {
              fread(read_buffer, PAGE_SIZE, 1, input);
              str = "";
              str += read_buffer;
              i = 0;
            }
          int_.b[j] = str[i++];
        }
        ser.sum = int_.a;
      cout << "get ser sum (the length of data)\n";
      //get ser len
      for (int j = 0; j <= ser.count; j++) {
        ser.len[j] = ser.offset[j+1]-ser.offset[j];
      }
      cout << "get ser len\n";
      //get ser data
      while (ser.data.length() < ser.sum) {
        if (i >= str.size()) {
              fread(read_buffer, PAGE_SIZE, 1, input);
              str = "";
              str += read_buffer;
              i = 0;
            }
        ser.data += str[i++];
      }
      cout << "get ser data\n";
      //judge whethe the valueB and the found_value from this json is equals
      for (int j = 0; j < found_num; j++) {
        string found_value = ser.data.substr(ser.offset[found_id_[j]], ser.len[found_id_[j]]);
        if (value_ == found_value) {
          cout << sertojson(ser, catalog) << endl;
          is_find = true;
          break;
        }
      }
      cout << "judge whethe the valueB and the found_value from this json is equals\n";
    } else {
      i += 4*ser.count;
      if (i >= str.size()) {
          fread(read_buffer, PAGE_SIZE, 1, input);
          str = "";
          str += read_buffer;
          i -= PAGE_SIZE;
        }

      for (int j = 0; j < 4; j++) {
          if (i >= str.size()) {
              fread(read_buffer, PAGE_SIZE, 1, input);
              str = "";
              str += read_buffer;
              i = 0;
            }
          int_.b[j] = str[i++];
        }
        ser.sum = int_.a;
      i += ser.sum;
      if (i >= str.size()) {
          if (fread(read_buffer, PAGE_SIZE, 1, input)) {
          break;
        }
          str = "";
          str += read_buffer;
          i -= PAGE_SIZE;
        }
    }
  }
  }
  if (!is_find) {
    cout << "NONE\n";
  }
  fclose(input);
  input = NULL;
}
catalog read_catalog() {
    struct catalog c;
  string str = "";
  string str1 = "";
  string str2 = "";
  string str3 = "";
  string str4 = "";
  c.num = 0;
  string data = "";
  ifstream input;
  input.open("catalog.data");
  while(getline(input,str)) {
    int i = 0;
    while(str[i] != ' ') i++;
    i++;
    while(str[i] != ' ') {
      str1 += str[i];
      i++;
    }
    i++;
    while(str[i] != ' ') {
      str2 += str[i];
      i++;
    }
    while(str[i] != ' ') {
      str4 += str[i];
      i++;
    }
    c.key_name[c.num] = str1;
    c.key_type[c.num] = str2;
    c.count[c.num] = strtonum(str4);
    c.num++;
    str2 = "";
    str1 = "";
    str4 = "";
  }
  input.close();
  return c;
}
