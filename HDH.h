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
using namespace std;

#define bytePerSectorIndex 11 // size la 2
#define SectorPerCluster 13 // size la 1 // Sectors Per Cluster
#define ReservedSectors 14 // size la 2 //Reserved Sectors
#define SectorPerTrack 24 // size 2
#define numberOfHead 26 // size la 2
#define TotalSectors 40// size 8
#define LogicalClusterNumber 48 // size 8//Logical Cluster Number for the file $MFT
#define LogicalClusterNumberforthefile$MFTMirr 56 // size 8//Logical Cluster Number for the file $MFTMirr
#define ClustersPerFileRecordSegment 64 // size 4
#define ClustersPerIndexBuffer 68//size 1
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

unsigned long charToInt(unsigned char* arr, int size);
int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[], unsigned int numberOfBytes);
class NTFS {
public:
	int bpS, SpC, RS, nH, SpT, TotalSec, LCN, LCN_2, CFRS, CIB;
	NTFS(unsigned char sector[]) {
		bpS = charToInt(&sector[bytePerSectorIndex], 2);
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
	unsigned int dataIndex;
	long size = 0;
public:
	virtual wstring getName() = 0;
	virtual long getSize() = 0;
	unsigned int getStartingCluster() { return dataIndex; }
	Component(wstring fullName, long size, int clusterIndex) {

		this->size = size;
		dataIndex = clusterIndex;
		//wcout << fullName << " has been created " << endl;
	}
	virtual Component* findMe(wstring myName) = 0;
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
	int id_Folder;
public:
	Folder(wstring fullName, long size, int startIndexData, Component* parent, int id_folder = -1) : Component(fullName, size, startIndexData) {
		copy(fullName.begin(), fullName.end(), back_inserter(name));
		my_parent = parent;
		id_Folder = id_folder;
	}
	wstring getName() { return name; }
	
	bool isDulpicate(wstring wantedName) {
		for (int i = 0; i < components.size(); i++) {
			if (components[i]->getName() == wantedName) {
				return true;
			}
		}
		return false;
	}

	void displayContent(int padding) {
		wcout << setw(padding + this->getName().size());
		wcout << this->getName() << "     " << L"DIR" << "     " << endl;
		padding += 5;
		for (int i = 0; i < components.size(); ++i) {

			components[i]->displayContent(padding);
		}
	}
	Component* findMe(wstring myName) {
		if (name == myName) return this;
		for (int i = 0; i < components.size(); i++) {
			if (components[i]->getName() == myName) return components[i];
		}
		return NULL;
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
	Component* findMe(wstring myName) {
		if (myName == (name + extension)) {
			return this;
		}
		else {
			return NULL;
		}
	}
	wstring getName() { return name + extension; }
	void displayContent(int padding) {
		wcout << setw(padding + name.size()) << name << extension << "     " << L"FILE" << "     " << size << endl;
	}
	
	wstring getExtension() 
	{
		return extension; 
	}
	unsigned int getStartingCluster()
	{
		return dataIndex;
	}
	void setSize(int value)
	{
		size = value;
	}
	long getSize()
	{
		return size;
	}
};


void drawMenu();
void quanlywindow();
void printfNtfs();
void prinfFat32();
void GotoXY(int x, int y);
void handleFakeEntries(LPCWSTR drive, int readPoint, unsigned char sector[512], int checkValid, wstring& fullName);
void readEntries(LPCWSTR  drive, int readPoint, Folder*& root, FAT32* currDisk);
void formmingUniStr(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead);
int extractText(unsigned char sector[], int& startIndex, int maxCount, wstring& fullName, int limitByteRead);
wstring readContent(Component* obj, FAT32* disk, LPCWSTR drive);
void readContentFAT(Component* obj, FAT32* disk, LPCWSTR drive);
NTFS* readNTFS(LPCWSTR path);
FAT32* readFAT32(LPCWSTR path);


