#pragma once
#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>
#include <locale>
#include <uchar.h>
#include <memory.h>
#include <iomanip>
#include <map>
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
//New define
#define $Parent_directory_file_record_number  176
#define $ID_of_this_record  44



#define bytePerSectorIndex 11 // size la 2
#define ScIndex 13 // size la 1 // sector per cluster
#define SbIndex 14 // size la 2 // sector per boot sector
#define nFIndex 16 // size la 1 // number of fat32 table
#define SectorperTrackIndex 24 // size 2 // sector per track
#define numberOfHeadIndex 26 // size la 2 
#define SvIndex 32 // size 4 // size of volume
#define SfIndex 36 // size 4 // size of fat table
#define RDETstartIndex 44 // size 4 // started RDET index
#define sectorEmptyClusterIndex 48 // size 2
#define sectorBackUpBSIndex 50 // size 2    

unsigned int charToInt(unsigned char* arr, int size);
int ReadSector(LPCWSTR  drive, double readPoint, unsigned char sector[], int bytetoread);
void formmingUniStr(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead);
void formmingUniStrNTFS(unsigned char sector[], int startIndex, int maxCount, wstring& fullName);
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
        int byteStartOfFAT() {
            return Sb * bytePerSector;
        }
        unsigned int clusterToByte(unsigned int cluster) {
            return (Sb + Sf * nF + (cluster - 2) * Sc) * bytePerSector;
        }
        vector<unsigned int> readFAT(int startedCluster, LPCWSTR path) {
            unsigned char* sector = new unsigned char[bytePerSector * Sf];
            vector<unsigned int> kq;
            this->byteStartOfFAT();
            ReadSector(path, this->byteStartOfFAT(), sector, Sf * bytePerSector);
            int i = startedCluster * 4;
            kq.push_back(startedCluster);
            for (; i < Sf * bytePerSector;) {
                unsigned int temp = charToInt(&sector[i], 4);
                if (temp == 0x0FFFFFFF || temp == 0 || temp == 0xFFFFFFFF) {
                    break;
                }
                else if (temp == 0xFFFFFF7) {
                    wcout << L"Corrupt file !!! " << endl;
                }
                i = temp * 4;
                kq.push_back(temp);
            }
            if (sector != NULL)
                delete[] sector;
            return kq;
        }
    };

class Component
    {
    protected:
        wstring name;
        unsigned int dataIndex = 0;
        long size = 0, myParentID = 0, myID = 0;
    public:
        virtual wstring getName() = 0;
        virtual long getSize() = 0;
        long getMyID() { return myID; }
        long getParentID() { return myID; }
        Component(wstring fullName, long size, int clusterIndex) {
            this->size = size;
            dataIndex = clusterIndex;
        }
        virtual void AddComponent(Component* obj)
        {

        }
        virtual void displayContent(int padding) = 0;
        virtual void RemoveComponent(Component* obj)
        {

        }
        virtual ~Component() {
        }
    };

class Folder : public Component {
        Component* my_parent = nullptr;
        vector<Component*> components;
    public:
        Folder(wstring fullName, long size, int startIndexData, Component* parent) : Component(fullName, size, startIndexData) {
            copy(fullName.begin(), fullName.end(), back_inserter(name));
            my_parent = parent;
        }
        Folder(wstring fullName, long size, int startIndexData, unsigned int myParent, unsigned int ID) : Component(fullName, size, startIndexData) {
            copy(fullName.begin(), fullName.end(), back_inserter(name));
            dataIndex = startIndexData;
            myParentID = myParent;
            myID = ID;
        }
        wstring getName() { return name; }
        void setMyParent(Folder* parent) {
            if (parent == NULL) return;
            this->my_parent = parent;
        }
        bool isDulpicate(wstring wantedName) {
            for (int i = 0; i < components.size(); i++) {
                if (components[i]->getName() == wantedName) {
                    return true;
                }
            }
            return false;
        }

        void displayContent(int padding) {
            long temp = padding + this->getName().size();
            wcout << setw(temp);
            wcout << this->getName() << "     " << L"DIR" << "     " << endl;
            padding += 4;
            for (int i = 0; i < components.size(); ++i) {
                components[i]->displayContent(padding);
            }
        }
        long getSize() {
            for (int i = 0; i < components.size(); ++i)
            {
                size += components[i]->getSize();
            }
            return size;
        }
        void AddComponent(Component* obj)
        {
            if (obj != NULL && isDulpicate(obj->getName()) == false) {
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
    private:
        wstring extension;
    public:
        File(wstring fullName, long size, int clusterIndex) : Component(fullName, size, clusterIndex) {
            int flag = 0;
            for (int i = 0; i < fullName.size(); i++) {
                if (fullName[i] == 0x002E && flag == 0) {
                    flag++;
                }
                if (flag == 0) {
                    name.push_back(fullName[i]);
                }
                else {
                    extension.push_back(fullName[i]);
                }
            }
        }
        File(wstring fullName, long size, int startIndexData, unsigned int myParent, unsigned int ID) : Component(fullName, size, startIndexData) {
            int flag = 0;
            for (int i = 0; i < fullName.size(); i++) {
                if (fullName[i] == 0x002E && flag == 0) {
                    flag++;
                }
                if (flag == 0) {
                    name.push_back(fullName[i]);
                }
                else {
                    extension.push_back(fullName[i]);
                }
            }
            dataIndex = startIndexData;
            myParentID = myParent;
            myID = ID;
        }
        wstring getName() { return name + extension; }
        void displayContent(int padding) {
            long temp = padding + this->getName().size();
            wcout << setw(temp) << this->getName() << "     " << L"FILE" << "     " << size << endl;
        }
        wstring getExtension() { return extension; }
        void setSize(int value)
        {
            size = value;
        }
        long getSize()
        {
            return size;
        }
    };

//Class NTFS
void print_file_NFTS_content(LPCWSTR path, double content_start_offset, int Byte_length_content);
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
    bool check_active_or_deleted(unsigned char entry[], bool& check_file) {
        if (entry[22] == 1)
        {
            check_file = true;
            return 1;
        }
        if (entry[22] == 3)
            return 1;
        return 0;
    }
    void Get_Size_content_resident(unsigned char entry[], int $DATA_Atribute_ID, int& Byte_length_content, double& Start_content_index) {
        int Size_Data_ID = $DATA_Atribute_ID + 16;
        Byte_length_content = charToInt(&entry[Size_Data_ID], 4);
        //wcout << Byte_length_content << " Bytes" << endl;

        Start_content_index = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 20], 1);
        //for (int i = 0; i < Byte_length_content; ++i) {
        //   if (i % 16 == 0 && i != 0) wcout<<"\n";
        //   wcout << std::hex << charToInt(&entry[Start_content_index + i], 1) << " ";
        //}
        //UTF 16
        //wstring content;
        //formmingUniStr(entry, Start_content_index, Byte_length_content, content);
        //wcout << content << endl;


        // UTF 8
        //wstring content;
        ////formmingUniStr(entry, Start_content_index, Byte_length_content, content, 1024);
        //for (int i = 0; i < Byte_length_content; i++)
        //    content += char(charToInt(&entry[Start_content_index + i], 1));
        //wcout << content << endl;
    }
public:
    MFT() {};
    MFT(LPCWSTR path, double MFT_entry_ID) {
        unsigned char entry[1024];
        for (int i = 0; i < i + 1; i++) {
            ReadSector(path, MFT_entry_ID + i * 1024, entry, 1024);
            //Check end entry    
            if (charToInt(&entry[0], 1) == 0)
                break;

            //-----------------------------------------CHECK FILE OR FOLDER----------CHECK ACTIVE OR DELETED
            bool check_file = false;
            bool active = check_active_or_deleted(entry, check_file);
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
                formmingUniStrNTFS(entry, Name_index, Byte_of_Name_length, name);
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
                int Byte_length_content = 0;
                double Start_content_index = 0;
                if (check_resident) {
                    Get_Size_content_resident(entry, $DATA_Atribute_ID, Byte_length_content, Start_content_index);
                    Start_content_index = MFT_entry_ID + i * 1024;
                    wcout << Start_content_index << endl;
                    print_file_NFTS_content(path, Start_content_index, Byte_length_content);
                }
                else {
                    if (check_file) {
                        int Size_Data_ID = $DATA_Atribute_ID + charToInt(&entry[$DATA_Atribute_ID + 32], 2);
                        int left_byte, right_byte;
                        this->Read_half_byte(entry, Size_Data_ID, 1, left_byte, right_byte);
                        int number_cluster_content_size = charToInt(&entry[Size_Data_ID + 1], right_byte);
                        //--------------Size
                        int Byte_length_content = 512 * 8 * number_cluster_content_size;
                        //wcout << Byte_length_content << " Bytes" << endl;

                        //-------------------------Tackle content
                        int cluster_in_offset = charToInt(&entry[Size_Data_ID + 1 + right_byte], left_byte);
                        double content_start_offset = cluster_in_offset * 8 * 512;
                        //wcout << cluster_in_offset << endl;
                        //-------------------------------ReadContent
                        //print_file_content(path, content_start_offset, Byte_length_content);
                        Start_content_index = content_start_offset;
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
                    FileList.push_back(new File(name, Byte_length_content, Start_content_index, myParentID, myID));
                    //wcout << "Push to Filelist succeed." << endl;
                }
                else {
                    FolderList.push_back(new Folder(name, Byte_length_content, $DATA_Atribute_ID, myParentID, myID));
                    //wcout << "Push to Folderlist succeed." << endl;
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
    int SectorPerCluster, HiddenSectors, unused4, unused5;
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
        MasterFileTable = MFT(path, this->location_sector_start_MFT());
    }
    double location_sector_start_MFT() {
        return LogicalClusterNumber$MFT * SectorPerCluster * 512 + 39936;
    }
};







void drawMenu();
void quanlywindow();
void printfNtfs();
void prinfFat32();
void GotoXY(int x, int y);
void handleFakeEntries(LPCWSTR drive, int readPoint, unsigned char sector[512], int checkValid, wstring& fullName);
void readEntries(LPCWSTR  drive, int readPoint, Folder*& root, FAT32* currDisk);

NTFS* readNTFS(LPCWSTR path);

FAT32* readFAT32(LPCWSTR path);
void formingTree(vector<File*> listFile, vector<Folder*> listFolder, Folder*& root, wstring path);

// tim ten trong folder;