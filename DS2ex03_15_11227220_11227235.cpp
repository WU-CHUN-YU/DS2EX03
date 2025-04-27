//資訊二乙 11227220張季平、11227235吳君友
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <cmath>
#include <string>
struct DataType {
  std::array<char, 10> sid;
  std::array<char, 10> sname;
  unsigned char score[6];
  float average_score;
};

struct HashContent {
  int hash_value;
  std::array<char, 10> sid;
  std::array<char, 10> sname;
  float avg_score;  
  HashContent() { // 初始化內容，確保hash table創建時是空的
    hash_value = 0;
    sid.fill('\0');
    sname.fill('\0');
    avg_score = 0;
  }
};

class LinearHash {
 private:
  int hash_table_size;
  int load;
  int avg_success;
  int avg_unsuccess;
 public:
  LinearHash(){
    int hash_table_size = 0;
    int load = 0;
    int avg_success = 0;
    int avg_unsuccess = 0;
  }

  bool IsPrime(int num) {
    // 排除掉較好判斷的情況
    if (num <= 1) { 
      return false; 
    } else if (num == 2) {
      return true;  
    } else if (num % 2 == 0) {
      return false; 
    } 

    // 檢查num能不能被整除
    for (int i = 3; i <= std::sqrt(num); i += 2) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
  }

  int CalcHashSize(int dataset_size) {
    int cur = dataset_size * 1.1 + 1; //start value
    if(cur % 2 == 0) { // 確保當前值是奇數
      cur += 1;        // 確保當前值是奇數
    }
    
    while(true) {
      if (IsPrime(cur)) {
        break;
      } // else
      cur += 2;
    }

    return cur;
    // 運算avg search要用的值
    hash_table_size = cur;
    load = hash_table_size / dataset_size;

    avg_success   = (1 + 1/(1-load))/2;
    avg_unsuccess = (1 + 1/((1-load)*(1-load)))/2;
  }

  // processing
  int CalcHashValue(std::array<char, 10> sid) {
    int hash_value = 1; // 隱測數字可能會很大，優化時把int改成long int

    int i = 0;
    for(int i = 0; i < sid.size(); i += 1) {
      // hash key
      hash_value *= sid[i];
      hash_value %= hash_table_size; 
    }
    return hash_value;
  }

  bool StructIsMpt(HashContent stru){
    if (stru.sid['\0'] == 0) {
      return true;
    } // else
    return false;
  }

  void StoreHash(struct DataType cur, struct HashContent hash_table[]) {
    struct HashContent temp;
    temp.hash_value = CalcHashValue(cur.sid);
    temp.sid = cur.sid;
    temp.sname = cur.sname;
    temp.avg_score = cur.average_score;

    int insert_pos = temp.hash_value;
    // 如果發生碰撞，找下一個位置直到找到空位為止
    while(StructIsMpt(hash_table[insert_pos])) {
      insert_pos += 1;
      // 如果索引值超出陣列大小，回到陣列開頭
      if (insert_pos > (sizeof(hash_table) - 1)) {
        insert_pos = 0;
      }
    }

    // insert temp到hash_table裡
    hash_table[insert_pos] = temp;
  }

  void WriteFile(std::string file_name) {
    // 將hash table寫入 linear___.txt
  }
  // output
  void Output() {
    std::cout << "Hash table has been successfully created by Linear probing" << std::endl
              << "unsuccessful search: " << avg_success << "comparisons on average" << std::endl
              << "successful search: " << avg_unsuccess  << "comparisons on average" << std::endl;
  } 
};

class DoubleHash {
  // 找最小質數(上網找演算法)
};

class ProgramPackage {
 private:
  std::vector<DataType> dataset;
  LinearHash linear_hash;
 public:
 std::string SplitString(std::string &line) {  //  切割檔案字串，取得欄目數據
  std::string splited;
  int end = line.find("\t");
  splited = line.substr(0, end);
  line = line.substr(end + 1, line.size());
  return splited;
}
  DataType ProcessData(std::string line) {
    int column = 0;
    DataType data;
    while (column < 9) {
      if (column == 0) {
        data.sid = TransToChar(SplitString(line));
      } else if (column == 1) {
        data.sname = TransToChar(SplitString(line));
      } else if (column == 8) {
        std::string s = SplitString(line);
        if (s == "") {
          s = "0";
        }
        data.average_score = std::stof(s);
      } else {
        std::string s = SplitString(line);
        if (s == "") {
          s = "0";
        }
        data.score[column - 2] = std::stoi(s);
      }
      column++;
    }
    return data;
  }
  bool ReadBinFile() {
    std::ifstream file;
    std::string file_name, bin_file, file_line;
    std::cout << "Input a file number ([0] Quit): ";
    std::cin >> file_name;
    if (file_name == "0") {
      std::cout << std::endl;
      return false;
    }
    bin_file = "input" + file_name + ".bin";
    file.open(bin_file, std::ios::in | std::ios::binary);
    if (!file.is_open()) { //確認檔案是否存在
      std::cout << std::endl << "### " << bin_file << " does not exist!" << " ###" << std::endl << std::endl;
      if (!ReadTXTFile(file_name)) {
        return false;
      } else {
        file.open(bin_file, std::ios::in | std::ios::binary);
      }
    }
    //真正讀檔部分
    DataType data;
    while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
      dataset.push_back(data);
    }
    file.close();
    return true;
  }
  bool ReadTXTFile(std::string file_name) {  //讀取TXT檔案
    std::ifstream file;
    std::string txt_file, file_line;
    std::vector<DataType> temp_dataset;
    DataType data;
    txt_file = "input" + file_name + ".txt";
    file.open(txt_file);
    if (!file.is_open()) { //確認檔案是否存在
      std::cout << "### " << txt_file << " does not exist!" << " ###" << std::endl << std::endl << std::endl;
      return false;
    }
    //開始建立bin檔
    while (getline(file, file_line)) {
      data = ProcessData(file_line);
      temp_dataset.push_back(data);
    }
    file.close();
    WriteBinFile(temp_dataset, file_name);
    return true;
  }
  void WriteBinFile(std::vector<DataType> temp, std::string file_name) {
    std::ofstream file;
    std::string bin_file = "input" + file_name + ".bin";
    file.open(bin_file, std::ios::out | std::ios::binary);
    for (DataType data : temp) {
      file.write(reinterpret_cast<char*>(&data), sizeof(data));
    }
    file.close();
    std::cout << "---" + bin_file + "has been created ---" << std::endl << std::endl;
    return;
  }
  
  std::array<char, 10> TransToChar(std::string str) {
    std::array<char, 10> arr;
    str.resize(10, '\0');
    for(int i = 0; i < str.length(); i++) {
      arr[i] = str[i];
    }
    return arr;
  }

  void BuildHashByLinear () {
    struct HashContent hash_table[linear_hash.CalcHashSize(dataset.size())];
    for(int i = 0; i < dataset.size(); i += 1) {
      linear_hash.StoreHash(dataset[i], hash_table);
    }
    linear_hash.Output();
  }

  void BuildHashByDouble () {
    
  }
};

// 讀檔邏輯
// 載入ReadBinFile()，發現有bin檔，繼續讀檔，沒有bin檔，建立bin檔後再繼續讀檔
class System {
 private:
  ProgramPackage program_package;
 public:
  void PrintUI() {
    std::cout << "******* Hash Table *****" << std::endl
              << "* 0. QUIT              *" << std::endl 
              << "* 1. Linear probing    *" << std::endl
              << "* 2. Double hashing    *" << std::endl
              << "************************" << std::endl;
    return;
  }
  int ReadCommand() {  //讀取執行指令
    int command;
    PrintUI();
    std::cout << "Input a choice(0, 1, 2): ";
    std::cin >> command;
    if (command == 1 || command == 2) {
      CallProgram(command);
    } else if (command != 0) {
      std::cout << std::endl << "Command does not exist!" << std::endl << std::endl << std::endl;
    }
    return command;
  }

  void CallProgram(int command) {  //執行指定程式
    std::cout << std::endl;
    program_package.ReadBinFile();
    if (command == 1) {
    } else if (command == 2) {
    }
    return;
  }
};

int main() {
  System system;
  while (true) {
    if (system.ReadCommand() == 0) {
      break;
    }
  }
  return 0;
}