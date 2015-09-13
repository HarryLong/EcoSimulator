#ifndef CALLBACK_LISTENER_H
#define CALLBACK_LISTENER_H

#include <functional>

class CallbackListener
{
public:
    CallbackListener(std::function<void()> callback_function);
    ~CallbackListener();
    void complete();

private:
    std::function<void()> m_callback_function;
};

#endif // CALLBACK_LISTENER_H
