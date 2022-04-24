#include <stdio.h>

typedef struct
{
    const char ch;
    int num;
} roman_map;

roman_map roman_mapping[] = {
    { 'I', 1 },
    { 'V', 5 },
    { 'X', 10 },
    { 'L', 50 },
    { 'C', 100 },
    { 'D', 500 },
    { 'M', 1000 },
};

/*
  https://leetcode-cn.com/problems/roman-to-integer/
  输入: s = "MCMXCIV"
  输出: 1994
  解释: M = 1000, CM = 900, XC = 90, IV = 4.
*/

int romanToInt(char* s)
{
    char* p;
    int result = 0;
    int type_num = sizeof(roman_mapping) / sizeof(roman_map);
    for (p = s; *p != '\0'; p++) {
        if (*p == 'I' && *(p + 1) != '\0') {
            if (*(p + 1) == 'V') {
                result += 4;
                p++;
                continue;
            } else if (*(p + 1) == 'X') {
                result += 9;
                p++;
                continue;
            }
        } else if (*p == 'X' && *(p + 1) != '\0') {
            if (*(p + 1) == 'L') {
                result += 40;
                p++;
                continue;
            } else if (*(p + 1) == 'C') {
                result += 90;
                p++;
                continue;
            }
        } else if (*p == 'C' && *(p + 1) != '\0') {
            if (*(p + 1) == 'D') {
                result += 400;
                p++;
                continue;
            } else if (*(p + 1) == 'M') {
                result += 900;
                p++;
                continue;
            }
        }
        for (int i = 0; i < type_num; i++) {
            if (*p == roman_mapping[i].ch) {
                result += roman_mapping[i].num;
            }
        }
    }

    return result;
}
