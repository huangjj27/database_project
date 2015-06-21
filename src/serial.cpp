#include"serial.h"
#define PAGE_SIZE 8192
int strtonum(string s);
string numtostr(int n);

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
  getline(cin,str);
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
                                if (str3[count] == "true")str3[count] = "t";
                                else str3[count] = "f";
      } else {
        str2 = "int";
        string t = "";
        while(str[i] != ',' && str[i] != '}') {
          t += str[i];
          //str3[count] += str[i];
          i++;
          l++;
        }
        str3[count] += (char)strtonum(t);
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
    tmp += (char)count;
    //tmp += ' ';
    for (int j = 0; j < count; j++) {
      //out << aid[j] << ' ';
      tmp += (char)aid[j];
      //tmp += ' ';
    }
    for (int j = 0; j < count; j++) {
      //out << ss << ' ';
      tmp += (char)ss;
      //tmp += ' ';
      ss += off[j];
      sum += off[j];
    }
    //out << sum << ' ';
    tmp += (char)sum;
    //tmp += ' ';
    for (int j = 0; j < count; j++) {
      //for (unsigned int i = 0; i < str3[j].length(); i++) out << str3[j][i];
      //out << str3[j] << ' ';
      tmp += str3[j];
      //tmp += ' ';
    }
    //out << endl;
    tmp += "\n";

    if (tmp.size()/(PAGE_SIZE/32)) {
      fwrite(tmp.substr(0, (PAGE_SIZE/32)).data(), 32, PAGE_SIZE/32, out);
      tmp = tmp.substr((PAGE_SIZE/32), tmp.size()%(PAGE_SIZE/32));
    }
  }
  fwrite(tmp.data(), 32, tmp.length(), out);
  fclose(out);
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
          for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
              //ans += ser.data[j];
          int tmp = ser.data[j];
          ans += numtostr(tmp);
          }
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

serial jsontoser(string json, catalog* scanlog1) {
  int i = 2;

  serial ser;
  string str[50];
    string data = "";

  string key_name = "", key_type = "", flag[5] = {"json", "array", "text", "int", "bool"};
  str[0] = "";
  ser.count = 0;

  while(i < json.size()) {
    while (json[i] != '"') {
      key_name += json[i++];
    }

    i += 2;
    if (json[i] == '"') {
      i++;
      while (json[i] != '"') str[ser.count] += json[i++];
      key_type = flag[2];
    } else if (json[i] == '[') {
      while (json[i] != ']') str[ser.count] += json[i++];
      str[ser.count] += ']';
      key_type = flag[1];
    } else if (json[i] == '{') {
      while (json[i] != '}') str[ser.count] += json[i++];
      str[ser.count] += '}';
      key_type = flag[0];
    } else if (json[i] == 't' || json[i] == 'f') {
      while (json[i] != ',' && json[i] != '}') str[ser.count] += json[i++];
      key_type = flag[4];
    } else {
      string tmp = "";
      while (json[i] != ',') {
        tmp += json[i++];
      }
      str[ser.count] += (char)strtonum(tmp);
      key_type = flag[3];
    }

    for (int j = 0; j < scanlog1->num; j++) {
      if (key_name == scanlog1->key_name[j] && key_type == scanlog1->key_type[j]) {
        ser.aid[ser.count] = j + 1;
      }
    }
    ser.count++;
    str[ser.count] = "";
    key_name = "";
    key_type = "";

    while (json[i] != '"') {
      i++;
      if (i >= json.size()) break;
    }
  }

  for (int j = 1; j < ser.count; j++) {
    for (int k = 0; k < ser.count - j; k++) {
      if (ser.aid[k] > ser.aid[k + 1]) {
        int tmp = ser.aid[k];
        ser.aid[k] = ser.aid[k+1];
        ser.aid[k+1] = tmp;
        string temp = str[k];
        str[k] = str[k+1];
        str[k+1] = temp;
      }
    }
  }

  for (int j = 0; j < ser.count; j++) {
    ser.offset[j] = ser.data.size();
    ser.data += str[j];
    ser.len[j] = ser.data.size() - ser.offset[j];
  }
  ser.sum = ser.data.size();

  return ser;
}

void find_A_equals_B(string key, string value, catalog* catalog) {

  if (catalog->num == 0) {
    cout << "NONE\n";
    return;
  }

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

  int aid[100000];
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
  char* read_buffer;
  string str = "", str1 = "";
  FILE* input = fopen("create.data", "wb+");
  for (int i_ = 0; i_ < id_num; i_++) {
  int count = catalog->count[aid[i_]];

  bool is_find_key_in_this_one;

  serial ser;
  int found_id_[50] = {0};
  int found_num = 0;
  int i = 0;
  while(1) {
    //if (str.size()%(PAGE_SIZE/32)) is_read_more_data = false;
    if (str.size() == 0) {
      if (fread(read_buffer, 32, PAGE_SIZE/32, input) == 0) break;
      str += read_buffer;
      i = 0;
    }
    if (count <= 0) break;
    is_find_key_in_this_one = false;
    found_num = 0;
    //get ser count (attributes number)
      if (i > str.size()) {
      fread(read_buffer, 32, PAGE_SIZE/32, input);
      str = "";
      str += read_buffer;
      i = 0;
    }
    ser.count = str[i++];
    //get ser aid
    for (int j = 0; j < ser.count; j++) {
      if (i > str.size()) {
        fread(read_buffer, 32, PAGE_SIZE/32, input);
        str = "";
        str += read_buffer;
        i = 0;
      }
      ser.aid[j] = str[i++];
    }
    //judge whether the keyA is in this json
    for (int j = 0; j < ser.count; j++) {
      if (aid[i_] == ser.aid[j]) {
        is_find_key_in_this_one = true;
        found_id_[found_num++] = j;
        count--;
      }
    }
    if (is_find_key_in_this_one) {
      for (int j = 0; j < ser.count; j++) {
          for (int j = 0; j < ser.count; j++) {
              if (i > str.size()) {
                fread(read_buffer, 32, PAGE_SIZE/32, input);
                str = "";
                str += read_buffer;
                i = 0;
              }
          ser.offset[j] = str[i++];
            }
        }
      //get ser sum (the length of data)
      if (i > str.size()) {
          fread(read_buffer, 32, PAGE_SIZE/32, input);
          str = "";
          str += read_buffer;
          i = 0;
        }
        ser.sum = str[i++];
      //get ser len
      for (int j = 0; j <= ser.count; j++) {
        ser.len[j] = ser.offset[j+1]-ser.offset[j];
      }
      //get ser data
      while (ser.data.length() < ser.sum) {
        if (i > str.size()) {
              fread(read_buffer, 32, PAGE_SIZE/32, input);
              str = "";
              str += read_buffer;
              i = 0;
            }
        ser.data += str[i++];
      }
      //judge whethe the valueB and the found_value from this json is equals
      for (int j = 0; j < found_num; j++) {
        string found_value = ser.data.substr(ser.offset[found_id_[j]], ser.len[found_id_[j]]);
        if (value == found_value) {
          cout << sertojson(ser, catalog) << endl;
          is_find = true;
          break;
        }
      }
    }
  }
  }
  if (!is_find) {
    cout << "NONE\n";
  }
  fclose(input);
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
