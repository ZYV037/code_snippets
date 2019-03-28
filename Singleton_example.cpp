#pragma once

/*  Best Singleton
    noncopyable
    use stack, not new from heap
    easy use
*/

#define  Declare_Singleton(classname) \
public: \
static classname& Instance() \
{ \
    static classname _instance; \
    return _instance; \
} \
protected: \
    classname() = default; \
    ~classname() = default; \
                            \
    classname(const classname& rInstance); \
    classname& operator = (const classname& rInstance) \


class XmlConfigManager
{
    Declare_Singleton(XmlConfigManager);

public:
    bool add(int a, int b){ return a+b;};
};

#define XmlManager (XmlConfigManager::Instance())

#include <iostream>

int main()
{
    std::cout << XmlManager.add(1, 2) << std::endl;
    return 0;
}
