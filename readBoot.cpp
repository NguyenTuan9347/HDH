#include "HDH.h"

NTFS* readNTFS(LPCWSTR path) {
    unsigned char sector[512];
    ReadSector(path, 0, sector);
    printf("Bytes Per Sector : %u \n", charToInt(&sector[bytePerSectorIndex], 2));
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
    return new NTFS(sector);
}

FAT32* readFAT32(LPCWSTR path) {
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
    return new FAT32(sector);
}

void RDET::handleFakeEntries(LPCWSTR  drive, int readPoint, unsigned char sector[512], int checkValid, string& fullName)
{
	fullName += "-FakeEntry-";
	int indexByte = 0;
	checkValid -= 32;
	while (sector[checkValid] == 15)
	{
		indexByte = checkValid - 11 + 1;
		cout << "Found a fake entry" << endl;
		for (int i = 0; i < 10; ++i)
		{
			fullName += (char)(charToInt(&sector[indexByte], 1));
			indexByte++;
		}
		indexByte = checkValid - 11 + 14;
		for (int i = 0; i < 12; ++i)
		{
			fullName += (char)(charToInt(&sector[indexByte], 1));
			indexByte++;
		}
		indexByte = checkValid - 11 + 28;
		for (int i = 0; i < 4; ++i)
		{
			fullName += (char)(charToInt(&sector[indexByte], 1));
			indexByte++;
		}
		if (checkValid - 32 < 0)
		{
			ReadSector(drive, readPoint - 512, sector);
			checkValid = checkValid - 32 + 512;
		}
		else checkValid -= 32;
	}
}
void RDET::readEntries(LPCWSTR  drive, int readPoint)
{
	unsigned char sector[512];
	ReadSector(drive, readPoint, sector);
	int checkValid = 11; //Kiểm tra có phải là file, folder hay Read-Only không
	int indexByte;
	string fullName;
	string fileExtension;
	int objSize = 0;
	//int count = 0;
	while (checkValid < 512)
	{
		if (sector[checkValid] == 32)
		{
			//File không có entry phụ
			indexByte = checkValid - 11;
			//Lấy tên từ 8 byte đầu
			for (int i = 0; i < 8; ++i)
			{
				fullName.push_back((char)sector[indexByte]);
				indexByte++;
			}
			//Lấy phần mở rộng trong 3 byte sau
			for (int j = 0; j < 3; ++j)
			{
				fileExtension.push_back((char)sector[indexByte]);
				indexByte++;
			}
			//Lấy kích cỡ object
			indexByte = checkValid - 11 + 28;
			objSize = charToInt(&sector[indexByte], 4);


			//Kiểm tra file có entry phụ nằm ở sector khác
			if (checkValid - 32 < 0)
			{
				cout << "Found fake entry in another sector" << endl;
				readPoint -= 512;
				ReadSector(drive, readPoint, sector);
				checkValid = checkValid + 512;
			}
			//Trường hợp có entry phụ
			if (sector[checkValid - 32] == 15)
			{
				//fullName.erase();
				handleFakeEntries(drive, readPoint, sector, checkValid, fullName);
				cout << fullName << endl;
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
		checkValid += 32;
	}
	system("pause");
}