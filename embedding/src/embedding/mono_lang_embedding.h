#ifndef EMBEDDING_MONO_LANG_EMBEDDING_H
#define EMBEDDING_MONO_LANG_EMBEDDING_H

#include "embedding.h"

using namespace std;

class MonoLangEmbedding: public Embedding {
public:
    static MonoLangEmbedding* getInstance();

    void setLanguage(const std::string& language);
    std::string getLanguage();

    void setEmbeddingMatrix(const EmbeddingMatrix& embeddingMatrix);
    EmbeddingMatrix getEmbeddingMatrix();
private:
    static MonoLangEmbedding* instance;
    EmbeddingMatrix mEmbeddingMatrix;
    std::string mLanguage;
};


#endif //EMBEDDING_MONO_LANG_EMBEDDING_H
