#ifndef GPTHELPER_H
#define GPTHELPER_H
#include "liboai/include/liboai.h"
class GPTHelper
{
    GPTHelper();
    ~GPTHelper();

public:
    void SetAPIKey(std::string key);

private:
    liboai::OpenAI oai;
    bool isKeySet;
    const std::string defaultModel = "gpt-3.5-turbo";
};

#endif // GPTHELPER_H
