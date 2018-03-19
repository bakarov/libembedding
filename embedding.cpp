#include "embedding.h"

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

crossLangEmbeddingMatrix createCrossLangEmbeddingMatrix(const EmbeddingMatrix& sourceMatrix,
                                                        const EmbeddingMatrix& targetMatrix, const std::string& sourceLanguage,
                                                        const std::string& targetLanguage) {
    crossLangEmbeddingMatrix matrix;
    matrix.sourceEmbeddingMatrix = sourceMatrix;
    matrix.targetEmbeddingMatrix = targetMatrix;
    matrix.sourceLanguage = sourceLanguage;
    matrix.targetLanguage = targetLanguage;
    return matrix;
};


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

std::vector<std::string> Embedding::splitString(const std::string &s, const char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(std::back_inserter(elems)++) = item;
    }
    return elems;
}

bool Embedding::contains(const VectorName& key, const EmbeddingMatrix& embeddingMatrix) {
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

void Embedding::setCrossLangEmbeddingMatrix(const std::string& filename, const std::string& sourceLanguage,
                                            const std::string& targetLanguage) {
    std::string sourceEmbeddingFilename = filename + "." + sourceLanguage;
    std::string targetEmbeddingFilename = filename + "." + targetLanguage;

    EmbeddingMatrix sourceEmbeddingMatrix = readEmbeddingMatrixFromFile(sourceEmbeddingFilename);
    EmbeddingMatrix targetEmbeddingMatrix = readEmbeddingMatrixFromFile(targetEmbeddingFilename);

    mCrossLangWordVectors =  createCrossLangEmbeddingMatrix(sourceEmbeddingMatrix, targetEmbeddingMatrix, sourceLanguage, targetLanguage);
}

double Embedding::cosineVectorSimilarity(const Vector &A, const Vector &B) {
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
    for(unsigned int i = 0u; i < A.size(); ++i) {
        dot += A[i] * B[i] ;
        denom_a += A[i] * A[i] ;
        denom_b += B[i] * B[i] ;
    }
    return 1 - dot / (sqrt(denom_a) * sqrt(denom_b)) ;
}

double Embedding::wordSimilarity(const std::string &A, const std::string &B, const std::string &language) {
    const EmbeddingMatrix& embeddingMatrix = getEmbeddingMatrixByLanguage(language);
    if (contains(A, embeddingMatrix) and contains(B, embeddingMatrix)) {
        return cosineVectorSimilarity(embeddingMatrix.at(A), embeddingMatrix.at(B));
    } else {
        return 2;
    }

}

double Embedding::crossLangWordSimilarity(const std::string &A, const std::string &B) {
    return cosineVectorSimilarity(mCrossLangWordVectors.sourceEmbeddingMatrix[A],
                                  mCrossLangWordVectors.targetEmbeddingMatrix[B]);
}

const EmbeddingMatrix& Embedding::getEmbeddingMatrixByLanguage(const std::string& language) {
    if (language == mCrossLangWordVectors.sourceLanguage) {
        return mCrossLangWordVectors.sourceEmbeddingMatrix;
    } else {
        return mCrossLangWordVectors.targetEmbeddingMatrix;
    }
}

Vector Embedding::getSentenceVector(const std::string &text, const std::string &language) {
    EmbeddingMatrix embeddingMatrix = getEmbeddingMatrixByLanguage(language);
    Vector composed_vector(mVectorSize); // shape of the vector
    for (auto i: splitString(text, ' ')) {
        if (contains(i, embeddingMatrix)) {
            composed_vector = composed_vector + embeddingMatrix.at(i);
        }
    }
    return composed_vector;
}

std::vector<Vector> Embedding::getDocumentVectors(const std::vector<std::string> &sentences, const std::string &language) {
    std::vector<Vector> documentVectors;
    for (auto sent: sentences) {
        documentVectors.push_back(getSentenceVector(sent, language));
    }
    return documentVectors;
}

void Embedding::setSourceDocument(const std::vector<std::string> &source_document, const std::string &language) {
    mSourceDocumentVectors = getDocumentVectors(source_document, language);
    mSourceLanguage = language;
}

void Embedding::setThreshold(const double &threshold) {
    mSimilarityThreshold = threshold;
}

Vector Embedding::getWordVector(const std::string &word, const std::string &language) {
    EmbeddingMatrix embeddingMatrix = getEmbeddingMatrixByLanguage(language);
    if (contains(word, embeddingMatrix)) {
        return embeddingMatrix.at(word);
    } else {
        return Vector(mVectorSize);
    }

}

void Embedding::setVectorSize(const int &size) {
    mVectorSize = size;
}

int Embedding::getVectorSize() {
    return mVectorSize;
}

std::vector<pairwisePlagIndex> Embedding::compareDocuments(std::vector<std::string> &targetDocument, const std::string& targetLanguage) {
    std::vector<Vector> targetDocumentVectors = getDocumentVectors(targetDocument, targetLanguage);
    std::vector<pairwisePlagIndex> indices;

    for (auto& sourceVectorIterator: mSourceDocumentVectors) {
        for (auto& targetVectorIterator: targetDocumentVectors) {
            Vector sourceVector = sourceVectorIterator;
            Vector targetVector = targetVectorIterator;

            int sourcePos = &sourceVectorIterator-&mSourceDocumentVectors[0];
            int targetPos = &targetVectorIterator-&targetDocumentVectors[0];

            double similarity = cosineVectorSimilarity(sourceVector, targetVector);

            if (similarity < mSimilarityThreshold) {
                indices.push_back(createPairwisePlagIndices(sourcePos, targetPos, similarity));
            }
        }
    }
    return indices;
}

std::vector<plagIndex> Embedding::compareSentence(std::string &sentence, const std::string &targetLanguage) {
    std::vector<plagIndex> indices;
    Vector targetVector = getSentenceVector(sentence, targetLanguage);

    for (auto& sourceVectorIterator: mSourceDocumentVectors) {
        Vector sourceVector = sourceVectorIterator;

        int sourcePos = &sourceVectorIterator-&mSourceDocumentVectors[0];
        double similarity = cosineVectorSimilarity(sourceVector, targetVector);

        if (cosineVectorSimilarity(sourceVector, targetVector) < mSimilarityThreshold) {
            indices.push_back(createTargetPlagIndex(sourcePos, similarity));
        }
    }
    return indices;
}