#pragma once

#include <Windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
using namespace std;

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        if(a[i]== '\0') return s;
        s = s + a[i];
    }
    return s;
}

string openFileDialog()
{
    char szFile[MAX_PATH] = {0};
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrTitle = "Choose image";
    ofn.lpstrInitialDir = "D:";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Supported Files(*.png, *.jpg, *.jpeg, *.gif)\0*.png;*.jpg;*.jpeg;*.gif\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileName(&ofn))
    {
        string filepath = convertToString(szFile, MAX_PATH);
        return filepath;
    }
    else return "Unable to get dir";
}

bool saveFileDialog(char* path)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrTitle = "Choose folder to save";
    ofn.lpstrInitialDir = "D:";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "*.png";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    return GetSaveFileName(&ofn);
}

