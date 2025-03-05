/*******************************************************************************
 * Mini Unit Test Of Circular Buffer
 *******************************************************************************/
#include <stdio.h>

#include "cbuff_uint8.h"

#define BUFF_TEST_SIZE 4

void circularBuffer_inspect(cbuff_uint8_t *cb, char *msg)
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

char *cbuff_uint8_test_prefill(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    cbuff_uint8_t initBuff = {0};
    mu_assert("", !cbuff_uint8_is_init(&initBuff));
    cbuff_uint8_init(&initBuff, BUFF_TEST_SIZE, cbuffer);
    mu_assert("", cbuff_uint8_is_init(&initBuff));
    mu_assert("", cbuff_uint8_capacity(&prefilledBuff) == BUFF_TEST_SIZE);
    mu_assert("", cbuff_uint8_count(&prefilledBuff) == 0);
    mu_assert("", !cbuff_uint8_is_full(&prefilledBuff));
    mu_assert("", cbuff_uint8_is_empty(&prefilledBuff));
    mu_assert("", prefilledBuff.capacity == initBuff.capacity);
    mu_assert("", prefilledBuff.buffer == initBuff.buffer);
    mu_assert("", prefilledBuff.head == initBuff.head);
    mu_assert("", prefilledBuff.tail == initBuff.tail);
    cbuff_uint8_reset(&initBuff);
    mu_assert("", prefilledBuff.head == initBuff.head);
    mu_assert("", prefilledBuff.tail == initBuff.tail);
    return 0;
}

char *cbuff_uint8_test_general(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "XXX");
        mu_assert("", !cbuff_uint8_is_full(&prefilledBuff));
        mu_assert("", cbuff_uint8_enqueue(&prefilledBuff, i + 1));
        mu_assert("", !cbuff_uint8_is_empty(&prefilledBuff));
        mu_assert("", cbuff_uint8_count(&prefilledBuff) == (i + 1));
    }
    mu_assert("", !cbuff_uint8_enqueue(&prefilledBuff, 0x33));
    mu_assert("", cbuff_uint8_is_full(&prefilledBuff));
    mu_assert("", !cbuff_uint8_is_empty(&prefilledBuff));
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        uint8_t d = -1;
        mu_assert("", cbuff_uint8_dequeue(&prefilledBuff, &d));
        mu_assert("", d == i + 1);
    }
    return 0;
}

char *cbuff_uint8_test_overwrite(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "WWW");
        cbuff_uint8_enqueue(&prefilledBuff, i);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "OOO");
        cbuff_uint8_enqueue_overwrite(&prefilledBuff, i + 1);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        //circularBuffer_inspect(&prefilledBuff, "DDD");
        uint8_t d = -1;
        mu_assert("", cbuff_uint8_dequeue(&prefilledBuff, &d));
        mu_assert("", d == i + 1);
    }
    return 0;
}

char *cbuff_uint8_test_peek(void)
{
    uint8_t cbuffer[3] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    for (int i = 1; i < 5; i++)
    {
        cbuff_uint8_enqueue_overwrite(&prefilledBuff, i);
    }
    for (int i = 0; i < 3; i++)
    {
        uint8_t d = -1;
        mu_assert("", cbuff_uint8_peek(&prefilledBuff, &d, i));
        mu_assert("", d == i + 2);
    }
    return 0;
}

static char *all_tests()
{
    mu_run_test(cbuff_uint8_test_prefill);
    mu_run_test(cbuff_uint8_test_general);
    mu_run_test(cbuff_uint8_test_overwrite);
    mu_run_test(cbuff_uint8_test_peek);
    return 0;
}

int main(void)
{
    char *result = all_tests();
    printf("\n%s\n", (result) ? result : "ALL TESTS PASSED\n");
    return result != 0;
}
