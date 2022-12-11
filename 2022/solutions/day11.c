#include "aoc.h"

enum
{
    OP_ADD,
    OP_MUL
};

typedef struct item
{
    uint64_t value;
    struct item* next;
} item;

typedef struct
{
    item* items_first;
    item* items_last;

    uint64_t inspected;

    int operation;
    int operands[2]; // 0 means 'old'

    int test_operand;
    int true_monkey;
    int false_monkey;
} monkey;

static void put_item(monkey* m, item* item)
{
    item->next = NULL;
    if (m->items_last)
    {
        m->items_last->next = item;
    }

    m->items_last = item;
    if (!m->items_first)
    {
        m->items_first = item;
    }
}

static item* get_item(monkey* m)
{
    item* result = m->items_first;
    if (result)
    {
        m->items_first = result->next;
        result->next = NULL;
    }
    if (m->items_last == result)
    {
        m->items_last = NULL;
    }
    return result;
}

static int parse_operand(string str)
{
    if (str.data[0] == 'o')
    {
        return 0;
    }
    else
    {
        return parse_int(str);
    }
}

static void parse_monkey(monkey* m, string* desc)
{
        // parse starting items
        {
            int len = sizeof("  Starting items: ") - 1;
            string str = {
                .data = desc[1].data + len,
                .length = desc[1].length - len
            };
            string_split_result tokens = split_by(str, ' ');

            for (int i = 0; i < tokens.count; i++)
            {
                item* it = alloc_struct(item);
                it->value = parse_int(tokens.strings[i]);
                put_item(m, it);
            }
        }

        // parse operation
        {
            int len = sizeof("  Operation: new = ") - 1;
            string str = {
                .data = desc[2].data + len,
                .length = desc[2].length - len
            };
            string_split_result tokens = split_by(str, ' ');
            
            m->operands[0] = parse_operand(tokens.strings[0]);
            m->operands[1] = parse_operand(tokens.strings[2]);

            switch (tokens.strings[1].data[0])
            {
                case '+': m->operation = OP_ADD; break;
                case '*': m->operation = OP_MUL; break;
                default: ASSERT(false);
            }
        }

        {
            int len = sizeof("  Test: divisible by ") - 1;
            string str = {
                .data = desc[3].data + len,
                .length = desc[3].length - len
            };
            m->test_operand = parse_int(str);
        }

        {
            int len = sizeof("    If true: throw to monkey ") - 1;
            string str = {
                .data = desc[4].data + len,
                .length = desc[4].length - len
            };
            m->true_monkey = parse_int(str);
        }

        {
            int len = sizeof("    If false: throw to monkey ") - 1;
            string str = {
                .data = desc[5].data + len,
                .length = desc[5].length - len
            };
            m->false_monkey = parse_int(str);
        }
}

AOC_SOLUTION(11)(char* input, int input_length)
{
    string_split_result lines = split_by((string){input, input_length}, '\n');

    int monkey_count = lines.count / 6;
    monkey* monkeys = alloc_array(monkey_count, monkey);

    uint64_t monkeymod = 1;
    for (int i = 0; i < monkey_count; i++)
    {
        monkey* m = monkeys + i;
        string* description = lines.strings + 6 * i;
        parse_monkey(m, description);
        monkeymod *= m->test_operand;
    }

    for (int round = 0; round < 10000; round++)
    {
        for (int i = 0; i < monkey_count; i++)
        {
            monkey* m = monkeys + i;
            item* it = NULL;
            while ((it = get_item(m)))
            {
                m->inspected++;
                uint64_t op1 = m->operands[0] != 0 ? m->operands[0] : it->value;
                uint64_t op2 = m->operands[1] != 0 ? m->operands[1] : it->value;
                switch (m->operation)
                {
                    case OP_ADD: it->value = op1 + op2; break;
                    case OP_MUL: it->value = op1 * op2; break;
                    default: ASSERT(false);
                }
                //it->value /= 3;
                it->value %= monkeymod;
                if (it->value % m->test_operand == 0)
                {
                    put_item(monkeys + m->true_monkey, it);
                }
                else
                {
                    put_item(monkeys + m->false_monkey, it);
                }
            }
        }
    }

    uint64_t part2 = 0;
    for (int i = 0; i < monkey_count; i++)
    {
        for (int j = i+1; j < monkey_count; j++)
        {
            uint64_t product = monkeys[i].inspected * monkeys[j].inspected;
            part2 = part2 > product ? part2 : product;
        }
    }

    return ANSWER(0,part2);
}