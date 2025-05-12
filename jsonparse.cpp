#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

// Hàm đọc chuỗi trong JSON (ví dụ: "hello")
std::string readString(const std::string &json, size_t &index) {
    std::string result;
    if (json[index] == '\"') {
        index++; // bỏ qua dấu "
        while (index < json.size() && json[index] != '\"') {
            // xử lý escape (cơ bản)
            if (json[index] == '\\' && index + 1 < json.size()) {
                result += json[index];
                index++;
                result += json[index];
                index++;
            } else {
                result += json[index++];
            }
        }
        index++; // bỏ qua dấu "
    }
    return result;
}

// Hàm đọc số
std::string readNumber(const std::string &json, size_t &index) {
    std::string num;
    while (index < json.size() && (isdigit(json[index]) || json[index] == '.' || json[index] == '-')) {
        num += json[index++];
    }
    return num;
}

// Hàm bỏ qua các ký tự whitespace
void skipWhitespace(const std::string &json, size_t &index) {
    while (index < json.size() && isspace(json[index])) {
        index++;
    }
}

// Chuyển đổi JSON thành biểu thức tiền tố
std::string jsonToPrefix(const std::string &json, size_t &index);

// Đọc một giá trị JSON (object, array, string, number, boolean, null)
std::string parseValue(const std::string &json, size_t &index) {
    skipWhitespace(json, index);
    if (index >= json.size()) return "";

    if (json[index] == '{') {
        return jsonToPrefix(json, index); // object
    } else if (json[index] == '[') {
        return jsonToPrefix(json, index); // array
    } else if (json[index] == '\"') {
        std::string str = readString(json, index);
        return "\""+str+"\"";
    } else if (isdigit(json[index]) || json[index] == '-' ) {
        std::string num = readNumber(json, index);
        return num;
    } else if (json.compare(index, 4, "true") == 0) {
        index += 4;
        return "true";
    } else if (json.compare(index, 5, "false") == 0) {
        index += 5;
        return "false";
    } else if (json.compare(index, 4, "null") == 0) {
        index += 4;
        return "null";
    }
    return ""; // lỗi hoặc không xác định
}

// Chuyển JSON object thành biểu thức tiền tố
std::string jsonToPrefix(const std::string &json, size_t &index) {
    std::stringstream result;
    skipWhitespace(json, index);

    if (json[index] == '{') 
    {
        index++; // bỏ qua '{'
        skipWhitespace(json, index);
        while (index < json.size() && json[index] != '}') {
            result << "object ";

            // đọc key
            std::string key = readString(json, index) ;
            skipWhitespace(json, index);
            if (json[index] == ':') {
                index++; // bỏ qua ':'
            }
            // đọc value
            std::string value = parseValue(json, index) +"\n";
            // tạo biểu thức
            result << key << " " << value << " ";
            skipWhitespace(json, index);
            if (json[index] == ',') {
                index++; // bỏ qua ','
                skipWhitespace(json, index);
            }
        }
        if (json[index] == '}') index++; // bỏ qua '}'
        return result.str();
    } 
    else if (json[index] == '[')
    {
        // xử lý array
        result << "array ";
        index++; // bỏ qua '['
        skipWhitespace(json, index);
        while (index < json.size() && json[index] != ']') {
            std::string value = parseValue(json, index)  ;
            result << value << " ";
            skipWhitespace(json, index);
            if (json[index] == ',') {
                index++;
                skipWhitespace(json, index);
            }
        }
        if (json[index] == ']') index++; // bỏ qua ']'
        result << "\n";

        return result.str();
    }
    return "";
}

// Hàm chính để chuyển đổi JSON thành biểu thức tiền tố
std::string convertJSONToPrefix(const std::string &json) {
    size_t index = 0;
    return parseValue(json, index) ;
}

// Ví dụ sử dụng
int main() {
    std::string jsonStr = R"(
         {
                        "ID": "SGML",
                        "SortAs": "SGML",
                        "GlossTerm": "Standard Generalized Markup Language",
                        "Acronym": "SGML",
                        "Abbrev": "ISO 8879:1986",
                        "GlossSee": "markup"
        }

    )";
    
    std::string prefixExpr = convertJSONToPrefix(jsonStr);
    std::cout << "Prefix Expression:\n" << prefixExpr << std::endl;

    return 0;
}