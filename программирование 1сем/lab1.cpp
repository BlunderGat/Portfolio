#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>
#include <numeric>
#include <windows.h>
using namespace std;
string GetClipboardText() {
    if (!OpenClipboard(nullptr))
        return "";
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }
    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }
    string text(pszText);
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}

void count(const string& name, map<unsigned char, int>& ch){
    ifstream open_(name, ios::binary);
    const int buffer = 4096;
    char read[buffer];
    while(open_.read(read, buffer)){
        for(int i = 0; i < buffer; ++i){
            ch[static_cast<unsigned char>(read[i])]++;
        }
    }
    int last = open_.gcount();
    for(int i = 0; i < last; ++i){
        ch[static_cast<unsigned char>(read[i])]++;
    }
    open_.close();
}
void gistogram(const string& selectedFileName){
    map<unsigned char, int> ch;
    count(selectedFileName, ch);
    int maxCount = 0;
    for(const auto& pair : ch){
        maxCount = max(maxCount, pair.second);
    }
    for(int i = 0; i < 256; i += 16){                                                  
        cout << setw(5) <<  dec << i << " - " << setw(5) << i + 15 << "  |  ";
        bool Flag = false;
        for(int j = i; j < i + 16; j++){
            int sum = 0;
            for(int k = j; k < j + 1; k++){sum += ch[k];}
            if(sum > 0){
                Flag = true;
                int Sum = max(sum * 10 / maxCount, 1);
                cout << string(Sum, '*');
            }
        }
        if(!Flag){
		    cout << "*";
	    }
        cout << endl;
    }
    cout << endl;
}
void table(const string& name, int form){
    if(form != 0 && form != 1){
	exit(1);
    }
    map<unsigned char, int> ch;
    count(name, ch);
    for(int i = 0; i < 17 * 8 + 1; ++i){
	cout << "=";
    }
    cout << "\t" << endl;
    cout << "|\t";
    for(int i = 0; i < 16; ++i){
	cout << "|" << hex << uppercase << i << "\t";
    }
    cout << "|" << "\t" << endl;
    for(int i = 0; i < 17 * 8 + 1; ++i){
	cout << "=";
    }
    cout << "\t" << endl;
    for(int i = 0; i < 16; ++i){
        cout << "|" << hex << uppercase << i << "\t";
        for (int j = 0; j < 16; ++j){
            unsigned char Value = static_cast<unsigned char>((i << 4) + j);
            if(ch.find(Value) != ch.end()){
                double gran = static_cast<double>(ch[Value]) / accumulate(ch.begin(), ch.end(), 0,
                        [](int total, const pair<unsigned char, int>& p) { return total + p.second; });
                if(form == 1){
                    if(ch[Value] > 2500 || gran > 0.4){
                        cout << "|" << "\033[1;32m" << dec << ch[Value] << "\033[0m\t";
                    }
                    else{
                        cout << "|" << dec << ch[Value] << "\t";
                    }
                }
                else if(form == 0){
                    if(ch[Value] > 2500 || gran > 0.4){
                        cout << "|" << "\033[1;32m" << fixed << setprecision(3) << gran << "\033[0m\t";
                    }
                    else{
                        cout << "|" << fixed << setprecision(3) << gran << "\t";
                    }
                }
            }
            else{
                if(form == 1){
		            cout << "| \t";
		        }
                else if(form == 0){
		            cout << "| \t";
		        }
	        }
        }
        cout << "|" << "\t" << endl;
        for(int i = 0; i < 17 * 8 + 1; ++i){
            cout << "=";
	    }
        cout << "\t" << endl;
    }
}
int main(){
    setlocale(LC_ALL, "Russian");
    string FileName = GetClipboardText();
    int style;
    cout << "Введите формат отображения - 1 - int / 0 - процент / иначе выход: ";
    cin >> style;
    cout << endl;
    system("cls"); 
    while(1 != 0){
        cout << endl;
        gistogram(FileName);
        cout << endl;
        table(FileName, style);
        cout << endl;
        cout << "1 - int / 0 - процент / иначе выход: ";
        cin >> style;
        system("cls");}
    return 0;
}