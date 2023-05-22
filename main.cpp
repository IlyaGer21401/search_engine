#include "ConverterJSON.h"
#include "SearchServer.h"
#include <iomanip>

using std::cout;

int main() {
    ConverterJSON readJson;
    InvertedIndex index;

    readJson.ClearRequests();
    std::vector<std::string> text = readJson.GetTextDocuments(); ///Считываем текст документов
    int limitAnswers = readJson.GetResponsesLimit(); /// Считываем колиество возможных ответов
    index.UpdateDocumentBase(text); /// Обновление базы данных (где записаны все ключевые слова,
                                             /// взятые из текстов документов)
    SearchServer server(index);

    /// Цикл работы программы
    cout << readJson.GetNameProgram() << " ";
    cout << readJson.GetVersionProgram() << "\n";
    while (true) {
        std::string line;
        cout << "Enter keywords from the document you want to find or enter 'q' to exit:\n";
        std::getline(std::cin, line); /// user делает запрос
        if (line == "q" || line == "Q") { /// Проверка на выход
            break;
        } else {
            readJson.PutRequests(line); /// Записываем запрос
        }
        auto answers = server.search(
                                readJson.GetRequests(),
                                limitAnswers,
                                text.size()); /// передаем запросы в поисковый сервер
        readJson.PutAnswers(answers); /// Публикуем ответы

        std::map<int, std::string> name_doc = readJson.GetNameDocuments(); /// Считываем имена документов

        /// Двем ответ user
        auto id = answers.size() - 1; /// номер последнего запроса
        if (answers[id].size() == 0) /// Если ничего не найдено
            cout << "Nothing was found for your query!\n";
        else { /// Есть результат
            cout << "\t\tSearch results\n";
            cout << "Document name" << "\t\t\t" << "Coincidence\n";
            for (int i = 0; i < answers[id].size(); ++i) {
                std::string name = name_doc.find(answers[id][i].first)->second;
                int n = 38 - (name.length());
                cout << name << std::setw(n) << answers[id][i].second << "\n";
            }
        }
        cout << "=====================================================\n";
    }
    return 0;
}
/// Проверка открытия файлов
/*
std::ifstream file2("\\resources\\file001.txt");
if (file2.is_open())
while(!file2.eof()) {
std::string str;
std::getline(file2, str);
std::cout << str << "\n";
}
else
std::cout << "Didn't open!\n";
 */
/// Проверка 1ой функции
/*
    ConverterJSON js;
    std::vector<std::string> text = js.GetTextDocuments();
    std::string str_text;
    for (int i = 0; i < text.size(); ++i)
        str_text += "\n" + text[i];
    std::cout << str_text << std::endl;
*/
/// Проверка 2ой функции
/*
ConverterJSON json;
std::cout << json.GetResponsesLimit() << std::endl;
return 0;
*/
/// Проверка 3ей функции
/*
    ConverterJSON json;
    std::vector<std::string> vec = json.GetRequests();
    int it = 1;
    for (auto i : vec) {
        std::cout << "request " << it << ": \"" << i << "\"\n";
        it++;
    }
     */
/// Проверка 4ой функции
/*
    ConverterJSON js;
    std::vector<std::vector<std::pair<int, float>>> answers;

    std::vector<std::pair<int, float>> request001;
    {
        request001.push_back(std::pair<int, float> (0, 0.989));
        request001.push_back(std::pair<int, float> (1, 0.897));
        request001.push_back(std::pair<int, float> (2, 0.750));
        request001.push_back(std::pair<int, float> (3, 0.670));
        request001.push_back(std::pair<int, float> (4, 0.561));
    }

    std::vector<std::pair<int, float>> request002;
    request002.push_back(std::pair<int, float> (0, 0.769));

    std::vector<std::pair<int, float>> request003;

    answers.push_back(request001);
    answers.push_back(request002);
    answers.push_back(request003);

    js.putAnswers(answers);
*/
/// Проверка 1ой и 2ой функции (InvertedIndex)
/*
ConverterJSON js;
InvertedIndex index;

std::vector<std::string> text = js.GetTextDocuments();
index.UpdateDocumentBase(text);
std::vector<Entry> entry = index.GetWordCount("milk");

for (int i = 0; i < entry.size(); ++i) {
std::cout << "[\"milk\"]  " << entry[i].doc_id << " " << entry[i].count << std::endl;
}
std::string str_text;
for (int i = 0; i < text.size(); ++i)
str_text += "\n" + text[i];
std::cout << str_text << std::endl;
 */
/// Тест 2 (InvertedIndex)
/*
    ConverterJSON js;
    auto *index = new InvertedIndex();

    std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };

    std::vector<std::string> text = js.GetTextDocuments();
    index->UpdateDocumentBase(docs);
    std::vector<Entry> entry = index->GetWordCount("cappuccino");


    for (int i = 0; i < entry.size(); ++i) {
        std::cout << "[\"cappuccino\"]  " << entry[i].doc_id << " " << entry[i].count << std::endl;
    }
    std::string str_text;
    for (int i = 0; i < docs.size(); ++i)
        str_text += "\n" + docs[i];
    std::cout << str_text << std::endl;
    delete index;
    */
/// Сама программа
/*
    ConverterJSON json;
    InvertedIndex index;

    std::vector<std::string> text = json.GetTextDocuments();
    int limitRequests = json.GetResponsesLimit();

    index.UpdateDocumentBase(text);
    SearchServer server(index);
    auto answers = server.search(json.GetRequests());
    json.putAnswers(answers);
    return 0;
 */