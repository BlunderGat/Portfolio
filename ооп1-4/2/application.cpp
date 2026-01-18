#include "application.h"
#include <conio.h>
#include <iostream>

using namespace std;
Application::Application() : array(5,0) {}

void Application::run(){
    bool isRun = true;
    while (isRun) {
        showMenu();
        input();
    }
}

void Application::showMenu(){
    cout << "Commands: \n";
    cout << "1. Enter values \n";
    cout << "2. Find average and SKO \n";
    cout << "3. Sort array \n";
    cout << "4. Change size \n";
    cout << "5. Change element \n";
    cout << "6. Print array \n";
    cout << "7. Exit \n";
}

void Application::input() {
    int command;
    cin >> command;
    switch(command){
    case 1:
        //array.in(array.getSize());
        cin >> array;

        cout << "Array saved. \nPress any button to continue" << endl;
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
        exit(0);
    default:
        cout<<"wrong command!\n";
    }
}
void Application::findSrAndSKO() {
    cout<<"Average: " << array.findSr() << endl;
    cout<<"SKO: " << array.findSKO() << endl;
    cout << "Press any button" << endl;
    _getch();
    system("cls");
}

void Application::sortArray(){
    int choice;
    cout << "choose the way of sorting (1 - DownToUp, 2 - UpToDown)" << endl;
    cin>>choice;
    array.Sort(choice==1);
    cout << "Array sorted!" << endl;
    cout<<"Press any button" << endl;
    _getch();
    system("cls");
}

void Application::resizeArray() {
    int newSize;
    cout<<"Enter new size"<< endl;
    cin>>newSize;
    array.Resize(newSize);
    cout << "New size of array is " << newSize << endl;
    cout << "Press any button" << endl;
    _getch();
    system("cls");
}

void Application::setElement(){
    int index;
    number value;
    cout<<"Enter index: ";
    cin>>index;
    cout<<"Enter value: ";
    cin>>value;
    array.setElement(index, value);
    cout << "Element at index " << index << " is " << value;
    cout << "\nPress any button" << endl;
    _getch();
    system("cls");
}

void Application::printArray(){
    //cout << "size of array is " << array.getSize() << endl;
    cout << array;
    cout << "Press any button" << endl;
    _getch();
    system("cls");
}
