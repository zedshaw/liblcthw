#undef NDEBUG
#include <lcthw/sarray.h>
#include <lcthw/dbg.h>
#include <stdlib.h>


int SuffixArray_compare(void *thunk, const void *a, const void *b)
{
    SuffixArray *sarry = thunk;
    int a_at = *(int *)a;
    int b_at = *(int *)b;
    char *a_str = sarry->source + a_at;
    char *b_str = sarry->source + b_at;

    int a_len = sarry->length - a_at;
    int b_len = sarry->length - b_at;
    int len = a_len < b_len ? a_len : b_len;

    int cmp = strncmp(a_str, b_str, len);

    return cmp;
}

int SuffixArray_find_suffix(SuffixArray *sarry, char *data, int length)
{
    int min = 0;
    int max = sarry->length;
    int cmp = 0;
    int mid = 0;

    do {
        mid = (min + max) / 2;
        // BUG: wrong, need to use the minimum length
        cmp = strncmp(SuffixArray_substr(sarry, mid), data, length);

        if(cmp < 0) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    } while(cmp != 0 && min < max);

    if(cmp == 0) {
        return mid;
    } else {
        return -1;
    }
}


SuffixArray *SuffixArray_create(char *data, int length)
{
    SuffixArray *sarry = calloc(1, sizeof(SuffixArray));
    check_mem(sarry);

    sarry->source = malloc(length + 1);
    check_mem(sarry->source);
    memcpy(sarry->source, data, length);
    sarry->source[length] = '\0';

    sarry->length = length;

    sarry->indices = malloc(length * sizeof(int));
    check_mem(sarry->indices);

    int i = 0;
    for(i = 0; i < length; i++) {
        sarry->indices[i] = i;
    }

    qsort_r(sarry->indices, length, sizeof(int), sarry, SuffixArray_compare);

    return sarry;

error:
    if(sarry) {
        SuffixArray_destroy(sarry);
    }

    return NULL;
}


void SuffixArray_destroy(SuffixArray *sarry)
{
    if(sarry) {
        free(sarry->source);
        free(sarry->indices);
        free(sarry);
    }
}

