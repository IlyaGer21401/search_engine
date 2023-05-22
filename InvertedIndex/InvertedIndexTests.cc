#include "gtest/gtest.h"
#include "InvertedIndex.h"

using std::string, std::vector;

void TestInvertedIndexFunctionality(
        const vector<string>& docs,
        const vector<string>& requests,
        const std::vector<vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    auto *idx = new InvertedIndex;
    idx->UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx->GetWordCount(request);
        result.push_back(word_count);
    }
    delete idx;
    ASSERT_EQ(result, expected);
}
TEST(InvertedIndexTest, Basic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(InvertedIndexTest, Basic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuccino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(InvertedIndexTest, MissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

/*
TEST(ConverterJSONTests, Test_1_GetTextDocuments) {
    ConverterJSON json;

    std::vector<std::string> files;
    std::vector<std::string> path_files;
    std::ifstream file;
    file.open("..\\JSON_files\\config.json");
    ASSERT_TRUE(file.is_open());
    std::string word;
    while (true) {
        file >> word;
        if (word == "\"files\":") {
            break;
        }
    }
    file >> word;
    while (true) {
        file >> word;
        if (word[word.size() - 1] == ',') {
            word.pop_back();
            word.pop_back();
            for (int i = 0; i < word.size(); ++i) {
                word[i] = word[i + 1];
            }
            word.pop_back();
            path_files.push_back(word);
        } else
            break;
    }
    file.close();

    std::string text;
    std::ifstream file2;
    for (int i = 0; i < path_files.size(); ++i) {
        file2.open(path_files[i]);
        ASSERT_TRUE(file2.is_open());
        while (!file.eof()) {
            std::string line;
            std::getline(file, line);
            text += line + "\n";
        }
        files.push_back(text);
        text = "";
        file2.close();
    }
    const std::vector<std::string> testFiles = files;
    //const std::vector<std::string> funcFiles = json.GetTextDocuments();
    //ASSERT_EQ(testFiles, funcFiles);
}
TEST(ConverterJSONTests, Test_2_GetResponsesLimit) {
    ConverterJSON json;
    std::ifstream file("..\\JSON_files\\config.json");
    ASSERT_TRUE(file.is_open());

    std::string line;
    while (true) {
        file >> line;
        if (line == "\"max_responses\":") {
            break;
        }
    }
    file >> line;
    file.close();
    line.pop_back();
    const int max_responses = std::stoi(line);
    //const int func_max_responses = json.GetResponsesLimit();
    //ASSERT_EQ(max_responses, func_max_responses);
}
TEST(ConverterJSONTests, Test_3_GetRequests) {
    ASSERT_LT(1,2);
}
TEST(ConverterJSONTests, Test_4_putAnswers) {
    ASSERT_LT(1,2);
}*/