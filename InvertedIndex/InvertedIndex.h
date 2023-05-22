#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <string>
#include <exception>

struct Entry {
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
public:
    InvertedIndex() = default;
    /**
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param input_docs содержимое документов
*/
    void UpdateDocumentBase(std::vector<std::string> input_docs);
/**
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> GetWordCount(const std::string& word);
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
