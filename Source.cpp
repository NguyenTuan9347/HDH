#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <iomanip>
using namespace std;

//New define
#define $Parent_directory_file_record_number  176
#define $ID_of_this_record  44

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


class MFT {
    vector<File*> FileList; vector<Folder*> FolderList;
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
    void Get_Size_content_resident(unsigned char entry[],int $DATA_Atribute_ID,int &Byte_length_content,int &Start_content_index) {
        int Size_Data_ID = $DATA_Atribute_ID + 16;
        Byte_length_content = charToInt(&entry[Size_Data_ID], 4);
        wcout << Byte_length_content << " Bytes" << endl;

        Start_content_index = $DATA_Atribute_ID+charToInt(&entry[$DATA_Atribute_ID + 20], 1);
        //for (int i = 0; i < Byte_length_content; ++i) {
        //   if (i % 16 == 0 && i != 0) wcout<<"\n";
        //   wcout << std::hex << charToInt(&entry[Start_content_index + i], 1) << " ";
        //}
        //UTF 16
        wstring content;
        formmingUniStr(entry, Start_content_index, Byte_length_content, content);
        wcout << content << endl;


        // UTF 8
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
        for (int i = 0; i < i + 1; i++) {
            ReadSector(path, MFT_entry_ID+i*1024, entry, 1024);
            //Check end entry    
            if (charToInt(&entry[0], 1) == 0)
                    break;

            //-----------------------------------------CHECK FILE OR FOLDER----------CHECK ACTIVE OR DELETED
            bool check_file = false;
            bool active = check_active_or_deleted(entry,check_file);
            int flag = 0;//file or 1 folder
            if (active) {
                if (check_file) 
                    wcout << "File" << endl;
                else {
                    wcout << "Folder" << endl;
                    flag = 1;
                }
                    //---------------------------------------------------GET NAME
                int $FILE_NAME_Atribute_ID = 152;
                int Length_name_ID = $FILE_NAME_Atribute_ID + 88;
                int Name_index = $FILE_NAME_Atribute_ID + 90;
                //Unicode 2 byte each character
                int Byte_of_Name_length = 2 * charToInt(&entry[Length_name_ID], 1);
                wstring name;
                formmingUniStr(entry, Name_index, Byte_of_Name_length, name);
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
                bool  check_resident = resident_content_check(entry, resident_content_id);
                int Byte_length_content=0;
                int Start_content_index = 0;
                if (check_resident) {
                    Get_Size_content_resident(entry, $DATA_Atribute_ID, Byte_length_content, Start_content_index);
                }
                else {
                    if (check_file) {
                        int Size_Data_ID = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 32], 2);
                        int left_byte, right_byte;
                        this->Read_half_byte(entry, Size_Data_ID, 1, left_byte, right_byte);
                        int number_cluster_content_size = charToInt(&entry[Size_Data_ID + 1], right_byte);
                        //--------------Size
                        int Byte_length_content = 512 * 8 * number_cluster_content_size;
                        wcout << Byte_length_content << " Bytes" << endl;

                        //-------------------------Tackle content
                        int cluster_in_offset = charToInt(&entry[Size_Data_ID + 1 + right_byte], left_byte);
                        double content_start_offset = cluster_in_offset * 8 * 512;
                        wcout << cluster_in_offset << endl;
                        //-------------------------------ReadContent
                        //unsigned char sector_content[512];
                        //for (int i = 0; i < Byte_length_content; i += 512) {
                        //    ReadSector(path, content_start_offset + i, sector_content, 512);
                        //    wstring content;
                        //    formmingUniStr(sector_content,Start_content_index, 512, content);
                        //    wcout << content;
                        //}
                    }
                }
                //wcout << "$DATA_Atribute_ID++: " << $DATA_Atribute_ID++ << endl;
                //File(wstring fullName, long size, int startIndexData, unsigned int myParent, unsigned int ID)
                //--------------------------------------------------- GET ID 03/07/2023 20:04pm
                unsigned int myParentID = charToInt(&entry[$Parent_directory_file_record_number], 6);
                unsigned int myID = charToInt(&entry[$ID_of_this_record], 4);
                //wcout << "myParentID:" << myParentID << endl;
                //wcout << "myID:" << myID << endl;
                if (flag == 0) {
                    FileList.push_back(new File(name, Byte_length_content, $DATA_Atribute_ID, myParentID, myID));
                    wcout << "Push to Filelist succeed." << endl;
                }
                else {
                    FolderList.push_back(new Folder(name, Byte_length_content, $DATA_Atribute_ID, myParentID, myID));
                    wcout << "Push to Folderlist succeed." << endl;
                }
                wcout << endl;
            }
        }
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

        //--short
        bytePerSector = charToInt(&sector[bytePerSector_], 2);
        ReservedSectors = charToInt(&sector[ReservedSectors_], 2);
        unused2 = charToInt(&sector[u2_], 2);
        unused3 = charToInt(&sector[u3_], 2);
        SectorPerTrack = charToInt(&sector[SectorPerTrack_], 2);
        numberOfHead = charToInt(&sector[numberOfHead_], 2);

        //--int
        HiddenSectors = charToInt(&sector[HiddenSectors_], 4);
        unused4 = charToInt(&sector[u4_], 4);
        unused5 = charToInt(&sector[u5_], 4);

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
