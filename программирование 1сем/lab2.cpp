#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
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
class node{
public:
    char data;
    int freq;
    node* left;
    node* right;
    node(char data, int freq){
        this->data = data;
        this->freq = freq;
        left = nullptr;
        right = nullptr;
    }
    node(char data, int freq, node* left, node* right){
        this->data = data;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};
struct compare{ 
    bool operator() (node* a, node* b){
        return a->freq > b->freq;
    }
};
node* buildhuffmantree(map<char, int>& freqmap){
    priority_queue<node*, vector<node*>, compare> pq;
    for (auto it = freqmap.begin(); it != freqmap.end(); it++){
        pq.push(new node(it->first, it->second));
    }
    while (pq.size() != 1){
        node* left = pq.top();
        pq.pop();
        node* right = pq.top();
        pq.pop();
        node* combined = new node('\0', left->freq + right->freq, left, right);
        pq.push(combined);
    }
    return pq.top();
}
void getcode(node* root, string code, map<char, string>& codes){
    if(!root){
        return;
    }
    if(!root->left && !root->right){
        codes[root->data] = code;
    }
    getcode(root->left, code + "0", codes);
    getcode(root->right, code + "1", codes);
}
void compressfile(ifstream& inputfile, ofstream& outputfile, map<char, string>& codes){
    char c;
    while (inputfile.get(c)){
        outputfile << codes[c];
    }
}
void decompressfile(ifstream& inputfile, ofstream& outputfile, node* root){
    node* current = root;
    char bit;
    while(inputfile.get(bit)){
        if(bit == '0'){
            current = current->left;
        }
        else{
            current = current->right;
        }
        if(!current->left && !current->right){
            outputfile << current->data;
            current = root;
        }
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    string inputfile = GetClipboardText();;
    string inputfile2 = GetClipboardText();;
    string compressedfile;
    string decompressedfile;
    ifstream input(inputfile, ios::binary);
    map<char, int> freqmap;
    char c;
    while (input.get(c)) {
        freqmap[c]++;
    }
    node* root = buildhuffmantree(freqmap);
    map<char, string> codes;
    getcode(root, "", codes);
    input.clear();
    input.seekg(0);
    string BINfileName = inputfile2.substr(inputfile2.find_last_of("\\") + 1);
    compressedfile = BINfileName.substr(0, BINfileName.find_last_of('.')) + ".bin";
    ofstream compressed(compressedfile, ios::binary);
    compressfile(input, compressed, codes);
    compressed.close();
    input.close();
    string fileName = inputfile.substr(inputfile.find_last_of("\\") + 1);
    decompressedfile = "N" + fileName;
    ifstream compressedinput(compressedfile, ios::binary);
    ofstream decompressed(decompressedfile, ios::binary);
    decompressfile(compressedinput, decompressed, root);
    compressedinput.close();
    decompressed.close();
    return 0;
}