#include "ConverterJSON.h"
#include "nlohmann/json.hpp"
#include "../INVISIBLE_EXCEPTIONS.h"

class fileOpeningError : public std::exception {
public: const char* what() const noexcept override {
        return "File opening Error";
    }
};
class noProgramNameError :public std::exception {
public: const char* what() const noexcept override {
        return "No program name";
    }
};
class noProgramVersionError :public std::exception {
public: const char* what() const noexcept override {
        return "The program version is not specified error";
    }
};
class maxResponsesError : public std::exception {
public: const char* what() const noexcept override {
        return "Max Responses < 0";
    }
};

std::string ConverterJSON::GetNameProgram() {
    std::string name;
    nlohmann::json config;
    std::ifstream if_config("..\\JSON_files\\config.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!if_config.is_open())
            throw fileOpeningError();
        nlohmann::ordered_json j_config;
        if_config >> j_config;
        if_config.close();
        name = j_config["config"]["name"];
        if (name == "")
            throw noProgramNameError();
        return name;
    }
    catch(const fileOpeningError& exp) {
        std::cerr << "Error: " << exp.what() << " (config.json)\n";
        return "";
    }
    catch(const noProgramNameError& exp) {
        std::cerr << "Error: " << exp.what() << "\nfile - config.json\npath(\"config\"->\"name\")\n";
        return "";
    }
#else
    nlohmann::ordered_json j_config;
    if_config >> j_config;
    if_config.close();
    name = j_config["config"]["name"];
    return name;
#endif
}
std::string ConverterJSON::GetVersionProgram() {
    std::string version;
    nlohmann::json config;
    std::ifstream if_config("..\\JSON_files\\config.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!if_config.is_open())
            throw fileOpeningError();
        nlohmann::json j_config;
        if_config >> j_config;
        if_config.close();


        version = j_config["config"]["version"];
        if (version == "")
            throw noProgramNameError();
        return version;
    }
    catch(const fileOpeningError& exp) {
        std::cerr << "Error: " << exp.what() << " (config.json)\n";
        return "";
    }
    catch(const noProgramVersionError& exp) {
        std::cerr << "Error: " << exp.what() << "\nfile - config.json\npath(\"config\"->\"version\")\n";
        return "";
    }
#else
    nlohmann::json j_config;
    if_config >> j_config;
    if_config.close();
    version = j_config["config"]["version"];
    return version;
#endif
}
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> files;
    std::vector<std::string> path_files;
    nlohmann::json config;

    std::ifstream f_config;
    f_config.open("..\\JSON_files\\config.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!f_config.is_open())
            throw fileOpeningError();
    }
    catch(const std::exception& exp) {
        std::cerr << "Error: " << exp.what() << " (config.json)\n";
        return files;
    }
#endif
    f_config >> config;
    f_config.close();

    ///Записываем в вектор пути к файлам
    auto it = config["files"].begin();
    while (it != config["files"].end()) {
        std::string value = it.value();
        path_files.push_back(value);
        it++;
    }

    ///Записываем в вектор содержимое файлов
    std::string text;
    std::ifstream file;
    for (int i = 0; i < path_files.size(); ++i) {
        file.open(path_files[i]);
        try {
            if (!file.is_open())
                throw fileOpeningError();
        }
        catch(const std::exception& exp) {
            std::cerr << "Error: " << exp.what() << " (" << files[i] << ")\n";
            return files;
        }
        while (!file.eof()) {
            std::string line;
            std::getline(file, line);
            text += line + "\n";
        }
        files.push_back(text);
        text = "";
        file.close();
    }

    return files;
}
std::map<int, std::string> ConverterJSON::GetNameDocuments() {
    std::map<int, std::string> m_doc_names;
    std::vector<std::string> path_files;
    nlohmann::ordered_json config;

    std::ifstream f_config;
    f_config.open("..\\JSON_files\\config.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!f_config.is_open())
            throw fileOpeningError();
    }
    catch(const std::exception& exp) {
        std::cerr << "Error: " << exp.what() << " (config.json)\n";
        return m_doc_names;
    }
#endif
    f_config >> config; /// Считали config.json
    f_config.close();
    /// Считываем имена
    int id_doc = 0;
    auto it = config["files"].begin();
    while (it != config["files"].end()) {
        std::string name_file; /// Имя файла
        std::string path = it.value(); /// Считываем путь к файлу
        for (int i = 0; i < path.size(); ++i) {
            name_file += path[i];
            if (path[i] == '\\')
                name_file = "";
        }
        m_doc_names.insert(std::pair<int, std::string> (id_doc, name_file));
        it++;
        id_doc++;
    }

    return m_doc_names;
}
int ConverterJSON::GetResponsesLimit() {
    int max_responses = 0;
    std::ifstream if_config("..\\JSON_files\\config.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!if_config.is_open())
            throw fileOpeningError();
    }
    catch(const fileOpeningError& exp) {
        std::cerr << "Error: " << exp.what() << " (config.json)\n";
        return 0;
    }
#endif
    try {
        nlohmann::ordered_json j_config;
        if_config >> j_config;
        if_config.close();
        max_responses = j_config["config"]["max_responses"];
        if (max_responses == 0) {
            std::ofstream of_config("..\\JSON_files\\config.json");
            j_config["config"]["max_responses"] = 5;
            of_config << j_config;
            max_responses = 5;
        } else if (max_responses > 500) {
            std::ofstream of_config("..\\JSON_files\\config.json");
            j_config["config"]["max_responses"] = 500;
            of_config << j_config;
            max_responses = 500;
        }
        else if (max_responses < 0)
            throw maxResponsesError();
    }
    catch(const maxResponsesError& exp) {
        std::cerr << "Error: " << exp.what() << "\nfile - config.json\npath(\"config\"->\"max_responses\")\n";
        return 0;
    }
    return max_responses;
}
void ConverterJSON::PutRequests(std::string request) {
    std::vector<std::string> requests = GetRequests(); /// Считали все запросы из файла requests.json
    requests.push_back(request); /// Занесли новый запрос в список

    nlohmann::ordered_json j_requests;
    for (auto i = 0; i < requests.size(); i++) {
        j_requests["requests"][i] = requests[i];
    }
    std::ofstream of_requests("..\\JSON_files\\requests.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!of_requests.is_open())
            throw fileOpeningError();
    }
    catch(const std::exception& exp) {
        std::cerr << "Error: " << exp.what() << " (requests.json)\n";
        return;
    }
#endif
    of_requests << j_requests;
    of_requests.close();
}
std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    std::ifstream if_requests("..\\JSON_files\\requests.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!if_requests.is_open())
            throw fileOpeningError();
    }
    catch(const fileOpeningError& exp) {
        std::cerr << "Error: " << exp.what() << " (requests.json)\n";
        return requests;
    }
#endif
    nlohmann::json j_requests;
    if_requests >> j_requests;
    for (int i = 0; i < j_requests["requests"].size(); ++i) {
        requests.push_back(j_requests["requests"][i]);
    }
    return requests;
}
void ConverterJSON::ClearRequests() {
    nlohmann::ordered_json j_requests;
    std::ofstream of_requests("..\\JSON_files\\requests.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!of_requests.is_open())
            throw fileOpeningError();
    }
    catch(const std::exception& exp) {
        std::cerr << "Error: " << exp.what() << " (requests.json)\n";
        return;
    }
#endif
    of_requests << j_requests;
    of_requests.close();
}
void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::ordered_json j_answers;
    std::ofstream of_answers("..\\JSON_files\\answers.json");
#if INVISIBLE_EXCEPTIONS
    try {
        if (!of_answers.is_open())
            throw fileOpeningError();
    }
    catch(const std::exception& exp) {
        std::cerr << "Error: " << exp.what() << " (answers.json)\n";
        return;
    }
#endif
    for (int i = 0; i < answers.size(); ++i) {
        if (!answers[i].empty()) {
            j_answers["answers"]["request " + std::to_string(i + 1)]["result"] = "true";
            for (int j = 0; j < answers[i].size(); ++j) {
                j_answers["answers"]["request " + std::to_string(i + 1)]["relevance"][j]["docid"] = answers[i][j].first;
                double value = round(answers[i][j].second * 1000) / 1000;
                j_answers["answers"]["request " + std::to_string(i + 1)]["relevance"][j]["rank"] = round(answers[i][j].second * 1000) / 1000;
            }
        } else
            j_answers["answers"]["request " + std::to_string(i + 1)]["result"] = "false";
    }
    of_answers << j_answers;
    of_answers.close();
}