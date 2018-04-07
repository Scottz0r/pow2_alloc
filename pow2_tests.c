#include "pow2.h"
#include <stdio.h>
#include <string.h>

/******************************************************************************
** Test Harness
******************************************************************************/
#define TEST_CASE

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define REQUIRE_TRUE(expr) do{ if(!_require_true(__LINE__, #expr, expr)) return; } while(0)

#define REQUIRE_STR_EQ(s1, s2) do{ if(!_require_str_equal(__LINE__, #s1, #s2, s1, s2)) return; } while(0)

#define REQUIRE_NOT_NULL(expr) do{ if(!_require_not_null(__LINE__, #expr, expr)) return; } while(0)

static int passed;
static int failed;

/*
** Tests that the given expression evaluates to true.
*/
static int _require_true(int line, const char *expr, int result)
{
    if (!result)
    {
        ++failed;

        printf("REQUIRE_TRUE failed! \n");
        printf("    Line: %d \n", line);
        printf("    Expr: %s\n", expr);
        printf("\n");

        return 0;
    }
    else
    {
        ++passed;
        return 1;
    }
}

/*
** Tests that two strings are equal.
*/
static int _require_str_equal(int line, const char *expr1, const char *expr2, const char *str1, const char *str2)
{
    if (strcmp(str1, str2) != 0)
    {
        ++failed;

        printf("REQUIRE_STR_EQ failed! \n");
        printf("    Line: %d \n", line);
        printf("    Expr: %s != %s \n", expr1, expr2);
        printf("    s1  : \"%s\" \n", str1);
        printf("    s2  : \"%s\" \n", str2);
        printf("\n");

        return 0;
    }
    else
    {
        ++passed;
        return 1;
    }
}

/*
** Tests that a pointer is not null.
*/
static int _require_not_null(int line, const char *expr, void* ptr)
{
    if (ptr == NULL)
    {
        ++failed;

        printf("REQUIRE_NOT_NULL failed! \n");
        printf("    Line: %d \n", line);
        printf("    Expr: %s \n", expr);
        printf("\n");

        return 0;
    }
    else
    {
        ++passed;
        return 1;
    }
}


/******************************************************************************
** Tests
******************************************************************************/
#define MEM_SIZE (1024 * 1024 * 16)
static char s_mem_buffer[MEM_SIZE];
static char s_small_buffer[16];

void test_alloc()
{
    char *d0, *d1;

    d0 = pow2_alloc(s_mem_buffer, MEM_SIZE, 10);
    REQUIRE_TRUE(d0 != NULL);
    REQUIRE_TRUE(*(d0 - 1) == 4);

    d1 = pow2_alloc(s_mem_buffer, MEM_SIZE, 98);
    REQUIRE_TRUE(d1 != NULL);
    REQUIRE_TRUE(*(d1 - 1) == 7);
}

void test_free()
{
    char *s;

    s = pow2_alloc(s_mem_buffer, MEM_SIZE, 15);
    REQUIRE_TRUE(s != NULL);

    memcpy(s, "abc", 3);
    REQUIRE_TRUE(s[0] == 'a');
    REQUIRE_TRUE(s[1] == 'b');
    REQUIRE_TRUE(s[2] == 'c');

    pow2_free(s);

    /*
    ** This is bad pratice IRL because the pointer was deallocated, 
    ** but we know these should be reset to NULL
    */
    REQUIRE_TRUE(s[0] == 0);
    REQUIRE_TRUE(s[1] == 0);
    REQUIRE_TRUE(s[2] == 0);
}

void test_out_mem()
{
    char *s;

    s = pow2_alloc(s_small_buffer, 10, 16);
    REQUIRE_TRUE(s == NULL);
}

void test_free_alloc()
{
    const char str1[] = "Test string 1";
    const char str2[] = "Test string 2";
    const char str3[] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    char *s1, *s2, *s3;

    s1 = pow2_alloc(s_mem_buffer, MEM_SIZE, sizeof(str1));
    s2 = pow2_alloc(s_mem_buffer, MEM_SIZE, sizeof(str2));

    REQUIRE_NOT_NULL(s1);
    REQUIRE_NOT_NULL(s2);

    strcpy(s1, str1);
    strcpy(s2, str2);
    
    pow2_free(s1);

    s3 = pow2_alloc(s_mem_buffer, MEM_SIZE, sizeof(str3));

    REQUIRE_NOT_NULL(s3);

    strcpy(s3, str3);

    /*
    ** This is confirming that s3 was placed after s2, even though memory
    ** used by s1 was freed. Make sure data copied untouched.
    */
    REQUIRE_TRUE(s3 > s2);
    REQUIRE_STR_EQ(s2, str2);
    REQUIRE_STR_EQ(s3, str3);
}


/******************************************************************************
** Entry point.
******************************************************************************/
void main()
{
    printf("Running tests for pow2_alloc... \n");
    printf("------------------------------- \n\n");

    test_alloc();
    test_free();
    test_free_alloc();

    printf("------------------------------- \n");
    printf("%d / %d Passed \n\n", passed, (passed + failed));
}