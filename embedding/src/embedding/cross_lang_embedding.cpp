#include "cross_lang_embedding.h"

using namespace std;

CrossLangEmbedding* CrossLangEmbedding::instance = nullptr;

CrossLangEmbedding::CrossLangEmbedding(const std::string& filename,
                                       const std::vector<std::string> &sourceLines,
                                       const std::vector<std::string> &targetLines,
                                       const std::string& sourceLanguage,
                                       const std::string& targetLanguage) : Embedding() {
    std::pair<EmbeddingMatrix, EmbeddingMatrix> crossLangMatrices;
    if (filename.empty()) {
        crossLangMatrices = readEmbeddingMatrixFromStrings(sourceLines, targetLines);
    } else {
        crossLangMatrices = readEmbeddingMatrixFromFile(filename, sourceLanguage, targetLanguage);
    }
    mSourceEmbeddingMatrix = crossLangMatrices.first;
    mTargetEmbeddingMatrix = crossLangMatrices.second;
    mSourceLanguage = sourceLanguage;
    mTargetLanguage = targetLanguage;
}

CrossLangEmbedding* CrossLangEmbedding::getInstance(const std::string& filename,
                                                        const std::vector<std::string> &sourceLines,
                                                        const std::vector<std::string> &targetLines,
                                                        const std::string& sourceLanguage,
                                                        const std::string& targetLanguage)  {
    if (instance == 0)
    {
        instance = new CrossLangEmbedding(filename, sourceLines, targetLines,sourceLanguage, targetLanguage);
    }

    return instance;
}

std::pair<EmbeddingMatrix, EmbeddingMatrix> CrossLangEmbedding::readEmbeddingMatrixFromFile(const std::string &filename,
                                                                                            const std::string &sourceLanguage,
                                                                                            const std::string &targetLanguage) {
    std::string sourceEmbeddingFilename = filename + "." + sourceLanguage;
    std::string targetEmbeddingFilename = filename + "." + targetLanguage;
    return std::make_pair(Embedding::readEmbeddingMatrixFromFile(sourceEmbeddingFilename), Embedding::readEmbeddingMatrixFromFile(targetEmbeddingFilename));
}


std::pair<EmbeddingMatrix, EmbeddingMatrix> CrossLangEmbedding::readEmbeddingMatrixFromStrings(const std::vector<std::string> &sourceLines,
                                                                           const std::vector<std::string> &targetLines) {
    return std::make_pair(Embedding::readEmbeddingMatrixFromStrings(sourceLines), Embedding::readEmbeddingMatrixFromStrings(targetLines));
};

void CrossLangEmbedding::setEmbeddingMatrixByLanguage(const EmbeddingMatrix& embeddingMatrix, const std::string& language) {
    if (language == mSourceLanguage) {
        mSourceEmbeddingMatrix = embeddingMatrix;
    } else {
        mTargetEmbeddingMatrix = embeddingMatrix;
    }
}

const EmbeddingMatrix& CrossLangEmbedding::getEmbeddingMatrixByLanguage(const std::string& language) {
    if (language == mSourceLanguage) {
        return mSourceEmbeddingMatrix;
    } else {
        return mTargetEmbeddingMatrix;
    }
}

void CrossLangEmbedding::setSourceLanguage(const std::string& language) {
    mSourceLanguage = language;
}

std::string CrossLangEmbedding::getSourceLanguage() {
    return mSourceLanguage;
}

void CrossLangEmbedding::setTargetLanguage(const std::string& language) {
    mTargetLanguage = language;
}

std::string CrossLangEmbedding::getTargetLanguage() {
   return mTargetLanguage;
}

bool CrossLangEmbedding::contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix, const std::string& language) const {
    if (language == mSourceLanguage) {
        return Embedding::contains(key, mSourceEmbeddingMatrix);
    } else {
        return Embedding::contains(key, mTargetEmbeddingMatrix);
    }
}

double CrossLangEmbedding::getWordSimilarityValue(const std::string &A, const std::string &B,
                                                  const std::string &language) const {
    if (language == mSourceLanguage) {
        return Embedding::getWordSimilarityValue(A, B, mSourceEmbeddingMatrix);
    } else {
        return Embedding::getWordSimilarityValue(A, B, mTargetEmbeddingMatrix);
    }
}

double CrossLangEmbedding::getCrossLangWordSimilarityValue(const std::string &A, const std::string &B) const {
    if (Embedding::contains(A, mSourceEmbeddingMatrix) and Embedding::contains(B, mTargetEmbeddingMatrix)) {
        return getVectorSimilarityValue(mSourceEmbeddingMatrix.at(A),
                                        mTargetEmbeddingMatrix.at(B));
    } else {
        return Embedding::mNotContainsValue;
    }
}

Vector CrossLangEmbedding::getSentenceVector(const std::string& text, const std::string& language) const {
    if (language == mSourceLanguage) {
        return Embedding::getSentenceVector(text, mSourceEmbeddingMatrix);
    } else {
        return Embedding::getSentenceVector(text, mTargetEmbeddingMatrix);
    }
}

Vector CrossLangEmbedding::getWordVector(const std::string &word, const std::string &language) const {
    if (language == mSourceLanguage) {
        return Embedding::getWordVector(word, mSourceEmbeddingMatrix);
    } else {
        return Embedding::getWordVector(word, mTargetEmbeddingMatrix);
    }
}