//
// Created by jan97 on 08.01.2026.
//
#include "Logic.h"
#include "AppState.h"
#include <iostream>
#include<thread>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <opencv2/core/ocl.hpp>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <math.h>
#include <algorithm>

void Logic::getSSID(AppState& state) {
    char buffer[128];
    std::string tempSsid = "Disconnected";
    FILE* pipe = _popen("for /f \"tokens=2 delims=: \" %a in ('netsh wlan show interfaces ^| findstr /C:\" SSID\"') do @echo %a", "r");
    if (pipe) {
        if (fgets(buffer, 128, pipe)) {
            tempSsid = buffer;
            while (!tempSsid.empty() && (tempSsid.back() == '\n' || tempSsid.back() == '\r' || tempSsid.back() == ' ')) {
                tempSsid.pop_back();
            }
        }
        _pclose(pipe);
    }
    state.SSID = tempSsid;
}

void Logic::ConnectToNetwork(AppState& state) {
    if (networkInitialized) {
        closesocket(sock);
        WSACleanup();
    }
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Błąd WSAStartup" << std::endl;
        return;
    }
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Nie można utworzyć socketu" << std::endl;
        WSACleanup();
        return;
    }
    robotAddr.sin_family = AF_INET;
    robotAddr.sin_port = htons(state.robot_port);
    inet_pton(AF_INET, state.robot_ip.c_str(), &robotAddr.sin_addr);
    networkInitialized = true;
    std::cout << "[NETWORK]: Polaczono z robotem (UDP Ready)" << std::endl;
}

void Logic::SendToNetwork(std::string packet) {
    if (!networkInitialized) {
        return;
    }
    int result = sendto(sock, packet.c_str(), (int)packet.length(), 0,
                        (sockaddr*)&robotAddr, sizeof(robotAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Blad wysylki UDP: " << WSAGetLastError() << std::endl;
    }
}
void Logic::DisconnectNetwork() {
    if (networkInitialized) {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
        WSACleanup();
        networkInitialized = false;
        std::cout << "[NETWORK]: Rozłączono pomyślnie i zwolniono zasoby." << std::endl;
    }
}


bool Logic::ConnectUSB(AppState& state) {
    std::string portName = state.portName;
    std::string fullPortName = "\\\\.\\" + portName;

    // Próba otwarcia portu
    hSerial = CreateFileA(
        fullPortName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cout << "[USB]: Nie udalo sie otworzyc " << portName << " (Blad: " << GetLastError() << ")" << std::endl;
        state.is_usb_connected = false;
        return false;
    }
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams)) {
        dcbSerialParams.BaudRate = CBR_115200;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cout << "[USB]: Blad ustawiania parametrow portu." << std::endl;
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
            return false;
        }
    }
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    std::cout << "[USB]: Polaczono pomyslnie z " << portName << std::endl;
    state.is_usb_connected = true;
    return true;
}

void Logic::WriteToUSB(const std::string& data) {
    if (hSerial == INVALID_HANDLE_VALUE) return;
    DWORD bytesSent;
    if (!WriteFile(hSerial, data.c_str(), (DWORD)data.length(), &bytesSent, NULL)) {
        std::cout << "[BŁĄD USB]: Utracono połączenie z portem." << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}
void Logic::DisconnectUSB(AppState& state) {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
    state.is_usb_connected = false;
}
void Logic::ZerowanieRamienia(AppState& state) {
    state.M_1 = 500;
    state.M_2 = 90;
    state.M_3 = 30;
    state.M_4 = 34;
    state.M_5 = 90;
    state.wspolrzednaX = 7;
    state.wspolrzednaY = 13;
    state.zerowanie_ramienia = true;
}

void Logic::AddLog(AppState& state, std::string cmd) {
    state.historia_komend.push_back(cmd);
    if (state.historia_komend.size() > state.max_logow) {
        state.historia_komend.erase(state.historia_komend.begin());
    }
}

void Logic::SendCommand(AppState& state, std::string cmd) {
    std::string packet = cmd + "\n";
    AddLog(state, cmd);
    std::cout << "[WYSLANO]: " << packet;

    // DO WYSYLU PO USB, ZMIENIC NA SIECIOWYM
    if (hSerial != INVALID_HANDLE_VALUE) {
        WriteToUSB(packet);
    }
    SendToNetwork(packet);
}

std::vector<std::string> Logic::GetAvailableComPorts() {
    std::vector<std::string> ports;
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char valueName[256];
        char portName[256];
        DWORD valueNameLen, portNameLen, type;

        for (DWORD i = 0; ; ++i) {
            valueNameLen = 256;
            portNameLen = 256;
            if (RegEnumValueA(hKey, i, valueName, &valueNameLen, NULL, &type, (LPBYTE)portName, &portNameLen) != ERROR_SUCCESS)
                break;
            ports.push_back(std::string(portName));
        }
        RegCloseKey(hKey);
    }
    if (ports.empty()) ports.push_back("Brak portów");
    return ports;
}
void Logic::ParseCommand(AppState& state) {

    bool last_przod = false;
    bool last_tyl = false;
    bool last_lewo = false;
    bool last_prawo = false;
    bool last_skret_lewo = false;
    bool last_skret_prawo = false;

    int last_M1 = -1;
    int last_M2 = -1;
    int last_M3 = -1;
    int last_M4 = -1;
    int last_M5 = -1;
    int last_pwm = -1;
    int last_sila_skretu = -1;
    int last_opoznienie_chwytaka = -1;
    int last_oposnienie_serwa = -1;

    while (state.is_running) {
        if (state.ruch_przod != last_przod) {
            last_przod = state.ruch_przod;
            last_przod ? SendCommand(state,"Przod\n") : SendCommand(state,"STOP");
        }
        if (state.ruch_tyl != last_tyl) {
            last_tyl = state.ruch_tyl;
            last_tyl ? SendCommand(state,"Tyl") : SendCommand(state,"STOP");
        }
        if (state.ruch_lewo != last_lewo) {
            last_lewo = state.ruch_lewo;
            last_lewo ? SendCommand(state,"Lewo") : SendCommand(state,"STOP");
        }
        if (state.ruch_prawo != last_prawo) {
            last_prawo = state.ruch_prawo;
            last_prawo ? SendCommand(state,"Prawo") : SendCommand(state,"STOP");
        }
        if (state.skret_lewo != last_skret_lewo) {
            last_skret_lewo = state.skret_lewo;
            last_skret_lewo ? SendCommand(state,"WMiejscuWLewo") : SendCommand(state,"STOP");
        }
        if (state.skret_prawo != last_skret_prawo) {
            last_skret_prawo = state.skret_prawo;
            last_skret_prawo ? SendCommand(state,"WMiejscuWPrawo") : SendCommand(state,"STOP");
        }
        if (state.zatrzymanie) {
            SendCommand(state,"STOP");
            state.zatrzymanie = false;
        }
        if (state.zerowanie_ramienia) {
            SendCommand(state,"ZerowaPozycja");
            state.zerowanie_ramienia = false;
        }

        if (state.M_1 != last_M1) { last_M1 = state.M_1; SendCommand(state,"M1:" + std::to_string(last_M1)); }
        if (state.M_2 != last_M2) { last_M2 = state.M_2; SendCommand(state,"M2:" + std::to_string(last_M2)); }
        if (state.M_3 != last_M3) { last_M3 = state.M_3; SendCommand(state,"M3:" + std::to_string(last_M3)); }
        if (state.M_4 != last_M4) { last_M4 = state.M_4; SendCommand(state,"M4:" + std::to_string(last_M4)); }
        if (state.M_5 != last_M5) { last_M5 = state.M_5; SendCommand(state,"M5:" + std::to_string(last_M5)); }

        if (state.PWM != last_pwm) { last_pwm = state.PWM; SendCommand(state,"PWM:" + std::to_string(last_pwm)); }
        if (state.silaSkretu != last_sila_skretu) { last_sila_skretu = state.silaSkretu; SendCommand(state,"silaSkretu:" + std::to_string(last_sila_skretu)); }
        if (state.opoznienieChwytaka != last_opoznienie_chwytaka) { last_opoznienie_chwytaka = state.opoznienieChwytaka; SendCommand(state,"opoznienieChwytaka:" + std::to_string(last_opoznienie_chwytaka)); }
        if (state.opoznienieServa != last_oposnienie_serwa) { last_oposnienie_serwa = state.opoznienieServa; SendCommand(state,"opoznienieRuchuServa:" + std::to_string(last_oposnienie_serwa)); }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void Logic::CameraLoop(AppState& state) {
    int current_device = -1;
    while (state.is_running) {
        if (state.camera_is_running) {
            if (state.camera_needs_reset || current_device != state.selected_camera_index) {
                state.cap.release();
                current_device = state.selected_camera_index;
                if (current_device >= 0) {
                    state.cap.open(current_device);
                }
                state.camera_needs_reset = false;
            }
            if (!state.cap.isOpened()) {
                if (current_device >= 0) {
                    state.cap.open(current_device);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
            cv::Mat frame;
            if (state.cap.read(frame) && !frame.empty()) {
                std::lock_guard<std::mutex> lock(frameMutex);
                frame.copyTo(sharedFrame);
            }
        } else {
            if (state.cap.isOpened()) {
                state.cap.release();
                current_device = -1;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void Logic::UpdateTexture(AppState& state) {
    if (!state.camera_is_running) return;
    cv::Mat frameToUpload;
    {
        std::lock_guard<std::mutex> lock(frameMutex);
        if (sharedFrame.empty()) return;
        sharedFrame.copyTo(frameToUpload);
    }
    cv::Mat rgbaFrame;
    cv::cvtColor(frameToUpload, rgbaFrame, cv::COLOR_BGR2RGBA);
    if (state.cameraTexture == 0) {
        glGenTextures(1, &state.cameraTexture);
    }
    glBindTexture(GL_TEXTURE_2D, state.cameraTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaFrame.cols, rgbaFrame.rows, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, rgbaFrame.data);
}

std::vector<std::string> Logic::GetAvailableCameras() {
    std::vector<std::string> devices;
    cv::VideoCapture temp_cap;
    for (int i = 0; i < 5; i++) {
        temp_cap.open(i, cv::CAP_DSHOW);
        if (temp_cap.isOpened()) {
            devices.push_back("Kamera " + std::to_string(i));
            temp_cap.release();
        }
    }
    if (devices.empty()) {
        devices.push_back("Brak dostępnych kamer");
    }
    return devices;
}


///////////////////////////////////////////
///                                     ///
///             KINEMATYKA              ///
///                                     ///
///////////////////////////////////////////


float Logic::CalculateDSide(AppState& state) {
    float bokD = std::sqrt((state.wspolrzednaX * state.wspolrzednaX) + (state.wspolrzednaY * state.wspolrzednaY));
    return bokD;
}

float Logic::CalculateLokiec(AppState& state) {
    float d = CalculateDSide(state);
    float arg = (((state.dlugoscPrzedramienia * state.dlugoscPrzedramienia) + (state.dlugoscPodstawy * state.dlugoscPodstawy) - (d * d)) / (2 * state.dlugoscPrzedramienia * state.dlugoscPodstawy));
    float alfa_wewn = ((std::acos(std::clamp(arg, -1.0f, 1.0f))) * (180.0f / 3.1415926535f));

    if (state.wspolrzednaX >= 0) {
        return std::round(alfa_wewn);
    }
    else {
        return std::round(360.0f - alfa_wewn);
    }
}

float Logic::CalculatePodstawa(AppState& state) {
    float bokD = CalculateDSide(state);
    float gamma = std::atan2(state.wspolrzednaY, state.wspolrzednaX) * (180.0f / 3.1415926535f);
    float arg = ((state.dlugoscPodstawy * state.dlugoscPodstawy) + (bokD * bokD) - (state.dlugoscPrzedramienia * state.dlugoscPrzedramienia)) / (2 * state.dlugoscPodstawy * bokD);
    float beta = std::acos(std::clamp(arg, -1.0f, 1.0f)) * (180.0f / 3.1415926535f);
    float kat_koncowy;
    if (state.wspolrzednaX >= 0) {
        kat_koncowy = gamma + beta;
    }
    else {
        kat_koncowy = gamma - beta;
    }
    return std::round(kat_koncowy);
}

void Logic::CalculateKinematics(AppState& state) {
    float katPodstawy = CalculatePodstawa(state);
    float katLokiec = CalculateLokiec(state);

    state.M_4 = static_cast<int>(120 - katPodstawy + state.offsetPodstawy);
    state.M_3 = static_cast<int>(katLokiec);
    std::cout << "[IK]: Przeliczono X:" << state.wspolrzednaX << " Y:" << state.wspolrzednaY << " Na katy: M4:" << state.M_4 << " M3:" << state.M_3 << std::endl;
}
