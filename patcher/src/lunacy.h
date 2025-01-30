#pragma once

#include <elf.h>
#include <string>
#include <fstream>
#include <cstdint>
#include <stdarg.h>

#include "dobby.h"
#include "logger.h"

using fn_curl_easy_setopt   =   void*(*)(void* instance, int tag, ...);
using fn_crypt_response     =   void(*)(void* keys, void* buffer, const char* output, size_t size, int mode);

#define BINARY_PATH "/data/local/tmp/program"

#define CURL_EASE_SETOPT    0x36BE10
#define CRYPT_RESPONSE      0x2D0C00

#define CURLOPT_HTTPHEADER      10023
#define CURLOPT_URL             10002
#define CURLOPT_FOLLOWLOCATION  52
#define CURLOPT_WRITEFUNCTION   20011
#define CURLOPT_FILE            10001
#define CURLOPT_POST            47
#define CURLOPT_POSTFIELDS      10015
#define CURLOPT_SSL_VERIFYPEER  64
#define CURLOPT_SSL_VERIFYHOST  81

#define CRYPTED_RESPONSE_LENGTH 236
#define CRYPTED_RESPONSE_DATA   "vBgN0nR2b4/FZy7eADRBibR9VMqco2ivuOgHlB3jblIFG1uxlHP5QTdnBheYrWZ7b0hhHP2LknP9w3NaJL6mLmDAy/O8T8daA5sK/f13y1lkLUwiwf+CPg+pNhXROYFk0Qg/CyXPncSTmYSN+w/9nunRAO8/eVz7LNzh4BpIs51EgtfrdpmTFzppDWXHSGEDi9ZbFlhr4zngpD9Y3J99CMnUHjsJjAKYBYCNHnCN4A4="

#define JSON_RESPONSE_LENGTH    165
#define JSON_RESPONSE_DATA      "{\"timestamp\":%lu,\"status\":\"key_success\",\"hwid\":\"%s\",\"malware\":0}"

struct lunacy_t
{
    unsigned long long memory;

    unsigned long timestamp;

    std::string key;

    std::string device;

    bool find_memory();

    void setup_hooks();

    void open_telegram();
};

extern lunacy_t* lunacy;