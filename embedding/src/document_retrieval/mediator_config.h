#ifndef EMBEDDING_MEDIATOR_CONFIG_H
#define EMBEDDING_MEDIATOR_CONFIG_H

#include <iostream>

using namespace std;


class MediatorConfig {
public:
    MediatorConfig();

    MediatorConfig getMediatorConfig(const std::string& sourceLanguage, const std::string& targetLanguage) const;
    std::string searchMany(const std::string& supportDoc) const;
};


#endif
