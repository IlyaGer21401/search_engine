#include "SearchServer.h"

int IdCount(std::vector<Entry> vec) {
    int count = 0;
    for (auto i : vec) {
        count += i.count;
    }
    return count;
}
void Relevance_Calculation(std::vector<std::vector<std::pair<int, float>>> &vec,
                           std::vector<std::vector<Entry>> words,
                           int limit_Answers,
                           int num_doc) {
    std::vector<std::pair<int, float>> answer;
    if (words.size() == 0)
        return;
    ///Подсчет релевантности
    for (int id = 0; id < num_doc; ++id) { /// Проходимся по каждому документу
        float relevance = 0;
        for (auto word : words) { /// Проходимся по каждому слову в документе с номером i
            for (auto entry : word) {  /// Проходимся по Entry слова. Ищем нужный id
                if (entry.doc_id == id) {
                    relevance += entry.count; /// считаем релевантность
                    break;
                }
            }
        }
        answer.push_back(std::pair<int, float> (id, relevance));
    }
    /// Cортировка по релевантности
    if (answer.size() != 1) {
        int list_length = answer.size();
        while(list_length--) {
            bool swapped = false;
            for(int i = 0; i < list_length; i++) {
                if(answer[i].second < answer[i + 1].second) {
                    std::swap(answer[i], answer[i + 1]);
                    swapped = true;
                }
            }
            if(!swapped)
                break;
        }
    }
    /// Расчет отнсительной релевантности
    if (answer.size() != 0) {
        float max_relevance = answer[0].second;
        if (max_relevance != 0) {
            /// Максимальная релевантнасть
            for (int i = 0; i < answer.size(); ++i)
                answer[i].second /= max_relevance;/// Расчет отнсительной релевантности
        }

    }


    /// Удление ответов где релевантность 0
        while (true) {
            if (answer.size() == 0)
                break;
            bool sig = true; /// Сигнализирует нам о том, что все ответы с нулевой релевантностью удалены
            if (answer[answer.size() - 1].second == 0) {
                answer.pop_back();
                sig = false;
            }
            if (sig)
                break;
        }

    /// Удаление лишних ответов (количество ответов соответствует максимально допустимому количеству ответов)
    while (answer.size() > limit_Answers)
        answer.pop_back();
    vec.push_back(answer);
}

std::vector<std::vector<std::pair<int, float>>> SearchServer::search(const std::vector<std::string> &queries_input, int limit_Answers, int num_doc) {
    std::vector<std::vector<std::pair<int, float>>> answers; /// ответы
    std::vector<std::vector<std::string>> words; /// слова распределенные по запросам
    /// Переписали слова из запросов
    for (auto i : queries_input) {
        std::string word; /// сюда записываем слово
        std::vector<std::string> vec; /// Сюда мы помещаем слова, который только что разделили
        for (int j = 0; j < i.size() + 1; ++j) {
            if (i[j] == ' ' && word == "")
                continue;
            if (i[j] == ' ' || j == i.size()) {
                vec.push_back(word);
                word = "";
            } else
                word += i[j];
        }
        words.push_back(vec);
    }

    /// Сортировка по редкости слова
    std::vector<std::vector<std::string>> sorted_words; /// отсортированные слова по редкости и по номеру запроса
    for (auto i : words) {
        if (i.size() == 1) {
            sorted_words.push_back(i);
            continue;
        }
        int listLength = i.size();
        while(listLength--) {
            bool swapped = false;
            for(int j = 0; j < listLength; j++)
            {
                if(IdCount(_index.GetWordCount(i[j])) > IdCount(_index.GetWordCount(i[j + 1]))) {
                    std::swap(i[j], i[j + 1]);
                    swapped = true;
                }
            }
            if(!swapped)
                break;
        }
        sorted_words.push_back(i);
    }

    /// Делаем запрос GetWordCount для каждого слова
    for (auto i : sorted_words) { /// Проходимся по sorted_words
        std::vector<std::vector<Entry>> entry_words; /// Временное хранилище для слов (Entry)
        for (int j = 0; j < i.size(); ++j) {
            entry_words.push_back(_index.GetWordCount(i[j]));
        }
        Relevance_Calculation(answers, entry_words, limit_Answers, num_doc);
    }
    return answers;
}