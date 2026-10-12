/*
 * Windows 10/11 레지스트리에 저장된 백업 제품 키를 조회합니다.
 * 조회한 값이 현재 사용 중인 라이선스나 정품 인증 상태를 보장하지는 않습니다.
 *
 * Windows에서 컴파일하는 방법:
 * MSVC: cl /W4 productKey.c advapi32.lib
 * MinGW: gcc -Wall -Wextra productKey.c -o productKey.exe -ladvapi32
 */
#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

/* TypeScript의 대소문자 구분 없는 [\dA-Z] 패턴에 해당하는 문자 판별. */
static int is_key_character(unsigned char character)
{
    return (character >= '0' && character <= '9') ||
           (character >= 'A' && character <= 'Z') ||
           (character >= 'a' && character <= 'z');
}

/*
 * 하이픈으로 연결된 영숫자 다섯 그룹을 찾아 key 버퍼에 복사합니다.
 * 발견하면 1, 찾지 못하거나 버퍼가 부족하면 0을 반환합니다.
 * 원본 정규식과 동일하게 각 그룹의 길이는 고정하지 않습니다.
 */
static int find_product_key(const char *text, char *key, size_t capacity)
{
    const char *start;

    /* 문자열의 각 위치를 시작점으로 삼아 첫 번째 일치 항목을 찾습니다. */
    for (start = text; *start != '\0'; ++start) {
        const char *cursor = start;
        int group;

        for (group = 0; group < 5; ++group) {
            const char *group_start = cursor;

            while (is_key_character((unsigned char)*cursor)) {
                ++cursor;
            }
            /* 빈 그룹이나 누락된 구분자는 제품 키 패턴으로 인정하지 않습니다. */
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
            /* 문자열 끝을 표시하는 널 문자까지 저장할 공간이 필요합니다. */
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
    /* 조회 결과와 추출한 키를 분리하며, 실패 시 기본 표시값을 유지합니다. */
    char value[256] = {0};
    char product_key[256] = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX";
    DWORD value_size = sizeof(value);
    LSTATUS status;

    /*
     * 문자열(REG_SZ)만 읽으며, 32비트 빌드에서도 64비트 레지스트리 뷰를 사용합니다.
     * value_size에는 버퍼 크기를 전달하고, API가 실제 읽은 크기를 기록합니다.
     */
    status = RegGetValueA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform",
        "BackupProductKeyDefault",
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY,
        NULL,
        value,
        &value_size
    );

    /* 조회 실패 또는 패턴 불일치 시 기본 표시값으로 출력합니다. */
    if (status == ERROR_SUCCESS) {
        find_product_key(value, product_key, sizeof(product_key));
    }

    printf("Product y: %s\n", product_key);
    printf("Press any key to exit...\n");
    /* 안내 문구를 출력한 뒤 Enter 없이 한 번의 키 입력을 기다립니다. */
    fflush(stdout);
    _getch();
    return 0;
}

#else

/* Windows 외 플랫폼에서는 레지스트리를 조회하지 않고 오류로 종료합니다. */
#include <stdio.h>

int main(void)
{
    fprintf(stderr, "This program requires Windows 10 or Windows 11.\n");
    return 1;
}

#endif
