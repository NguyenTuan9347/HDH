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




int main() {
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
    return 0;
}
