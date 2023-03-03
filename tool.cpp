#include "HDH.h"





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


int ReadSector(LPCWSTR  drive, int readPoint, unsigned char sector[512])
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

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
}



void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void prinfFat32()
{
    GotoXY(40, 10);
    cout << "  ______   _______ ____ ___   " << endl;
    GotoXY(40, 11);
    cout << " |  ____/\\|__   __|___ \\__ \\  " << endl;
    GotoXY(40, 12);
    cout << " | |__ /  \\  | |    __) | ) |  " << endl;
    GotoXY(40, 13);
    cout << " |  __/ /\\ \\ | |   |__ < / / " << endl;
    GotoXY(40, 14);
    cout << " | | / ____ \\| |   ___) / /_ " << endl;
    GotoXY(40, 15);
    cout << " |_|/_/    \\_\\_|  |____/____|" << endl;

}
void printfNtfs()
{
    GotoXY(40, 10);
    cout << "  _   _ _______ ______ _____  " << endl;
    GotoXY(40, 11);
    cout << " | \\ | |__   __|  ____/ ____| " << endl;
    GotoXY(40, 12);
    cout << " |  \\| |  | |  | |__ | (___  " << endl;
    GotoXY(40, 13);
    cout << " | . ` |  | |  |  __| \\___ \\ " << endl;
    GotoXY(40, 14);
    cout << " | |\\  |  | |  | |    ____) |" << endl;
    GotoXY(40, 15);
    cout << " |_| \\_|  |_|  |_|   |_____/ " << endl;

}
void quanlywindow()
{
    GotoXY(20, 0);
    cout << "   ____                      _         _                _   _    " << endl;
    GotoXY(20, 1);
    cout << "  / __ \\                    | |       | |              | | (_)      " << endl;
    GotoXY(20, 2);
    cout << " | |  | |_   _  __ _ _ __   | |_   _  | |_ __ _ _ __   | |_ _ _ __  " << endl;
    GotoXY(20, 3);
    cout << " | |  | | | | |/ _` | '_ \\  | | | | | | __/ _` | '_ \\  | __| | '_ \\" << endl;
    GotoXY(20, 4);
    cout << " | |__| | |_| | (_| | | | | | | |_| | | || (_| | |_) | | |_| | | | |" << endl;
    GotoXY(20, 5);
    cout << "  \\___\\_\\\\__,_|\\__,_|_| |_| |_|\\__, |  \\__\\__,_| .__/   \\__|_|_| |_|" << endl;
    GotoXY(20, 6);
    cout << "                                __/ |          | |                  " << endl;
    GotoXY(20, 7);
    cout << "                               |___/           |_|                  " << endl;
    GotoXY(20, 10);
    cout << "  _                             _           _                   " << endl;
    GotoXY(20, 11);
    cout << " | |                           (_)         | |                  " << endl;
    GotoXY(20, 12);
    cout << " | |_ _ __ ___ _ __   __      ___ _ __   __| | _____      _____ " << endl;
    GotoXY(20, 13);
    cout << " | __| '__/ _ \\ '_ \\  \\ \\ /\\ / / | '_ \\ / _` |/ _ \\ \\ /\\ / / __|" << endl;
    GotoXY(20, 14);
    cout << " | |_| | |  __/ | | |  \\ V  V /| | | | | (_| | (_) \\ V  V /\\__ \\" << endl;
    GotoXY(20, 15);
    cout << "  \\__|_|  \\___|_| |_|   \\_/\\_/ |_|_| |_|\\__,_|\\___/ \\_/\\_/ |___/" << endl;
}
void drawMenu()
{
    unsigned char vborder = 205, hborder = 186, lowerleft = 200, lowerright = 188, upperleft = 201, upperright = 187;
    GotoXY(17 + 25, 19);
    for (int i = 0; i < 26; i++)
        cout << vborder;
    GotoXY(16 + 25, 19); cout << upperleft;
    GotoXY(16 + 25, 20); cout << hborder;
    GotoXY(16 + 25, 21); cout << hborder;
    GotoXY(16 + 25, 22); cout << hborder;
    GotoXY(16 + 25, 23); cout << hborder;
    GotoXY(16 + 25, 24); cout << hborder;
    GotoXY(16 + 25, 25); cout << hborder;
    GotoXY(16 + 25, 26); cout << lowerleft;
    GotoXY(17 + 25, 26);
    for (int i = 0; i < 26; i++)
        cout << vborder;
    GotoXY(16 + 27 + 25, 19); cout << upperright;
    GotoXY(16 + 27 + 25, 20); cout << hborder;
    GotoXY(16 + 27 + 25, 21); cout << hborder;
    GotoXY(16 + 27 + 25, 22); cout << hborder;
    GotoXY(16 + 27 + 25, 23); cout << hborder;
    GotoXY(16 + 27 + 25, 24); cout << hborder;
    GotoXY(16 + 27 + 25, 25); cout << hborder;
    GotoXY(16 + 27 + 25, 26); cout << lowerright;
}





