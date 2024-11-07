#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;

string outputLog = "";

int dumpLogs(){
    cout << outputLog;
    ofstream output;
    output.open("output.txt");
    output << outputLog;
    output.close();
    return 0;
}
int main(int argc, char *argv[]){ // CLargs in case i want to do stuff with it later
    outputLog += "Evil code is running.\n"; // startup message
    char buffer[MAX_PATH];
    string currentPath = "";
    string targetFile = "straycatj.jpg"; // hard coded for now, could do with CLargs or encrypted stuff later
    const CHAR* keyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP";
    if (GetModuleFileNameA(NULL, buffer, MAX_PATH)) {
        currentPath = buffer; // get our current path
        currentPath = currentPath.substr(0, currentPath.find_last_of("\\/") + 1);
        targetFile = currentPath + targetFile;
        // cout << "TEST: Filename = " << targetFile << endl;
    }
    // open (or create) registry key
    HKEY hKey;
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS){
        outputLog += "Error opening registry key for wallpaper. Creating a new one instead.\n";
        if(RegCreateKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS){
            outputLog += "Error creating registry key.\n";
            dumpLogs();
            return 0;
        }
        outputLog += "Successfully created registry key!\n";
    }
    // set registry value
    if(RegSetValueExA(hKey, "DesktopImagePath", 0, REG_SZ, reinterpret_cast<const BYTE*>(targetFile.c_str()), targetFile.size() + 1) != ERROR_SUCCESS){
        outputLog += "Error setting registry key for wallpaper.\n";
        dumpLogs();
        return 0;
    }
    RegCloseKey(hKey);
    outputLog += "All your regkeys are belong to us! Mwahahaha!\n"; // log to signal no errors yippeee!!!
    dumpLogs();
    return 0;
}