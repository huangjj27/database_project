#include"serial.h"
#define PAGE_SIZE 8192

int strtonum(string s);

string numtostr(int n);

union intandchar {
  // a basic unit of data. can be either of the list.
  int a;      //  a integer of 32 bits
  char b[4];  //  or 4 bytes of characters
};

int strtonum(string s) {
  int num;
  stringstream ss(s);
  ss >> num;
  return num;
}

string numtostr(int n) {
  string str;
  stringstream ss;
  ss << n;
  ss >> str;
  return str;
}

void get_bin(string file_name) {
  struct catalog c;
  string str = "";
  string str1 = "";
  string str2 = "";
  string str3 = "";
  string strf = "";
  c.num = 0;

  ifstream infile;
  infile.open(file_name, ios::in);

  string data = "";

  c = read_catalog();
  //freopen(file_name, "r", stdin);
  //freopen("data2.json", "r", stdin);
  FILE* out = fopen("create.data", "wb+");
  string tmp = "";
  while (getline(infile, str)) {
    if (str == "[") continue;     // sign of file beginning
    if (str == "]") break;        // sign of file end
    int sum = 0;
    int count = 0;
    int aid[50];
    int off[50];
    string str3[50];
    str3[0] = "";
    int i = 2;                  // ignore '{' and '\“' at the line beginning
    int len = str.size();
    while (i < len - 1) {
      int l = 0;                // offset of key_name
      while (str[i] != '"') {
        str1 += str[i];
        i++;
      }
      i += 3;                   // ignore the string "\": "
      if (str[i] == '{') {      // offset of key_type
        str2 = "json";
        while (str[i] != '}') {
          str3[count] += str[i];
          i++;
          l++;
        }
        str3[count] += '}';
        l++;
      } else if (str[i] == '[') {
        str2 = "array";
        while (str[i] != ']') {
          str3[count] += str[i];
          i++;
          l++;
        }
        str3[count] += ']';
        l++;
      } else if (str[i] == '"') {
        str2 = "text";
        i++;
        while (str[i] != '"') {
          str3[count] += str[i];
          i++;
          l++;
        }
      } else if (str[i] == 'f' || str[i] == 't') {
        str2 = "bool";
        while (str[i] != ',' && str[i] != '}') {
          str3[count] += str[i];
          i++;
          l++;
        }
      } else {
        str2 = "int";
        while (str[i] != ',' && str[i] != '}') {
          str3[count] += str[i];
          i++;
          l++;
        }
      }
      while (str[i] != ',' && i < len - 1) i++;
      if (i != len - 1) i += 3;
      off[count] = l;
      for (int j = 0; j < c.num; j++) {
        if (str1 == c.key_name[j] && str2 == c.key_type[j]) {
          aid[count] = j + 1;
          break;
        }
      }
      str2 = "";
      str1 = "";
      count++;
      str3[count] = "";
    }

    int ss = 0;
    intandchar int_;
    int_.a = count;

    tmp += int_.b[0];
    tmp += int_.b[1];
    tmp += int_.b[2];
    tmp += int_.b[3];
    for (int j = 0; j < count; j++) {
      int_.a = aid[j];

      tmp += int_.b[0];
      tmp += int_.b[1];
      tmp += int_.b[2];
      tmp += int_.b[3];
    }
    for (int j = 0; j < count; j++) {
      int_.a = ss;
      tmp += int_.b[0];
      tmp += int_.b[1];
      tmp += int_.b[2];
      tmp += int_.b[3];
      ss += off[j];
      sum += off[j];
    }
    int_.a = sum;
    tmp += int_.b[0];
    tmp += int_.b[1];
    tmp += int_.b[2];
    tmp += int_.b[3];
    for (int j = 0; j < count; j++) {
      tmp += str3[j];
    }
    // when we write binary data, we don't need to seperate different record.
    // tmp += "\n";

    if (tmp.size() / PAGE_SIZE) {
      fwrite(tmp.substr(0, PAGE_SIZE).data(), PAGE_SIZE, 1, out);
      tmp = tmp.substr(PAGE_SIZE, tmp.size() % PAGE_SIZE);
    }
  }
  fwrite(tmp.data(), 1, tmp.length(), out);


  fclose(out);
  out = NULL;
  infile.close();
  //fclose(stdin);
}


string sertojson(serial ser, catalog* scanlog1) {
  string ans = "", flag[5] = { "json", "array", "text", "int", "bool" };
  ans += "{";

  for (int i = 0; i < ser.count; i++) {
    catalog* scanlog = scanlog1;
    int id = ser.aid[i];

    ans += "\"";
    ans += scanlog->key_name[id];
    ans += "\"";
    ans += ": ";
    if ((scanlog->key_type[id]) == flag[0]) {

      for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
        ans += ser.data[j];
      }

    }
    if ((scanlog->key_type[id]) == flag[1]) {

      for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
        ans += ser.data[j];
      }

    }

    if ((scanlog->key_type[id]) == flag[2]) {
      ans += "\"";
      for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
        ans += ser.data[j];
      }
      ans += "\"";
    }

    if ((scanlog->key_type[id]) == flag[3]) {
      for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
        ans += ser.data[j];
      }
    }
    if ((scanlog->key_type[id]) == flag[4]) {
      for (int j = ser.offset[i]; j < ser.offset[i] + ser.len[i]; j++) {
        ans += ser.data[j];  // bool, true of false, add to ans as a string
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
  string type = "", flag[5] = { "json", "array", "text", "int", "bool" };
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
  value_ = value;

  int aid;
  for (int j = 1; j <= catalog->num; j++) {
    if (key == catalog->key_name[j - 1] && type == catalog->key_type[j - 1]) {
      aid = j;
      break;
    }
  }
  if (aid == 0) {
    cout << "NONE\n";
    return;
  }

  bool is_find = false;
  char read_buffer[PAGE_SIZE];
  int buffer_size = 0;
  string str = "", str1 = "";
  FILE* input = fopen("create.data", "rb+");
  if (input == NULL) {
    cout << "no create.data\n";
    return;
  }


  int count = catalog->count[aid - 1];

  bool is_find_key_in_this_one;

  serial ser;
  int found_id_[50] = { 0 };
  int found_num = 0;
  int i = 0;
  while (1) {
    //if (str.size()%(PAGE_SIZE/32)) is_read_more_data = false;
    ser.data = "";
    if (i == buffer_size) {
      if ((buffer_size = fread(read_buffer, 1, PAGE_SIZE, input)) == 0) break;
      i = 0;
    }
    if (count <= 0) break;
    is_find_key_in_this_one = false;
    found_num = 0;
    //get ser count (attributes number)

    for (int j = 0; j < 4; j++) {
      if (i >= buffer_size) {
        buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
        i = 0;
      }
      int_.b[j] = read_buffer[i++];
    }
    ser.count = int_.a;
    // get ser aid
    for (int j_ = 0; j_ < ser.count; j_++) {
      for (int j = 0; j < 4; j++) {
        if (i >= buffer_size) {
          buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
          i = 0;
        }
        int_.b[j] = read_buffer[i++];
      }
      ser.aid[j_] = int_.a;
    }
    // judge whether the keyA is in this json
    for (int j = 0; j < ser.count; j++) {
      if (aid == ser.aid[j]) {
        is_find_key_in_this_one = true;
        found_id_[found_num++] = j;
        count--;
      }
    }
    if (is_find_key_in_this_one) {
      for (int j_ = 0; j_ < ser.count; j_++) {
        for (int j = 0; j < 4; j++) {
          if (i >= buffer_size) {
            buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
            i = 0;
          }
          int_.b[j] = read_buffer[i++];
        }
        ser.offset[j_] = int_.a;
      }
      // get ser sum (the length of data)
      for (int j = 0; j < 4; j++) {
        if (i >= buffer_size) {
          buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
          i = 0;
        }
        int_.b[j] = read_buffer[i++];
      }
      ser.sum = int_.a;
      // get ser len
      for (int j = 0; j <= ser.count; j++) {
        ser.len[j] = ser.offset[j + 1] - ser.offset[j];
      }
      // get ser data
      while (ser.data.length() < ser.sum) {
        if (i >= buffer_size) {
          buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
          i = 0;
        }
        ser.data += read_buffer[i++];
      }
      // judge whethe the valueB and the found_value from this json is equals
      for (int j = 0; j < found_num; j++) {
        string found_value = ser.data.substr(ser.offset[found_id_[j]], ser.len[found_id_[j]]);
        if (value_ == found_value) {
          cout << sertojson(ser, catalog) << endl;
          is_find = true;
          break;
        }
      }
    } else {
      i += 4 * ser.count;
      if (i >= buffer_size) {
        buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
        i -= PAGE_SIZE;
      }

      for (int j = 0; j < 4; j++) {
        if (i >= buffer_size) {
          buffer_size = fread(read_buffer, 1, PAGE_SIZE, input);
          i = 0;
        }
        int_.b[j] = read_buffer[i++];
      }
      ser.sum = int_.a;
      i += ser.sum;
      if (i >= buffer_size) {
        if ((buffer_size = fread(read_buffer, 1, PAGE_SIZE, input)) == 0) {
          break;
        }
        i -= PAGE_SIZE;
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
  getline(input, str);
  while (getline(input, str)) {
    int i = 0;
    while (str[i] != '\t') i++;
    i++;
    while (str[i] != '\t') {
      str1 += str[i];
      i++;
    }
    i++;
    while (str[i] != '\t') {
      str2 += str[i];
      i++;
    }
    i++;
    while (str[i] != '\t') {
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
