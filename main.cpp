#include "HDH.h"


int main()
{
    int dump = _setmode(_fileno(stdout), _O_U16TEXT);
    int dump2 = _setmode(_fileno(stdin), _O_U16TEXT);
    int menu_item = 0;
    int x = 21;
    bool menu = true;
    FAT32* fat32Disk = NULL;
    NTFS* ntfsDisk = NULL;
    LPCWSTR* path = NULL;
    Folder* root = NULL;
    wstring str_turned_to_wstr;
    int toggle = 0;
    while (menu)
    {
        quanlywindow();
        drawMenu();
        GotoXY(18 + 25, x);
        wcout << "-> ";
        GotoXY(18 + 20 + 25, x);
        wcout << " <-";
        GotoXY(25 + 25, 21);
        wcout << "FAT32";
        GotoXY(25 + 25, 22);
        wcout << "NTFS";
        GotoXY(25 + 25, 23);
        wcout << "Exit";

        system("pause>nul"); // the >nul bit causes it the print no message

        if (GetAsyncKeyState(VK_DOWN) && x != 23) // down button pressed
        {
            GotoXY(18 + 25, x);
            wcout << "  ";
            GotoXY(18 + 20 + 25, x);
            wcout << "  ";
            GotoXY(18 + 21 + 25, x);
            wcout << "  ";
            x++;
            GotoXY(18 + 25, x);
            wcout << "-> ";
            GotoXY(18 + 20 + 25, x);
            wcout << " <-";
            menu_item++;
            continue;
        }

        if (GetAsyncKeyState(VK_UP) && x != 21) // up button pressed
        {
            GotoXY(18 + 25, x);
            wcout << "  ";
            GotoXY(18 + 20 + 25, x);
            wcout << "  ";
            GotoXY(18 + 21 + 25, x);
            wcout << "  ";
            x--;
            GotoXY(18 + 25, x);
            wcout << "-> ";
            GotoXY(18 + 20 + 25, x);
            wcout << " <-";
            menu_item--;
            continue;
        }
        system("cls");
        if (GetAsyncKeyState(VK_RETURN))
        { // Enter key pressed
            switch (menu_item)
            {
            case 0:
            {
                int menu_fat32 = 0;
                int f32 = 21;
                int offset = 0;
                bool mf32 = true;
                while (mf32)
                {
                    prinfFat32();
                    drawMenu();
                    GotoXY(18 + 25, f32);
                    wcout << "-> ";
                    GotoXY(18 + 20 + 25, f32);
                    wcout << " <-";
                    GotoXY(25 + 25, 21);
                    wcout << "Doc thong tin ";
                    GotoXY(25 + 25, 22);
                    wcout << "Cay thu muc";
                    if (root != NULL) {
                        offset = 1;
                        GotoXY(25 + 25, 23);
                        wcout << "Doc file/folder";
                    }
                    GotoXY(25 + 25, 23 + offset);
                    wcout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && f32 != 23 + offset) // down button pressed
                    {
                        GotoXY(18 + 25, f32);
                        wcout << "  ";
                        GotoXY(18 + 20 + 25, f32);
                        wcout << "  ";
                        GotoXY(18 + 21 + 25, f32);
                        wcout << "  ";
                        f32++;
                        GotoXY(18 + 25, f32);
                        wcout << "-> ";
                        GotoXY(18 + 20 + 25, f32);
                        wcout << " <-";
                        menu_fat32++;
                        continue;
                    }

                    if (GetAsyncKeyState(VK_UP) && f32 != 21) // up button pressed
                    {
                        GotoXY(18 + 25, f32);
                        wcout << "  ";
                        GotoXY(18 + 20 + 25, f32);
                        wcout << "  ";
                        GotoXY(18 + 21 + 25, f32);
                        wcout << "  ";
                        f32--;
                        GotoXY(18 + 25, f32);
                        wcout << "-> ";
                        GotoXY(18 + 20 + 25, f32);
                        wcout << " <-";
                        menu_fat32--;
                        continue;
                    }
                    system("cls");
                    if (GetAsyncKeyState(VK_RETURN))
                    { // Enter key pressed

                        switch (menu_fat32)
                        {
                        case 0:
                        {
                            wcout << "Drive letter of USB?: ";
                            string disk = "\\\\.\\?:";
                            wchar_t name;
                            wcin >> name;
                            disk[4] = name;

                            str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(disk);
                            path = new LPCWSTR(str_turned_to_wstr.c_str());                           
                            fat32Disk = readFAT32(*path);
                            system("pause");
                            break;
                        }

                        case 1: {
                            if (fat32Disk == NULL || path == NULL) {
                                wcout << "No data of disk has been found, please try again " << endl;
                                system("pause");
                            }
                            else {
                                if(root == NULL) root = new Folder(str_turned_to_wstr, 0, -1, NULL);
                                readEntries(*path, fat32Disk->byteStartOfRDET(), root, fat32Disk);
                                root->displayContent(0,fat32Disk,ntfsDisk);
                                unsigned long reserveSize = fat32Disk->Sv * fat32Disk->bytePerSector, consumedSize = root->getSize();
                                wcout << "Size has been used : " << consumedSize << endl;
                                wcout << "Size left  : " << reserveSize << endl;

                                system("pause");
                            }
                            break;
                        }
                        case 2: {
                            if (fat32Disk == NULL || path == NULL || root == NULL) {
                                delete root;
                                delete fat32Disk;
                                root = NULL;
                                fat32Disk = NULL;
                                mf32 = false;
                                break;
                            }
                            else {
                               
                                int flag = -3;
                                bool endFlag = false;
                                Component* next, * temp = root;
                                while (endFlag == false) {
                                    wstring command, objName, extension;
                                    temp->displaySurfaceContent();
                                    wcout << "_____________________________________________________________________________________________________" << endl;
                                    wcout << "                                             Command guide" << endl;
                                    wcout << "- cd [folder name]: to change directory view to the designated folder" << endl;
                                    wcout << "- open [file name]: to view contents of .txt file (or view app suggestions to open other file types)" << endl;
                                    wcout << "NOTE: Please enter the name exactly as displayed here" << endl;
                                    wcout << "Type here: ";
                                    if (toggle == 0)
                                    {
                                        wcin.ignore();
                                        toggle = 1;
                                    }
                                    getline(wcin, command);
                                    wcout << "_____________________________________________________________________________________________________" << endl;
                                    flag = parseCommand(command, objName, extension);
                                    if (flag == 1) {
                                        if (extension == L"txt" || extension == L"TXT") {
                                            Component* fake = root->findMe(objName);
                                            if (fake != NULL)  readContentFAT(fake, fat32Disk, *path);
                                        }
                                        else handleOtherFiles(extension);
                                        system("pause");
                                    }
                                    else if (flag == 0) {
                                        next = temp->findMe(objName);
                                        if (next != NULL) {
                                            temp = next;
                                            if (temp->getType() == L"FILE")
                                            {
                                                wcout << "Wrong command syntax!" << endl;

                                            }
                                            
                                        }
                                    }
                                    else if (flag == -1) {
                                        next = temp;
                                        Folder* sudo = (Folder*)temp;
                                        temp = (Component*)sudo->getMyParent();
                                        if (temp == NULL) {                                          
                                            endFlag = true;
                                            toggle = 0;
                                        }               
                                    }
                                    system("cls");
                                }
                            }
                            break;
                        }
                        case 3:
                            mf32 = false;
                            delete root;
                            delete fat32Disk;
                            root = NULL;
                            fat32Disk = NULL;                       
                        default:
                            mf32 = false;
                            break;
                        }


                        system("cls");

                    }
                }
                break;
            }

            case 1:
            {
                int menu_ntfs = 0;
                int ntf = 21;
                int offset = 0;
                bool mntf = true;
                while (mntf)
                {
                    printfNtfs();
                    drawMenu();
                    GotoXY(18 + 25, ntf);
                    wcout << "-> ";
                    GotoXY(18 + 20 + 25, ntf);
                    wcout << " <-";
                    GotoXY(25 + 25, 21);
                    wcout << "Doc thong tin ";
                    GotoXY(25 + 25, 22);
                    wcout << "Cay thu muc";
                    if (root != NULL) {
                        GotoXY(25 + 25, 23);
                        wcout << "Doc file/folder ";
                        offset = 1;
                    }
                    GotoXY(25 + 25, 23+ offset);
                    wcout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && ntf != 23 + offset) // down button pressed
                    {
                        GotoXY(18 + 25, ntf);
                        wcout << "  ";
                        GotoXY(18 + 20 + 25, ntf);
                        wcout << "  ";
                        GotoXY(18 + 21 + 25, ntf);
                        wcout << "  ";
                        ntf++;
                        GotoXY(18 + 25, ntf);
                        wcout << "-> ";
                        GotoXY(18 + 20 + 25, ntf);
                        wcout << " <-";
                        menu_ntfs++;
                        continue;
                    }
                    if (GetAsyncKeyState(VK_UP) && ntf != 21) // up button pressed
                    {
                        GotoXY(18 + 25, ntf);
                        wcout << "  ";
                        GotoXY(18 + 20 + 25, ntf);
                        wcout << "  ";
                        GotoXY(18 + 21 + 25, ntf);
                        wcout << "  ";
                        ntf--;
                        GotoXY(18 + 25, ntf);
                        wcout << "-> ";
                        GotoXY(18 + 20 + 25, ntf);
                        wcout << " <-";
                        menu_ntfs--;
                        continue;
                    }
                    system("cls");
                    if (GetAsyncKeyState(VK_RETURN))
                    { // Enter key pressed
                        switch (menu_ntfs)
                        {
                        case 0:
                        {
                            wcout << "Drive letter of USB?: ";
                            string disk = "\\\\.\\?:";
                            wchar_t name;
                            wcin >> name;
                            disk[4] = name;
                            str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(disk);
                            path = new LPCWSTR(str_turned_to_wstr.c_str());
                            ntfsDisk = readNTFS(*path);
                            system("pause");
                            break;
                        }

                        case 1: {
                            if (ntfsDisk == NULL || path == NULL) {
                                wcout << "No data of disk has been found, please try again " << endl;
                                system("pause");
                            }
                            else {
                                ntfsDisk->Read_MFT(*path);
                                formingTree(ntfsDisk->MasterFileTable.FileList, ntfsDisk->MasterFileTable.FolderList, root, str_turned_to_wstr);
                                root->displayContent(0,fat32Disk,ntfsDisk);
                                unsigned long reserveSize = ntfsDisk->TotalSectorsT * ntfsDisk->bytePerSector, consumedSize = root->getSize();
                                wcout << "Size has been used : " << consumedSize << endl;
                                wcout << "Size left  : " << reserveSize << endl;
                                system("pause");
                            }
                            break;
                        }

                        case 2: {                       
                            if (ntfsDisk == NULL || path == NULL || root == NULL) {
                                delete root;
                                delete ntfsDisk;
                                root = NULL;
                                ntfsDisk = NULL;
                                mntf = false;
                                break;
                            }
                            else {
                                int flag = -3;
                                bool endFlag = false;
                                Component* next, * temp = root;
                                while (endFlag == false) {
                                    wstring command, objName, extension;
                                    temp->displaySurfaceContent();
                                    wcout << "_____________________________________________________________________________________________________" << endl;
                                    wcout << "                                             Command guide" << endl;
                                    wcout << "- cd [folder name]: to change directory view to the designated folder" << endl;
                                    wcout << "- open [file name]: to view contents of .txt file (or view app suggestions to open other file types)" << endl;
                                    wcout << "NOTE: Please enter the name exactly as displayed here" << endl;
                                    wcout << "Type here: ";
                                    if (toggle == 0)
                                    {
                                        wcin.ignore();
                                        toggle = 1;
                                    }
                                    getline(wcin, command);
                                    wcout << "_____________________________________________________________________________________________________" << endl;
                                    flag = parseCommand(command, objName, extension);
                                    if (flag == 1) {
                                        if (extension == L"txt" || extension == L"TXT") {
                                            Component* fake = root->findMe(objName);
                                            if (fake != NULL)  print_file_NFTS_content(*path, fake->getStartingCluster(), fake->getSize());
                                        }
                                        else handleOtherFiles(extension);
                                        system("pause");
                                    }
                                    else if (flag == 0) {
                                        next = temp->findMe(objName);
                                        if (next != NULL) {
                                            temp = next;
                                            if (temp->getType() == L"FILE")
                                            {
                                                wcout << "Wrong command syntax!" << endl;

                                            }
                                        }
                                    }
                                    else if (flag == -1) {
                                        next = temp;
                                        Folder* sudo = (Folder*)temp;
                                        temp = (Component*)sudo->getMyParent();
                                        if (temp == NULL) {
                                            endFlag = true;
                                            toggle = 0;
                                        }

                                    }
                                    system("cls");                            
                                }
                            }                          
                            break;
                        }
                        case 3: {
                            delete root;
                            delete ntfsDisk;
                            root = NULL;
                            ntfsDisk = NULL;
                            mntf = false;
                            break;
                        }

                        default:
                            mntf = false;
                            break;
                        }
                        system("cls");

                    }
                }
                break;
            }

            case 2:
            {
                menu = false;
                break;
            }

            default:
                menu = false;
                break;
            }
            system("cls");

        }

    }
    if (ntfsDisk != NULL) delete ntfsDisk;
    if (fat32Disk != NULL) delete fat32Disk;
    if (path != NULL) delete path;
    return 0;
}