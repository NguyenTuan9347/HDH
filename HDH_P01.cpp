#include <cstdio>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

#define ScIndex 13
#define SbIndex 14 // size la 2
#define nFIndex 16 // size la 1
#define SvIndex 32 // size 4
#define SfIndex 36 // size 4
#define RDETstartIndex 44 // size 4
#define sectorEmptyClusterIndex 48 // size 2
#define sectorBackUpBSIndex 50 // size 2    

unsigned int charToInt(unsigned char* arr ,int size) {
    unsigned char* temp = new unsigned char[size];
    unsigned int kq = 0;
    int shifter = 0, flag = 0;
    memmove((unsigned char*)&temp[0],(unsigned char*) &arr[0], size); // Copy chuoi byte
    if (size == 4 && (temp[size-1] & 0x80) == 0x80) {
        temp[size-1] = 0;
        flag++;
    }
    kq |= (unsigned int)temp[0] << shifter; // Copy byte dau vao ben phai cung
    shifter += 8;
    for (int i = 1; i <size; i++) {
        kq |= (unsigned int)temp[i] << shifter; // copy cac byte con lai lui dan ve ben trai
        shifter += 8;
    }
    if (flag == 1) {
        kq += (unsigned int)pow(2, 31);
    }
    delete[] temp;
    return kq;
}



int main(){

  // for safety we restrict to 
  // USB less than 32 GB so that 
  // accidental access to hdd 
  // can be avoided 
  const int MAX_USB_GB = 32;
  // get the drive letter 
  // from the user 
  printf("Drive letter of USB?: ");
  // scanf used for readability 
  // and simplicity only. 
  char path[] = "\\\\.\\?:";
  scanf_s("%c", path + 4, 1);
  // open the volume 
  HANDLE hVol = CreateFileA(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, 0,NULL);
  // if handle fails 
  if (INVALID_HANDLE_VALUE == hVol) {
    printf("Check drive letter.");
    return -1;

  }
  // get the physicaldrive number 
  // and the capacity of the disk 
  VOLUME_DISK_EXTENTS vde = { 0 };
  DWORD dw;
  DeviceIoControl(hVol,IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,NULL,0,(LPVOID)&vde,(DWORD)sizeof(VOLUME_DISK_EXTENTS),&dw, NULL);
  // check disk size 
  if (
  vde.Extents[0].ExtentLength.QuadPart
  >
  LONGLONG(MAX_USB_GB * 1000000000LL)
  ) {
    printf("USB too large\n");
    printf("Use < %d GB", MAX_USB_GB);
    CloseHandle(hVol);
    return -2;

  }
  // open the disk now by using 
  // the disk number 
  // path format is \\.\PhysicalDriveN 
  // so a buffer of 20 sufficient 
  char diskPath[20] = { 0 };

  // use sprintf to construct 
  // the path by appending 
  // disk number to \\.\PhysicalDrive 
  sprintf_s(diskPath,sizeof(diskPath),"\\\\.\\PhysicalDrive%d",vde.Extents[0].DiskNumber);

  // open a handle to the disk 
  HANDLE hUsb = CreateFileA(diskPath, GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, 0,NULL);

  // if handle fails 
  if (INVALID_HANDLE_VALUE == hUsb) {
    printf("Run as admn.");
    CloseHandle(hVol);
    return -3;

  }
  // buffer to store the boot sector 
  // only 512 bytes to be read 
  unsigned char sector[512];
  dw = 0;
  // file pointer begins at 0 
  // boot sector also starts at 0 
  // so read now... 
  int endFlag = ReadFile(hUsb, sector, 512, &dw, NULL);
  if (endFlag != 0) {
      for (int i = 0; i < 512; ++i) {
          if (i % 16 == 0 && i != 0) printf("\n");
          printf(" %x ", sector[i]);
      }
      printf("\n");

      printf("Cluster per sector : %u \n", charToInt(&sector[ScIndex], 1));
      printf("Cluster per Bootsector %u \n", charToInt(&sector[SbIndex], 2));
      printf("Number of FAT table : %u \n", charToInt(&sector[nFIndex], 1));
      printf("Volume size : %u \n", charToInt(&sector[SvIndex], 4));
      printf("FAT table size : %u \n", charToInt(&sector[SfIndex], 4));
      printf("Cluster staring of RDET : %u \n", charToInt(&sector[RDETstartIndex], 4));
      printf("Empty cluster info sector : %u \n", charToInt(&sector[sectorEmptyClusterIndex], 2));
      printf("Backup bootsector : %u \n", charToInt(&sector[sectorBackUpBSIndex], 2));
      //static_cast<int>(*&sector[ScIndex]));

      // // release handles 
  }
  CloseHandle(hVol);
  CloseHandle(hUsb);
  return 0;

}

