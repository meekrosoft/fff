#ifndef FAKE_FUNCTIONS
#define FAKE_FUNCTIONS

#define FFF_MAX_ARGS (10u)
#ifndef FFF_ARG_HISTORY_LEN
    #define FFF_ARG_HISTORY_LEN (50u)
#endif
#ifndef FFF_CALL_HISTORY_LEN
  #define FFF_CALL_HISTORY_LEN (50u)
#endif
#ifdef __cplusplus

/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \

static void * call_history[FFF_CALL_HISTORY_LEN];
static unsigned int call_history_idx;
static void RESET_HISTORY() {
    call_history_idx = 0;
}
#define REGISTER_CALL(function) \
   if(call_history_idx < FFF_CALL_HISTORY_LEN) call_history[call_history_idx++] = (void *)function;
#define SET_RETURN_SEQ( FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \
                        FUNCNAME##_return_val_seq = ARRAY_POINTER; \
                        FUNCNAME##_return_val_seq_len = ARRAY_LEN;
#include <vector>
typedef void (*void_fptr)();
std::vector<void_fptr> reset_functions;
static void RESET_FAKES()
{
    std::vector<void_fptr>::iterator it = reset_functions.begin();
    for( ; it != reset_functions.end(); ++it)
    {
        void_fptr ptr = *it;
        ptr();
    }

}
#define STATIC_INIT(FUNCNAME) \
class StaticInitializer_##FUNCNAME \
{ \
public: \
    StaticInitializer_##FUNCNAME() \
    { \
        reset_functions.push_back(FUNCNAME##_reset); \
    } \
}; \
static StaticInitializer_##FUNCNAME staticInitializer_##FUNCNAME; \


/* Defining a void function with 0 parameters*/
#define FAKE_VOID_FUNC0(FUNCNAME) \
extern "C"{ \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(){ \
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 1 parameters*/
#define FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 2 parameters*/
#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 3 parameters*/
#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 4 parameters*/
#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 5 parameters*/
#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 6 parameters*/
#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 7 parameters*/
#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 8 parameters*/
#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 9 parameters*/
#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static ARG8_TYPE FUNCNAME##_arg8_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_arg8_val = arg8; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg8_history[FUNCNAME##_call_count] = arg8; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        memset(FUNCNAME##_arg8_history, 0, sizeof(FUNCNAME##_arg8_history)); \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 0 parameters*/
#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
extern "C"{ \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(){ \
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 1 parameters*/
#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 2 parameters*/
#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 3 parameters*/
#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 4 parameters*/
#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 5 parameters*/
#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 6 parameters*/
#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 7 parameters*/
#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 8 parameters*/
#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 9 parameters*/
#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static ARG8_TYPE FUNCNAME##_arg8_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_arg8_val = arg8; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg8_history[FUNCNAME##_call_count] = arg8; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        memset(FUNCNAME##_arg8_history, 0, sizeof(FUNCNAME##_arg8_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \

#else  /* ansi c */

/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \

static void * call_history[FFF_CALL_HISTORY_LEN];
static unsigned int call_history_idx;
static void RESET_HISTORY() {
    call_history_idx = 0;
}
#define REGISTER_CALL(function) \
   if(call_history_idx < FFF_CALL_HISTORY_LEN) call_history[call_history_idx++] = (void *)function;
#define SET_RETURN_SEQ( FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \
                        FUNCNAME##_return_val_seq = ARRAY_POINTER; \
                        FUNCNAME##_return_val_seq_len = ARRAY_LEN;

/* Defining a void function with 0 parameters*/
#define FAKE_VOID_FUNC0(FUNCNAME) \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(){ \
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 1 parameters*/
#define FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 2 parameters*/
#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 3 parameters*/
#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 4 parameters*/
#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 5 parameters*/
#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 6 parameters*/
#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 7 parameters*/
#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 8 parameters*/
#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 9 parameters*/
#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static ARG8_TYPE FUNCNAME##_arg8_history[FFF_ARG_HISTORY_LEN];\
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_arg8_val = arg8; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg8_history[FUNCNAME##_call_count] = arg8; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        memset(FUNCNAME##_arg8_history, 0, sizeof(FUNCNAME##_arg8_history)); \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 0 parameters*/
#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(){ \
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 1 parameters*/
#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 2 parameters*/
#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 3 parameters*/
#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 4 parameters*/
#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 5 parameters*/
#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 6 parameters*/
#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 7 parameters*/
#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 8 parameters*/
#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \


/* Defining a function returning a value with 9 parameters*/
#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG0_TYPE FUNCNAME##_arg0_history[FFF_ARG_HISTORY_LEN];\
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG1_TYPE FUNCNAME##_arg1_history[FFF_ARG_HISTORY_LEN];\
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG2_TYPE FUNCNAME##_arg2_history[FFF_ARG_HISTORY_LEN];\
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG3_TYPE FUNCNAME##_arg3_history[FFF_ARG_HISTORY_LEN];\
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG4_TYPE FUNCNAME##_arg4_history[FFF_ARG_HISTORY_LEN];\
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG5_TYPE FUNCNAME##_arg5_history[FFF_ARG_HISTORY_LEN];\
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG6_TYPE FUNCNAME##_arg6_history[FFF_ARG_HISTORY_LEN];\
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG7_TYPE FUNCNAME##_arg7_history[FFF_ARG_HISTORY_LEN];\
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static ARG8_TYPE FUNCNAME##_arg8_history[FFF_ARG_HISTORY_LEN];\
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_return_val_seq_len = 0; \
    static int FUNCNAME##_return_val_seq_idx = 0; \
    static RETURN_TYPE * FUNCNAME##_return_val_seq = 0; \
    static unsigned int FUNCNAME##_call_count = 0; \
    static unsigned int FUNCNAME##_arg_history_len = FFF_ARG_HISTORY_LEN;\
    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_arg8_val = arg8; \
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg0_history[FUNCNAME##_call_count] = arg0; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg1_history[FUNCNAME##_call_count] = arg1; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg2_history[FUNCNAME##_call_count] = arg2; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg3_history[FUNCNAME##_call_count] = arg3; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg4_history[FUNCNAME##_call_count] = arg4; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg5_history[FUNCNAME##_call_count] = arg5; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg6_history[FUNCNAME##_call_count] = arg6; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg7_history[FUNCNAME##_call_count] = arg7; \
        }\
        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg8_history[FUNCNAME##_call_count] = arg8; \
        }\
        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\
            FUNCNAME##_arg_histories_dropped++;\
        }\
        FUNCNAME##_call_count++; \
        REGISTER_CALL(FUNCNAME); \
        if(FUNCNAME##_return_val_seq_len){ /* then its a sequence */ \
            if(FUNCNAME##_return_val_seq_idx < FUNCNAME##_return_val_seq_len) {\
                return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_idx++];\
            }\
            return FUNCNAME##_return_val_seq[FUNCNAME##_return_val_seq_len-1]; /* return last element */\
        } \
        return FUNCNAME##_return_val; \
    } \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        memset(FUNCNAME##_arg0_history, 0, sizeof(FUNCNAME##_arg0_history)); \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        memset(FUNCNAME##_arg1_history, 0, sizeof(FUNCNAME##_arg1_history)); \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        memset(FUNCNAME##_arg2_history, 0, sizeof(FUNCNAME##_arg2_history)); \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        memset(FUNCNAME##_arg3_history, 0, sizeof(FUNCNAME##_arg3_history)); \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        memset(FUNCNAME##_arg4_history, 0, sizeof(FUNCNAME##_arg4_history)); \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        memset(FUNCNAME##_arg5_history, 0, sizeof(FUNCNAME##_arg5_history)); \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        memset(FUNCNAME##_arg6_history, 0, sizeof(FUNCNAME##_arg6_history)); \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        memset(FUNCNAME##_arg7_history, 0, sizeof(FUNCNAME##_arg7_history)); \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        memset(FUNCNAME##_arg8_history, 0, sizeof(FUNCNAME##_arg8_history)); \
        FUNCNAME##_call_count = 0; \
        memset(&FUNCNAME##_return_val, 0, sizeof(FUNCNAME##_return_val)); \
        FUNCNAME##_return_val_seq_len = 0; \
        FUNCNAME##_return_val_seq_idx = 0; \
        FUNCNAME##_return_val_seq = 0; \
    } \

#endif  /* cpp/ansi c */

#define PP_NARG_MINUS2(...)     PP_NARG_MINUS2_(__VA_ARGS__, PP_RSEQ_N_MINUS2())

#define PP_NARG_MINUS2_(...)     PP_ARG_MINUS2_N(__VA_ARGS__)

#define PP_ARG_MINUS2_N(returnVal, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...)   N

#define PP_RSEQ_N_MINUS2()     9,8,7,6,5,4,3,2,1,0


#define FAKE_VALUE_FUNC(...)     FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__)

#define FUNC_VALUE_(N,...)     FUNC_VALUE_N(N,__VA_ARGS__)

#define FUNC_VALUE_N(N,...)     FAKE_VALUE_FUNC ## N(__VA_ARGS__)



#define PP_NARG_MINUS1(...)     PP_NARG_MINUS1_(__VA_ARGS__, PP_RSEQ_N_MINUS1())

#define PP_NARG_MINUS1_(...)     PP_ARG_MINUS1_N(__VA_ARGS__)

#define PP_ARG_MINUS1_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...)   N

#define PP_RSEQ_N_MINUS1()     9,8,7,6,5,4,3,2,1,0

#define FAKE_VOID_FUNC(...)     FUNC_VOID_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__)

#define FUNC_VOID_(N,...)     FUNC_VOID_N(N,__VA_ARGS__)

#define FUNC_VOID_N(N,...)     FAKE_VOID_FUNC ## N(__VA_ARGS__)


#endif // FAKE_FUNCTIONS
