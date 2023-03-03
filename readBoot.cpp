#include "HDH.h"

NTFS readNTFS(LPCWSTR path) {
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
    return NTFS(sector);
}

FAT32 readFAT32(LPCWSTR path) {
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
    return FAT32(sector);
}