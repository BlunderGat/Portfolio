#ifndef APPLICATION_H
#define APPLICATION_H

#include "array.h"

class Application {
public:
    Application();
    void run();

private:
    Array array;
    void showMenu();
    void input();
    void findSrAndSKO();
    void sortArray();
    void resizeArray();
    void setElement();
    void printArray();
};

#endif // APPLICATION_H
