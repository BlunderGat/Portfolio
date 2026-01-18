#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>
#include <numeric>
#include <functional>
#include <queue>
#include <string>
#include <complex>
#include <cmath>
#include <windows.h>
using namespace std;
void SetClipboardText(const string& text) {
    if (!OpenClipboard(nullptr)) {
        cerr << "Failed to open clipboard" << endl;
        return;
    }
    if (!EmptyClipboard()) {
        cerr << "Failed to empty clipboard" << endl;
        CloseClipboard();
        return;
    }
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hGlobal == nullptr) {
        cerr << "Failed to allocate global memory" << endl;
        CloseClipboard();
        return;
    }
    char* pGlobal = static_cast<char*>(GlobalLock(hGlobal));
    if (pGlobal == nullptr) {
        cerr << "Failed to lock global memory" << endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return;
    }
    memcpy(pGlobal, text.c_str(), text.size() + 1);
    GlobalUnlock(hGlobal);
    if (SetClipboardData(CF_TEXT, hGlobal) == nullptr) {
        cerr << "Failed to set clipboard data" << endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return;
    }
    CloseClipboard();
}

int main2(string modifiedPath){
    int cnt;
    cout << "The file is being processed: " << modifiedPath << endl;
    cout << endl;
    cout << "1 - LAB1" << "\t" << "2 - LAB2" << "\t" << "3 - LAB3" << "\t"  << "4 - START" << "\t" << "ELSE - BREAKE FILE" << endl;
    cout << endl;
    cout << "Select the option: ";
    cin >> cnt;
    cout << endl;
    SetClipboardText(modifiedPath);
    if(cnt == 1){
        system("cls");
        system("lab1.exe");
        system("cls");
        system("pause");
    }
    if(cnt == 2){
        system("cls");
        system("lab2.exe");
        system("cls");
        system("pause");
    }
    if(cnt == 3){
        system("cls");
        system("lab3.exe");
        system("cls");
        system("pause");
    }
    if(cnt == 4){
        system("cls");
        cout << "The file is running:" << modifiedPath << endl;
        system(modifiedPath.c_str());
        system("pause");
    }
    system("cls");
}
void replaceAll(string& str, const string& from, const string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
string normalizePath(const string& path) {
    vector<string> components;
    stringstream ss(path);
    string item;
    while (getline(ss, item, '\\')) {
        if (item == "..") {
            if (!components.empty()) {
                components.pop_back();
            }
        } else if (item != "." && !item.empty()) {
            components.push_back(item);
        }
    }
    string normalizedPath;
    for (const auto& component : components) {
        normalizedPath += component + "\\";
    }

    if (!normalizedPath.empty() && normalizedPath.back() == '\\') {
        normalizedPath.pop_back();
    }
    return normalizedPath;
}

void processFile(const string& filePath) {
    string modifiedPath = filePath;
    replaceAll(modifiedPath, "://", ":\\");
    replaceAll(modifiedPath, "/", "\\");
    modifiedPath = normalizePath(modifiedPath);
    system("cls");
    main2(modifiedPath);
}
string formatSize(off_t size) {
    const double kilobyte = 1024.0;
    const double megabyte = kilobyte * 1024.0;
    const double gigabyte = megabyte * 1024.0;
    stringstream stream;
    if (size >= gigabyte) {
        stream << fixed << setprecision(2) << size / gigabyte << "  GB";
    } else if (size >= megabyte) {
        stream << fixed << setprecision(2) << size / megabyte << "  MB";
    } else if (size >= kilobyte) {
        stream << fixed << setprecision(2) << size / kilobyte << "  KB";
    } else {
        stream << fixed << setprecision(2) << 0.10 << "  KB";
    }
    return stream.str();
}
int chooseFile(string path) {
    DIR *dir;
    struct dirent *entry;
    struct stat filestat;
    vector<pair<string, off_t>> fileNames;

    dir = opendir(path.c_str());
    cout << string(106, '-') << endl;
    int fileCounter = 1;
    while ((entry = readdir(dir)) != NULL) {
        string fullPath = path + "/" + entry->d_name;
        if (stat(fullPath.c_str(), &filestat) == -1) {
            continue;
        }
        if (S_ISDIR(filestat.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "...") != 0) {
                string folderName = entry->d_name;
                fileNames.push_back(make_pair(folderName + "/", 0));
                cout << "| " << left << setw(6) << dec << fileCounter << "| " << setw(76) << folderName + "/" << "| " << right << setw(16) << "DIR" << " |" << endl;
                fileCounter++;
            }
        } else {
            string fileName = entry->d_name;
            fileNames.push_back(make_pair(fileName, filestat.st_size));
            cout << "| " << left << setw(6) << dec << fileCounter << "| " << setw(76) << fileName << "| " << right << setw(16) << formatSize(filestat.st_size) << " |" << endl;
            fileCounter++;
        }
    }
    closedir(dir);
    string Ppath;
    cout << string(106, '-') << endl;
    replaceAll(path, "://", ":\\");
    replaceAll(path, "/", "\\");
    Ppath = normalizePath(path);
    cout << Ppath << endl;
    cout << "Select the file by entering the appropriate number: ";
    int selectedFileIndex;
    cin >> selectedFileIndex;
    selectedFileIndex = selectedFileIndex - 1;
    string selectedFileName = fileNames[selectedFileIndex].first;
    if (selectedFileName.back() == '/') {
        system("cls");
        return chooseFile(path + "/" + selectedFileName.substr(0, selectedFileName.size() - 1));
    } else {
        system("cls");
        string fullPath = path + "/" + selectedFileName;
        processFile(fullPath);
    }
    return 0;
}

int main() {
#ifdef _WIN32
    char driveLetter = 67;
    system("cls");
    string path = string(1, driveLetter) + ":/";
#else
    string path = "/";
#endif
    while(true){
        chooseFile(path);
    }
    return 0;
}
