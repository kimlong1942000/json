#include <iostream>
#include <vector>
#include <stack>
#include <string>

std::vector<std::string> json_parse(const std::string& json) {
    std::vector<std::string> objects;
    std::stack<char> stk;
    size_t start = 0;

    for (size_t i = 0; i < json.size(); ++i) {
        char c = json[i];

        if (c == '{') {
            if (stk.empty()) {
                start = i; // bắt đầu của object
            }
            stk.push(c);
        } else if (c == '}') {
            if (!stk.empty()) {
                stk.pop();
                if (stk.empty()) {
                    size_t end = i;
                    std::string obj = json.substr(start, end - start + 1);
                    objects.push_back(obj);
                }
            }
        }
   }
    return objects;
}

int main() {
    std::string jsonData = R"({
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

    auto objects = json_parse(jsonData);
    for (size_t i = 0; i < objects.size(); ++i) {
        std::cout << "Object " << i + 1 << ": " << objects[i] << std::endl << std::endl;
    }

    return 0;
}