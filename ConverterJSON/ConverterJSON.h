#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <exception>

class ConverterJSON {
public:
    ConverterJSON() = default;
/**
* Метод считывает поле name
* @return name
*/
    std::string GetNameProgram();

/**
* Метод считывает поле version
* @return version
*/
    std::string GetVersionProgram();

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод получения имен файлов
* @return Возвращает список с имен файлов перечисленных
* в config.json
*/
    std::map<int, std::string> GetNameDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
*/
    int GetResponsesLimit();

/**
* Положить в файл requests.json запрос
*/
    void PutRequests(std::string requests);

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

/**
 * Метод чищает файл requests.json
 */
    void ClearRequests();

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
