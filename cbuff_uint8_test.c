/*******************************************************************************
 * Mini Unit Test Of Circular Buffer
 *******************************************************************************/
#include <stdio.h>

#include "cbuff_uint8.h"

#define BUFF_TEST_SIZE 4

void circularBuffer_inspect(cbuff_uint8_t *cb, char *msg)
{
    printf("\n%s : H=%zu T=%zu", msg, cb->head, cb->tail);
    printf("\n%s : ", msg);
    for (int i = 0; i < cb->capacity; i++)
    {
        printf(" %c", (((cb->head % cb->capacity) == i) && ((cb->tail % cb->capacity) == i)) ? 'X' : ((cb->head % cb->capacity) == i) ? 'H' : ((cb->tail % cb->capacity) == i) ? 'T' : ' ');
    }
    printf("\n%s : ", msg);
    for (int i = 0; i < cb->capacity; i++)
    {
        printf(" %llu", (unsigned long long)cb->buffer[i]);
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
        cbuff_uint8_enqueue(&prefilledBuff, i);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        cbuff_uint8_enqueue_overwrite(&prefilledBuff, i + 1);
    }
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
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
    // offset == count and offset > count must fail
    uint8_t d = -1;
    mu_assert("peek at count should fail", !cbuff_uint8_peek(&prefilledBuff, &d, 3));
    mu_assert("peek beyond count should fail", !cbuff_uint8_peek(&prefilledBuff, &d, 10));
    return 0;
}

char *cbuff_uint8_test_edge_cases(void)
{
    uint8_t cbuffer[4] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);

    // dequeue on empty must fail
    uint8_t d = -1;
    mu_assert("dequeue empty should fail", !cbuff_uint8_dequeue(&prefilledBuff, &d));

    // wrap-around: cycle indices well past 2*capacity
    for (int i = 0; i < 20; i++)
    {
        mu_assert("enqueue should succeed", cbuff_uint8_enqueue(&prefilledBuff, (uint8_t)(i + 1)));
        mu_assert("dequeue should succeed", cbuff_uint8_dequeue(&prefilledBuff, &d));
        mu_assert("value mismatch after wrap", d == (uint8_t)(i + 1));
    }
    mu_assert("should be empty after wrap cycling", cbuff_uint8_is_empty(&prefilledBuff));
    mu_assert("count should be 0 after wrap cycling", cbuff_uint8_count(&prefilledBuff) == 0);
    return 0;
}

char *cbuff_uint8_test_capacity_one(void)
{
    uint8_t cbuffer[1] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    mu_assert("capacity should be 1", cbuff_uint8_capacity(&prefilledBuff) == 1);
    mu_assert("should be empty", cbuff_uint8_is_empty(&prefilledBuff));
    mu_assert("enqueue should succeed", cbuff_uint8_enqueue(&prefilledBuff, 0xAB));
    mu_assert("should be full", cbuff_uint8_is_full(&prefilledBuff));
    mu_assert("count should be 1", cbuff_uint8_count(&prefilledBuff) == 1);
    mu_assert("second enqueue should fail", !cbuff_uint8_enqueue(&prefilledBuff, 0xCD));
    uint8_t d = 0;
    mu_assert("dequeue should succeed", cbuff_uint8_dequeue(&prefilledBuff, &d));
    mu_assert("dequeued value should match", d == 0xAB);
    mu_assert("should be empty after dequeue", cbuff_uint8_is_empty(&prefilledBuff));
    // overwrite on a full capacity-1 buffer: old item replaced, new item dequeued
    cbuff_uint8_enqueue(&prefilledBuff, 0x01);
    cbuff_uint8_enqueue_overwrite(&prefilledBuff, 0x02);
    mu_assert("should still be full after overwrite", cbuff_uint8_is_full(&prefilledBuff));
    mu_assert("overwrite value should replace old item", cbuff_uint8_dequeue(&prefilledBuff, &d) && d == 0x02);
    return 0;
}

char *cbuff_uint8_test_reset(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    cbuff_uint8_enqueue(&prefilledBuff, 1);
    cbuff_uint8_enqueue(&prefilledBuff, 2);
    mu_assert("count should be 2 before reset", cbuff_uint8_count(&prefilledBuff) == 2);
    cbuff_uint8_reset(&prefilledBuff);
    mu_assert("should be empty after reset", cbuff_uint8_is_empty(&prefilledBuff));
    mu_assert("count should be 0 after reset", cbuff_uint8_count(&prefilledBuff) == 0);
    mu_assert("enqueue after reset should succeed", cbuff_uint8_enqueue(&prefilledBuff, 0x42));
    uint8_t d = 0;
    mu_assert("dequeue after reset should get new value", cbuff_uint8_dequeue(&prefilledBuff, &d) && d == 0x42);
    return 0;
}

char *cbuff_uint8_test_fill_empty_refill(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
        cbuff_uint8_enqueue(&prefilledBuff, (uint8_t)(i + 1));
    mu_assert("should be full after first fill", cbuff_uint8_is_full(&prefilledBuff));
    for (int i = BUFF_TEST_SIZE; i > 0; i--)
    {
        mu_assert("count should decrement during drain", cbuff_uint8_count(&prefilledBuff) == (size_t)i);
        uint8_t d = 0;
        cbuff_uint8_dequeue(&prefilledBuff, &d);
    }
    mu_assert("should be empty after drain", cbuff_uint8_is_empty(&prefilledBuff));
    // second fill with distinct values to catch stale data bugs
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
        cbuff_uint8_enqueue(&prefilledBuff, (uint8_t)(i + 10));
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        uint8_t d = 0;
        mu_assert("dequeue on second fill should succeed", cbuff_uint8_dequeue(&prefilledBuff, &d));
        mu_assert("value mismatch on second fill", d == (uint8_t)(i + 10));
    }
    mu_assert("should be empty after second drain", cbuff_uint8_is_empty(&prefilledBuff));
    return 0;
}

char *cbuff_uint8_test_peek_after_dequeue(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    // enqueue 1,2,3,4 then dequeue 2 — peek offsets must be relative to new head
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
        cbuff_uint8_enqueue(&prefilledBuff, (uint8_t)(i + 1));
    uint8_t d = 0;
    cbuff_uint8_dequeue(&prefilledBuff, &d);
    cbuff_uint8_dequeue(&prefilledBuff, &d);
    mu_assert("count should be 2 after two dequeues", cbuff_uint8_count(&prefilledBuff) == 2);
    mu_assert("peek offset 0 should be 3", cbuff_uint8_peek(&prefilledBuff, &d, 0) && d == 3);
    mu_assert("peek offset 1 should be 4", cbuff_uint8_peek(&prefilledBuff, &d, 1) && d == 4);
    mu_assert("peek offset 2 should fail", !cbuff_uint8_peek(&prefilledBuff, &d, 2));
    // verify peek did not consume items
    mu_assert("count unchanged after peeks", cbuff_uint8_count(&prefilledBuff) == 2);
    // peek on empty
    cbuff_uint8_dequeue(&prefilledBuff, &d);
    cbuff_uint8_dequeue(&prefilledBuff, &d);
    mu_assert("peek on empty should fail", !cbuff_uint8_peek(&prefilledBuff, &d, 0));
    return 0;
}

char *cbuff_uint8_test_overwrite_nonfull(void)
{
    uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
    cbuff_uint8_t prefilledBuff = cbuff_uint8_struct_prefill(cbuffer);
    // enqueue_overwrite on a non-full buffer should behave like regular enqueue
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
        cbuff_uint8_enqueue_overwrite(&prefilledBuff, (uint8_t)(i + 1));
    mu_assert("should be full after overwrite-fill", cbuff_uint8_is_full(&prefilledBuff));
    for (int i = 0; i < BUFF_TEST_SIZE; i++)
    {
        uint8_t d = 0;
        mu_assert("dequeue should succeed", cbuff_uint8_dequeue(&prefilledBuff, &d));
        mu_assert("FIFO order should be preserved", d == (uint8_t)(i + 1));
    }
    return 0;
}

static char *all_tests()
{
    mu_run_test(cbuff_uint8_test_prefill);
    mu_run_test(cbuff_uint8_test_general);
    mu_run_test(cbuff_uint8_test_overwrite);
    mu_run_test(cbuff_uint8_test_peek);
    mu_run_test(cbuff_uint8_test_edge_cases);
    mu_run_test(cbuff_uint8_test_capacity_one);
    mu_run_test(cbuff_uint8_test_reset);
    mu_run_test(cbuff_uint8_test_fill_empty_refill);
    mu_run_test(cbuff_uint8_test_peek_after_dequeue);
    mu_run_test(cbuff_uint8_test_overwrite_nonfull);
    return 0;
}

int main(void)
{
    char *result = all_tests();
    printf("\n%s\n", (result) ? result : "ALL TESTS PASSED\n");
    return result != 0;
}
