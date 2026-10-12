/*
 * Windows 10/11 product key reader.
 * MSVC: cl /W4 productKey.c advapi32.lib
 * MinGW: gcc -Wall -Wextra productKey.c -o productKey.exe -ladvapi32
 */
#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

static int is_key_character(unsigned char character)
{
    return (character >= '0' && character <= '9') ||
           (character >= 'A' && character <= 'Z') ||
           (character >= 'a' && character <= 'z');
}

/* Equivalent to the original case-insensitive five-group JS pattern. */
static int find_product_key(const char *text, char *key, size_t capacity)
{
    const char *start;

    for (start = text; *start != '\0'; ++start) {
        const char *cursor = start;
        int group;

        for (group = 0; group < 5; ++group) {
            const char *group_start = cursor;

            while (is_key_character((unsigned char)*cursor)) {
                ++cursor;
            }
            if (cursor == group_start) {
                break;
            }
            if (group < 4) {
                if (*cursor != '-') {
                    break;
                }
                ++cursor;
            }
        }

        if (group == 5) {
            size_t length = (size_t)(cursor - start);
            if (length >= capacity) {
                return 0;
            }
            memcpy(key, start, length);
            key[length] = '\0';
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    char value[256] = {0};
    char product_key[256] = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX";
    DWORD value_size = sizeof(value);
    LSTATUS status;

    status = RegGetValueA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform",
        "BackupProductKeyDefault",
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY,
        NULL,
        value,
        &value_size
    );

    if (status == ERROR_SUCCESS) {
        find_product_key(value, product_key, sizeof(product_key));
    }

    printf("Product y: %s\n", product_key);
    printf("Press any key to exit...\n");
    fflush(stdout);
    _getch();
    return 0;
}

#else

#include <stdio.h>

int main(void)
{
    fprintf(stderr, "This program requires Windows 10 or Windows 11.\n");
    return 1;
}

#endif
