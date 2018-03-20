#include "paraphrase_search.h"

using namespace std;

pairwisePlagIndex createPairwisePlagIndices(const int& sourceDocIndex, const int& targetDocIndex, const double& sim) {
    pairwisePlagIndex indices;
    indices.sourceIndex = sourceDocIndex;
    indices.targetIndex = targetDocIndex;
    indices.similarity = sim;
    return indices;
};

plagIndex createTargetPlagIndex(const int& sourceDocIndex, const double& similarity) {
    plagIndex indices;
    indices.sourceIndex = sourceDocIndex;
    indices.similarity = similarity;
    return indices;
};

ParaphraseSearch::ParaphraseSearch(const CrossLangEmbedding& crossLangEmbedding,
                                   const std::vector<std::string> &source_document,
                                   const std::string &language):
        mCrossLangEmbedding(crossLangEmbedding) {
    setSourceDocumentVectors(source_document, language);
}


CrossLangEmbedding ParaphraseSearch::getCrossLangEmbedding() {
    return mCrossLangEmbedding;
}

std::vector<pairwisePlagIndex> ParaphraseSearch::compareDocuments(std::vector<std::string> &targetDocument,
                                                                  const std::string& targetLanguage) {
    std::vector<Vector> targetDocumentVectors = getDocumentVectors(targetDocument, targetLanguage);
    std::vector<pairwisePlagIndex> indices;

    for (auto& sourceVectorIterator: mSourceDocumentVectors) {
        for (auto& targetVectorIterator: targetDocumentVectors) {
            Vector sourceVector = sourceVectorIterator;
            Vector targetVector = targetVectorIterator;

            int sourcePos = &sourceVectorIterator-&mSourceDocumentVectors[0];
            int targetPos = &targetVectorIterator-&targetDocumentVectors[0];

            double similarity = mCrossLangEmbedding.getVectorSimilarityValue(sourceVector, targetVector);

            if (similarity < mSimilarityThreshold) {
                indices.push_back(createPairwisePlagIndices(sourcePos, targetPos, similarity));
            }
        }
    }
    return indices;
}

std::vector<plagIndex> ParaphraseSearch::compareSentence(std::string &sentence,
                                                         const std::string &targetLanguage) {
    std::vector<plagIndex> indices;
    Vector targetVector = mCrossLangEmbedding.getSentenceVector(sentence, targetLanguage);

    for (auto& sourceVectorIterator: mSourceDocumentVectors) {
        Vector sourceVector = sourceVectorIterator;

        int sourcePos = &sourceVectorIterator-&mSourceDocumentVectors[0];
        double similarity = mCrossLangEmbedding.getVectorSimilarityValue(sourceVector, targetVector);

        if (mCrossLangEmbedding.getVectorSimilarityValue(sourceVector, targetVector) < mSimilarityThreshold) {
            indices.push_back(createTargetPlagIndex(sourcePos, similarity));
        }
    }
    return indices;
}

void ParaphraseSearch::setSourceDocumentVectors(const std::vector<std::string> &source_document, const std::string &language) {
    mSourceDocumentVectors = getDocumentVectors(source_document, language);
}

std::vector<Vector> ParaphraseSearch::getDocumentVectors(const std::vector<std::string> &sentences, const std::string &language) {
    std::vector<Vector> documentVectors;
    for (auto sent: sentences) {
        documentVectors.push_back(mCrossLangEmbedding.getSentenceVector(sent, language));
    }
    return documentVectors;
}

void ParaphraseSearch::setThreshold(const double &threshold) {
    mSimilarityThreshold = threshold;
}

double ParaphraseSearch::getThreshold() {
    return mSimilarityThreshold;
}