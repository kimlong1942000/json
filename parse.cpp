#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>


using namespace std;

unordered_map<int, vector<string>> processString(const string& input) {
    stack<char> charStack;
    unordered_map<int, vector<string>> resultMap;

    int currentLevel = 0;

    for (char ch : input) {
        if (ch == '{') {
            // Đẩy '{' vào stack và tăng cấp độ
            charStack.push(ch);
            currentLevel++;
        } else if (ch == '}') {
            // Khi gặp '}', pop đến '{' và lưu các phần tử trong đó
            vector<string> temp;
            // Pop đến khi gặp '{'
            while (!charStack.empty()) {
                char topChar = charStack.top();
                charStack.pop();
                if (topChar == '{') 
                {
                    break;
                } else 
                {
                    // Lưu phần tử vào tạm
                    temp.push_back(string(1, topChar));
                }
            }
            // Đảo ngược để giữ thứ tự ban đầu
            reverse(temp.begin(), temp.end());
            // Lưu các phần tử vào map theo cấp độ
            int key = currentLevel;
            resultMap[key].insert(resultMap[key].end(), temp.begin(), temp.end());
            currentLevel--;

        } else 
        {
            // Các ký tự khác, thêm vào stack
            charStack.push(ch);
        }
    }

    return resultMap;
}

int main() {
    string input =R"(
{
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
}
    )";;
    auto result = processString(input);

    // In kết quả
    for (const auto& pair : result) {
        cout << pair.first << ": ";
        for (const auto& val : pair.second) {
            cout << val ;
        }
        cout << endl;
    }

    return 0;
}