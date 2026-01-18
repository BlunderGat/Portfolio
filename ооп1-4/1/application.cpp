#include "application.h"
#include <conio.h>
#include <iostream>

Application::Application() : array(5,0) {}

void Application::run(){
    bool isRun = true;
    while (isRun) {
        showMenu();
        input();
    }
}

void Application::showMenu(){
    std::cout << "Commands: \n";
    std::cout << "1. Enter values \n";
    std::cout << "2. Find average and SKO \n";
    std::cout << "3. Sort array \n";
    std::cout << "4. Change size \n";
    std::cout << "5. Change element \n";
    std::cout << "6. Print array \n";
    std::cout << "7. Exit \n";
}

void Application::input() {
    int command;
    std::cin >> command;
    switch(command){
    case 1:
        //array.in(array.getSize());
        std::cin >> array;

        std::cout << "Array saved. \nPress any button to continue" << std::endl;
        _getch();
        system("cls");
        break;
    case 2:
        findSrAndSKO();
        break;
    case 3:
        sortArray();
        break;
    case 4:
        resizeArray();
        break;
    case 5:
        setElement();
        break;
    case 6:
        printArray();
        break;
    case 7:
        std::exit(0);
    default:
        std::cout<<"wrong command!\n";
    }
}
void Application::findSrAndSKO() {
    std::cout<<"Average: " << array.findSr() << std::endl;
    std::cout<<"SKO: " << array.findSKO() << std::endl;
    std::cout << "Press any button" << std::endl;
    _getch();
    system("cls");
}

void Application::sortArray(){
    int choice;
    std::cout << "choose the way of sorting (1 - DownToUp, 2 - UpToDown)" << std::endl;
    std::cin>>choice;
    array.Sort(choice==1);
    std::cout << "Array sorted!" << std::endl;
    std::cout<<"Press any button" << std::endl;
    _getch();
    system("cls");
}

void Application::resizeArray() {
    int newSize;
    std::cout<<"Enter new size"<< std::endl;
    std::cin>>newSize;
    array.Resize(newSize);
    std::cout << "New size of array is " << newSize << std::endl;
    std::cout << "Press any button" << std::endl;
    _getch();
    system("cls");
}

void Application::setElement(){
    int index;
    number value;
    std::cout<<"Enter index: ";
    std::cin>>index;
    std::cout<<"Enter value: ";
    std::cin>>value;
    array.setElement(index, value);
    std::cout << "Element at index " << index << " is " << value;
    std::cout << "\nPress any button" << std::endl;
    _getch();
    system("cls");
}

void Application::printArray(){
    //std::cout << "size of array is " << array.getSize() << std::endl;
    std::cout << array;
    std::cout << "Press any button" << std::endl;
    _getch();
    system("cls");
}
