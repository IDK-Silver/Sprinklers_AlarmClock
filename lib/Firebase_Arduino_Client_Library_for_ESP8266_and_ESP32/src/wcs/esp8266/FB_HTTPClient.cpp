/**
 * HTTP Client wrapper v1.1.5
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FB_HTTPClient_CPP
#define FB_HTTPClient_CPP

#ifdef ESP8266

#include "FB_HTTPClient.h"

FB_HTTPClient::FB_HTTPClient()
{
}

FB_HTTPClient::~FB_HTTPClient()
{
  if (_wcs)
  {
    _wcs->stop();
    _wcs.reset(nullptr);
    _wcs.release();
  }
  std::string().swap(_host);
  std::string().swap(_CAFile);
  _cacert.reset(new char);
  _cacert = nullptr;
}

bool FB_HTTPClient::begin(const char *host, uint16_t port)
{
  if (strcmp(_host.c_str(), host) != 0)
    mflnChecked = false;

  _host = host;
  _port = port;

  //probe for fragmentation support at the specified size
  if (!mflnChecked)
  {
    fragmentable = _wcs->probeMaxFragmentLength(_host.c_str(), _port, chunkSize);
    if (fragmentable)
    {
      _bsslRxSize = chunkSize;
      _bsslTxSize = chunkSize;
      _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);
    }
    mflnChecked = true;
  }

  if (!fragmentable)
    _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);

  return true;
}

bool FB_HTTPClient::connected()
{
  if (_wcs)
    return (_wcs->connected());
  return false;
}


bool FB_HTTPClient::send(const char *header)
{
  if (!connected())
    return false;
  return (_wcs->write((uint8_t*)header, strlen(header)) == strlen(header));
}

int FB_HTTPClient::send(const char *header, const char *payload)
{
  size_t size = strlen(payload);
  if (strlen(header) > 0)
  {
    if (!connect())
    {
      return FIREBASE_ERROR_HTTPC_ERROR_CONNECTION_REFUSED;
    }

    if (!send(header))
    {
      return FIREBASE_ERROR_HTTPC_ERROR_SEND_HEADER_FAILED;
    }
  }

  if (size > 0)
  {
    if (_wcs->write((uint8_t *)&payload[0], size) != size)
    {
      return FIREBASE_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
    }
  }

  return 0;
}


WiFiClient *FB_HTTPClient::stream(void)
{
  if (connected())
    return _wcs.get();
  return nullptr;
}

bool FB_HTTPClient::connect(void)
{
  if (connected())
  {
    while (_wcs->available() > 0)
      _wcs->read();
    return true;
  }

  if (!_wcs->connect(_host.c_str(), _port))
    return false;

  return connected();
}

void FB_HTTPClient::setCACert(const char *caCert)
{

#ifndef USING_AXTLS
  _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);
#endif

  if (caCert)
  {
#ifndef USING_AXTLS
    _wcs->setTrustAnchors(new X509List(caCert));
#else
    _wcs->setCACert_P(caCert, strlen_P(caCert));
#endif
    _certType = 1;
  }
  else
  {
#ifndef USING_AXTLS
    _wcs->setInsecure();
#endif
    _certType = 0;
  }

  _wcs->setNoDelay(true);
}

void FB_HTTPClient::setCACertFile(const char* caCertFile, uint8_t storageType, uint8_t sdPin)
{

#ifndef USING_AXTLS
  _sdPin = sdPin;
  _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);

  if (_clockReady && strlen(caCertFile) > 0)
  {

    if (storageType == 1)
    {
      bool t = FLASH_FS.begin();
      if (t)
      {
        fs::File f;

        if (FLASH_FS.exists(caCertFile))
        {
          f = FLASH_FS.open(caCertFile, "r");
          size_t len = f.size();
          uint8_t *der = new uint8_t[len];

          if (f.available())
            f.read(der, len);

          f.close();
          _wcs->setTrustAnchors(new X509List(der, len));
          delete[] der;
        }
      }
    }
    else if (storageType == 2)
    {
      bool t = SD.begin(_sdPin);
      if (t)
      {
        File f;
        if (SD.exists(caCertFile))
        {
          f = SD.open(caCertFile, FILE_READ);
          size_t len = f.size();
          uint8_t *der = new uint8_t[len];
          if (f.available())
            f.read(der, len);

          f.close();
          _wcs->setTrustAnchors(new X509List(der, len));
          delete[] der;
        }
      }
    }
    _certType = 2;
  }
#endif
  _wcs->setNoDelay(true);
}




#endif /* ESP8266 */

#endif /* FB_HTTPClient_CPP */