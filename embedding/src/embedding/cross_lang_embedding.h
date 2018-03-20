#ifndef EMBEDDING_CROSS_LANG_EMBEDDING_H
#define EMBEDDING_CROSS_LANG_EMBEDDING_H

#include "embedding.h"

using namespace std;

class CrossLangEmbedding: public Embedding {
public:
    CrossLangEmbedding(const std::string& filename, const std::string& sourceLanguage, const std::string& targetLanguage);
    static CrossLangEmbedding* getInstance(const std::string& filename, const std::string& sourceLanguage, const std::string& targetLanguage);

    std::pair<EmbeddingMatrix, EmbeddingMatrix> readEmbeddingMatrixFromFile(const std::string &filename,
                                                                            const std::string &sourceLanguage,
                                                                            const std::string &targetLanguage);

    void setEmbeddingMatrixByLanguage(const EmbeddingMatrix& embeddingMatrix, const std::string& language);
    const EmbeddingMatrix& getEmbeddingMatrixByLanguage(const std::string& language);

    void setSourceLanguage(const std::string& language);
    std::string getSourceLanguage();

    void setTargetLanguage(const std::string& language);
    std::string getTargetLanguage();

    // Overrided functions from the Embedding superclass
    EmbeddingMatrix readEmbeddingMatrixFromFile(const std::string &filename) const;
    bool contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix, const std::string& language) const;
    double getWordSimilarityValue(const std::string &A, const std::string &B, const std::string &language) const;
    Vector getSentenceVector(const std::string& text, const std::string& language) const;
    Vector getWordVector(const std::string &word, const std::string &language) const;
    double getCrossLangWordSimilarityValue(const std::string &A, const std::string &B) const;
private:
    static CrossLangEmbedding* instance;
    std::string mSourceLanguage;
    std::string mTargetLanguage;
    EmbeddingMatrix mSourceEmbeddingMatrix;
    EmbeddingMatrix mTargetEmbeddingMatrix;

    // Vector size for each language is not defined since it is the same for every matrix
};


#endif //EMBEDDING_CROSS_LANG_EMBEDDING_H