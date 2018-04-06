#include <vector>
#include <boost/range/combine.hpp>
#include <unordered_map>
#include "document_retrieval.h"

int DocumentRetrieval::searchViaMediatorImpl(std::string query, std::vector<std::string> searchArgs, const std::string& sourceLanguage, const std::string& targetLanguage, const MediatorConfig& mediatorConfig) {
    int supportSearchTries = 10;
    int minSupportDocs = 10;
    MediatorConfig langToLangConfig = mediatorConfig.getMediatorConfig(sourceLanguage, targetLanguage);
    // id_mapping = lang_to_lang_config.coll_res_id_mapping();

    std::vector<std::string> supportDocs;
    // med_search_settings = dict(mediator_config.mediator_search_settings)
    for (int supportTry = 0; supportTry < supportSearchTries; supportTry++) {
        std::vector<std::string> supportDocs;
        if (int(supportDocs.size) >= minSupportDocs) {
            break;
        } else {
            int med_search_settings = 0.5;
        }
    }

    if (int(supportDocs.size) == 0) {
        return 0;
    }

    // int relevanceFunc = mediatorConfig.relevance_function();
    std::vector<std::string> targetSimDocs;
    for (auto supportDoc : supportDocs) {
        targetSimDocs.push_back(mediatorConfig.searchMany(supportDoc));
    }

    std::unordered_map<std::string, int> targetInfo;
    std::vector<int> srcRelNorm;
    for (auto supportDoc: supportDocs) {
        srcRelNorm.push_back(supportDoc);
    }

    for (auto pair: boost::combine(supportDocs, targetSimDocs)) {
        std::string supportDoc;
        std::vector<std::string> mappedSimDocs;
        boost::tie(supportDoc, mappedSimDocs) = pair;
        for (int rank; rank < mappedSimDocs.size; rank++) {
            int targetDocId = getCollResId(mappedSimDocs[rank]);
            auto notContains = targetInfo.find(targetDocId);
            if (notContains == targetInfo.end()) {
                targetInfo.insert(targetDocId, mappedSimDocs[rank])
            }
            // relevanceFunc.push_back(targetDocId, rank);
        }
    }

    // relevanceFunc.calculateResult();

    int docId;
    std::string<std::int> docInfo;
    for (auto & [docId, docInfo] : targetInfo) {
        docInfo = relevanceFunc[docId];
    }

    std::vector<std::string> resultDocList = std::sort(std::begin(targetInfo), std::end(targetInfo), [](int a, int b) {return a > b; });
    int allResults = int(resultDocList.size);
    int docsPerPage = int(searchArgs.get('DOCS_PER_PAGE', 10));
    int maxPageNumber = (allResults / docsPerPage) + (1 if allResults % docsPerPage > 0 else 0);
    int pageNo = min(maxPageNumber, int(searchArgs.get('PAGE_NO', 0)));;

    return allResults;
};

int DocumentRetrieval::simDocsViaMediatorWithFullInfo(std::string query, std::vector<std::string> searchArgs, std::string sourceLanguage, std::string targetLanguage, MediatorConfig& mediatorConfig) {
    if (sourceLanguage.empty()) {
        sourceLanguage = getLanguageFromSimilaritySearch(query, searchArgs);
    }
    return DocumentRetrieval::searchViaMediatorImpl(query, searchArgs, sourceLanguage, targetLanguage, mediatorConfig);
}

std::vector<int> DocumentRetrieval::simDocsViaMediator(std::string query, std::vector<std::string> searchArgs, std::string sourceLanguage, std::string targetLanguage, MediatorConfig& mediatorConfig) {
    int allResults = DocumentRetrieval::simDocsViaMediatorWithFullInfo(query, searchArgs, sourceLanguage, targetLanguage, mediatorConfig);
    std::vector<int> std::vector<int>;
    for (auto doc: curPageDocs) {
        collResIds.push_back(getCollResId(doc));
    }
    return std::vector<int>;
}