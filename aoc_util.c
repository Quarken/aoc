#include "aoc.h"

string_split_result split_by(string str, char by)
{
    string_split_result result = {
        .strings = NULL,
        .count = 0
    };

    const char* ptr = str.data;
    int count = 0;
    for (int i = 0; i < str.length; i++)
    {
        if (str.data[i] != by)
        {
            count++;
        }
        else
        {
            if (count > 0)
            {
                string* s = alloc_struct(string);
                s->data = ptr;
                s->length = count;
                ptr = str.data + i + 1;
                count = 0;
                result.count++;
                if (result.strings == NULL)
                {
                    result.strings = s;
                }
            }
            else
            {
                ptr++;
            }
        }
    }
    if (count > 0)
    {
        string* s = alloc_struct(string);
        s->data = ptr;
        s->length = count;
        result.count++;
    }

    return result;
}

int parse_int(string str)
{
    int result = 0;

    int start = 0;
    if (str.data[0] == '-')
    {
        start = 1;
    }

    for (int i = start; i < str.length; i++)
    {
        if (str.data[i] < '0' || str.data[i] > '9') break;
        result = 10 * result + str.data[i] - '0';
    }

    return start == 0 ? result : -result;
}