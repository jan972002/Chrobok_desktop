//
// Created by jan97 on 08.01.2026.
//

#pragma once
#include "AppState.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <opencv2/core/ocl.hpp>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <math.h>
#include <algorithm>

class Logic{
private:
    std::mutex frameMutex;
    cv::Mat sharedFrame;
    HANDLE hSerial = INVALID_HANDLE_VALUE;
    bool serialOpened = false;
    SOCKET sock = INVALID_SOCKET;
    sockaddr_in robotAddr;
    bool networkInitialized = false;

public:
    static void ZerowanieRamienia(AppState&);
    void SendCommand(AppState& state, std::string cmd);
    void ParseCommand(AppState& state);
    void AddLog(AppState& state, std::string cmd);

    void CameraLoop(AppState& state);
    void UpdateTexture(AppState& state);
    std::vector<std::string> GetAvailableCameras();

    // USB
    void WriteToUSB(const std::string& data);
    bool ConnectUSB(AppState& state);
    void DisconnectUSB(AppState& state);
    std::vector<std::string> GetAvailableComPorts();

    //SIEĆ
    void ConnectToNetwork(AppState& state);
    void SendToNetwork(std::string packet);
    void DisconnectNetwork();
    void getSSID(AppState& state);


    ///////////////////////////////////////////
    ///                                     ///
    ///             KINEMATYKA              ///
    ///                                     ///
    ///////////////////////////////////////////

    float CalculateDSide(AppState& state);
    float CalculateLokiec(AppState& state);
    float CalculatePodstawa(AppState& state);
    void CalculateKinematics(AppState& state);

};