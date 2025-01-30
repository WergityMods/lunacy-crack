#include "lunacy.h"

lunacy_t* lunacy = NULL;

void parse_request(const char* data)
{
    char key[20];
    char device[100];

    std::sscanf(data, "data={\"timestamp\":%lu, \"key\":\"%[^\"]\", \"device\":\"%[^\"]\", \"game\":\"%*s\"}", &lunacy->timestamp, key, device);

    lunacy->key = key;
    lunacy->device = device;
}

size_t write_callback(const char* contents, size_t size, size_t nmemb, std::string* userp)
{
    logger::print(LOG_LEVEL_DEBUG, "write_callback: %s", contents);
    
    userp->append(CRYPTED_RESPONSE_DATA);
    return CRYPTED_RESPONSE_LENGTH;
}

fn_curl_easy_setopt(og_curl_easy_setopt) = NULL;
void* hk_curl_easy_setopt(void* instance, int tag, ...)
{
    va_list args;
    va_start(args, tag);
    void* result = NULL;

    switch (tag)
    {
        case CURLOPT_HTTPHEADER:
        {
            const char* data = va_arg(args, const char*);
            result = og_curl_easy_setopt(instance, tag, data);
            break;
        }

        case CURLOPT_URL:
        {
            const char* data = va_arg(args, const char*);
            logger::print(LOG_LEVEL_DEBUG, "curl_easy_setopt->CURLOPT_URL: %s", data);

            result = og_curl_easy_setopt(instance, tag, data);
            break;
        }

        case CURLOPT_FOLLOWLOCATION:
        {
            result = og_curl_easy_setopt(instance, tag, true);
            break;
        }
        
        case CURLOPT_WRITEFUNCTION:
        {
            result = og_curl_easy_setopt(instance, tag, write_callback);
            break;
        }

        case CURLOPT_FILE:
        {
            void* data = va_arg(args, void*);
            logger::print(LOG_LEVEL_DEBUG, "curl_easy_setopt->CURLOPT_FILE: %p", data);

            result = og_curl_easy_setopt(instance, tag, data);
            break;
        }

        case CURLOPT_POST:
        {
            result = og_curl_easy_setopt(instance, tag, true);
            break;
        }
        
        case CURLOPT_POSTFIELDS:
        {
            const char* data = va_arg(args, const char*);
            logger::print(LOG_LEVEL_DEBUG, "curl_easy_setopt->CURLOPT_POSTFIELDS: %s", data);

            parse_request(data);
            result = og_curl_easy_setopt(instance, tag, data);
            break;
        }

        case CURLOPT_SSL_VERIFYPEER:
        {
            result = og_curl_easy_setopt(instance, tag, false);
            break;
        }

        case CURLOPT_SSL_VERIFYHOST:
        {
            result = og_curl_easy_setopt(instance, tag, false);
            break;
        }
        
        default:
        {
            logger::print(LOG_LEVEL_DEBUG, "curl_easy_setopt: %d", tag);
            result = og_curl_easy_setopt(instance, tag, args);
            break;
        }
    }
    
    va_end(args);
    return result;
}

fn_crypt_response(og_crypt_response) = NULL;
void hk_crypt_response(void* keys, void* buffer, const char* output, size_t size, int mode)
{
    og_crypt_response(keys, buffer, output, size, mode);
    logger::print(LOG_LEVEL_DEBUG, "crypt_response: %p | %p | %s | %lx | %d", keys, buffer, output, size, mode);
    
    char result[256];
    snprintf(result, sizeof(result), JSON_RESPONSE_DATA, lunacy->timestamp, lunacy->device.c_str());
    logger::print(LOG_LEVEL_DEBUG, "new decrypted data: %s", result);

    memcpy((void*)output, result, size);
}

bool lunacy_t::find_memory()
{
    std::ifstream maps_file("/proc/self/maps");
    if (!maps_file.is_open())
    {
        logger::print(LOG_LEVEL_WARNING, "Couldn't open /proc/self/maps");
        return false;
    }
    
    std::string line;
    while (std::getline(maps_file, line))
    {
        if (line.find(BINARY_PATH) == std::string::npos)
        {
            continue;
        }
        
        unsigned long long start_address;
        unsigned long long end_address;
        std::array<char, 5> perms{};

        sscanf(line.c_str(), "%llx-%llx %s", &start_address, &end_address, perms.data());

        size_t length = end_address - start_address;
        if (length < SELFMAG)
        {
            continue;
        }

        if (perms[0] != 'r')
        {
            continue;
        }

        if (std::memcmp((const void*)start_address, ELFMAG, SELFMAG) == 0)
        {
            this->memory = start_address;
            maps_file.close();
            return true;
        }
    }

    return false;
}

void lunacy_t::setup_hooks()
{
    logger::print(LOG_LEVEL_INFO, "Setting up hooks...");
    logger::print(LOG_LEVEL_DEBUG, "Cheat base: %p", (void*)this->memory);
    
    HOOK(this->memory + CURL_EASE_SETOPT, hk_curl_easy_setopt, og_curl_easy_setopt);
    HOOK(this->memory + CRYPT_RESPONSE, hk_crypt_response, og_crypt_response);
}

void lunacy_t::open_telegram()
{
    system("am start -a android.intent.action.VIEW -d \"https://t.me/wergity_mods\"");
}

/*
    START_SHELLCODE:
        mov     rbp, rsi        ; backup cmdline
        lea     rdi, file       ; "/data/local/tmp/libcrack.so"
        mov     esi, 2          ; RTLD_NOW | RTLD_LOCAL
        call    imp_dlopen      ; call dlopen from IAT
        mov     rsi, rbp        ; restore cmdline
        jmp     END_SHELLCODE
*/

extern "C" void __attribute__((constructor)) on_attach()
{
    lunacy = new lunacy_t(); // constructor started before init global variables

    if (!lunacy->find_memory())
    {
        logger::print(LOG_LEVEL_ERROR, "Cheat memory not found");
        return;
    }

    lunacy->setup_hooks();
    lunacy->open_telegram();
}