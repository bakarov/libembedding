#define BOOST_TEST_MODULE embedding_test

#include "../embedding/cross_lang_embedding.h"
#include "../paraphrase_search/paraphrase_search.h"
#include <boost/test/included/unit_test.hpp>

namespace bunt = boost::unit_test;

using namespace std;

struct Fixture {
    Fixture() {}

    ~Fixture() {}
};

BOOST_FIXTURE_TEST_CASE(embedding_test, Fixture) {

    // Primary initialization
    std::vector<std::string> sourceLines = {"кошка 1 2 3 3", "собака 3 4 5 8", "кошкособака 9 8 7 6"};
    std::vector<std::string> targetLines = {"cat 1 2 3 4", "dog 3 4 5 7", "catdog 9 8 7 6"};
    std::string sourceLanguage = "ru";
    std::string targetLanguage = "en";
    std::string embeddingFileName;
    CrossLangEmbedding* testEmbedding = CrossLangEmbedding::getInstance(embeddingFileName, sourceLines, targetLines, sourceLanguage, targetLanguage);

    // Similarity between single words
    std::string wordCatEn = "cat";
    std::string wordCatRu = "кошка";
    std::string wordDogEn = "dog";
    std::string wordDogRu = "собака";
    std::string wordCatDogEn = "cat dog";
    std::string wordMergedCatDogEn = "catdog";
    std::string wordCatDogRu = "кошка собака";
    std::string wordTestEn = "test";
    std::string wordTestRu = "тест";
    double beatifulMultiplier = 1000;

    double similarityEn = std::floor(double(testEmbedding->getWordSimilarityValue(wordCatEn, wordDogEn, targetLanguage))*beatifulMultiplier);
    double similarityRu = std::floor(double(testEmbedding->getWordSimilarityValue(wordCatRu, wordDogRu, sourceLanguage))*beatifulMultiplier);
    double trueSimilarityEn = 9;
    double trueSimilarityRu = 23;

    BOOST_TEST(similarityEn == 9);
    BOOST_TEST(similarityRu == 23);

    // Similarity between sentences
    Vector sentenceVector = testEmbedding->getSentenceVector(wordCatDogEn, targetLanguage);
    Vector catdogVector = testEmbedding->getWordVector(wordMergedCatDogEn, targetLanguage);
    double similarity_2 = std::floor(double(testEmbedding->getVectorSimilarityValue(catdogVector, sentenceVector))*beatifulMultiplier);

    // Compare documents
    std::vector<std::string> sourceDocument = {wordTestRu, wordTestRu, wordCatDogRu};
    std::vector<std::string> targetDocument = {wordCatDogEn, wordTestEn, wordTestEn};
    double threshold = 0.3;
    const CrossLangEmbedding* testConstEmbedding = CrossLangEmbedding::getInstance(embeddingFileName, sourceLines, targetLines, sourceLanguage, targetLanguage);
    ParaphraseSearch* paraphraseSearch = new ParaphraseSearch(*testConstEmbedding, sourceDocument, sourceLanguage);
    paraphraseSearch->setThreshold(threshold);


    paraphraseSearch->setSourceDocumentVectors(sourceDocument, sourceLanguage);
    std::vector<pairwisePlagIndex> result = paraphraseSearch->compareDocuments(targetDocument, targetLanguage);
    std::string cat_dog = "cat dog";
    std::vector<plagIndex> result_2 = paraphraseSearch->compareSentence(cat_dog, targetLanguage);
    double result_similarity = std::floor(result_2[0].similarity*beatifulMultiplier);


    BOOST_TEST(similarity_2 == 117);
    BOOST_TEST(result.size() == 1);
    BOOST_TEST(result[0].sourceIndex == 2);
    BOOST_TEST(result[0].targetIndex == 0);
    BOOST_TEST(result_2.size() == 1);
    BOOST_TEST(result_2[0].sourceIndex == 2);
    BOOST_TEST(result[0].similarity == 0);
    BOOST_TEST(testEmbedding->getVectorSize() == 4);
}