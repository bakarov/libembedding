#ifndef EMBEDDING_EMBEDDING_H
#define EMBEDDING_EMBEDDING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <functional>

using namespace std;

typedef double VectorValue;
typedef std::string VectorName;
typedef std::vector<VectorValue> Vector;
typedef std::unordered_map<VectorName, Vector> EmbeddingMatrix;

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

struct crossLangEmbeddingMatrix {
    EmbeddingMatrix sourceEmbeddingMatrix;
    EmbeddingMatrix targetEmbeddingMatrix;
    std::string sourceLanguage;
    std::string targetLanguage;
};
crossLangEmbeddingMatrix createCrossLangEmbeddingMatrix(const EmbeddingMatrix& sourceMatrix,
                                                           const EmbeddingMatrix& targetMatrix, const std::string& sourceLanguage,
                                                           const std::string& targetLanguage);


class Embedding
{

private:
    static Embedding* instance;
    std::vector<Vector> mSourceDocumentVectors;
    crossLangEmbeddingMatrix mCrossLangWordVectors;
    double mSimilarityThreshold;
    int mVectorSize;
    std::string mSourceLanguage;
    std::vector<std::string> splitString(const std::string &s, const char delim);

public:
    static Embedding* getInstance();
    int getVectorSize();
    bool contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix);
    EmbeddingMatrix readEmbeddingMatrixFromFile(const std::string &filename);
    double cosineVectorSimilarity(const Vector &A, const Vector &B);
    double wordSimilarity(const std::string &A, const std::string &B);
    Vector getSentenceVector(const std::string& text, const std::string& language);
    std::vector<Vector> getDocumentVectors(const std::vector<std::string> &sentences, const std::string &language);
    Vector getWordVector(const std::string &word, const std::string &language);
    void setSourceDocument(const std::vector<std::string> &source_document, const std::string &language);
    void setThreshold(const double &threshold);
    void setVectorSize(const int &size);
    std::vector<pairwisePlagIndex> compareDocuments(std::vector<std::string> &target_document, const std::string &targetLanguage);
    std::vector<plagIndex> compareSentence(std::string& sentence, const std::string &targetLanguage);
    void setCrossLangEmbeddingMatrix(const std::string& filename, const std::string& sourceLanguage, const std::string& target_language);
    double crossLangWordSimilarity(const std::string &A, const std::string &B);
    double wordSimilarity(const std::string &A, const std::string &B, const std::string& language);
    const EmbeddingMatrix& getEmbeddingMatrixByLanguage(const std::string& language);
};

#endif //EMBEDDING_EMBEDDING_H
