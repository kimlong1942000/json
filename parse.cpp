#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>


using namespace std;

unordered_map<string, vector<string>> processString(const string& input) {
    stack<char> charStack;
    stack<int> levelStack;
    unordered_map<string, vector<string>> resultMap;

    int currentLevel = 0;

    for (char ch : input) {
        if (ch == '{') {
            // Đẩy '{' vào stack và tăng cấp độ
            charStack.push(ch);
            levelStack.push(currentLevel);
            currentLevel++;
        } else if (ch == '}') {
            // Khi gặp '}', pop đến '{' và lưu các phần tử trong đó
            vector<string> temp;
            int startLevel = levelStack.top();
            levelStack.pop();
            currentLevel--;

            // Pop đến khi gặp '{'
            while (!charStack.empty()) {
                char topChar = charStack.top();
                charStack.pop();
                if (topChar == '{') {
                    break;
                } else {
                    // Lưu phần tử vào tạm
                    temp.push_back(string(1, topChar));
                }
            }

            // Đảo ngược để giữ thứ tự ban đầu
            reverse(temp.begin(), temp.end());

            // Lưu các phần tử vào map theo cấp độ
            string key = "level_" + to_string(startLevel);
            resultMap[key].insert(resultMap[key].end(), temp.begin(), temp.end());
        } else {
            // Các ký tự khác, thêm vào stack
            charStack.push(ch);
        }
    }

    return resultMap;
}

int main() {
    string input =R"(
    {
        "name": "John, Doe",
        "age": 30,
        "city": "New York",
        "skills": {"primary": "C++", "secondary": "Python"},
        "projects": [{"name": "Project1"}, {"name": "Project2"}],
        "emptyArray": [],
        "nullValue": null,
        "boolean": true
    }
    )";;
    auto result = processString(input);

    // In kết quả
    for (const auto& pair : result) {
        cout << pair.first << ": ";
        for (const auto& val : pair.second) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}