//資訊二乙 11227220張季平、11227235吳君友
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <cmath>
#include <string>
#include <iomanip> // 控制輸出至小數點後第幾位

struct DataType {
  std::array<char, 10> sid{};
  std::array<char, 10> sname{};
  unsigned char score[6];
  float average_score;
};

struct HashContent {
  int hash_value = 0;
  std::array<char, 10> sid{};
  std::array<char, 10> sname{};
  float avg_score = 0;
};

class Hash { // 放linear probing和double共用的函式
 protected: // 只有自身和子類別能存取
  int hash_table_size;
  int hash_content_count;

  int prob_count;
  float avg_success;
  float avg_unsuccess;
 public:
  Hash() {
    hash_table_size = 0;
    hash_content_count = 0;

    prob_count = 0;
    avg_success = 0;
    avg_unsuccess = 0;
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

    hash_content_count = dataset_size;
    hash_table_size = cur;

    return cur;
  }

  // processing
  int CalcHashValue(std::array<char, 10> sid) {
    int hash_value = 1; // 隱測數字可能會很大，優化時把int改成long int

    int i = 0;
    while (true) {
      if (i == sid.size()-1 || sid[i] == '\0') {
        break;
      }
      hash_value *= sid[i];
      hash_value %= hash_table_size;

      i += 1;
    }
    return hash_value;
  }
  // 找哈希表的空格時呼叫此函式
  bool StructIsMpt(HashContent stru){
    if (stru.sid[0] == '\0') {
      return true;
    } // else
    return false;
  }

  void WriteFile(std::string file_name, std::vector<HashContent>& hash_table) {
    // 將hash table寫入 linear___.txt
    std::string output_file_name = "linear" + file_name + ".txt";
    std::ofstream file;
    file.open(output_file_name);
    int index = 1;
    for (HashContent data : hash_table) {
      file << "[" << std::setw(2) << index << "]";
      // file << setW(11) << 
      index++;
    }
  }
};

class LinearHash : public Hash {
 private:
  
 public:
  void StoreHash(DataType cur, std::vector<HashContent>& hash_table) {
    HashContent temp;
    temp.hash_value = CalcHashValue(cur.sid);
    temp.sid = cur.sid;
    temp.sname = cur.sname;
    temp.avg_score = cur.average_score;

    int insert_pos = temp.hash_value;
    // 如果發生碰撞，找下一個位置直到找到空位為止
    while(StructIsMpt(hash_table[insert_pos]) == false) {
      insert_pos += 1;
      // 如果索引值超出陣列大小，回到陣列開頭
      if (insert_pos > (hash_table_size - 1)) {
        insert_pos = 0;
      }
      avg_success += 1;
    }
    avg_success += 1;

    // insert temp到hash_table裡
    hash_table[insert_pos] = temp;
  }

  void CalcAvgUnsuccess(std::vector<HashContent>& hash_table) {

    // 設定基本大小
    avg_unsuccess = 0;

    // 找出第一個空格(起始位置)
    int index = 0;
    for (index = 0; index < hash_table_size; index += 1) {
      if (StructIsMpt(hash_table[index])) {
        break;
      }
    }
    int start_point = index;
    index += 1;
    // 找區塊
    int block_size = 0;
    while(true) {
      // 超出陣列大小，index重設到0(開頭)
      if (index > (hash_table_size - 1)) {
        index = 0;
      }
      if (StructIsMpt(hash_table[index])) { // 找到空白，開始計算區塊大小
        if (block_size != 0) {
          avg_unsuccess += (block_size/2) * (block_size + 1); // 頭尾相加，例如把4+3+2+1變成(4+1) + (3+2)
          if (block_size %2 != 0) {
            avg_unsuccess += (block_size + 1) / 2; // 如果是奇數的話，要額外加沒加到的中間數
          }
          block_size = 0;
        }
      } else {
        block_size += 1;
      }

      // 如果index再次回到start_point(可能不為零)，代表走完一圈，遍歷完了。
      if (index == start_point) {
        break;
      }
      index += 1;
    }
    
    avg_unsuccess = avg_unsuccess / hash_table_size;
  }
  
  void CalcAvgSuccess() {
    avg_success = avg_success / hash_content_count;
  }
  
  void Output() {
    std::cout << std :: fixed << std :: setprecision(4)
              << "Hash table has been successfully created by Linear probing" << std::endl
              << "unsuccessful search: " << avg_unsuccess << " comparisons on average" << std::endl
              << "successful search: " << avg_success << " comparisons on average" << std::endl;
  } 
};

class DoubleHash : public Hash {
  
};

class ProgramPackage {
 private:
  std::vector<DataType> dataset;
  LinearHash linear_hash;
  std::string file_name;
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
    std::string bin_file, file_line;
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
      if (!ReadTXTFile()) {
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
  bool ReadTXTFile() {  //讀取TXT檔案
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
    WriteBinFile(temp_dataset);
    return true;
  }
  void WriteBinFile(std::vector<DataType> temp) {
    std::ofstream file;
    std::string bin_file = "input" + file_name + ".bin";
    file.open(bin_file, std::ios::out | std::ios::binary);
    for (DataType data : temp) {
      file.write(reinterpret_cast<char*>(&data), sizeof(data));
    }
    file.close();
    std::cout << "---" + bin_file + " has been created ---" << std::endl << std::endl;
    return;
  }
  std::array<char, 10> TransToChar(std::string str) {
    std::array<char, 10> arr{};
    for(int i = 0; i < str.length(); i++) {
      arr[i] = str[i];
    }
    return arr;
  }

  void BuildHashByLinear () {
    std::vector<HashContent> hash_table;
    hash_table.resize(linear_hash.CalcHashSize(dataset.size()));

    for(int i = 0; i < dataset.size(); i += 1) {
      linear_hash.StoreHash(dataset[i], hash_table);
    }
    linear_hash.WriteFile(file_name, hash_table);
    linear_hash.CalcAvgUnsuccess(hash_table);
    linear_hash.CalcAvgSuccess();
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
    if (command == 0 || command == 1 || command == 2) {
      CallProgram(command);
    } else {
      std::cout << std::endl << "Command does not exist!" << std::endl << std::endl << std::endl;
    }
    return command;
  }

  void CallProgram(int command) {  //執行指定程式
    if (command == 0) {
      return;
    }
    std::cout << std::endl;
    program_package.ReadBinFile();
    if (command == 1) {
      program_package.BuildHashByLinear();
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