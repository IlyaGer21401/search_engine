#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include "../InvertedIndex/InvertedIndex.h"

class SearchServer {
private:
    InvertedIndex _index;
public:
    /**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){};
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<std::pair<int, float>>> search(const std::vector<std::string> &queries_input, int number_responses, int num_doc);
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
