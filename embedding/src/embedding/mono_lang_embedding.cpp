#include "mono_lang_embedding.h"

using namespace std;

MonoLangEmbedding* MonoLangEmbedding::instance = nullptr;

MonoLangEmbedding* MonoLangEmbedding::getInstance()
{
    if (instance == 0)
    {
        instance = new MonoLangEmbedding();
    }

    return instance;
}

void MonoLangEmbedding::setLanguage(const std::string& language) {
    mLanguage = language;
}

std::string MonoLangEmbedding::getLanguage() {
    return mLanguage;
}

void MonoLangEmbedding::setEmbeddingMatrix(const EmbeddingMatrix& embeddingMatrix) {
    mEmbeddingMatrix = embeddingMatrix;
}

EmbeddingMatrix MonoLangEmbedding::getEmbeddingMatrix() {
    return mEmbeddingMatrix;
}
