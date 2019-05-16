#if ASSERTS_ENABLED
#define ASSERT_BARK(expr, message) if(expr) { } else { printf("ASSERT FAILED in %s:%d '%s'\n", __FILE__, __LINE__, message);  *(int*)0 = 0; }
#define ASSERT(expr) if(expr) { } else { printf("ASSERT FAILED in %s:%d\n", __FILE__, __LINE__); *(int*)0 = 0; }
#else
#define ASSERT_BARK(expr, message)
#define ASSERT(expr)
#endif
