#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <shlwapi.h>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h> 

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")


bool exitProgram = false;

COLORREF GetPixelColor(int x, int y) {
    HDC hdc = GetDC(NULL);
    COLORREF color = GetPixel(hdc, x, y);
    ReleaseDC(NULL, hdc);
    return color;
}

void ClickMouse() {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
}

bool IsColorClose(COLORREF a, COLORREF b, int tolerance) {
    int rA = GetRValue(a), gA = GetGValue(a), bA = GetBValue(a);
    int rB = GetRValue(b), gB = GetGValue(b), bB = GetBValue(b);
    return std::abs(rA - rB) <= tolerance && std::abs(gA - gB) <= tolerance && std::abs(bA - bB) <= tolerance;
}

bool IsMatchingTargetColor(COLORREF targetColor[], int count, int tolerance) {
    for (int i = 0; i < count; i++) {
        COLORREF pixelColor = GetPixelColor(960, 540);
        if (IsColorClose(pixelColor, targetColor[i], tolerance)) return true;
    }
    return false;
}

bool IsAimLabRunning() {
    HWND hwnd = GetForegroundWindow();
    DWORD processID;
    GetWindowThreadProcessId(hwnd, &processID);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (!hProcess) return false;
    char processName[MAX_PATH] = "<unknown>";
    GetModuleFileNameExA(hProcess, NULL, processName, MAX_PATH);
    CloseHandle(hProcess);
    return strcmp(PathFindFileNameA(processName), "AimLab_tb.exe") == 0;
}

bool FileExists(const std::string& path) {
    DWORD fileAttrib = GetFileAttributesA(path.c_str());
    return (fileAttrib != INVALID_FILE_ATTRIBUTES &&
        !(fileAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::string FindAimLabPath() {
    const std::vector<std::string> drives = { "C:", "D:", "E:" };
    for (const auto& drive : drives) {
        std::string path = drive + "\\SteamLibrary\\steamapps\\common\\Aim Lab\\AimLab_tb.exe";
        if (FileExists(path)) {
            return path;
        }
    }
    return "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Aim Lab\\AimLab_tb.exe";
}

void StartAimLab() {
    std::string aimLabPath = FindAimLabPath();
    if (FileExists(aimLabPath)) {
        std::cout << "AimLab baslatiliyor: " << aimLabPath << std::endl;
        ShellExecuteA(NULL, "open", aimLabPath.c_str(), NULL, NULL, SW_SHOW);
    }
    else {
        std::cout << "AimLab bulunamadi! Lütfen yolu kontrol edin." << std::endl;
    }
}

bool CloseProcessByName(const char* processName) {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);

    if (Process32First(hSnapShot, &pEntry)) {
        do {
            char exeFile[MAX_PATH];
            size_t convertedChars = 0;
            wcstombs_s(&convertedChars, exeFile, MAX_PATH, pEntry.szExeFile, _TRUNCATE);

            if (_stricmp(exeFile, processName) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                    CloseHandle(hSnapShot);
                    return true;
                }
            }
        } while (Process32Next(hSnapShot, &pEntry));
    }

    CloseHandle(hSnapShot);
    return false;
}

void CloseAimLab() {
    std::cout << "AimLab kapatiliyor..." << std::endl;
    if (CloseProcessByName("AimLab_tb.exe")) {
        std::cout << "Aimlab basariyla acildi." << std::endl;
    }
    else {
        std::cout << "Aimlab kapatilmadi veya calismiyor." << std::endl;
    }
}

DWORD WINAPI KeyCheckThread(LPVOID lpParam) {
    std::cout << "Program baslatildi. akunun Komutlar:\n";
    std::cout << "[Insert]  - Aim Lab i başlat\n";
    std::cout << "[Delete]  - Aim Lab i kapat\n";
    std::cout << "[Home]    - Uygulamayi kapat\n";

    while (!exitProgram) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 0 || key == 224) {
                key = _getch();

                if (key == 82) {
                    std::cout << "[Insert] tusuna basildigin da aimlab acilir...\n";
                    StartAimLab();
                }
                else if (key == 83) {
                    std::cout << "[Delete] tusuna basildigin da aimlab kapatilir...\n";
                    CloseAimLab();
                }
                else if (key == 71) {
                    std::cout << "[Home] Tusuna basildigin da AkuBot kapanir...\n";
                    exitProgram = true;
                }
                else {
                    std::cout << "Taninmayan Ozel TusKodu: " << key << std::endl;
                }
            }
            else {
                std::cout << "Taninmayan tus kodu yazdin: " << key << std::endl;
            }
        }
        Sleep(100);
    }

    return 0;
}

int main() {
    HANDLE hKeyThread = CreateThread(NULL, 0, KeyCheckThread, NULL, 0, NULL);

    COLORREF targetColors[] = { RGB(0, 0, 0) };
    int count = sizeof(targetColors) / sizeof(COLORREF);
    int tolerance = 30;

    while (!exitProgram) {
        if (IsAimLabRunning() && IsMatchingTargetColor(targetColors, count, tolerance)) {
            ClickMouse();
            Sleep(10);
        }
        Sleep(1);
    }

    if (hKeyThread) {
        WaitForSingleObject(hKeyThread, INFINITE);
        CloseHandle(hKeyThread);
    }

    return 0;
}
