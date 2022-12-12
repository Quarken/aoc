#include "aoc.h"

#define BUFFER_LEN 64

enum
{
    OP_ADD,
    OP_MUL
};

typedef struct
{
    uint64_t items[BUFFER_LEN];
    int count;
    int end;
    int start;

    uint64_t inspected;

    int operation;
    int operands[2]; // 0 means 'old'

    int test_operand;
    uint64_t test_operand_c;

    int true_monkey;
    int false_monkey;
} monkey;

// c must be precalculated as c = 1 + UINT64_MAX / d, then this checks whether n % d == 0
static inline bool is_divisible(uint64_t n, uint64_t c)
{
    return n * c <= c - 1;
}

static void give_item(monkey* m, uint64_t item)
{
    m->items[m->end++] = item;
    m->end = m->end & (BUFFER_LEN-1);
    m->count++;
}

static uint64_t take_item(monkey* m)
{
    uint64_t result = m->items[m->start++];
    m->start = m->start & (BUFFER_LEN-1);
    m->count--;
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
        {
            int len = sizeof("  Starting items: ") - 1;
            string str = {
                .data = desc[1].data + len,
                .length = desc[1].length - len
            };
            string_split_result tokens = split_by(str, ' ');

            for (int i = 0; i < tokens.count; i++)
            {
                give_item(m, parse_int(tokens.strings[i]));
            }
        }

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
            m->test_operand_c = 1 + UINT64_MAX / m->test_operand;
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

static uint64_t run_rounds(monkey* monkeys, int monkey_count, uint64_t modulo, int rounds, bool divide_worry)
{
    for (int round = 0; round < rounds; round++)
    {
        for (int i = 0; i < monkey_count; i++)
        {
            monkey* m = monkeys + i;
            int count = m->count;
            for (int j = 0; j < count; j++)
            {
                uint64_t item = take_item(m);
                m->inspected++;
                uint64_t op1 = m->operands[0] != 0 ? m->operands[0] : item;
                uint64_t op2 = m->operands[1] != 0 ? m->operands[1] : item;
                switch (m->operation)
                {
                    case OP_ADD: item = op1 + op2; break;
                    case OP_MUL: item = op1 * op2; break;
                    default: ASSERT(false);
                }
                if (divide_worry)
                {
                    item /= 3;
                }
                item %= modulo;
                if (is_divisible(item, m->test_operand_c))
                {
                    give_item(monkeys + m->true_monkey, item);
                }
                else
                {
                    give_item(monkeys + m->false_monkey, item);
                }
            }
        }
    }

    uint64_t solution = 0;
    for (int i = 0; i < monkey_count; i++)
    {
        for (int j = i+1; j < monkey_count; j++)
        {
            uint64_t product = monkeys[i].inspected * monkeys[j].inspected;
            solution = solution > product ? solution : product;
        }
    }

    return solution;
}

AOC_SOLUTION(11)(char* input, int input_length)
{
    string_split_result lines = split_by((string){input, input_length}, '\n');

    int monkey_count = lines.count / 6;
    monkey* monkeys = alloc_array(monkey_count, monkey);

    uint64_t modulo = 1;
    for (int i = 0; i < monkey_count; i++)
    {
        monkey* m = monkeys + i;
        string* description = lines.strings + 6 * i;
        parse_monkey(m, description);
        modulo *= m->test_operand;
    }

    monkey* monkeys_part2 = alloc_array(monkey_count, monkey);
    memcpy(monkeys_part2, monkeys, monkey_count * sizeof(monkey));

    uint64_t part1 = run_rounds(monkeys, monkey_count, modulo, 20, true);
    uint64_t part2 = run_rounds(monkeys_part2, monkey_count, modulo, 10000, false);

    return ANSWER(part1, part2);
}