#ifndef GPTHELPER_H
#define GPTHELPER_H
#include "liboai/include/liboai.h"
class GPTHelper
{
    GPTHelper();
    ~GPTHelper();

public:
    void SetAPIKey(std::string key);
    void Ask(std::string question);
private:
    bool IsKeySet();
    liboai::OpenAI oai;
    const std::string defaultModel = "gpt-3.5-turbo";
};

#endif // GPTHELPER_H
