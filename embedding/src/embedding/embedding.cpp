#include "embedding.h"

Vector operator+=(Vector& a, const Vector& b)
{
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

Vector operator+=(Vector& aVector, const VectorValue& aObject)
{
    aVector.push_back(aObject);
    return aVector;
}

Vector operator+(Vector& a, Vector& b)
{
    Vector result;
    result.reserve(a.size());
    std::transform(a.begin(), a.end(), b.begin(),
                   std::back_inserter(result), std::plus<VectorValue>());
    return result;
}

Embedding* Embedding::instance = nullptr;

Embedding* Embedding::getInstance()
{
    if (instance == 0)
    {
        instance = new Embedding();
    }

    return instance;
}

void Embedding::setVectorSize(const int &size) {
    mVectorSize = size;
    mEmptyVector = Vector(mVectorSize);
}

int Embedding::getVectorSize() {
    return mVectorSize;
}

void Embedding::setVocabSize(const int &size) {
    mVocabSize = size;
}

int Embedding::getVocabSize() {
    return mVocabSize;
}

bool Embedding::contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix) const {
    auto it = embeddingMatrix.find(key);
    if (it == embeddingMatrix.end()) {
        return false;
    }
    return true;
}

EmbeddingMatrix Embedding::readEmbeddingMatrixFromFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    std::string line;
    EmbeddingMatrix embeddingMatrix;

    if (inputFile.good()) {
        while (getline(inputFile, line)) {
            std::vector<string> splittedLine = splitString(line, ' ');

            VectorName word = splittedLine[0];
            Vector numbers;

            std::transform(splittedLine.begin() + 1, splittedLine.end(), std::back_inserter(numbers),
                           [](const std::string& str) { return std::stoi(str); });
            int numbersSize = numbers.size();
            setVectorSize(numbersSize);
            embeddingMatrix[word] = numbers;
        }
        inputFile.close();
    } else {
        cout << "Error opening file!";
        exit(0);
    }
    return embeddingMatrix;
}


double Embedding::getVectorSimilarityValue(const Vector &A, const Vector &B) const {
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
    for(unsigned int i = 0u; i < A.size(); ++i) {
        dot += A[i] * B[i] ;
        denom_a += A[i] * A[i] ;
        denom_b += B[i] * B[i] ;
    }
    return 1 - dot / (sqrt(denom_a) * sqrt(denom_b));
}

double Embedding::getWordSimilarityValue(const std::string &A, const std::string &B, const EmbeddingMatrix& embeddingMatrix) const {
    if (contains(A, embeddingMatrix) and contains(B, embeddingMatrix)) {
        return getVectorSimilarityValue(embeddingMatrix.at(A), embeddingMatrix.at(B));
    } else {
        return mNotContainsValue;
    }

}

Vector Embedding::getSentenceVector(const std::string &text, const EmbeddingMatrix& embeddingMatrix) const {
    Vector composed_vector(mVectorSize); // shape of the vector
    for (auto word: splitString(text, ' ')) {
        if (contains(word, embeddingMatrix)) {
            Vector wordVector = embeddingMatrix.at(word);
            composed_vector = composed_vector + wordVector;
        }
    }
    return composed_vector;
}


Vector Embedding::getWordVector(const std::string &word, const EmbeddingMatrix& embeddingMatrix) const {
    if (contains(word, embeddingMatrix)) {
        return embeddingMatrix.at(word);
    } else {
        return mEmptyVector;
    }
}

std::vector<std::string> Embedding::splitString(const std::string &s, const char delim) const {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(std::back_inserter(elems)++) = item;
    }
    return elems;
}