#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

#define bytePerSector 11 // size la 2
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

int main() {
    NTFS currentDisk = readNTFS(L"\\\\.\\D:");
    unsigned char sector[512];
    ReadSector(L"\\\\.\\/D:", 1, sector);
    for (int i = 0; i < 512; ++i) {
        if (i % 16 == 0 && i != 0) printf("\n");
        printf(" %4x ", sector[i]);
    }

    //printf("\n");
    //printf("Bytes Per Sector : %u \n", charToInt(&sector[bytePerSector], 2));
    //printf("Cluster per sector : %u \n", charToInt(&sector[SectorPerCluster], 1));
    //printf("Reserved Sectors :%u \n", charToInt(&sector[ReservedSectors], 2));
    //printf("Sectors Per Track : %u \n", charToInt(&sector[SectorPerTrack], 2));
    //printf("Number Of Heads : %u \n", charToInt(&sector[numberOfHead], 2));
    //printf("Total Sectors : %u \n", charToInt(&sector[TotalSectors], 8));
    //printf("Logical Cluster Number for the file $MFT : %u \n", charToInt(&sector[LogicalClusterNumber], 8));
    //printf("Logical Cluster Number for the file $MFTMirr : %u \n", charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr], 8));
    //printf("Clusters Per File Record Segment : %u \n", charToInt(&sector[ClustersPerFileRecordSegment], 4));
    //printf("Clusters Per Index Buffer : % u \n", charToInt(&sector[ClustersPerIndexBuffer], 1));
    return 0;

}