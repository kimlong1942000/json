#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> 
std::string getValueForKey(const std::string& jsonStr, const std::string& key) {
    std::string searchKey = "\"" + key + "\"";
    size_t keyPos = jsonStr.find(searchKey);
    if (keyPos == std::string::npos) {
        return ""; // Không tìm thấy key
    }
    // Tìm dấu ':' sau key
    size_t colonPos = jsonStr.find(":", keyPos);
    if (colonPos == std::string::npos) {
        return "";
    }
    // Lấy phần sau dấu ':'
    size_t valueStart = colonPos + 1;

    // Cố gắng xác định kiểu dữ liệu
    // Nếu value bắt đầu bằng " thì là chuỗi
    // Nếu bắt đầu bằng số hoặc false/true thì là kiểu khác

    while (valueStart < jsonStr.size() && isspace(jsonStr[valueStart])) {
        valueStart++;
    }

    if (jsonStr[valueStart] == '\"') {
        // Chuỗi
        size_t valueEnd = jsonStr.find('\"', valueStart + 1);
        if (valueEnd == std::string::npos) return "";
        return jsonStr.substr(valueStart + 1, valueEnd - valueStart - 1);
    } else {
        // Có thể là số hoặc true/false hoặc null
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

int main() {
    std::ifstream file("data.txt");
    if (!file.is_open()) {
        std::cerr << "Không thể mở file." << std::endl;
        return 1;
    }

    std::string jsonStr((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    jsonStr.erase(std::remove_if(jsonStr.begin(), jsonStr.end(), ::isspace), jsonStr.end());

    std::string name = getValueForKey(jsonStr, "name");
    std::cout << "name: " << name << std::endl;

    std::string isStudentStr = getValueForKey(jsonStr, "is_student");
    std::cout << "is_student: " << isStudentStr << std::endl;

    return 0;
}