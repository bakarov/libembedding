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

class Embedding
{
public:
    static Embedding* getInstance();

    void setVectorSize(const int &size);
    int getVectorSize();

    void setVocabSize(const int &size);
    int getVocabSize();

    double getVectorSimilarityValue(const Vector &A, const Vector &B) const;

    virtual EmbeddingMatrix readEmbeddingMatrixFromFile(const std::string &filename);
    virtual EmbeddingMatrix readEmbeddingMatrixFromStrings(const std::vector<std::string> &lines);
    virtual bool contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix) const;
    virtual double getWordSimilarityValue(const std::string &A, const std::string &B, const EmbeddingMatrix& embeddingMatrix) const;
    virtual Vector getSentenceVector(const std::string& text, const EmbeddingMatrix& embeddingMatrix) const;
    virtual Vector getWordVector(const std::string &word, const EmbeddingMatrix& embeddingMatrix) const;
private:
    static Embedding* instance;
    int mVectorSize;
    int mVocabSize;

    std::vector<std::string> splitString(const std::string &s, const char delim) const;
protected:
    double mNotContainsValue = 2.0;
    Vector mEmptyVector;
};

#endif //EMBEDDING_EMBEDDING_H
