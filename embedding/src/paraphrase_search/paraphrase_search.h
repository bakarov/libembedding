#ifndef EMBEDDING_PARAPHRASE_SEARCH_H
#define EMBEDDING_PARAPHRASE_SEARCH_H

#include "../embedding/cross_lang_embedding.h"

using namespace std;

struct pairwisePlagIndex {
    int sourceIndex;
    int targetIndex;
    double similarity;
};
pairwisePlagIndex createPairwisePlagIndices(const int& sourceDocIndex, const int& targetDocIndex, const double& similarity);


struct plagIndex {
    int sourceIndex;
    double similarity;
};
plagIndex createTargetPlagIndex(const int& sourceDocIndex, const double& similarity);

class ParaphraseSearch
{
public:
    ParaphraseSearch(const CrossLangEmbedding& crossLangEmbedding,
                     const std::vector<std::string> &source_document,
                     const std::string &language);

    CrossLangEmbedding getCrossLangEmbedding();
    std::vector<pairwisePlagIndex> compareDocuments(std::vector<std::string> &target_document, const std::string &targetLanguage);
    std::vector<plagIndex> compareSentence(std::string& sentence, const std::string &targetLanguage);

    void setSourceDocumentVectors(const std::vector<std::string> &source_document, const std::string &language);
    std::vector<Vector> getDocumentVectors(const std::vector<std::string> &sentences, const std::string &language);

    void setThreshold(const double &threshold);
    double getThreshold(void);
private:
    const CrossLangEmbedding& mCrossLangEmbedding;
    std::vector<Vector> mSourceDocumentVectors;
    double mSimilarityThreshold;
};


#endif //EMBEDDING_PARAPHRASE_SEARCH_H
