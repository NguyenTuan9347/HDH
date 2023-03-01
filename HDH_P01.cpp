
#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
#define bytePerSectorIndex 11 // size la 2
#define ScIndex 13 // size la 1
#define SbIndex 14 // size la 2
#define nFIndex 16 // size la 1
#define SectorperTrackIndex 24 // size 2
#define numberOfHeadIndex 26 // size la 2
#define SvIndex 32 // size 4
#define SfIndex 36 // size 4
#define RDETstartIndex 44 // size 4
#define sectorEmptyClusterIndex 48 // size 2
#define sectorBackUpBSIndex 50 // size 2    

constexpr auto bytePerSector = 11;
#define SectorPerCluster 13 // size la 1 // Sectors Per Cluster
#define ReservedSectors 14 // size la 2 //Reserved Sectors
#define SectorPerTrack 24 // size 2
#define numberOfHead 26 // size la 2
#define TotalSectors 40// size 8
#define LogicalClusterNumber 48 // size 8//Logical Cluster Number for the file $MFT
#define LogicalClusterNumberforthefile$MFTMirr 56 // size 8//Logical Cluster Number for the file $MFTMirr
#define ClustersPerFileRecordSegment 64 // size 4
#define ClustersPerIndexBuffer 68//size 1

unsigned int charToInt(unsigned char* arr, int size) {
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

class NTFS {
public:
    int bpS, SpC, RS, nH, SpT, TotalSec, LCN, LCN_2, CFRS, CIB;
    NTFS(unsigned char sector[]) {
        bpS = charToInt(&sector[bytePerSector], 2);
        SpC = charToInt(&sector[SectorPerCluster], 1);
        RS = charToInt(&sector[ReservedSectors], 2);
        nH = charToInt(&sector[numberOfHead], 2);
        SpT = charToInt(&sector[SectorPerTrack], 2);//Sectors Per Track
        TotalSec = charToInt(&sector[TotalSectors], 8);
        LCN = charToInt(&sector[LogicalClusterNumber], 8);//Logical Cluster Number for the file $MFT 
        LCN_2 = charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr], 8);//Logical Cluster Number for the file $MFTMirr 
        CFRS = charToInt(&sector[ClustersPerFileRecordSegment], 4);//Clusters Per File Record Segment
        CIB = charToInt(&sector[ClustersPerIndexBuffer], 1);//Clusters Per Index Buffer
    }
};




//int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[512])
//{
//    int retCode = 0;
//    DWORD bytesRead;
//    HANDLE device = NULL;
//
//    device = CreateFile(drive,    // Drive to open
//        GENERIC_READ,           // Access mode
//        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
//        NULL,                   // Security Descriptor
//        OPEN_EXISTING,          // How to create
//        0,                      // File attributes
//        NULL);                  // Handle to template
//
//    if (device == INVALID_HANDLE_VALUE) // Open Error
//    {
//        printf("CreateFile: %u\n", GetLastError());
//        return 1;
//    }
//
//    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read
//
//    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
//    {
//        printf("ReadFile: %u\n", GetLastError());
//    }
//    else
//    {
//        printf("Success!\n");
//    }
//}

int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[512])
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
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
}
NTFS readNTFS(LPCWSTR path) {
    unsigned char sector[512];
    ReadSector(path, 0, sector);
    printf("Bytes Per Sector : %u \n", charToInt(&sector[bytePerSector], 2));
    printf("Cluster per sector : %u \n", charToInt(&sector[SectorPerCluster], 1));
    printf("Reserved Sectors : %u \n", charToInt(&sector[ReservedSectors], 2));
    printf("Sectors Per Track : %u \n", charToInt(&sector[SectorPerTrack], 2));
    printf("Number Of Heads : %u \n", charToInt(&sector[numberOfHead], 2));
    printf("Total Sectors : %u \n", charToInt(&sector[TotalSectors], 8));
    printf("Logical Cluster Number for the file $MFT : %u \n", charToInt(&sector[LogicalClusterNumber], 8));
    printf("Logical Cluster Number for the file $MFTMirr : %u \n", charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr], 8));
    printf("Clusters Per File Record Segment : %u \n", charToInt(&sector[ClustersPerFileRecordSegment], 4));
    printf("Clusters Per Index Buffer : % u \n", charToInt(&sector[ClustersPerIndexBuffer], 1));
    printf("\n");
    return NTFS(sector);
}



//unsigned int charToInt(unsigned char* arr, int size) {
//    unsigned char* temp = new unsigned char[size];
//    unsigned int kq = 0;
//    int shifter = 0, flag = 0;
//    memmove((unsigned char*)&temp[0], (unsigned char*)&arr[0], size); // Copy chuoi byte
//    if (size == 4 && (temp[size - 1] & 0x80) == 0x80) {
//        temp[size - 1] = 0;
//        flag++;
//    }
//    kq |= (unsigned int)temp[0] << shifter; // Copy byte dau vao ben phai cung
//    shifter += 8;
//    for (int i = 1; i < size; i++) {
//        kq |= (unsigned int)temp[i] << shifter; // copy cac byte con lai lui dan ve ben trai
//        shifter += 8;
//    }
//    if (flag == 1) {
//        kq += (unsigned int)pow(2, 31);
//    }
//    delete[] temp;
//    return kq;
//}





class FAT32 {
public:
    int Sf, Sb, Sc, nF, clusterRDETFirstIndex, bytePerSector, Sv, nH;
    FAT32(unsigned char sector[]) {
        bytePerSector = charToInt(&sector[bytePerSectorIndex], 2);
        Sc = charToInt(&sector[ScIndex], 1);
        Sb = charToInt(&sector[SbIndex], 2);
        nF = charToInt(&sector[nFIndex], 1);
        nH = charToInt(&sector[numberOfHeadIndex], 2);
        Sv = charToInt(&sector[SvIndex], 4);
        Sf = charToInt(&sector[SfIndex], 4);
        clusterRDETFirstIndex = charToInt(&sector[RDETstartIndex], 4);
    }
    int byteStartOfRDET() {
        return (nF * Sf + Sb) * bytePerSector;
    }





};

FAT32 readFAT32(LPCWSTR path) {
    unsigned char sector[512];
    ReadSector(path, 0, sector);
    printf("Byte per sector : %u \n", charToInt(&sector[bytePerSectorIndex], 2));
    printf("Cluster per sector : %u \n", charToInt(&sector[ScIndex], 1));
    printf("Sector per Bootsector %u \n", charToInt(&sector[SbIndex], 2));
    printf("Number of FAT table : %u \n", charToInt(&sector[nFIndex], 1));
    printf("Sector per track : %u \n", charToInt(&sector[SectorperTrackIndex], 2));
    printf("Number of head : %u \n", charToInt(&sector[numberOfHeadIndex], 2));
    printf("Volume size : %u \n", charToInt(&sector[SvIndex], 4));
    printf("FAT table size : %u \n", charToInt(&sector[SfIndex], 4));
    printf("Cluster staring of RDET : %u \n", charToInt(&sector[RDETstartIndex], 4));
    printf("Empty cluster info sector : %u \n", charToInt(&sector[sectorEmptyClusterIndex], 2));
    printf("Backup bootsector : %u \n", charToInt(&sector[sectorBackUpBSIndex], 2));
    printf("Type of FAT : ");
    for (int i = 82; i < 90; i++) {
        printf("%c", sector[i]);
    }
    printf("\n");
    return FAT32(sector);
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
    cout << "  ______   _______ ____ ___   " << endl;
    GotoXY(40, 11);
    cout << " |  ____/\\|__   __|___ \\__ \\  " << endl;
    GotoXY(40, 12);
    cout << " | |__ /  \\  | |    __) | ) |  " << endl;
    GotoXY(40, 13);
    cout << " |  __/ /\\ \\ | |   |__ < / / " << endl;
    GotoXY(40, 14);
    cout << " | | / ____ \\| |   ___) / /_ " << endl;
    GotoXY(40, 15);
    cout << " |_|/_/    \\_\\_|  |____/____|" << endl;
    
}
void printfNtfs()
{
    GotoXY(40, 10);
    cout << "  _   _ _______ ______ _____  " << endl;
    GotoXY(40, 11);
    cout << " | \\ | |__   __|  ____/ ____| " << endl;
    GotoXY(40, 12);
    cout << " |  \\| |  | |  | |__ | (___  " << endl;
    GotoXY(40, 13);
    cout << " | . ` |  | |  |  __| \___ \\ " << endl;
    GotoXY(40, 14);
    cout << " | |\\  |  | |  | |    ____) |" << endl;
    GotoXY(40, 15);
    cout << " |_| \\_|  |_|  |_|   |_____/ " << endl;

}
void quanlywindow()
{
    GotoXY(20, 0);
    cout << "   ____                      _         _                _   _    " << endl;
    GotoXY(20, 1);
    cout << "  / __ \\                    | |       | |              | | (_)      " << endl;
    GotoXY(20, 2);
    cout << " | |  | |_   _  __ _ _ __   | |_   _  | |_ __ _ _ __   | |_ _ _ __  " << endl;
    GotoXY(20, 3);
    cout << " | |  | | | | |/ _` | '_ \\  | | | | | | __/ _` | '_ \\  | __| | '_ \\" << endl;
    GotoXY(20, 4);
    cout << " | |__| | |_| | (_| | | | | | | |_| | | || (_| | |_) | | |_| | | | |" << endl;
    GotoXY(20, 5);
    cout << "  \\___\\_\\\\__,_|\\__,_|_| |_| |_|\\__, |  \\__\\__,_| .__/   \\__|_|_| |_|" << endl;
    GotoXY(20, 6);
    cout << "                                __/ |          | |                  " << endl;
    GotoXY(20, 7);
    cout << "                               |___/           |_|                  " << endl;
    GotoXY(20, 10);
    cout << "  _                             _           _                   " << endl;
    GotoXY(20, 11);
    cout << " | |                           (_)         | |                  " << endl;
    GotoXY(20, 12);
    cout << " | |_ _ __ ___ _ __   __      ___ _ __   __| | _____      _____ " << endl;
    GotoXY(20, 13);
    cout << " | __| '__/ _ \\ '_ \\  \\ \\ /\\ / / | '_ \\ / _` |/ _ \\ \\ /\\ / / __|" << endl;
    GotoXY(20, 14);
    cout << " | |_| | |  __/ | | |  \\ V  V /| | | | | (_| | (_) \\ V  V /\\__ \\" << endl;
    GotoXY(20, 15);
    cout << "  \\__|_|  \\___|_| |_|   \\_/\\_/ |_|_| |_|\\__,_|\\___/ \\_/\\_/ |___/" << endl;
}
void drawMenu()
{
    unsigned char vborder = 205, hborder = 186, lowerleft = 200, lowerright = 188, upperleft = 201, upperright = 187;
    GotoXY(17 + 25, 19);
    for (int i = 0; i < 26; i++)
        cout << vborder;
    GotoXY(16 + 25, 19); cout << upperleft;
    GotoXY(16 + 25, 20); cout << hborder;
    GotoXY(16 + 25, 21); cout << hborder;
    GotoXY(16 + 25, 22); cout << hborder;
    GotoXY(16 + 25, 23); cout << hborder;
    GotoXY(16 + 25, 24); cout << hborder;
    GotoXY(16 + 25, 25); cout << hborder;
    GotoXY(16 + 25, 26); cout << lowerleft;
    GotoXY(17 + 25, 26);
    for (int i = 0; i < 26; i++)
        cout << vborder;
    GotoXY(16 + 27 + 25, 19); cout << upperright;
    GotoXY(16 + 27 + 25, 20); cout << hborder;
    GotoXY(16 + 27 + 25, 21); cout << hborder;
    GotoXY(16 + 27 + 25, 22); cout << hborder;
    GotoXY(16 + 27 + 25, 23); cout << hborder;
    GotoXY(16 + 27 + 25, 24); cout << hborder;
    GotoXY(16 + 27 + 25, 25); cout << hborder;
    GotoXY(16 + 27 + 25, 26); cout << lowerright;
}



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
