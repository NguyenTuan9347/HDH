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

unsigned int charToInt(unsigned char* arr, int size);
int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[512]);
void drawMenu();
void quanlywindow();
void printfNtfs();
void prinfFat32();
void GotoXY(int x, int y);
void formmingUniStr(unsigned char sector[512], int& startIndex, int maxCount, wstring& fullName);

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

};
NTFS* readNTFS(LPCWSTR path);
FAT32* readFAT32(LPCWSTR path);
class Component
{
protected:
	Component* parent = nullptr;
	wstring name, extension;
	long size = 0;
public:
	virtual wstring getName() = 0;
	virtual wstring getExtension() = 0;
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
	Folder(wstring namePara) { name = namePara; }
	wstring getName() { return name; }
	wstring getExtension() { return L""; }
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
	File(wstring namePara, wstring extensionPara) { name = namePara; extension = extensionPara; }
	wstring getName() { return name; }
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

void handleFakeEntries(LPCWSTR drive, int readPoint, unsigned char sector[512], int checkValid, wstring& fullName);
void readEntries(LPCWSTR  drive, int readPoint, Folder* USB);