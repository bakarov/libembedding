#define BOOST_TEST_MODULE embedding_test

#include "embedding.h"
#include <boost/test/included/unit_test.hpp>

namespace bunt = boost::unit_test;

using namespace std;

struct Fixture {
    Fixture() {}

    ~Fixture() {}
};

BOOST_FIXTURE_TEST_CASE(embedding_test, Fixture) {

    // Primary initialization
    double threshold = 0.3;
    std::string sourceLanguage = "ru";
    std::string targetLanguage = "en";

    Embedding* test_embedding = Embedding::getInstance();
    test_embedding->setCrossLangEmbeddingMatrix("embedding", "ru", "en");
    test_embedding->setThreshold(threshold);

    // Similarity between single words
    double similarity_en = std::floor(double(test_embedding->wordSimilarity("cat", "dog", targetLanguage))*1000);
    double similarity_ru = std::floor(double(test_embedding->wordSimilarity("кошка", "собака", sourceLanguage))*1000);

    // Similarity between sentences
    Vector sentence_vector = test_embedding->getSentenceVector("cat dog", targetLanguage);
    std::string catdog = "catdog";
    Vector catdog_vector = test_embedding->getWordVector(catdog, targetLanguage);
    double similarity_2 = std::floor(double(test_embedding->cosineVectorSimilarity(catdog_vector, sentence_vector))*1000);

    // Compare documents
    std::vector<std::string> source_document = {"тест", "тест", "собака кошка"};
    std::vector<std::string> target_document = {"dog cat", "test", "test"};
    test_embedding->setSourceDocument(source_document, sourceLanguage);
    std::vector<pairwisePlagIndex> result = test_embedding->compareDocuments(target_document, targetLanguage);
    std::string cat_dog = "cat dog";
    std::vector<plagIndex> result_2 = test_embedding->compareSentence(cat_dog, targetLanguage);
    double result_similarity = std::floor(result_2[0].similarity*1000);

    BOOST_TEST(similarity_en == 9);
    BOOST_TEST(similarity_ru == 23);
    BOOST_TEST(similarity_2 == 117);
    BOOST_TEST(result.size() == 1);
    BOOST_TEST(result[0].sourceIndex == 2);
    BOOST_TEST(result[0].targetIndex == 0);
    BOOST_TEST(result_2.size() == 1);
    BOOST_TEST(result_2[0].sourceIndex == 2);
    BOOST_TEST(result[0].similarity == 0);
    BOOST_TEST(test_embedding->getVectorSize() == 4);
}