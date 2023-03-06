#include "HDH.h"

NTFS* readNTFS(LPCWSTR path) {
	unsigned char sector[512];
	ReadSector(path, 0, sector,512);
	wprintf(L"Bytes Per Sector : %u \n", charToInt(&sector[bytePerSectorIndex], 2));
	wprintf(L"Cluster per sector : %u \n", charToInt(&sector[SectorPerCluster], 1));
	wprintf(L"Reserved Sectors : %u \n", charToInt(&sector[ReservedSectors], 2));
	wprintf(L"Sectors Per Track : %u \n", charToInt(&sector[SectorPerTrack], 2));
	wprintf(L"Number Of Heads : %u \n", charToInt(&sector[numberOfHead], 2));
	wprintf(L"Total Sectors : %u \n", charToInt(&sector[TotalSectors], 8));
	wprintf(L"Logical Cluster Number for the file $MFT : %u \n", charToInt(&sector[LogicalClusterNumber], 8));
	wprintf(L"Logical Cluster Number for the file $MFTMirr : %u \n", charToInt(&sector[LogicalClusterNumberforthefile$MFTMirr], 8));
	wprintf(L"Clusters Per File Record Segment : %u \n", charToInt(&sector[ClustersPerFileRecordSegment], 4));
	wprintf(L"Clusters Per Index Buffer : % u \n", charToInt(&sector[ClustersPerIndexBuffer], 1));
	wprintf(L"\n");
	return new NTFS(sector);
}

FAT32* readFAT32(LPCWSTR path) {
	unsigned char sector[512];
	ReadSector(path, 0, sector,512);
	wprintf(L"Byte per sector : %u \n", charToInt(&sector[bytePerSectorIndex], 2));
	wprintf(L"Cluster per sector : %u \n", charToInt(&sector[ScIndex], 1));
	wprintf(L"Sector per Bootsector %u \n", charToInt(&sector[SbIndex], 2));
	wprintf(L"Number of FAT table : %u \n", charToInt(&sector[nFIndex], 1));
	wprintf(L"Sector per track : %u \n", charToInt(&sector[SectorperTrackIndex], 2));
	wprintf(L"Number of head : %u \n", charToInt(&sector[numberOfHeadIndex], 2));
	wprintf(L"Volume size : %u \n", charToInt(&sector[SvIndex], 4));
	wprintf(L"FAT table size : %u \n", charToInt(&sector[SfIndex], 4));
	wprintf(L"Cluster staring of RDET : %u \n", charToInt(&sector[RDETstartIndex], 4));
	wprintf(L"Empty cluster info sector : %u \n", charToInt(&sector[sectorEmptyClusterIndex], 2));
	wprintf(L"Backup bootsector : %u \n", charToInt(&sector[sectorBackUpBSIndex], 2));
	wprintf(L"Type of FAT : ");
	for (int i = 82; i < 90; i++) {
		wprintf(L"%c", sector[i]);
	}
	wprintf(L"\n");
	return new FAT32(sector);
}


void handleFakeEntries(LPCWSTR drive, int readPoint, unsigned char sector[512], int checkValid, wstring& fullName)
{
	int indexByte = 0;
	if (checkValid - 32 < 0)
	{
		ReadSector(drive, readPoint - 512, sector, 512);
		checkValid = 491;
	}
	else
	{
		checkValid -= 32;
	}

	if (sector[checkValid] == 15) fullName.erase(fullName.begin(), fullName.end());
	else return;
	while (sector[checkValid] == 15) {
		indexByte = checkValid - 10;
		formmingUniStr(sector, indexByte, 10, fullName,512);
		indexByte = checkValid - 11 + 14;
		formmingUniStr(sector, indexByte, 12, fullName,512);
		indexByte = checkValid - 11 + 28;
		formmingUniStr(sector, indexByte, 4, fullName,512);
		if (checkValid - 32 < 0)
		{
			ReadSector(drive, readPoint - 512, sector,512);
			checkValid = 491;
		}
		else checkValid -= 32;
	}
	
}

void readEntries(LPCWSTR  drive, int readPoint, Folder*& root, FAT32* currDisk){
	int sectorOfRDET = 8;
	//int count = 0;
	int i = 0;
	while(1) {
		unsigned char sector[512];
		ReadSector(drive, readPoint+ (i * 512), sector,512);
		if (sector[0] == 0x00) break;
		int checkValid = 11; //Kiểm tra có phải là file, folder hay Read-Only không
		int indexByte = 0;
		int objSize = 0;
		while (checkValid < 512) {
			wstring fullName = L"";
			long size = 0;
			unsigned int clusterStarted ;
			if (checkValid - 11 >= 0 && sector[checkValid - 11] != 0xE5) {
				int originPoin = checkValid - 11;
				if (sector[checkValid] == 32) {
					//Lấy info cluster bắt đầu
					size = charToInt(&sector[originPoin + 28], 4);
					clusterStarted = sector[originPoin + 20] << 16;
					clusterStarted |= sector[originPoin + 21] << 24;
					clusterStarted |= sector[originPoin + 26];
					clusterStarted |= sector[originPoin + 27] << 8;
					indexByte = checkValid - 11;
					//Lấy tên từ 8 byte đầu
					for (int i = 0; i < 8; ++i)
					{
						fullName.push_back((wchar_t)sector[indexByte]);
						indexByte++;
					}
					//Lấy phần mở rộng trong 3 byte sau
					for (int j = 0; j < 3; ++j)
					{
						fullName.push_back((wchar_t)sector[indexByte]);
						indexByte++;
					}
					//Xử lý tồn tại entry phụ (hoặc không)
					handleFakeEntries(drive, readPoint, sector, checkValid, fullName);
					File* newFile = new File(fullName, size, clusterStarted);
					root->AddComponent(newFile);
				}
				else if (sector[checkValid] == 16 && sector[checkValid - 11] != 0x2E) {
					indexByte = checkValid - 11;
					//Lấy cluster bắt đầu
					size = charToInt(&sector[originPoin + 28], 4);
					clusterStarted = sector[originPoin + 20] << 16;
					clusterStarted |= sector[originPoin + 21] << 24;
					clusterStarted |= sector[originPoin + 26];
					clusterStarted |= sector[originPoin + 27] << 8;					
					//Lấy tên từ 8 byte đầu				
					for (int i = 0; i < 8; ++i)
					{						
						fullName.push_back((wchar_t)sector[indexByte]);
						indexByte++;
					}
					//Lấy phần mở rộng trong 3 byte sau
					for (int j = 0; j < 3; ++j)
					{
						fullName.push_back((wchar_t)sector[indexByte]);
						indexByte++;
					}
					//Xử lý tồn tại entry phụ (hoặc không)
					handleFakeEntries(drive, readPoint, sector, checkValid, fullName);

					Folder* newFolder = new Folder(fullName, size, clusterStarted, root);
					vector<unsigned int> SDET = currDisk->readFAT(clusterStarted, drive);
					for (int i = 0; i < SDET.size(); i++) {
						unsigned char subCon[512];
						unsigned int byteData = currDisk->clusterToByte(SDET[i]);
						readEntries(drive, byteData,newFolder,currDisk);
					}
					root->AddComponent(newFolder);
				}
				else if (sector[checkValid] == 1)
				{
					//Read-only
				}

			}
			checkValid += 32;
		}
		i++;
	}
}