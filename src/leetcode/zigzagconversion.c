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
#include <lcthw/darray.h>
#include <lcthw/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* convert(char* s, int numRows)
{
    DArray* temp;
    int size;
    List* res_list;
    char* res;

    if (s == NULL) {
        return NULL;
    }

    size = strlen(s);

    if (size == 0 || numRows < 1) {
        return NULL;
    }

    if (numRows == 1) {
        return s;
    }

    temp = DArray_create(sizeof(List*), numRows);
    if (temp == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        int ans = i / (numRows - 1);
        int remain = i % (numRows - 1);
        if (ans % 2 == 0) { //结果为偶数或0
            List* temp_list = (List*)DArray_get(temp, remain);
            if (temp_list == NULL) {
                temp_list = List_create();
                if (temp_list == NULL) {
                    return NULL;
                }
                DArray_push(temp, temp_list);
            }
            List_push(temp_list, s[i]); //按余数正序保存
        }
        if (ans % 2 != 0) { //结果为奇数
            List* temp_list = (List*)DArray_get(temp, numRows - remain - 1);
            if (temp_list == NULL) {
                temp_list = List_create();
                if (temp_list == NULL) {
                    return NULL;
                }
                DArray_push(temp, temp_list);
            }
            List_push(temp_list, s[i]); //按余数倒序保存
        }
    }

    res_list = List_create();
    if (res_list == NULL) {
        return NULL;
    }
    for (int i = 0; i < numRows; i++) {
        List* temp_list = (List*)DArray_get(temp, i);
        ListNode* node = temp_list->first;
        while (node != NULL) {
            List_push(res_list, node->value);
            node = node->next;
        }
    }

    res = (char*)malloc(sizeof(char) * List_count(res_list));
    if (res == NULL) {
        return NULL;
    }

    ListNode* node = res_list->first;
    int m = 0;
    while (node != NULL) {
        res[m] = node->value;
        node = node->next;
        m++;
    }

    return res;
}

// refer: 
// https://blog.csdn.net/qq_38537503/article/details/95031638
// vector<string> temp(numRows);
// string res;
// if (s.empty() || numRows < 1)
//     return res;
// if (numRows == 1)
//     return s;
// for (int i = 0; i < s.size(); i++) {
//     int ans = i / (numRows - 1);
//     int remain = i % (numRows - 1);
//     if (ans % 2 == 0) { //结果为偶数或0
//         temp[remain].push_back(s[i]); //按余数正序保存
//     }
//     if (ans % 2 != 0) { //结果为奇数
//         temp[numRows - remain - 1].push_back(s[i]); //按余数倒序保存
//     }
// }
// for (int i = 0; i < temp.size(); i++) {
//     res += temp[i];
// }
// return res;
