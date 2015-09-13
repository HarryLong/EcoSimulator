#include "callback_listener.h"

CallbackListener::CallbackListener(std::function<void()> callback_function) :
    m_callback_function(callback_function)
{

}
CallbackListener::~CallbackListener()
{

}

void CallbackListener::complete()
{
    m_callback_function();
}
