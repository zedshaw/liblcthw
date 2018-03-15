#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_compare) (const void *a, const void *b);

int List_bubble_sort(List * list, List_compare cmp);

List *List_merge_sort(List * list, List_compare cmp);

inline void ListNode_swap(ListNode * a, ListNode * b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

inline List *List_merge(List * left, List * right, List_compare cmp)
{
    List *result = List_create();
    void *val = NULL;

    while (List_count(left) > 0 || List_count(right) > 0)
    {
        if (List_count(left) > 0 && List_count(right) > 0)
        {
            if (cmp(List_first(left), List_first(right)) <= 0)
            {
                val = List_shift(left);
            }
            else
            {
                val = List_shift(right);
            }

            List_push(result, val);
        }
        else if (List_count(left) > 0)
        {
            val = List_shift(left);
            List_push(result, val);
        }
        else if (List_count(right) > 0)
        {
            val = List_shift(right);
            List_push(result, val);
        }
    }

    return result;
}

#endif
