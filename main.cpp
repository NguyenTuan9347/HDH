#include "HDH.h"

int main()
{
    int menu_item = 0;
    int x = 21;
    bool menu = true;
    while (menu)
    {
        quanlywindow();
        drawMenu();
        GotoXY(18 + 25, x);
        cout << "-> ";
        GotoXY(18 + 20 + 25, x);
        cout << " <-";
        GotoXY(25 + 25, 21);
        cout << "FAT32";
        GotoXY(25 + 25, 22);
        cout << "NTFS";
        GotoXY(25 + 25, 23);
        cout << "Exit";

        system("pause>nul"); // the >nul bit causes it the print no message

        if (GetAsyncKeyState(VK_DOWN) && x != 23) // down button pressed
        {
            GotoXY(18 + 25, x);
            cout << "  ";
            GotoXY(18 + 20 + 25, x);
            cout << "  ";
            GotoXY(18 + 21 + 25, x);
            cout << "  ";
            x++;
            GotoXY(18 + 25, x);
            cout << "-> ";
            GotoXY(18 + 20 + 25, x);
            cout << " <-";
            menu_item++;
            continue;
        }

        if (GetAsyncKeyState(VK_UP) && x != 21) // up button pressed
        {
            GotoXY(18 + 25, x);
            cout << "  ";
            GotoXY(18 + 20 + 25, x);
            cout << "  ";
            GotoXY(18 + 21 + 25, x);
            cout << "  ";
            x--;
            GotoXY(18 + 25, x);
            cout << "-> ";
            GotoXY(18 + 20 + 25, x);
            cout << " <-";
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
                    cout << "-> ";
                    GotoXY(18 + 20 + 25, f32);
                    cout << " <-";
                    GotoXY(25 + 25, 21);
                    cout << "Doc thong tin ";
                    GotoXY(25 + 25, 22);
                    cout << "Cay thu muc";
                    GotoXY(25 + 25, 23);
                    cout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && f32 != 23) // down button pressed
                    {
                        GotoXY(18 + 25, f32);
                        cout << "  ";
                        GotoXY(18 + 20 + 25, f32);
                        cout << "  ";
                        GotoXY(18 + 21 + 25, f32);
                        cout << "  ";
                        f32++;
                        GotoXY(18 + 25, f32);
                        cout << "-> ";
                        GotoXY(18 + 20 + 25, f32);
                        cout << " <-";
                        menu_fat32++;
                        continue;
                    }

                    if (GetAsyncKeyState(VK_UP) && f32 != 21) // up button pressed
                    {
                        GotoXY(18 + 25, f32);
                        cout << "  ";
                        GotoXY(18 + 20 + 25, f32);
                        cout << "  ";
                        GotoXY(18 + 21 + 25, f32);
                        cout << "  ";
                        f32--;
                        GotoXY(18 + 25, f32);
                        cout << "-> ";
                        GotoXY(18 + 20 + 25, f32);
                        cout << " <-";
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
                            NTFS currentDisk = readNTFS(L"\\\\.\\D:");
                            unsigned char sector[512];
                            ReadSector(L"\\\\.\\/D:", 1, sector);
                            for (int i = 0; i < 512; ++i) {
                                if (i % 16 == 0 && i != 0) printf("\n");
                                printf(" %4x ", sector[i]);
                            }
                            system("pause");
                            break;
                        }

                        case 1:
                        {

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
                    cout << "-> ";
                    GotoXY(18 + 20 + 25, ntf);
                    cout << " <-";
                    GotoXY(25 + 25, 21);
                    cout << "Doc thong tin ";
                    GotoXY(25 + 25, 22);
                    cout << "Cay thu muc";
                    GotoXY(25 + 25, 23);
                    cout << "Exit";

                    system("pause>nul"); // the >nul bit causes it the print no message

                    if (GetAsyncKeyState(VK_DOWN) && ntf != 23) // down button pressed
                    {
                        GotoXY(18 + 25, ntf);
                        cout << "  ";
                        GotoXY(18 + 20 + 25, ntf);
                        cout << "  ";
                        GotoXY(18 + 21 + 25, ntf);
                        cout << "  ";
                        ntf++;
                        GotoXY(18 + 25, ntf);
                        cout << "-> ";
                        GotoXY(18 + 20 + 25, ntf);
                        cout << " <-";
                        menu_ntfs++;
                        continue;
                    }

                    if (GetAsyncKeyState(VK_UP) && ntf != 21) // up button pressed
                    {
                        GotoXY(18 + 25, ntf);
                        cout << "  ";
                        GotoXY(18 + 20 + 25, ntf);
                        cout << "  ";
                        GotoXY(18 + 21 + 25, ntf);
                        cout << "  ";
                        ntf--;
                        GotoXY(18 + 25, ntf);
                        cout << "-> ";
                        GotoXY(18 + 20 + 25, ntf);
                        cout << " <-";
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
                            FAT32 currentDisk = readFAT32(L"\\\\.\\E:");
                            int RDETindex = currentDisk.byteStartOfRDET();
                            unsigned char sector[512];
                            ReadSector(L"\\\\.\\E:", RDETindex, sector);
                            for (int j = 0; j < 512; j++) {
                                if (j % 16 == 0) {
                                    printf("\n");
                                    //   if (j <= 10) printf("0");
                                     //  printf("%x  ", j);
                                }
                                if (sector[j] >= 0 && sector[j] <= 15) printf(" 0%x ", sector[j]);
                                else printf(" %c ", sector[j]);

                            }
                            printf("\n");
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
    return 0;
}
