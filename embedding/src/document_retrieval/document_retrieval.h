#ifndef EMBEDDING_DOCUMENT_RETRIEVAL_H
#define EMBEDDING_DOCUMENT_RETRIEVAL_H

#include "mediator_config.h"
#include <iostream>

using namespace std;

class DocumentRetrieval {
    int searchViaMediatorImpl(std::string query, std::vector<std::string> searchArgs, const std::string& sourceLanguage, const std::string& targetLanguage, const MediatorConfig& mediatorConfig);
    int simDocsViaMediatorWithFullInfo(std::string query, std::vector<std::string> searchArgs, std::string sourceLanguage, std::string targetLanguage, MediatorConfig& mediatorConfig);
    std::vector<int> simDocsViaMediator(std::string query, std::vector<std::string> searchArgs, std::string sourceLanguage, std::string targetLanguage, MediatorConfig& mediatorConfig);
};


#endif //EMBEDDING_DOCUMENT_RETRIEVAL_H
