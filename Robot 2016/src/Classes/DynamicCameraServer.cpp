/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DynamicCameraServer.h"
#include "WPIErrors.h"
#include "Utility.h"

#include <iostream>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

constexpr uint8_t DynamicCameraServer::kMagicNumber[];

DynamicCameraServer* DynamicCameraServer::GetInstance() {
  static DynamicCameraServer instance;
  return &instance;
}

DynamicCameraServer::DynamicCameraServer()
    : m_camera(),
      m_serverThread(&DynamicCameraServer::Serve, this),
      m_captureThread(),
      m_imageMutex(),
      m_newImageVariable(),
      m_dataPool(3),
      m_quality(50),
      m_autoCaptureStarted(false),
	  m_autoCaptureRunning(false),
      m_hwClient(true),
      m_imageData(nullptr, 0, 0, false) {
  for (int i = 0; i < 3; i++) m_dataPool.push_back(new uint8_t[kMaxImageSize]);
}

void DynamicCameraServer::FreeImageData(
    std::tuple<uint8_t*, unsigned int, unsigned int, bool> imageData) {
  if (std::get<3>(imageData))
    imaqDispose(std::get<0>(imageData));
  else if (std::get<0>(imageData) != nullptr) {
    std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
    m_dataPool.push_back(std::get<0>(imageData));
  }
}

void DynamicCameraServer::SetImageData(uint8_t* data, unsigned int size,
                                unsigned int start, bool imaqData) {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  FreeImageData(m_imageData);
  m_imageData = std::make_tuple(data, size, start, imaqData);
  m_newImageVariable.notify_all();
}

void DynamicCameraServer::SetImage(Image const* image) {
  unsigned int dataSize = 0;
  uint8_t* data =
      (uint8_t*)imaqFlatten(image, IMAQ_FLATTEN_IMAGE, IMAQ_COMPRESSION_JPEG,
                            10 * m_quality, &dataSize);

  // If we're using a HW camera, then find the start of the data
  bool hwClient;
  {
    // Make a local copy of the hwClient variable so that we can safely use it.
    std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
    hwClient = m_hwClient;
  }
  unsigned int start = 0;
  if (hwClient) {
    while (start < dataSize - 1) {
      if (data[start] == 0xFF && data[start + 1] == 0xD8)
        break;
      else
        start++;
    }
  }
  dataSize -= start;

  wpi_assert(dataSize > 2);
  SetImageData(data, dataSize, start, true);
}

void DynamicCameraServer::AutoCapture() {
	m_autoCaptureRunning = true;
	std::cout << "Started AutoCapture" << std::endl;
  Image* frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);

  while (m_autoCaptureStarted) {
    bool hwClient;
    uint8_t* data = nullptr;
    {
      std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
      hwClient = m_hwClient;
      if (hwClient) {
        data = m_dataPool.back();
        m_dataPool.pop_back();
      }
    }

    if (hwClient) {
      unsigned int size = m_camera->GetImageData(data, kMaxImageSize);
      SetImageData(data, size);
    } else {
      m_camera->GetImage(frame);
      SetImage(frame);
    }
  }
  std::cout << "Ended AutoCapture" << std::endl;
  m_autoCaptureRunning = false;
}

void DynamicCameraServer::StartAutomaticCapture(char const* cameraName) {
  std::shared_ptr<USBCamera> camera =
      std::make_shared<USBCamera>(cameraName, true);
  camera->OpenCamera();
  StartAutomaticCapture(camera);
}

void DynamicCameraServer::StartAutomaticCapture(std::shared_ptr<USBCamera> camera) {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  if (m_autoCaptureStarted) return;

  m_camera = camera;
  m_camera->SetExposureAuto();
  m_camera->StartCapture();

  m_autoCaptureStarted = true;
  m_captureThread = std::thread(&DynamicCameraServer::AutoCapture, this);
  m_captureThread.detach();
}

void DynamicCameraServer::SwitchAutomaticCapture(char const* cameraName) {
	//std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);

	if (!m_autoCaptureStarted) {
		StartAutomaticCapture(cameraName);
		return;
	}

	m_autoCaptureStarted = false;
	std::cout << "Looping to wait for cap death" << std::endl;
	while (m_autoCaptureRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	std::cout << "It's dead, Jim" << std::endl;
	m_camera->StopCapture();
	m_camera->CloseCamera();

	StartAutomaticCapture(cameraName);
}

bool DynamicCameraServer::IsAutoCaptureStarted() {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  return m_autoCaptureStarted;
}

void DynamicCameraServer::SetSize(unsigned int size) {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  if (!m_camera) return;
  if (size == kSize160x120)
    m_camera->SetSize(160, 120);
  else if (size == kSize320x240)
    m_camera->SetSize(320, 240);
  else if (size == kSize640x480)
    m_camera->SetSize(640, 480);
}

void DynamicCameraServer::SetQuality(unsigned int quality) {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  m_quality = quality > 100 ? 100 : quality;
}

unsigned int DynamicCameraServer::GetQuality() {
  std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
  return m_quality;
}

void DynamicCameraServer::Serve() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    wpi_setErrnoError();
    return;
  }

  int reuseAddr = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddr,
                 sizeof(reuseAddr)) == -1)
    wpi_setErrnoError();

  sockaddr_in address, clientAddress;

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(kPort);

  if (bind(sock, (struct sockaddr*)&address, sizeof(address)) == -1)
    wpi_setErrnoError();

  if (listen(sock, 10) == -1) wpi_setErrnoError();

  while (true) {
    socklen_t clientAddressLen = sizeof(clientAddress);

    int conn =
        accept(sock, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (conn == -1) {
      wpi_setErrnoError();
      continue;
    }

    Request req;
    if (read(conn, &req, sizeof(req)) == -1) {
      wpi_setErrnoError();
      close(conn);
      continue;
    } else {
      req.fps = ntohl(req.fps);
      req.compression = ntohl(req.compression);
      req.size = ntohl(req.size);
    }

    // TODO: Support the SW Compression. The rest of the code below will work as
    // though this
    // check isn't here
    if (req.compression != kHardwareCompression) {
      wpi_setWPIErrorWithContext(IncompatibleState,
                                 "Choose \"USB Camera HW\" on the dashboard");
      close(conn);
      continue;
    }

    {
      // Wait for the camera to be setw
      std::unique_lock<priority_recursive_mutex> lock(m_imageMutex);
      if (!m_camera) {
        std::cout << "Camera not yet ready, awaiting first image" << std::endl;
        m_newImageVariable.wait(lock);
      }
      m_hwClient = req.compression == kHardwareCompression;
      if (!m_hwClient)
        SetQuality(100 - req.compression);
      else if (m_camera)
        m_camera->SetFPS(req.fps);
      SetSize(req.size);
    }

    auto period = std::chrono::microseconds(1000000) / req.fps;
    while (true) {
      auto startTime = std::chrono::steady_clock::now();
      std::tuple<uint8_t*, unsigned int, unsigned int, bool> imageData;
      {
        std::unique_lock<priority_recursive_mutex> lock(m_imageMutex);
        m_newImageVariable.wait(lock);
        imageData = m_imageData;
        m_imageData = std::make_tuple<uint8_t*>(nullptr, 0, 0, false);
      }

      unsigned int size = std::get<1>(imageData);
      unsigned int netSize = htonl(size);
      unsigned int start = std::get<2>(imageData);
      uint8_t* data = std::get<0>(imageData);

      if (data == nullptr) continue;

      if (write(conn, kMagicNumber, sizeof(kMagicNumber)) == -1) {
        wpi_setErrnoErrorWithContext(
            "[DynamicCameraServer] Error sending magic number");
        FreeImageData(imageData);
        break;
      }
      if (write(conn, &netSize, sizeof(netSize)) == -1) {
        wpi_setErrnoErrorWithContext("[DynamicCameraServer] Error sending image size");
        FreeImageData(imageData);
        break;
      }
      if (write(conn, &data[start], sizeof(uint8_t) * size) == -1) {
        wpi_setErrnoErrorWithContext("[DynamicCameraServer] Error sending image data");
        FreeImageData(imageData);
        break;
      }
      FreeImageData(imageData);
      std::this_thread::sleep_until(startTime + period);
    }
    close(conn);
  }
  close(sock);
}
