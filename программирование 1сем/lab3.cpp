#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
void encodeFileToBmp(const string& inputFileName, const string& outputbmpName){
    ifstream inputFile(inputFileName, ios::binary);
    vector<unsigned char> buffer((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    inputFile.close();
    ofstream bmpFile(outputbmpName, ios::binary);
    bmpFile << "BM";
    int fileSize = 54 + buffer.size();
    bmpFile.write((char*)&fileSize, 4);
    bmpFile.write((char*)&fileSize, 4);
    int offset = 54;
    bmpFile.write((char*)&offset, 4);
    int headSize = 40, width = 500, height = (buffer.size() / 4 / width) + 1, pl = 1, colorD = 24, compression = 0, dataS = buffer.size(), horizontal = 2800, vertical = 2800, colors = 0, importantC = 0;
    bmpFile.write((char*)&headSize, 4);
    bmpFile.write((char*)&width, 4);
    bmpFile.write((char*)&height, 4);
    bmpFile.write((char*)&pl, 2);
    bmpFile.write((char*)&colorD, 2);
    bmpFile.write((char*)&compression, 4);
    bmpFile.write((char*)&dataS, 4);
    bmpFile.write((char*)&horizontal, 4);
    bmpFile.write((char*)&vertical, 4);
    bmpFile.write((char*)&colors, 4);
    bmpFile.write((char*)&importantC, 4);
    for(int i = 0; i < buffer.size(); i++){
        bmpFile.write((char*)&buffer[i], 1);
    }
    bmpFile.close();
}
void decodeBmpToFile(const string& inputbmpName, const string& outputFileName){
    ifstream bmpFile(inputbmpName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);
    bmpFile.seekg(18, ios::beg);
    int width;
    bmpFile.read((char*)&width, 4);
    bmpFile.seekg(22, ios::beg);
    int height;
    bmpFile.read((char*)&height, 4);
    bmpFile.seekg(54, ios::beg);
    vector<unsigned char> buffer(width * height * 4);
    bmpFile.read((char*)&buffer[0], buffer.size());
    outputFile.write((char*)&buffer[0], buffer.size());
    bmpFile.close();
    outputFile.close();
}
int main(){
    setlocale(LC_ALL, "Russian");
    string ffName = GetClipboardText();
    string ffName2 = GetClipboardText();
    string fileName = ffName2.substr(ffName2.find_last_of("\\") + 1);
    string BINfileName = ffName2.substr(ffName2.find_last_of("\\") + 1);
    string bmpName = BINfileName.substr(0, BINfileName.find_last_of('.')) + ".bmp";
    string newName = "N" + fileName;
    encodeFileToBmp(ffName, bmpName);
    decodeBmpToFile(bmpName, newName);
    return 0;
}
