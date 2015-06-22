#include"serial.h"

int strtonum(string s);

int strtonum(string s) {
	int num;
	stringstream ss(s);
	ss>>num;
	return num;
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

	freopen("data2.json","r",stdin);
	//getline(cin,str);
	ofstream out("create.data");
	while(getline(cin,str)) {
		if (str == "[") continue;			//文件开头标志
		if (str == "]") break;				//文件结束标志
		int sum = 0;
		int count = 0;
		int aid[50];
		int off[50];
		string str3[50];
		/*for (int i = 0; i < 50; i++) {
			aid[i] = 0;
		}*/
		str3[0] = "";
		int i = 2;					//忽略开头的{“
		int len = str.size();
		while(i < len - 1) {
			int l = 0;				//key_name的偏移亮
			while(str[i] != '"') {
				str1 += str[i];
				i++;
			}
			i += 3;					//忽略”：和空格
			if (str[i] == '{') {			//key_type的偏移量
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
                                //if (str3[count] == "true")str3[count] = "t";
                                //else str3[count] = "f";
			} else {
				str2 = "int";
				while(str[i] != ',' && str[i] != '}') {
					str3[count] += str[i];
					i++;
					l++;
				}
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
		/*for (int j = 1; j < count; j++) {
			for (int k = 0; k < count - j; k++) {
				if (aid[k] > aid[k + 1]) {
					int tmp = aid[k];
					aid[k] = aid[k+1];
					aid[k+1] = tmp;
					tmp = off[k];
					off[k] = off[k+1];
					off[k+1] = tmp;
					string temp = str3[k];
					str3[k] = str3[k+1];
					str3[k+1] = temp;
				}
			}
		}*/
		int ss = 0;
		out << count << ' ';
		for (int j = 0; j < count; j++) {
			out << aid[j] << ' ';
		}
		for (int j = 0; j < count; j++) {
			out << ss << ' ';
			ss += off[j];
			sum += off[j];
		}
		out << sum << ' ';
		for (int j = 0; j < count; j++) {
			out << str3[j];
		}
		out << endl;
	}
	out.close();
	fclose(stdin);
}
		

string sertojson(serial ser, catalog* scanlog1) {
	string ans = "", flag[5] = {"json", "array", "text", "int", "bool"};
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
	
	int aid = 0;
	for (int i = 0; i < catalog->num; i++) {
		if (key == catalog->key_name[i]) {
                    aid = i;
                    break;
                }
        }
//cout << "wuchunshishabi"<<endl;
	if (aid == 0) {
		cout << "NONE\n";
		return;
	}
 
	int count = catalog->count[aid];

	bool is_find_key_in_this_one;
	bool is_find = false;
	string str, str1 = "";
	ifstream input;
	serial ser;
	int found_id_[50] = {0};
	int found_num = 0;
	input.open("create.data");

	while(getline(input,str)) {
		if (count <= 0) break;
		is_find_key_in_this_one = false;
		found_num = 0;
		int i = 0;
		while(1) {
			if (str[i] != ' ') {
				str1 += str[i++];
			} else {
				break;
			}
		}
		ser.count = strtonum(str1);
	    i++;
		for (int j = 0; j < ser.count; j++) {
			while(1) {
			    if (str[i] != ' ') {
				    str1 += str[i++];
			    } else {
				    break;
			    }
		    }
			ser.aid[j] = strtonum(str1);
	        i++;
		}
		for (int j = 0; j < ser.count; j++) {
			if (aid == ser.aid[j]) {
				is_find_key_in_this_one = true;
				found_id_[found_num++] = j;
				count--;
			}
		}
		if (is_find_key_in_this_one) {
			for (int j = 0; j < ser.count; j++) {
			    while(1) {
			        if (str[i] != ' ') {
				       str1 += str[i++];
			        } else {
				       break;
			        }
		        }
				ser.offset[j] = strtonum(str1);
	            i++;
		    }
			while(1) {
			    if (str[i] != ' ') {
				    str1 += str[i++];
			    } else {
				    break;
			    }
		    }
			for (int j = 0; j <= ser.count; j++) {
				ser.len[j] = ser.offset[j+1]-ser.offset[j];
			}
			ser.sum = strtonum(str1);
	        i++;
			for (unsigned int j = i; j < str.length(); j++) {
				ser.data += str[j];
			}
			for (int j = 0; j < found_num; j++) {
				string check_value = ser.data.substr(ser.offset[found_id_[j]], ser.len[found_id_[j]]);
				if (value == check_value) {
					cout << sertojson(ser, catalog) << endl;
					is_find = true;
					break;
				}
			}
		}
	}
	if (!is_find) {
		cout << "NONE\n";
	}
	input.close();
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
	getline(input,str);
	while(getline(input,str)) {
		int i = 0;
		while(str[i] != '\t') i++;
		i++;
		while(str[i] != '\t') {
			str1 += str[i];
			i++;
		}
		i++;
		while(str[i] != '\t') {
			str2 += str[i];
			i++;
		}
		i++;
		while(str[i] != '\t') {
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
