#include "HDH.h"

NTFS* readNTFS(LPCWSTR path) {
	unsigned char sector[512];
	ReadSector(path, 0, sector);
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
	ReadSector(path, 0, sector);
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
//	fullName += L"-FakeEntry-";
	fullName.erase(fullName.begin(), fullName.end());
	int indexByte = 0;
	checkValid -= 32;
	while (sector[checkValid] == 15)
	{
		indexByte = checkValid - 10;
		formmingUniStr(sector, indexByte, 10, fullName);
		indexByte = checkValid - 11 + 14;
		formmingUniStr(sector, indexByte, 12, fullName);
		indexByte = checkValid - 11 + 28;
		formmingUniStr(sector, indexByte, 4, fullName);
		if (checkValid - 32 < 0)
		{
			ReadSector(drive, readPoint - 512, sector);
			checkValid = checkValid - 32 + 512;
		}
		else checkValid -= 32;
	}
	
}

void readEntries(LPCWSTR  drive, int readPoint)
{
	int sectorOfRDET = 8;
	//int count = 0;
	int i = 0;
	while(1) {
		unsigned char sector[512];
		ReadSector(drive, readPoint+ (i * 512), sector);
		if (sector[0] == 0x00) break;
		int checkValid = 11; //Kiểm tra có phải là file, folder hay Read-Only không
		int indexByte;
		int objSize = 0;
		while (checkValid < 512)
		{
			wstring fullName = L"";
			wstring fileExtension;
			if (sector[checkValid - 11] != 0xE5) {
				if (sector[checkValid] == 32)
				{
					//File không có entry phụ
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
						fileExtension.push_back((wchar_t)sector[indexByte]);
						indexByte++;
					}
					//Lấy kích cỡ object
					indexByte = checkValid - 11 + 28;
					objSize = charToInt(&sector[indexByte], 4);
					//Kiểm tra file có entry phụ nằm ở sector khác
					if (checkValid - 32 < 0)
					{
					//	wcout << L"Found fake entry in another sector" << endl;
						readPoint -= 512;
						ReadSector(drive, readPoint, sector);
						checkValid = 512 - 64;
					}
					//Trường hợp có entry phụ
					if (sector[checkValid - 32] == 15)
					{
						//fullName.erase();
						handleFakeEntries(drive, readPoint, sector, checkValid, fullName);

					}
					else {
						readPoint += 512;
						ReadSector(drive, readPoint, sector);
						checkValid = checkValid - 512;
					}
				}
				else if (sector[checkValid] == 16)
				{
					//Folder
				}
				else if (sector[checkValid] == 1)
				{
					//Read-only
				}

			}
			wcout << fullName << endl;
			checkValid += 32;
		}
		i++;
	}
	
	system("pause");
}