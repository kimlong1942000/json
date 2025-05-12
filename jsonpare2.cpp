#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_set>

struct Element {
    std::string key;
    std::string value;
    int level; // Mức độ của key
};

std::string trim(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(str[start])) start++;
    size_t end = str.size();
    while (end > start && std::isspace(str[end - 1])) end--;
    return str.substr(start, end - start);
}

bool checkNextIsBrace(const std::string& inner, size_t afterColonPos) {
    size_t pos = afterColonPos;
    while (pos < inner.length() && std::isspace(inner[pos])) pos++;
    return inner.compare(pos, 1, "{") == 0;
}

bool checkNextIsBracket(const std::string& inner, size_t afterColonPos) {
    size_t pos = afterColonPos;
    while (pos < inner.length() && std::isspace(inner[pos])) pos++;
    return inner.compare(pos, 1, "[") == 0;
}

// Hàm đệ quy để parse object, lưu kết quả và theo cấp độ
void parseObject(const std::string& inner, int currentLevel, std::vector<Element>& result, std::unordered_set<std::string>& keysSet) {
    size_t innerPos = 0;
    while (innerPos < inner.length()) {
        size_t keyStart = inner.find('"', innerPos);
        if (keyStart == std::string::npos) break;
        size_t keyEnd = inner.find('"', keyStart + 1);
        if (keyEnd == std::string::npos) break;

        std::string key = inner.substr(keyStart + 1, keyEnd - keyStart - 1);
        if (keysSet.find(key) != keysSet.end()) {
            innerPos = keyEnd + 1;
        } else {
            keysSet.insert(key);
            innerPos = keyEnd + 1;
            size_t colonPos = inner.find(':', innerPos);
            if (colonPos == std::string::npos) break;
            size_t afterColonPos = colonPos + 1;

            // Kiểm tra xem có object hay array
            if (checkNextIsBrace(inner, afterColonPos)) {
                // Object
                result.push_back({key, "", currentLevel});
                size_t braceStart = inner.find('{', afterColonPos);
                if (braceStart != std::string::npos) {
                    int braceCount = 1;
                    size_t j = braceStart + 1;
                    while (j < inner.length() && braceCount > 0) {
                        if (inner[j] == '{') braceCount++;
                        else if (inner[j] == '}') braceCount--;
                        j++;
                    }
                    std::string innerSub = inner.substr(braceStart + 1, j - braceStart - 2);
                    parseObject(trim(innerSub), currentLevel + 1, result, keysSet);
                    innerPos = j;
                } else {
                    innerPos = afterColonPos;
                }
            } else if (checkNextIsBracket(inner, afterColonPos)) {
                // Array
                size_t bracketStart = inner.find('[', afterColonPos);
                if (bracketStart != std::string::npos) {
                    int bracketCount = 1;
                    size_t j = bracketStart + 1;
                    while (j < inner.length() && bracketCount > 0) {
                        if (inner[j] == '[') bracketCount++;
                        else if (inner[j] == ']') bracketCount--;
                        j++;
                    }
                    std::string arrayContent = inner.substr(bracketStart, j - bracketStart);
                    // Lưu array dưới dạng chuỗi
                    result.push_back({key, arrayContent, currentLevel});
                    innerPos = j;
                } else {
                    innerPos = afterColonPos;
                }
            } else {
                // Chuỗi hoặc giá trị đơn
                size_t tempPos = afterColonPos;
                while (tempPos < inner.length() && std::isspace(inner[tempPos])) tempPos++;
                if (inner.compare(tempPos, 1, "\"") == 0) {
                    size_t valStart = tempPos;
                    size_t valEnd = inner.find('"', valStart + 1);
                    if (valEnd == std::string::npos) break;
                    std::string value = inner.substr(valStart + 1, valEnd - valStart - 1);
                    result.push_back({key, value, currentLevel});
                    innerPos = valEnd + 1;
                } else {
                    // Không phải chuỗi, có thể là số, bool, null, bỏ qua hoặc xử lý khác
                    innerPos = afterColonPos;
                }
            }
            // Tìm tiếp phần tử tiếp theo
            size_t commaPos = inner.find(',', innerPos);
            if (commaPos != std::string::npos) {
                innerPos = commaPos + 1;
            } else {
                break;
            }
        }
    }
}

int main() {
    std::string jsonInput = R"({
    "glossary": 
    {
        "title": "example glossary",
		"GlossDiv": 
        {
            "title": "S",
			"GlossList": 
            {
                "GlossEntry": 
                {
                    "ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": 
                    {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
                    },
					"GlossSee": "markup"
                }
            }
        }
    }
})";

    size_t firstBrace = jsonInput.find('{');
    size_t lastBrace = jsonInput.rfind('}');
    std::string inner = jsonInput.substr(firstBrace + 1, lastBrace - firstBrace - 1);

    std::vector<Element> result;
    std::unordered_set<std::string> keysSet;
    parseObject(trim(inner), 0, result, keysSet);

    // Xuất kết quả với định dạng thể hiện level
    for (const auto& elem : result) {
        std::cout << std::string(elem.level * 2, ' ') 
                  << "\""<< elem.key << "\" : \"" << elem.value << "\" (Level " << elem.level << ")\n";
    }

    return 0;
}