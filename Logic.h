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
#include <windows.h>
#include <string>

class Logic{
private:
    std::mutex frameMutex;
    cv::Mat sharedFrame;
    HANDLE hSerial = INVALID_HANDLE_VALUE;
    bool serialOpened = false;

public:
    static void ZerowanieRamienia(AppState&);
    void SendCommand(AppState& state, std::string cmd);
    void ParseCommand(AppState& state);
    void AddLog(AppState& state, std::string cmd);

    void CameraLoop(AppState& state);
    void UpdateTexture(AppState& state);
    std::vector<std::string> GetAvailableCameras();

    void WriteToUSB(const std::string& data);
    bool ConnectUSB(AppState& state);
    void DisconnectUSB(AppState& state);
    std::vector<std::string> GetAvailableComPorts();
};