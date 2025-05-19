#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm> // for remove_if

class JsonX {
private:
    std::map<std::string, std::string> _data;

public:
    JsonX(const std::string& jsonStr) {
        parseJson(jsonStr);
    }

    std::string operator[](const std::string& key) const {
        auto it = _data.find(key);
        if (it != _data.end()) {
            return it->second;
        }
        return ""; // Không tìm thấy
    }

private:
    void parseJson(const std::string& jsonStr) {
        // Loại bỏ khoảng trắng để dễ phân tích
        std::string s = jsonStr;
        s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

        // Giả sử JSON dạng đơn giản: { "key": "value", "key2": "value2" }
        size_t pos = 0;
        while ((pos = s.find("\"", pos)) != std::string::npos) {
            size_t keyStart = pos + 1;
            size_t keyEnd = s.find("\"", keyStart);
            if (keyEnd == std::string::npos) break;
            std::string key = s.substr(keyStart, keyEnd - keyStart);

            size_t colonPos = s.find(":", keyEnd);
            if (colonPos == std::string::npos) break;
            size_t valueStart = colonPos + 1;

            // Tìm giá trị
            if (s[valueStart] == '\"') {
                size_t valueEnd = s.find("\"", valueStart + 1);
                if (valueEnd == std::string::npos) break;
                std::string value = s.substr(valueStart + 1, valueEnd - valueStart - 1);
                _data[key] = value;
                pos = valueEnd + 1;
            } else {
                // Trường hợp giá trị không phải chuỗi
                size_t valueEnd = valueStart;
                while (valueEnd < s.size() && s[valueEnd] != ',' && s[valueEnd] != '}') {
                    valueEnd++;
                }
                std::string value = s.substr(valueStart, valueEnd - valueStart);
                _data[key] = value;
                pos = valueEnd;
            }
        }
    }
};

int main() {
    // Đọc file JSON
    std::ifstream file("data.txt");
    if (!file.is_open()) {
        std::cerr << "Không thể mở file." << std::endl;
        return 1;
    }
    std::string jsonStr((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    JsonX json(jsonStr);

    std::cout << "name: " << json["name"] << std::endl;
    std::cout << "age: " << json["age"] << std::endl;
    std::cout << "city: " << json["city"] << std::endl;

    return 0;
}