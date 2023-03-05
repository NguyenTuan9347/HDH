#include "HDH.h"


int main()
{
    int dump = _setmode(_fileno(stdout), _O_U16TEXT);
    int menu_item = 0;
    int x = 21;
    bool menu = true;
    FAT32* fat32Disk = NULL;
    NTFS* ntfsDisk = NULL;
    LPCWSTR* path = NULL;
    Component* root = NULL;
    wstring str_turned_to_wstr;
    wstring temp = L"Xin chào thế giới.txt";
   
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
                    GotoXY(25 + 25, 23);
                    wcout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && f32 != 23) // down button pressed
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
                            char name;
                            cin >> name;
                            disk[4] = name;
                            
                            str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(disk);
                            path = new LPCWSTR(str_turned_to_wstr.c_str());
                            root = new Folder(str_turned_to_wstr, 0, -1, NULL);
                            fat32Disk = readFAT32(*path);
                      
                            system("pause");
                            break;
                        }

                        case 1:
                        {
                            if (fat32Disk == NULL || path == NULL) {
                                wcout << "No data of disk has been found, please try again " << endl;
                                system("pause");
                            }
                            else { 
                                readEntries(*path, fat32Disk->byteStartOfRDET(),root);
                                wcout << endl;
                            }
                            break;
                        }
                        case 2:
                        {
                            mf32 = false;
                            break;
                        }


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
                    GotoXY(25 + 25, 23);
                    wcout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && ntf != 23) // down button pressed
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
                            char name;
                            cin >> name;
                            disk[4] = name;
                            str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(disk);
                            path = new LPCWSTR(str_turned_to_wstr.c_str());
                            root = new Folder(str_turned_to_wstr, 0, -1, NULL);
                            ntfsDisk = readNTFS(*path);
                            system("pause");
                            break;
                        }

                        case 1:
                        {

                            break;
                        }

                        case 2:
                        {
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


