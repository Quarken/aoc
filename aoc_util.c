#include "aoc.h"

string_split_result split_by_until(string str, char by, char until)
{
    string_split_result result = {0};

    const char* ptr = str.data;
    int len = 0;
    int i = 0;
    for (int i = 0; i <= str.length; i++)
    {
        char c = until;
        if (i < str.length)
        {
            c = str.data[i];
        }

        if ((c == by || c == until))
        {
            if (len > 0)
            {
                string* s = alloc_struct(string);
                s->data = ptr;
                s->length = len;

                ptr = str.data + i + 1;
                len = 0;

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
        else
        {
            len++;
        }

        if (c == until) break;
    }

    return result;
}

string_split_result split_by(string str, char split)
{
    return split_by_until(str, split, '\0');
}

int parse_int(string str)
{
    int result = 0;
    for (int i = 0; i < str.length; i++)
    {
        result = 10 * result + str.data[i] - '0';
    }
    return result;
}