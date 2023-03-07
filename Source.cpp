#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <iomanip>
using namespace std;

#define bytePerSector_ 11 // size la 2
#define SectorPerCluster_ 13 // size la 1 // Sectors Per Cluster
#define ReservedSectors_ 14 // size la 2 //Reserved Sectors
#define u1_ 16//unused1//size 3
#define u2_ 19//unused2//size 2
#define MediaDescriptor_ 21// size 1
#define u3_ 22 //unused3 //size2
#define SectorPerTrack_ 24 // size 2
#define numberOfHead_ 26 // size la 2
#define HiddenSectors_ 28//size 4
#define u4_ 32 //unused4 //size4
#define u5_ 36//unused5 //size4
#define TotalSectors_ 40// size 8
#define LogicalClusterNumber_ 48 // size 8//Logical Cluster Number for the file $MFT
#define LogicalClusterNumberforthefile$MFTMirr_ 56 // size 8//Logical Cluster Number for the file $MFTMirr
#define ClustersPerFileRecordSegment_ 64 // size 4
#define ClustersPerIndexBuffer_ 68//size 1
#define u6_ 69 //unused6 //size 3
#define VolumeSerialNumber_ 72 // size8
#define Checksum_ 78 //size4
int ReadSector(LPCWSTR  drive, double readPoint, unsigned char sector[], int bytetoread = 512)
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

    LARGE_INTEGER liDistanceToMove;
    liDistanceToMove.QuadPart = readPoint;
    SetFilePointerEx(device, liDistanceToMove, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, bytetoread, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        //printf("Success!\n");
    }
    CloseHandle(device);
}
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
void formmingUniStr(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead) {
    for (int i = 0; i < maxCount / 2 && startIndex < limitByteRead; i++) {
        if (sector[startIndex] == 0xff) return;
        wchar_t temp;
        temp = sector[startIndex + 1] << 8;
        temp |= sector[startIndex] << 0;
        fullName.push_back(temp);
        startIndex += 2;
    }
}
class Component
{
protected:
    Component* parent = nullptr;
    string name, extension;
    long size = 0;
public:
    Component() {};
    Component(string Name) {
        name = name;
    }
    virtual string getName() = 0;
    virtual string getExtension() = 0;
    virtual long getSize() = 0;
    virtual void AddComponent(Component* obj)
    {

    }
    virtual void RemoveComponent(Component* obj)
    {

    }
    virtual ~Component() {
        if (parent != nullptr) delete parent;
    }
};
class Folder : public Component
{
    vector<Component*> components;
public:
    Folder(string name) :Component(name) {};
    string getName() { return name; }
    string getExtension() { return ""; }
    long getSize() {
        for (int i = 0; i < components.size(); ++i)
        {
            size += components[i]->getSize();
        }
        return size;
    }
    void AddComponent(Component* obj)
    {
        if (obj != NULL)
        {
            components.push_back(obj);
        }
    }
    ~Folder()
    {
        for (int i = 0; i < components.size(); ++i)
        {
            delete components[i];
        }
        components.clear();
    }
};
class File : public Component
{

public:
    File(string name):Component(name) {}
    string getName() { return name; }
    string getExtension() { return extension; }
    void setSize(int value)
    {
        size = value;
    }
    long getSize()
    {
        return size;
    }
};
class MFT {
    vector<string> File; vector<string> Folder;
    bool resident_content_check(unsigned char sector[], int id) {
        if (sector[id] == 0)
            return 1;
        return 0;
    }
    void Read_half_byte(unsigned char sector[], int id, int len, int& a, int& b) {
        int number = charToInt(&sector[id], len);
        b = number % 16;
        a = (number - b) / 16;
    }
    bool check_active_or_deleted(unsigned char entry[],bool &check_file) {
        if (entry[22] == 1)
        {
            check_file = true;
            return 1;
        }
        if (entry[22] == 3)
            return 1;
        return 0;
    }
    void Get_Size_content_resident(unsigned char entry[],int $DATA_Atribute_ID,int &Byte_length_content) {
        int Size_Data_ID = $DATA_Atribute_ID + 16;
        Byte_length_content = charToInt(&entry[Size_Data_ID], 4);
        wcout << Byte_length_content << " Bytes" << endl;

        int Start_content_index = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 20], 2);
        //wstring content;
        ////formmingUniStr(entry, Start_content_index, Byte_length_content, content, 1024);
        //for (int i = 0; i < Byte_length_content; i++)
        //    content += char(charToInt(&entry[Start_content_index + i], 1));
        //wcout << content << endl;
    }
public:
    MFT() {};
    MFT(LPCWSTR path,double MFT_entry_ID) {
        unsigned char entry[1024];
        bool found = false;
        //3221265408
        for (int i = 0; i < i + 1; i++) {
            ReadSector(path, MFT_entry_ID+i*1024, entry, 1024);
            //Check end entry    
            if (charToInt(&entry[0], 1) == 0)
                    break;

            //-----------------------------------------CHECK FILE OR FOLDER----------CHECK ACTIVE OR DELETED
            bool check_file = false;
            wcout << check_active_or_deleted(entry,check_file) << endl;
            if (check_file) {
                wcout << "File" << endl;
            }
            else
                wcout << "Folder" << endl;
            //---------------------------------------------------GET NAME
            int $FILE_NAME_Atribute_ID=152;
            int Length_name_ID = $FILE_NAME_Atribute_ID + 88;
            int Name_index = $FILE_NAME_Atribute_ID + 90;
            //Unicode 2 byte each character
            int Byte_of_Name_length = 2*charToInt(&entry[Length_name_ID], 1);
            wstring name;
            formmingUniStr(entry, Name_index, Byte_of_Name_length, name, 1024);
            wcout << name << endl;
            //--------------------------------------------------GET FILE SIZE AND CONTENT
            //Find $DATA_Atribute_ID
            //16*5+8  + lenght_name_index + Byte_of_Name_length
            int $FILE_NAME_Atribute_length = 16 * 5 + 8 + 2 + Byte_of_Name_length;
            int $DATA_Atribute_ID = $FILE_NAME_Atribute_ID + $FILE_NAME_Atribute_length;
            while (charToInt(&entry[$DATA_Atribute_ID], 1) != 128)
                $DATA_Atribute_ID++;
            
            //----------------
            int resident_content_id = $DATA_Atribute_ID + 8;
            bool  check_resident=resident_content_check(entry, resident_content_id);
            //wcout << check_resident << endl;
            if (check_resident) {
                int Byte_length_content;
                Get_Size_content_resident(entry, $DATA_Atribute_ID, Byte_length_content);
            }
            else {
                if (check_file) {
                    int Size_Data_ID = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 32], 2);
                    int left_byte, right_byte;
                    this->Read_half_byte(entry, Size_Data_ID, 1, left_byte, right_byte);
                    int number_cluster_content_size = charToInt(&entry[Size_Data_ID + 1], left_byte);
                    wcout << number_cluster_content_size << endl;
                    //--------------Size
                    int Byte_length_content = 512 * 8 * charToInt(&entry[Size_Data_ID+1], left_byte);
                    wcout << Byte_length_content << " Bytes" << endl;

                    //-------------------------Tackle content
                    int cluster_in_offset = charToInt(&entry[Size_Data_ID + 1 + left_byte], right_byte);
                    wcout << cluster_in_offset << endl;
                    
                    //for (int i = 0; i < i + 1; i++) {
                    //    ReadSector(path, cluster_in_offset*512*8 + i * 1024, entry, 1024);
                    //    //check null entry
                    //    if (charToInt(&entry[0], 1) == 0)
                    //        break;

                    //printf(" %4x ", entry[Size_Data_ID]);
                    //wcout << std::hex<<charToInt(&entry[Size_Data_ID], 1) << endl;
                    //int Byte_length_content = charToInt(&entry[Size_Data_ID], 4);
                    //wcout << Byte_length_content << " Bytes" << endl;
                }
            }
            wcout << endl;
        }
#define $DATA 
    }
};
class NTFS {
public:
    MFT MasterFileTable;
    char oemID[8], unused1[3], MediaDescriptor, unused6[3], ClustersPerIndexBuffer;
    short bytePerSector, ReservedSectors, unused2, unused3, SectorPerTrack, numberOfHead;
    int SectorPerCluster,HiddenSectors, unused4, unused5;
    long long TotalSectors, LogicalClusterNumber$MFT, LogicalClusterNumber$MFTMirr, VolumeSerialNumber;
    NTFS(unsigned char sector[]) {
        //--char
        SectorPerCluster = charToInt(&sector[SectorPerCluster_], 1);
        MediaDescriptor = char(charToInt(&sector[MediaDescriptor_], 1));
        ClustersPerIndexBuffer = char(charToInt(&sector[ClustersPerIndexBuffer_], 1));

        //cout << "SectorPerCluster:  " << SectorPerCluster << endl;
        //cout << "MediaDescriptor:  " << MediaDescriptor << endl;
        //cout << "ClustersPerIndexBuffer:  " << ClustersPerIndexBuffer << endl;

        //--short
        bytePerSector = charToInt(&sector[bytePerSector_], 2);
        ReservedSectors = charToInt(&sector[ReservedSectors_], 2);
        unused2 = charToInt(&sector[u2_], 2);
        unused3 = charToInt(&sector[u3_], 2);
        SectorPerTrack = charToInt(&sector[SectorPerTrack_], 2);
        numberOfHead = charToInt(&sector[numberOfHead_], 2);

        //cout << "bytePerSector " << bytePerSector << endl;
        //cout << "ReservedSectors " << ReservedSectors << endl;
        //cout << "unused2 :" << unused2 << endl;
        //cout << "unused3 :" << unused3 << endl;
        //cout << "SectorPerTrack : " << SectorPerTrack << endl;
        //cout << "numberOfHead : " << numberOfHead << endl;
        //--int
        HiddenSectors = charToInt(&sector[HiddenSectors_], 4);
        unused4 = charToInt(&sector[u4_], 4);
        unused5 = charToInt(&sector[u5_], 4);

        //cout << "SectorPerCluster" << HiddenSectors << endl;
        //cout << "unused4" << unused4 << endl;
        //cout << "unused5" << unused5 << endl;
        //--long long
        TotalSectors = charToInt(&sector[TotalSectors_], 8);
        LogicalClusterNumber$MFT = charToInt(&sector[LogicalClusterNumber_], 8);
        LogicalClusterNumber$MFTMirr = charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr_], 8);
        VolumeSerialNumber = charToInt(&sector[VolumeSerialNumber_], 8);
    }
    void Read_MFT(LPCWSTR path) {
        MasterFileTable = MFT(path,this->location_sector_start_MFT());
    }
    double location_sector_start_MFT() {
        return LogicalClusterNumber$MFT * SectorPerCluster *512 + 39936;
    }
};
NTFS readNTFS(LPCWSTR path) {
    unsigned char sector[512];
    ReadSector(path, 0, sector);
    //printf("Bytes Per Sector : %u \n", charToInt(&sector[bytePerSector_], 2));
    //printf("Sectors Per Cluster : %u \n", charToInt(&sector[SectorPerCluster_], 1));
    //printf("Reserved Sectors : %u \n", charToInt(&sector[ReservedSectors_], 2));
    //printf("Sectors Per Track : %u \n", charToInt(&sector[SectorPerTrack_], 2));
    //printf("Number Of Heads : %u \n", charToInt(&sector[numberOfHead_], 2));
    //printf("Total Sectors : %u \n", charToInt(&sector[TotalSectors_], 8));
    //printf("Logical Cluster Number for the file $MFT : %u \n", charToInt(&sector[LogicalClusterNumber_], 8));
    //printf("Logical Cluster Number for the file $MFTMirr : %u \n", charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr_], 8));
    //printf("Clusters Per File Record Segment : %u \n", charToInt(&sector[ClustersPerFileRecordSegment_], 4));
    //printf("Clusters Per Index Buffer : % u \n", charToInt(&sector[ClustersPerIndexBuffer_], 1));
    //printf("\n");
    return NTFS(sector);
}

int main() {
    int dump = _setmode(_fileno(stdout), _O_U16TEXT);
    NTFS currentDisk = readNTFS(L"\\\\.\\E:");
    //unsigned char sector[512];
    //ReadSector(L"\\\\.\\/D:", 0, sector);
    currentDisk.Read_MFT(L"\\\\.\\E:");
    

    //w//cout << L"    0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F" << endl;
    //w//cout << L"-----------------------------------------------------------------------------------------------" << endl;
    //for (int i = 0; i < 512; ++i) {
    //    if (i % 16 == 0 && i != 0) printf("\n");
    //    printf(" %4x ", sector[i]);
    //}

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