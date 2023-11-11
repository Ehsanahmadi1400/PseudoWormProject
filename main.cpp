/*
*** This program copies itself in each run
*** in the Appdata directory of the file system
*** Author: Ehsan Ahmadi 12.11.2023
*/

#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <iomanip>  // Include for setw
#include <sstream>  // Include for wstringstream

// Link against the shlwapi.lib library
#pragma comment(lib, "shlwapi.lib")

int main() {
    // Buffer to store the file path
    wchar_t filePath[MAX_PATH];

    // Get the full path of the current executable
    // If first argument is NULL, this function returns the path of the executable file of the current process
    DWORD result = GetModuleFileNameW(nullptr, filePath, MAX_PATH);

    if (result == 0) {
        // If an error occurred
        std::wcerr << L"Failed to retrieve the module file name. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Display the file path
    std::wcout << L"Module file path: " << filePath << std::endl;

    // Source file path
    LPCWSTR sourceFilePath = filePath;

    // Get the AppData directory
    wchar_t appDataPath[MAX_PATH];
    // SHGetFolderPathW is a function in the Windows Shell API 
    // used to retrieve the path of a special folder on the system. 
    if (FAILED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, appDataPath))) {
        std::wcerr << L"Failed to retrieve the AppData directory." << std::endl;
        std::cout << appDataPath << std::endl;
        return 1;
    }

    // Find the next available version number
    int fileCounter = 1;

    // This stream will be used to construct the file path
    std::wstringstream destinationStream;

    do {
        destinationStream.str(L""); // Clear the stream
        destinationStream << appDataPath << L"\\NewFile_" << std::setw(10) << std::setfill(L'0') << fileCounter << L".exe";
        fileCounter++;
    } while (GetFileAttributesW(destinationStream.str().c_str()) != INVALID_FILE_ATTRIBUTES);

    std::wstring destinationFilePath = destinationStream.str();

    // Attempt to copy the file
    BOOL copyResult = CopyFileW(sourceFilePath, destinationFilePath.c_str(), FALSE);

    if (copyResult == FALSE) {
        // An error occurred
        DWORD error = GetLastError();
        std::wcerr << L"Failed to copy file. Error code: " << error << std::endl;
        return 1;
    }

    std::wcout << L"File copied successfully to: " << destinationFilePath << std::endl;

    return 0;
}
