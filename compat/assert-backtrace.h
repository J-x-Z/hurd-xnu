/*
 * assert-backtrace.h - XNU compatibility version
 * 
 * Simplified version that falls back to standard assert.
 * We don't have backtrace_X functions easily, so we just use assert.
 */
#ifndef __ASSERT_BACKTRACE__
#define __ASSERT_BACKTRACE__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG

#define assert_backtrace(expr)        ((void) 0)
#define assert_perror_backtrace(errnum)  ((void) 0)

#else /* NDEBUG */

/* Simple assert with message - no backtrace on macOS */
#define assert_backtrace(expr) \
  do { \
    if (!(expr)) { \
      fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", \
              #expr, __FILE__, __LINE__); \
      abort(); \
    } \
  } while (0)

#define assert_perror_backtrace(errnum) \
  do { \
    if ((errnum) != 0) { \
      fprintf(stderr, "Assertion failed (errno %d), file %s, line %d\n", \
              (errnum), __FILE__, __LINE__); \
      abort(); \
    } \
  } while (0)

/* Stubs for backtrace functions */
static inline void backtrace_stderr(void) { /* no-op on XNU compat */ }
static inline void backtrace_mach(void) { /* no-op on XNU compat */ }

#endif /* NDEBUG */
#endif /* __ASSERT_BACKTRACE__ */
