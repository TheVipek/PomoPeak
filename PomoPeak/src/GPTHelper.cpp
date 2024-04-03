#include "GPTHelper.h"

GPTHelper::GPTHelper()
{

}
GPTHelper::~GPTHelper()
{

}

std::string GPTHelper::Ask(std::string question)
{
    try
    {
        if(IsKeySet())
        {
            liboai::Conversation conversation;
            conversation.AddUserData(question);

            auto x = oai.ChatCompletion->create_async(defaultModel, conversation);
            x.wait();

            auto response = x.get();

            return response.content;
        }
        throw std::invalid_argument("API key is not assigned.");
    }
    catch(std::exception& ex)
    {
        return ex.what();
    }
}

bool GPTHelper::IsKeySet()
{
    return !oai.auth.GetKey().empty();
}
