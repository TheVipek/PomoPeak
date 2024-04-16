#ifndef QOBJECTINITIALIZATION_H
#define QOBJECTINITIALIZATION_H
class QObjectInitialization
{
protected:
    virtual void InitializeDataContainer() {};
    virtual void InitializeObjects() {};
    virtual void SubscribeToEvents() {};
    virtual void UnsubscribeFromEvents() {};
    virtual ~QObjectInitialization() = 0;
};

inline QObjectInitialization::~QObjectInitialization() {};

#endif // QOBJECTINITIALIZATION_H
