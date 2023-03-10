#include "HDH.h"




unsigned long charToInt(unsigned char* arr, int size) {
    unsigned char* temp = new unsigned char[size];
    unsigned int kq = 0;
    int shifter = 0, flag = 0;
    memmove((unsigned char*)&temp[0], (unsigned char*)&arr[0], size); // Copy chuoi byte
    if (size == 4 && (temp[size - 1] & 0x80) == 0x80) {
        temp[size - 1] = 0;
        flag++;
    }
    kq |= (unsigned int)temp[0] << shifter; // Copy byte dau vao ben phai cung
    shifter += 8;
    for (int i = 1; i < size; i++) {
        kq |= (unsigned int)temp[i] << shifter; // copy cac byte con lai lui dan ve ben trai
        shifter += 8;
    }
    if (flag == 1) {
        kq += (unsigned int)pow(2, 31);
    }
    delete[] temp;
    return kq;
}


int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[],unsigned int numberOfBytes)
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        wprintf(L"CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, numberOfBytes, &bytesRead, NULL))
    {
        wprintf(L"ReadFile: %u\n", GetLastError());
    }
}



void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void prinfFat32()
{
    GotoXY(40, 10);
    wcout << "  ______   _______ ____ ___   " << endl;
    GotoXY(40, 11);
    wcout << " |  ____/\\|__   __|___ \\__ \\  " << endl;
    GotoXY(40, 12);
    wcout << " | |__ /  \\  | |    __) | ) |  " << endl;
    GotoXY(40, 13);
    wcout << " |  __/ /\\ \\ | |   |__ < / / " << endl;
    GotoXY(40, 14);
    wcout << " | | / ____ \\| |   ___) / /_ " << endl;
    GotoXY(40, 15);
    wcout << " |_|/_/    \\_\\_|  |____/____|" << endl;

}
void printfNtfs()
{
    GotoXY(40, 10);
    wcout << "  _   _ _______ ______ _____  " << endl;
    GotoXY(40, 11);
    wcout << " | \\ | |__   __|  ____/ ____| " << endl;
    GotoXY(40, 12);
    wcout << " |  \\| |  | |  | |__ | (___  " << endl;
    GotoXY(40, 13);
    wcout << " | . ` |  | |  |  __| \\___ \\ " << endl;
    GotoXY(40, 14);
    wcout << " | |\\  |  | |  | |    ____) |" << endl;
    GotoXY(40, 15);
    wcout << " |_| \\_|  |_|  |_|   |_____/ " << endl;

}
void quanlywindow()
{
    GotoXY(20, 0);
    wcout << "   ____                      _         _                _   _    " << endl;
    GotoXY(20, 1);
    wcout << "  / __ \\                    | |       | |              | | (_)      " << endl;
    GotoXY(20, 2);
    wcout << " | |  | |_   _  __ _ _ __   | |_   _  | |_ __ _ _ __   | |_ _ _ __  " << endl;
    GotoXY(20, 3);
    wcout << " | |  | | | | |/ _` | '_ \\  | | | | | | __/ _` | '_ \\  | __| | '_ \\" << endl;
    GotoXY(20, 4);
    wcout << " | |__| | |_| | (_| | | | | | | |_| | | || (_| | |_) | | |_| | | | |" << endl;
    GotoXY(20, 5);
    wcout << "  \\___\\_\\\\__,_|\\__,_|_| |_| |_|\\__, |  \\__\\__,_| .__/   \\__|_|_| |_|" << endl;
    GotoXY(20, 6);
    wcout << "                                __/ |          | |                  " << endl;
    GotoXY(20, 7);
    wcout << "                               |___/           |_|                  " << endl;
    GotoXY(20, 10);
    wcout << "  _                             _           _                   " << endl;
    GotoXY(20, 11);
    wcout << " | |                           (_)         | |                  " << endl;
    GotoXY(20, 12);
    wcout << " | |_ _ __ ___ _ __   __      ___ _ __   __| | _____      _____ " << endl;
    GotoXY(20, 13);
    wcout << " | __| '__/ _ \\ '_ \\  \\ \\ /\\ / / | '_ \\ / _` |/ _ \\ \\ /\\ / / __|" << endl;
    GotoXY(20, 14);
    wcout << " | |_| | |  __/ | | |  \\ V  V /| | | | | (_| | (_) \\ V  V /\\__ \\" << endl;
    GotoXY(20, 15);
    wcout << "  \\__|_|  \\___|_| |_|   \\_/\\_/ |_|_| |_|\\__,_|\\___/ \\_/\\_/ |___/" << endl;
}
void drawMenu()
{
    wchar_t vborder = 9552, hborder = 9553, lowerleft =9562, lowerright = 9565, upperleft = 9556, upperright = 9559;
    GotoXY(17 + 25, 19);
    for (int i = 0; i < 26; i++)
        wcout << vborder;
    GotoXY(16 + 25, 19); wcout << upperleft;
    GotoXY(16 + 25, 20); wcout << hborder;
    GotoXY(16 + 25, 21); wcout << hborder;
    GotoXY(16 + 25, 22); wcout << hborder;
    GotoXY(16 + 25, 23); wcout << hborder;
    GotoXY(16 + 25, 24); wcout << hborder;
    GotoXY(16 + 25, 25); wcout << hborder;
    GotoXY(16 + 25, 26); wcout << lowerleft;
    GotoXY(17 + 25, 26);
    for (int i = 0; i < 26; i++)
        wcout << vborder;
    GotoXY(16 + 27 + 25, 19); wcout << upperright;
    GotoXY(16 + 27 + 25, 20); wcout << hborder;
    GotoXY(16 + 27 + 25, 21); wcout << hborder;
    GotoXY(16 + 27 + 25, 22); wcout << hborder;
    GotoXY(16 + 27 + 25, 23); wcout << hborder;
    GotoXY(16 + 27 + 25, 24); wcout << hborder;
    GotoXY(16 + 27 + 25, 25); wcout << hborder;
    GotoXY(16 + 27 + 25, 26); wcout << lowerright;
}


void formmingUniStr(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead) {
    
    for (int i = 0; i < (maxCount + 1) / 2 && startIndex < limitByteRead - 1; i++) {
        if ((sector[startIndex] == 0xff && sector[startIndex + 1] == 0xff  )|| (sector[startIndex] == 0x00 && sector[startIndex + 1] == 0x00)) return;
        wchar_t temp;
        temp = sector[startIndex + 1] << 8;
        temp |= sector[startIndex] << 0;
        fullName.push_back(temp);
        startIndex += 2;
    }
}

void formingTree(vector<File*> listFile, vector<Folder*> listFolder,Folder*& root,wstring path) {
    if (root != NULL) delete root;
    root = new Folder(path,0,-1,-1,-1);
    map<long, Folder*> tree;
    for (int i = 0; i < listFolder.size(); i++) {
        tree[listFolder[i]->getMyID()] = listFolder[i];
    }
    for (int i = 0; i < listFolder.size(); i++) {
       // wcout << i << endl;
        if (i == 162) {
            wcout << listFolder[i]->getName();
        }
        if (listFolder[i]->getParentID() == 5) {
            root->AddComponent(listFolder[i]);
        }
        else {

            if (tree.find(listFolder[i]->getParentID()) != tree.end()) {
                Folder* temp = tree[listFolder[i]->getParentID()];
                temp->AddComponent(listFolder[i]);
                listFolder[i]->setMyParent(temp);
            }
        }

    }
    for (int i = 0; i < listFile.size(); i++) {
        if (listFile[i]->getParentID() == 5) {
            root->AddComponent(listFile[i]);
        }
        else {
            if (tree.find(listFile[i]->getParentID()) != tree.end()) {
                Folder* temp = tree[listFile[i]->getParentID()];
                temp->AddComponent(listFile[i]);
            }
        }
    }
    return;
}

int extractText(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead) {

    for (int i = 0; i < (maxCount + 1) / 2 && startIndex < limitByteRead - 1; i++) {
        if ((sector[startIndex] == 0xff && sector[startIndex + 1] == 0xff) || (sector[startIndex] == 0x00 && sector[startIndex + 1] == 0x00)) return 1;
        wchar_t temp;
        temp = sector[startIndex + 1] << 8;
        temp |= sector[startIndex] << 0;
        fullName.push_back(temp);
        startIndex += 2;
    }
}

void formmingUniStrNTFS(unsigned char sector[], int startIndex, int maxCount, wstring& fullName) {
    int i = 0;
    if (sector[startIndex] == 0xff) i += 2;
    for (; i < maxCount; i = i + 2) {
        wchar_t temp;
        temp = sector[startIndex + i + 1] << 8;
        temp |= sector[startIndex + i] << 0;
        fullName.push_back(temp);
    }
}

void print_file_NFTS_content(LPCWSTR path, unsigned long content_start_offset, int Byte_length_content) {
    if (Byte_length_content > 700) {
        wstring content;
        unsigned char* sector_content = new unsigned char[Byte_length_content];
        ReadSector(path, content_start_offset, sector_content, Byte_length_content);
        formmingUniStrNTFS(sector_content, 0, Byte_length_content, content);
        delete[] sector_content;
        wcout << content << endl;
    }
    else {
        unsigned char entry[1024];
        ReadSector(path, content_start_offset, entry, 1024);
        int $FILE_NAME_Atribute_ID = 152;
        int Length_name_ID = $FILE_NAME_Atribute_ID + 88;
        int Name_index = $FILE_NAME_Atribute_ID + 90;
        int Byte_of_Name_length = 2 * charToInt(&entry[Length_name_ID], 1);
        int $FILE_NAME_Atribute_length = 16 * 5 + 8 + 2 + Byte_of_Name_length;
        int $DATA_Atribute_ID = $FILE_NAME_Atribute_ID + $FILE_NAME_Atribute_length;
        while (charToInt(&entry[$DATA_Atribute_ID], 1) != 128)
            $DATA_Atribute_ID++;

        int Start_content_index = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 20], 1);
        wstring content;
        formmingUniStrNTFS(entry, Start_content_index, Byte_length_content, content);
        wcout << content << endl;
    }
}

wstring parseExtension(wstring objName)
{
    int i = objName.find_last_of(L'.');
    return objName.substr(i + 1, objName.length() - i - 1);
}

int parseCommand(wstring command, wstring & objName, wstring & extension)
{
    wstring temp;
    if (command == L" " || command == L"") {
        return -1;
    }
    for (int i = 0; i < command.length(); ++i)
    {
        if (command[i] == L' ')
        {
            objName = command.substr(i + 1, command.length() - i - 1);
            if (temp == L"cd") return 0;
            if (temp == L"open") {
                extension = parseExtension(objName);
                return 1;
            }
            
        }
        temp.push_back((wchar_t)command[i]);
    }
}

void handleOtherFiles(wstring extension)
{
    if (extension == L"xlsx") {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Excel" << endl;
        return;
    }
    if (extension == L"docx") {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Word" << endl;
        return;
    }
    if (extension == L"pdf" || extension == L"PDF")
    {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Foxit Reader / Other PDF file viewers" << endl;
        return;
    }
    if (extension == L"epub" || extension == L"mobi" || extension == L"pcr")
    {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Calibre" << endl;
        return;
    }
    if (extension == L"mp3" || extension == L"MP3" || extension == L"flac" || extension == L"FLAC" || extension == L"wav" || extension == L"WAV" || extension == L"mp4" || extension == L"MP4" || extension == L"avi" || extension == L"AVI" || extension == L"mov" || extension == L"MOV")
    {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Windows Media Player / VLC Player / Other media players" << endl;
        return;
    }
    if (extension == L"png" || extension == L"PNG" || extension == L"jpg" || extension == L"JPG" || extension == L"gif" || extension == L"GIF")
    {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Windows Photos / Other photo viewers" << endl;
        return;
    }
    if (extension == L"cpp" || extension == L"py" || extension == L"jav" || extension == L"sql" || extension == L"xml")
    {
        wcout << "This program cannot read this file type. \nPlease refer to using other file viewers such as: ";
        wcout << "Visual Studio Code" << endl;
        return;
    }
}