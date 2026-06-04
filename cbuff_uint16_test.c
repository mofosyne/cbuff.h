/* Generated from cbuff_uint8_test.c by generate.sh -- do not edit directly */
/*******************************************************************************
 * Mini Unit Test Of Circular Buffer
 *******************************************************************************/
#include <stdio.h>

#include "cbuff_uint16.h"

#define BUFF_TEST_SIZE 4

void circularBuffer_inspect(cbuff_uint16_t *cb, char *msg)
{
    printf("\n%s : H=%ld T=%ld", msg, cb->head, cb->tail);
    printf("\n%s : ", msg);
    for (int i = 0; i < cb->capacity; i++)
    {
        printf(" %c", (((cb->head % cb->capacity) == i) && ((cb->tail % cb->capacity) == i)) ? 'X' : ((cb->head % cb->capacity) == i) ? 'H' : ((cb->tail % cb->capacity) == i) ? 'T' : ' ');
    }
    printf("\n%s : ", msg);
    for (int i = 0; i < cb->capacity; i++)
    {
        printf(" %d", cb->buffer[i]);
    }
    printf("\n");
}

// Minimum Assert Unit (https://jera.com/techinfo/jtns/jtn002)
#define mu_assert(message, test)                                                                                                                                                                       \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        if (!(test))                                                                                                                                                                                   \
        {                                                                                                                                                                                              \
            circularBuffer_inspect(&prefilledBuff, "ERR");                                                                                                                                             \
            return LINEINFO " : (expect:" #test ") " message;                                                                                                                                          \
        }                                                                                                                                                                                              \
    } while (0)
#define mu_run_test(test)                                                                                                                                                                              \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        char *message = test();                                                                                                                                                                        \
        printf("* Testing %-30s - %s\n", #test, message ? "FAIL" : "PASS");                                                                                                                                                                                 \
        if (message)                                                                                                                                                                                   \
            return message;                                                                                                                                                                            \
    } while (0)

// Line Info (Ref: __LINE__ to string http://decompile.com/cpp/faq/file_and_line_error_string.htm)
#define LINEINFO_STR(X) #X
#define LINEINFO__STR(X) LINEINFO_STR(X)
#define LINEINFO __FILE__ " : " LINEINFO__STR(__LINE__)

char *cbuff_uint16_test_prefill(void)
{
    uint16_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint16_t prefilledBuff = cbuff_uint16_struct_prefill(cbuffer);
    cbuff_uint16_t initBuff = {0};
    mu_assert("", !cbuff_uint16_is_init(&initBuff));
    cbuff_uint16_init(&initBuff, BUFF_TEST_SIZE, cbuffer);
    mu_assert("", cbuff_uint16_is_init(&initBuff));
    mu_assert("", cbuff_uint16_capacity(&prefilledBuff) == BUFF_TEST_SIZE);
    mu_assert("", cbuff_uint16_count(&prefilledBuff) == 0);
    mu_assert("", !cbuff_uint16_is_full(&prefilledBuff));
    mu_assert("", cbuff_uint16_is_empty(&prefilledBuff));
    mu_assert("", prefilledBuff.capacity == initBuff.capacity);
    mu_assert("", prefilledBuff.buffer == initBuff.buffer);
    mu_assert("", prefilledBuff.head == initBuff.head);
    mu_assert("", prefilledBuff.tail == initBuff.tail);
    cbuff_uint16_reset(&initBuff);
    mu_assert("", prefilledBuff.head == initBuff.head);
    mu_assert("", prefilledBuff.tail == initBuff.tail);
    return 0;
}

char *cbuff_uint16_test_general(void)
{
    uint16_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint16_t prefilledBuff = cbuff_uint16_struct_prefill(cbuffer);
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "XXX");
        mu_assert("", !cbuff_uint16_is_full(&prefilledBuff));
        mu_assert("", cbuff_uint16_enqueue(&prefilledBuff, i + 1));
        mu_assert("", !cbuff_uint16_is_empty(&prefilledBuff));
        mu_assert("", cbuff_uint16_count(&prefilledBuff) == (i + 1));
    }
    mu_assert("", !cbuff_uint16_enqueue(&prefilledBuff, 0x33));
    mu_assert("", cbuff_uint16_is_full(&prefilledBuff));
    mu_assert("", !cbuff_uint16_is_empty(&prefilledBuff));
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        uint16_t d = -1;
        mu_assert("", cbuff_uint16_dequeue(&prefilledBuff, &d));
        mu_assert("", d == i + 1);
    }
    return 0;
}

char *cbuff_uint16_test_overwrite(void)
{
    uint16_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint16_t prefilledBuff = cbuff_uint16_struct_prefill(cbuffer);
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "WWW");
        cbuff_uint16_enqueue(&prefilledBuff, i);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "OOO");
        cbuff_uint16_enqueue_overwrite(&prefilledBuff, i + 1);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "DDD");
        uint16_t d = -1;
        mu_assert("", cbuff_uint16_dequeue(&prefilledBuff, &d));
        mu_assert("", d == i + 1);
    }
    return 0;
}

char *cbuff_uint16_test_peek(void)
{
    uint16_t cbuffer[3] = {0};
    cbuff_uint16_t prefilledBuff = cbuff_uint16_struct_prefill(cbuffer);
    for (int i = 1; i < 5; i++)
    {
        cbuff_uint16_enqueue_overwrite(&prefilledBuff, i);
    }
    for (int i = 0; i < 3; i++)
    {
        uint16_t d = -1;
        mu_assert("", cbuff_uint16_peek(&prefilledBuff, &d, i));
        mu_assert("", d == i + 2);
    }
    // offset == count and offset > count must fail
    uint16_t d = -1;
    mu_assert("peek at count should fail", !cbuff_uint16_peek(&prefilledBuff, &d, 3));
    mu_assert("peek beyond count should fail", !cbuff_uint16_peek(&prefilledBuff, &d, 10));
    return 0;
}

char *cbuff_uint16_test_edge_cases(void)
{
    uint16_t cbuffer[4] = {0};
    cbuff_uint16_t prefilledBuff = cbuff_uint16_struct_prefill(cbuffer);

    // dequeue on empty must fail
    uint16_t d = -1;
    mu_assert("dequeue empty should fail", !cbuff_uint16_dequeue(&prefilledBuff, &d));

    // wrap-around: cycle indices well past 2*capacity
    for (int i = 0; i < 20; i++)
    {
        mu_assert("enqueue should succeed", cbuff_uint16_enqueue(&prefilledBuff, (uint16_t)(i + 1)));
        mu_assert("dequeue should succeed", cbuff_uint16_dequeue(&prefilledBuff, &d));
        mu_assert("value mismatch after wrap", d == (uint16_t)(i + 1));
    }
    mu_assert("should be empty after wrap cycling", cbuff_uint16_is_empty(&prefilledBuff));
    mu_assert("count should be 0 after wrap cycling", cbuff_uint16_count(&prefilledBuff) == 0);
    return 0;
}

static char *all_tests()
{
    mu_run_test(cbuff_uint16_test_prefill);
    mu_run_test(cbuff_uint16_test_general);
    mu_run_test(cbuff_uint16_test_overwrite);
    mu_run_test(cbuff_uint16_test_peek);
    mu_run_test(cbuff_uint16_test_edge_cases);
    return 0;
}

int main(void)
{
    char *result = all_tests();
    printf("\n%s\n", (result) ? result : "ALL TESTS PASSED\n");
    return result != 0;
}
