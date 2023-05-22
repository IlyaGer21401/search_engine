#include "InvertedIndex.h"
#include "../INVISIBLE_EXCEPTIONS.h"

std::mutex indexEntry;
class ThereIsNoSuchWordException : public std::exception {
public: const char* what() const noexcept override{
        return "There is no such word";
    }
};

bool ListOfSeparators (char symbol) {
    std::string list = "~!@№#;%^:?&*()_+=[]{}\"\\|/,.<>";
    for (auto i : list) {
        if (symbol == i || symbol == '\n' || symbol == ' ')
            return true;
    }
    return false;
}
std::vector<Entry> SortingEntryId (std::vector<Entry> vec){
    std::vector<Entry> newVec = vec;
    if (vec.size() == 1 || vec.size() == 0)
        return vec;
    int listLength = newVec.size();
    while(listLength--) {
        bool swapped = false;
        for(int i = 0; i < listLength; i++)
        {
            if(newVec[i].doc_id > newVec[i + 1].doc_id) {
                std::swap(newVec[i], newVec[i + 1]);
                swapped = true;
            }
        }
        if(!swapped)
            break;
    }
    return newVec;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    std::map<std::string, std::vector<Entry>> indexing;
    std::vector<std::thread> documents;
    for (int i = 0; i < input_docs.size(); ++i) {
        documents.push_back(std::thread([&indexing](std::string text, int index) {
            /// индексирование
            std::string word;
            for (int j = 0; j < text.size() + 1; ++j) {
                /// Если найден разделитель слов
                if (ListOfSeparators(text[j]) || j == text.size()) {
                    if (word == "")
                        continue;
                    indexEntry.lock();
                    /// Есть ли такое слово
                    if (indexing.count(word)) {
                        auto it = indexing.find(word);
                        bool sig = false; /// Если мы найдем в векторе нужный нам индекс, то
                                          /// увеличиваем значение count
                        for (int k = 0; k < it->second.size(); k++) {
                            if (it->second[k].doc_id == index) {
                                it->second[k].count++;
                                sig = true;
                                break;
                            }
                        }/// Если нет, то добавим новый элемент
                        if (!sig) {
                            Entry entry {static_cast<size_t>(index), 1};
                            it->second.push_back(entry);
                        }
                    } else { ///Если нет, то добавляем слово
                        Entry entry {static_cast<size_t>(index), 1};
                        std::vector<Entry> vec;
                        vec.push_back(entry);
                        indexing.insert(std::pair<std::string, std::vector<Entry>> (word, vec));
                    }
                    indexEntry.unlock();
                    word = "";
                } else { /// Eсли нет, то пишем слово дальше
                    word += text[j];
                }
            }

        }, input_docs[i], i));
    }
    for (int i = 0; i < input_docs.size(); ++i)
        documents[i].join();
    /// Сортировка элементов Entry
    for (auto &it : indexing) {
        indexEntry.lock();
        it.second = SortingEntryId(it.second);
        indexEntry.unlock();
    }
    freq_dictionary = indexing;
}
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> wordEntry;
#if INVISIBLE_EXCEPTIONS
    try {
        if (!freq_dictionary.count(word)) {
            throw ThereIsNoSuchWordException();
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "! (" << word << ")\n";
        return wordEntry;
    }
#else
    if (!freq_dictionary.count(word))
        return wordEntry;
#endif
    wordEntry = freq_dictionary.find(word)->second;
    return wordEntry;
}