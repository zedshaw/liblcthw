/*
6. Z 字形变换(https://leetcode-cn.com/problems/zigzag-conversion/)
将一个给定字符串 s 根据给定的行数 numRows ，以从上往下、从左到右进行 Z 字形排列。

比如输入字符串为 "PAYPALISHIRING" 行数为 3 时，排列如下： nLines = 7
P   A   H   N
A P L S I I G
Y   I   R
之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如："PAHNAPLSIIGYIR"。  // (14/3 - 1)(3-1) + 1 = 3 * 2 + 1

请你实现这个将字符串进行指定行数变换的函数：

string convert(string s, int numRows);
 

示例 1：

输入：s = "PAYPALISHIRING", numRows = 3
输出："PAHNAPLSIIGYIR"
示例 2：
输入：s = "PAYPALISHIRING", numRows = 4   // nLines = 6      // (14/4 - 1)(4-1) = 3 * 2
输出："PINALSIGYAHRPI"
解释：
P     I    N
A   L S  I G
Y A   H R
P     I
示例 3：

输入：s = "A", numRows = 1
输出："A"
 

提示：

1 <= s.length <= 1000
s 由英文字母（小写和大写）、',' 和 '.' 组成
1 <= numRows <= 1000
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* convert(char* s, int numRows)
{
    if (s == NULL) {
        return NULL;
    }

    char* result = NULL;
    size_t len = strlen(s); // 14
    int nLines = (len / numRows - 1) * (numRows - 1) + 1; // (14 / 3)

    result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    memset(result, '\0', len + 1);

    char** p = (char**)malloc(sizeof(char*) * numRows);
    if (p == NULL) {
        free(result);
        return NULL;
    }

    for (int i = 0; i < numRows; i++) {
        int num = sizeof(char) * (nLines);
        p[i] = (char*)malloc(num);
        if (p[i] == NULL) {
            return NULL;
        }
        memset(p, '\0', num);
    }

    for (int i = 0; i < len; i++) {
        for (int m = 0; m < nLines; m++) {
            for (int j = 0; j < numRows; j++) // j = 0
            {
                if (j / numRows == 0) // 0
                {
                    p[j][m] = s[i]; // j = 0; m = 0; j = 0; m = 1
                } else if (j > numRows && j / numRows == 1) {
                    p[numRows - m - 1][j] = s[i];
                }
            }
        }
    }

    // output
    int n = 0;
    for (int m = 0; m < numRows; m++) {
        for (int i = 0; i < len / numRows + 1; i++) {
            if (p[m][i] != '\0') {
                result[n] = p[m][i];
                n++;
            }
        }
    }

    return result;
}