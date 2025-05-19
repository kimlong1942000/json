#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class JsonX {
private:
    std::string jsonStr;

public:
    // Constructor để khởi tạo từ chuỗi JSON
    JsonX(const std::string& jsonString) : jsonStr(jsonString) {}

    // Phương thức lấy giá trị của key
    std::string operator[](const std::string& key) {
        return getValueForKey(jsonStr, key);
    }

private:
    // Hàm tách và lấy giá trị dựa trên key
    std::string getValueForKey(const std::string& jsonStr, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t keyPos = jsonStr.find(searchKey);
        if (keyPos == std::string::npos) {
            return ""; // Không tìm thấy key
        }
        size_t colonPos = jsonStr.find(":", keyPos);
        if (colonPos == std::string::npos) {
            return "";
        }
        size_t valueStart = colonPos + 1;

        while (valueStart < jsonStr.size() && isspace(jsonStr[valueStart])) {
            valueStart++;
        }

        if (jsonStr[valueStart] == '\"') {
            size_t valueEnd = jsonStr.find('\"', valueStart + 1);
            if (valueEnd == std::string::npos) return "";
            return jsonStr.substr(valueStart + 1, valueEnd - valueStart - 1);
        } else {
            size_t valueEnd = valueStart;
            while (valueEnd < jsonStr.size() &&
                jsonStr[valueEnd] != ',' && jsonStr[valueEnd] != '}' && !isspace(jsonStr[valueEnd])) {
                valueEnd++;
            }
            std::string val = jsonStr.substr(valueStart, valueEnd - valueStart);
            val.erase(std::remove_if(val.begin(), val.end(), ::isspace), val.end());
            return val;
        }
    }
};

int main() {
    // Đọc dữ liệu JSON từ file
    std::ifstream file("data.txt");
    if (!file.is_open()) {
        std::cerr << "Không thể mở file." << std::endl;
        return 1;
    }
    std::string jsonStr((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    jsonStr.erase(std::remove_if(jsonStr.begin(), jsonStr.end(), ::isspace), jsonStr.end());

    JsonX json(jsonStr);
    std::string name = json["name"];
    std::cout << "name: " << name << std::endl;
    std::string age = json["age"];
    std::cout << "age: " << age << std::endl;
    std::string addressStr = json["address"]; 


    return 0;
}