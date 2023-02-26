#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

#define ScIndex 13
#define SbIndex 14 // size la 2
#define nFIndex 16 // size la 1
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


int main() {
    unsigned char sector[512];
    ReadSector(L"\\\\.\\E:",1, sector);
    for (int i = 0; i < 512; ++i) {
        if (i % 16 == 0 && i != 0) printf("\n");
        printf(" %x ", sector[i]);
    }
    printf("\n");



    printf("Cluster per sector : %u \n", charToInt(&sector[ScIndex], 1));
    printf("Cluster per Bootsector %u \n", charToInt(&sector[SbIndex], 2));
    printf("Number of FAT table : %u \n", charToInt(&sector[nFIndex], 1));
    printf("Volume size : %u \n", charToInt(&sector[SvIndex], 4));
    printf("FAT table size : %u \n", charToInt(&sector[SfIndex], 4));
    printf("Cluster staring of RDET : %u \n", charToInt(&sector[RDETstartIndex], 4));
    printf("Empty cluster info sector : %u \n", charToInt(&sector[sectorEmptyClusterIndex], 2));
    printf("Backup bootsector : %u \n", charToInt(&sector[sectorBackUpBSIndex], 2));
    //static_cast<int>(*&sector[ScIndex]));

    // // release handles 
    return 0;

}