/*
LICENSE

The MIT License (MIT)

Copyright (c) 2010 Michael Long

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef FAKE_FUNCTIONS
#define FAKE_FUNCTIONS

#include <stdarg.h>
#include <string.h> /* For memset and memcpy */

#define FFF_MAX_ARGS (20u)
#ifndef FFF_ARG_HISTORY_LEN
    #define FFF_ARG_HISTORY_LEN (50u)
#endif
#ifndef FFF_CALL_HISTORY_LEN
    #define FFF_CALL_HISTORY_LEN (50u)
#endif
#ifndef FFF_GCC_FUNCTION_ATTRIBUTES
    #define FFF_GCC_FUNCTION_ATTRIBUTES
#endif
/* -- INTERNAL HELPER MACROS -- */
#define SET_RETURN_SEQ(FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \
    FUNCNAME##_fake.return_val_seq = ARRAY_POINTER; \
    FUNCNAME##_fake.return_val_seq_len = ARRAY_LEN;

/* FFF_RETURN (begin) */
#define FFF_RETURN(...)                                             \
  _FFF_SEQ_DEF(__VA_ARGS__); if(PP_NARG(__VA_ARGS__) == 2) {FFF_RETURN_VAL(__VA_ARGS__, 0);} else {_FFF_RETURN_SEQ(HEAD(__VA_ARGS__));}
#define _FFF_RET_TYPE(FN) __typeof__(FFF_RET(FN))
#define _FFF_SEQ(FN) CAT(FN, __LINE__)
#define _FFF_SEQ_DEF(FN, ...) _FFF_RET_TYPE(FN) _FFF_SEQ(FN)[] = {__VA_ARGS__}
#define _FFF_SEQ_LEN(FN) (sizeof(_FFF_SEQ(FN))/sizeof(FFF_RET(FN)))
#define FFF_RETURN_VAL(FN, ...) FFF_RET(FN) = HEAD(__VA_ARGS__)
#define _FFF_RETURN_SEQ(FN) SET_RETURN_SEQ(FN, _FFF_SEQ(FN), _FFF_SEQ_LEN(FN)); \
    _FFF_ASSERT_TRUE_MSG(FFF(FN).return_val_seq_len>0, "No return values specified")
/* FFF_RETURN (end) */

#define SET_CUSTOM_FAKE_SEQ(FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \
    FUNCNAME##_fake.custom_fake_seq = ARRAY_POINTER; \
    FUNCNAME##_fake.custom_fake_seq_len = ARRAY_LEN;

/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \


#define DECLARE_ARG(type, n, FUNCNAME) \
    type arg##n##_val; \
    type arg##n##_history[FFF_ARG_HISTORY_LEN];

#define DECLARE_ALL_FUNC_COMMON \
    unsigned int call_count; \
    unsigned int arg_history_len; \
    unsigned int arg_histories_dropped; \

#define DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
    RETURN_TYPE return_val_history[FFF_ARG_HISTORY_LEN];

#define SAVE_ARG(FUNCNAME, n) \
    memcpy((void*)&FUNCNAME##_fake.arg##n##_val, (void*)&arg##n, sizeof(arg##n));

#define ROOM_FOR_MORE_HISTORY(FUNCNAME) \
    FUNCNAME##_fake.call_count < FFF_ARG_HISTORY_LEN

#define SAVE_RET_HISTORY(FUNCNAME, RETVAL) \
    if ((FUNCNAME##_fake.call_count - 1) < FFF_ARG_HISTORY_LEN) \
        memcpy((void *)&FUNCNAME##_fake.return_val_history[FUNCNAME##_fake.call_count - 1], (const void *) &RETVAL, sizeof(RETVAL)); \

#define SAVE_ARG_HISTORY(FUNCNAME, ARGN) \
    memcpy((void*)&FUNCNAME##_fake.arg##ARGN##_history[FUNCNAME##_fake.call_count], (void*)&arg##ARGN, sizeof(arg##ARGN));

#define HISTORY_DROPPED(FUNCNAME) \
    FUNCNAME##_fake.arg_histories_dropped++

#define DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
    RETURN_TYPE return_val; \
    int return_val_seq_len; \
    int return_val_seq_idx; \
    RETURN_TYPE * return_val_seq; \

#define DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
    int custom_fake_seq_len; \
    int custom_fake_seq_idx; \

#define INCREMENT_CALL_COUNT(FUNCNAME) \
    FUNCNAME##_fake.call_count++

#define RETURN_FAKE_RESULT(FUNCNAME) \
    if (FUNCNAME##_fake.return_val_seq_len){ /* then its a sequence */ \
        if(FUNCNAME##_fake.return_val_seq_idx < FUNCNAME##_fake.return_val_seq_len) { \
            SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx]) \
            return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx++]; \
        } \
        SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1]) \
        return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1]; /* return last element */ \
    } \
    SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val) \
    return FUNCNAME##_fake.return_val; \

#ifdef __cplusplus
    #define FFF_EXTERN_C extern "C"{
    #define FFF_END_EXTERN_C } 
#else  /* ansi c */
    #define FFF_EXTERN_C 
    #define FFF_END_EXTERN_C 
#endif  /* cpp/ansi c */

#define DEFINE_RESET_FUNCTION(FUNCNAME) \
    void FUNCNAME##_reset(void){ \
        memset(&FUNCNAME##_fake, 0, sizeof(FUNCNAME##_fake)); \
        FUNCNAME##_fake.arg_history_len = FFF_ARG_HISTORY_LEN; \
    }
#define FFF(FN) FN##_fake
#define FFF_CALLS(FN) FFF(FN).call_count
#define FFF_LAST_ARG_VAL(FN, ARG_IDX) FFF(FN).arg##ARG_IDX##_val
#define FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX) FFF(FN).arg##ARG_IDX##_history[CALL_IDX-1]
#define FFF_RET(FN) FFF(FN).return_val
#define _FFF_VERIFY_ARG(FN, VAL, ARG_IDX) VAL == FFF_LAST_ARG_VAL(FN, ARG_IDX)
#define _FFF_VERIFY_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX) (VAL == FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX))
#define _FFF_AND_VERIFY_NTH_CALL_ARG(FN, CALL_IDX, VAL, ARG_IDX) && _FFF_VERIFY_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX)
#define _FFF_VERIFY_NTH_CALL(FN, CALL_IDX, ...)                           ( (CALL_IDX > 0) && (CALL_IDX <= FFF_CALLS(FN)) PP_2PAR_EACH_IDX(_FFF_AND_VERIFY_NTH_CALL_ARG, FN, CALL_IDX, __VA_ARGS__) )

//This uses GCC compound statement expression:
//https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
#define _FFF_VERIFY_ANY_CALL(FN, ...) ({                                    int verified = 0;                                                     int call_idx = FFF_CALLS(FN);                                        while(call_idx && !verified) {                                          verified |= _FFF_VERIFY_NTH_CALL(FN, call_idx, __VA_ARGS__);          call_idx--;                                                         }                                                                     verified;                                                           })

/* -- END INTERNAL HELPER MACROS -- */

typedef void (*fff_function_t)(void);
typedef struct { 
    fff_function_t call_history[FFF_CALL_HISTORY_LEN];
    unsigned int call_history_idx;
} fff_globals_t;

FFF_EXTERN_C
extern fff_globals_t fff;
FFF_END_EXTERN_C

#define DEFINE_FFF_GLOBALS \
    FFF_EXTERN_C \
        fff_globals_t fff; \
    FFF_END_EXTERN_C

#define FFF_RESET_HISTORY() \
    fff.call_history_idx = 0; \
    memset(fff.call_history, 0, sizeof(fff.call_history));

#define REGISTER_CALL(function) \
    if(fff.call_history_idx < FFF_CALL_HISTORY_LEN) \
        fff.call_history[fff.call_history_idx++] = (fff_function_t)function;

#define DECLARE_FAKE_VOID_FUNC0(FUNCNAME) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(void); \
        void(**custom_fake_seq)(void); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(void); \

#define DEFINE_FAKE_VOID_FUNC0(FUNCNAME) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(void){ \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC0(FUNCNAME) \
    DECLARE_FAKE_VOID_FUNC0(FUNCNAME) \
    DEFINE_FAKE_VOID_FUNC0(FUNCNAME) \


#define DECLARE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0); \
        void(**custom_fake_seq)(ARG0_TYPE arg0); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0); \

#define DEFINE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0){ \
        SAVE_ARG(FUNCNAME, 0); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    DECLARE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    DEFINE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \


#define DECLARE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1); \

#define DEFINE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DECLARE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DEFINE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \


#define DECLARE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \

#define DEFINE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DECLARE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DEFINE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \


#define DECLARE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \

#define DEFINE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DECLARE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DEFINE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \


#define DECLARE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \

#define DEFINE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DECLARE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DEFINE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \


#define DECLARE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \

#define DEFINE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DECLARE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DEFINE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \


#define DECLARE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \

#define DEFINE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DECLARE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DEFINE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \


#define DECLARE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \

#define DEFINE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DECLARE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DEFINE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \


#define DECLARE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \

#define DEFINE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DECLARE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DEFINE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \


#define DECLARE_FAKE_VOID_FUNC10(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \

#define DEFINE_FAKE_VOID_FUNC10(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC10(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    DECLARE_FAKE_VOID_FUNC10(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    DEFINE_FAKE_VOID_FUNC10(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \


#define DECLARE_FAKE_VOID_FUNC11(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \

#define DEFINE_FAKE_VOID_FUNC11(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC11(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    DECLARE_FAKE_VOID_FUNC11(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    DEFINE_FAKE_VOID_FUNC11(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \


#define DECLARE_FAKE_VOID_FUNC12(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \

#define DEFINE_FAKE_VOID_FUNC12(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC12(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    DECLARE_FAKE_VOID_FUNC12(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    DEFINE_FAKE_VOID_FUNC12(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \


#define DECLARE_FAKE_VOID_FUNC13(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \

#define DEFINE_FAKE_VOID_FUNC13(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC13(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    DECLARE_FAKE_VOID_FUNC13(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    DEFINE_FAKE_VOID_FUNC13(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \


#define DECLARE_FAKE_VOID_FUNC14(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \

#define DEFINE_FAKE_VOID_FUNC14(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC14(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    DECLARE_FAKE_VOID_FUNC14(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    DEFINE_FAKE_VOID_FUNC14(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \


#define DECLARE_FAKE_VOID_FUNC15(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \

#define DEFINE_FAKE_VOID_FUNC15(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC15(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    DECLARE_FAKE_VOID_FUNC15(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    DEFINE_FAKE_VOID_FUNC15(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \


#define DECLARE_FAKE_VOID_FUNC16(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \

#define DEFINE_FAKE_VOID_FUNC16(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC16(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    DECLARE_FAKE_VOID_FUNC16(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    DEFINE_FAKE_VOID_FUNC16(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \


#define DECLARE_FAKE_VOID_FUNC17(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \

#define DEFINE_FAKE_VOID_FUNC17(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC17(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    DECLARE_FAKE_VOID_FUNC17(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    DEFINE_FAKE_VOID_FUNC17(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \


#define DECLARE_FAKE_VOID_FUNC18(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \

#define DEFINE_FAKE_VOID_FUNC18(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC18(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    DECLARE_FAKE_VOID_FUNC18(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    DEFINE_FAKE_VOID_FUNC18(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \


#define DECLARE_FAKE_VOID_FUNC19(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \

#define DEFINE_FAKE_VOID_FUNC19(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC19(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    DECLARE_FAKE_VOID_FUNC19(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    DEFINE_FAKE_VOID_FUNC19(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \


#define DECLARE_FAKE_VOID_FUNC20(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ARG(ARG19_TYPE, 19, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \

#define DEFINE_FAKE_VOID_FUNC20(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        SAVE_ARG(FUNCNAME, 19); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
            SAVE_ARG_HISTORY(FUNCNAME, 19); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
            } \
            else{ \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC20(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    DECLARE_FAKE_VOID_FUNC20(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    DEFINE_FAKE_VOID_FUNC20(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(void); \
        RETURN_TYPE(**custom_fake_seq)(void); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(void); \

#define DEFINE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(void){ \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    DECLARE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    DEFINE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \


#define DECLARE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0); \

#define DEFINE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0){ \
        SAVE_ARG(FUNCNAME, 0); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    DECLARE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    DEFINE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1); \

#define DEFINE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DECLARE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DEFINE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \

#define DEFINE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DECLARE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DEFINE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \

#define DEFINE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DECLARE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DEFINE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \

#define DEFINE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DECLARE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DEFINE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \

#define DEFINE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DECLARE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DEFINE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \

#define DEFINE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DECLARE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DEFINE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \

#define DEFINE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DECLARE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DEFINE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \

#define DEFINE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DECLARE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DEFINE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC10(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9); \

#define DEFINE_FAKE_VALUE_FUNC10(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC10(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    DECLARE_FAKE_VALUE_FUNC10(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \
    DEFINE_FAKE_VALUE_FUNC10(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC11(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10); \

#define DEFINE_FAKE_VALUE_FUNC11(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC11(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    DECLARE_FAKE_VALUE_FUNC11(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \
    DEFINE_FAKE_VALUE_FUNC11(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC12(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11); \

#define DEFINE_FAKE_VALUE_FUNC12(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC12(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    DECLARE_FAKE_VALUE_FUNC12(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \
    DEFINE_FAKE_VALUE_FUNC12(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC13(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12); \

#define DEFINE_FAKE_VALUE_FUNC13(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC13(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    DECLARE_FAKE_VALUE_FUNC13(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \
    DEFINE_FAKE_VALUE_FUNC13(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC14(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13); \

#define DEFINE_FAKE_VALUE_FUNC14(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC14(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    DECLARE_FAKE_VALUE_FUNC14(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \
    DEFINE_FAKE_VALUE_FUNC14(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC15(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14); \

#define DEFINE_FAKE_VALUE_FUNC15(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC15(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    DECLARE_FAKE_VALUE_FUNC15(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \
    DEFINE_FAKE_VALUE_FUNC15(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC16(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15); \

#define DEFINE_FAKE_VALUE_FUNC16(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC16(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    DECLARE_FAKE_VALUE_FUNC16(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \
    DEFINE_FAKE_VALUE_FUNC16(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC17(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16); \

#define DEFINE_FAKE_VALUE_FUNC17(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC17(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    DECLARE_FAKE_VALUE_FUNC17(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \
    DEFINE_FAKE_VALUE_FUNC17(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC18(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17); \

#define DEFINE_FAKE_VALUE_FUNC18(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC18(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    DECLARE_FAKE_VALUE_FUNC18(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \
    DEFINE_FAKE_VALUE_FUNC18(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC19(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18); \

#define DEFINE_FAKE_VALUE_FUNC19(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC19(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    DECLARE_FAKE_VALUE_FUNC19(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \
    DEFINE_FAKE_VALUE_FUNC19(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE) \


#define DECLARE_FAKE_VALUE_FUNC20(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ARG(ARG19_TYPE, 19, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19); \

#define DEFINE_FAKE_VALUE_FUNC20(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ARG19_TYPE arg19){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        SAVE_ARG(FUNCNAME, 19); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
            SAVE_ARG_HISTORY(FUNCNAME, 19); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
            } \
            else{ \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
            } \
        } \
        if (FUNCNAME##_fake.custom_fake){  \
            RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
            return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19); \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC20(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    DECLARE_FAKE_VALUE_FUNC20(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \
    DEFINE_FAKE_VALUE_FUNC20(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ARG19_TYPE) \


#define DECLARE_FAKE_VOID_FUNC2_VARARG(FUNCNAME, ARG0_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ...); \

#define DEFINE_FAKE_VOID_FUNC2_VARARG(FUNCNAME, ARG0_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg0); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg0); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg0); \
            FUNCNAME##_fake.custom_fake(arg0, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC2_VARARG(FUNCNAME, ARG0_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC2_VARARG(FUNCNAME, ARG0_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC2_VARARG(FUNCNAME, ARG0_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC3_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ...); \

#define DEFINE_FAKE_VOID_FUNC3_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg1); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg1); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg1); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC3_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC3_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC3_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC4_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ...); \

#define DEFINE_FAKE_VOID_FUNC4_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg2); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg2); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg2); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC4_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC4_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC4_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC5_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ...); \

#define DEFINE_FAKE_VOID_FUNC5_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg3); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg3); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg3); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC5_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC5_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC5_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC6_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ...); \

#define DEFINE_FAKE_VOID_FUNC6_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg4); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg4); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg4); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC6_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC6_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC6_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC7_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ...); \

#define DEFINE_FAKE_VOID_FUNC7_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg5); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg5); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg5); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC7_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC7_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC7_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC8_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ...); \

#define DEFINE_FAKE_VOID_FUNC8_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg6); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg6); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg6); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC8_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC8_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC8_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC9_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ...); \

#define DEFINE_FAKE_VOID_FUNC9_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg7); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg7); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg7); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC9_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC9_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC9_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC10_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ...); \

#define DEFINE_FAKE_VOID_FUNC10_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg8); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg8); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg8); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC10_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC10_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC10_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC11_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ...); \

#define DEFINE_FAKE_VOID_FUNC11_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg9); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg9); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg9); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC11_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC11_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC11_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC12_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ...); \

#define DEFINE_FAKE_VOID_FUNC12_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg10); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg10); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg10); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC12_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC12_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC12_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC13_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ...); \

#define DEFINE_FAKE_VOID_FUNC13_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg11); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg11); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg11); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC13_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC13_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC13_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC14_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ...); \

#define DEFINE_FAKE_VOID_FUNC14_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg12); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg12); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg12); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC14_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC14_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC14_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC15_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ...); \

#define DEFINE_FAKE_VOID_FUNC15_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg13); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg13); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg13); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC15_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC15_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC15_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC16_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ...); \

#define DEFINE_FAKE_VOID_FUNC16_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg14); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg14); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg14); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC16_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC16_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC16_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC17_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ...); \

#define DEFINE_FAKE_VOID_FUNC17_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg15); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg15); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg15); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC17_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC17_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC17_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC18_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ...); \

#define DEFINE_FAKE_VOID_FUNC18_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg16); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg16); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg16); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC18_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC18_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC18_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC19_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ...); \

#define DEFINE_FAKE_VOID_FUNC19_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg17); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg17); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg17); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC19_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC19_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC19_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \


#define DECLARE_FAKE_VOID_FUNC20_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, va_list ap); \
        void(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ...); \

#define DEFINE_FAKE_VOID_FUNC20_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    void FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg18); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
                va_end(ap); \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg18); \
                FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
                va_end(ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            va_list ap; \
            va_start(ap, arg18); \
            FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
            va_end(ap); \
        } \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VOID_FUNC20_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    DECLARE_FAKE_VOID_FUNC20_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    DEFINE_FAKE_VOID_FUNC20_VARARG(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC2_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ...); \

#define DEFINE_FAKE_VALUE_FUNC2_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg0); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg0); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg0); \
            ret = FUNCNAME##_fake.custom_fake(arg0, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC2_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC2_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC2_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC3_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ...); \

#define DEFINE_FAKE_VALUE_FUNC3_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg1); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg1); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg1); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC3_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC3_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC3_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC4_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ...); \

#define DEFINE_FAKE_VALUE_FUNC4_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg2); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg2); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg2); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC4_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC4_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC4_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC5_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ...); \

#define DEFINE_FAKE_VALUE_FUNC5_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg3); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg3); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg3); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC5_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC5_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC5_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC6_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ...); \

#define DEFINE_FAKE_VALUE_FUNC6_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg4); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg4); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg4); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC6_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC6_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC6_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC7_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ...); \

#define DEFINE_FAKE_VALUE_FUNC7_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg5); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg5); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg5); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC7_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC7_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC7_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC8_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ...); \

#define DEFINE_FAKE_VALUE_FUNC8_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg6); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg6); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg6); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC8_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC8_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC8_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC9_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ...); \

#define DEFINE_FAKE_VALUE_FUNC9_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg7); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg7); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg7); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC9_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC9_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC9_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC10_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ...); \

#define DEFINE_FAKE_VALUE_FUNC10_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg8); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg8); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg8); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC10_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC10_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC10_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC11_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ...); \

#define DEFINE_FAKE_VALUE_FUNC11_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg9); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg9); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg9); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC11_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC11_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC11_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC12_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ...); \

#define DEFINE_FAKE_VALUE_FUNC12_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg10); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg10); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg10); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC12_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC12_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC12_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC13_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ...); \

#define DEFINE_FAKE_VALUE_FUNC13_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg11); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg11); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg11); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC13_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC13_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC13_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC14_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ...); \

#define DEFINE_FAKE_VALUE_FUNC14_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg12); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg12); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg12); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC14_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC14_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC14_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC15_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ...); \

#define DEFINE_FAKE_VALUE_FUNC15_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg13); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg13); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg13); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC15_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC15_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC15_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC16_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ...); \

#define DEFINE_FAKE_VALUE_FUNC16_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg14); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg14); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg14); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC16_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC16_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC16_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC17_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ...); \

#define DEFINE_FAKE_VALUE_FUNC17_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg15); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg15); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg15); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC17_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC17_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC17_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC18_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ...); \

#define DEFINE_FAKE_VALUE_FUNC18_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg16); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg16); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg16); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC18_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC18_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC18_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC19_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ...); \

#define DEFINE_FAKE_VALUE_FUNC19_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg17); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg17); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg17); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC19_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC19_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC19_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ...) \


#define DECLARE_FAKE_VALUE_FUNC20_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    typedef struct FUNCNAME##_Fake { \
        DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
        DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
        DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
        DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
        DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
        DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
        DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
        DECLARE_ARG(ARG7_TYPE, 7, FUNCNAME) \
        DECLARE_ARG(ARG8_TYPE, 8, FUNCNAME) \
        DECLARE_ARG(ARG9_TYPE, 9, FUNCNAME) \
        DECLARE_ARG(ARG10_TYPE, 10, FUNCNAME) \
        DECLARE_ARG(ARG11_TYPE, 11, FUNCNAME) \
        DECLARE_ARG(ARG12_TYPE, 12, FUNCNAME) \
        DECLARE_ARG(ARG13_TYPE, 13, FUNCNAME) \
        DECLARE_ARG(ARG14_TYPE, 14, FUNCNAME) \
        DECLARE_ARG(ARG15_TYPE, 15, FUNCNAME) \
        DECLARE_ARG(ARG16_TYPE, 16, FUNCNAME) \
        DECLARE_ARG(ARG17_TYPE, 17, FUNCNAME) \
        DECLARE_ARG(ARG18_TYPE, 18, FUNCNAME) \
        DECLARE_ALL_FUNC_COMMON \
        DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
        DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE) \
        DECLARE_CUSTOM_FAKE_SEQ_VARIABLES \
        RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, va_list ap); \
        RETURN_TYPE(**custom_fake_seq)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, va_list ap); \
    } FUNCNAME##_Fake; \
    extern FUNCNAME##_Fake FUNCNAME##_fake; \
    void FUNCNAME##_reset(void); \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ...); \

#define DEFINE_FAKE_VALUE_FUNC20_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    FUNCNAME##_Fake FUNCNAME##_fake; \
    RETURN_TYPE FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8, ARG9_TYPE arg9, ARG10_TYPE arg10, ARG11_TYPE arg11, ARG12_TYPE arg12, ARG13_TYPE arg13, ARG14_TYPE arg14, ARG15_TYPE arg15, ARG16_TYPE arg16, ARG17_TYPE arg17, ARG18_TYPE arg18, ...){ \
        SAVE_ARG(FUNCNAME, 0); \
        SAVE_ARG(FUNCNAME, 1); \
        SAVE_ARG(FUNCNAME, 2); \
        SAVE_ARG(FUNCNAME, 3); \
        SAVE_ARG(FUNCNAME, 4); \
        SAVE_ARG(FUNCNAME, 5); \
        SAVE_ARG(FUNCNAME, 6); \
        SAVE_ARG(FUNCNAME, 7); \
        SAVE_ARG(FUNCNAME, 8); \
        SAVE_ARG(FUNCNAME, 9); \
        SAVE_ARG(FUNCNAME, 10); \
        SAVE_ARG(FUNCNAME, 11); \
        SAVE_ARG(FUNCNAME, 12); \
        SAVE_ARG(FUNCNAME, 13); \
        SAVE_ARG(FUNCNAME, 14); \
        SAVE_ARG(FUNCNAME, 15); \
        SAVE_ARG(FUNCNAME, 16); \
        SAVE_ARG(FUNCNAME, 17); \
        SAVE_ARG(FUNCNAME, 18); \
        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){ \
            SAVE_ARG_HISTORY(FUNCNAME, 0); \
            SAVE_ARG_HISTORY(FUNCNAME, 1); \
            SAVE_ARG_HISTORY(FUNCNAME, 2); \
            SAVE_ARG_HISTORY(FUNCNAME, 3); \
            SAVE_ARG_HISTORY(FUNCNAME, 4); \
            SAVE_ARG_HISTORY(FUNCNAME, 5); \
            SAVE_ARG_HISTORY(FUNCNAME, 6); \
            SAVE_ARG_HISTORY(FUNCNAME, 7); \
            SAVE_ARG_HISTORY(FUNCNAME, 8); \
            SAVE_ARG_HISTORY(FUNCNAME, 9); \
            SAVE_ARG_HISTORY(FUNCNAME, 10); \
            SAVE_ARG_HISTORY(FUNCNAME, 11); \
            SAVE_ARG_HISTORY(FUNCNAME, 12); \
            SAVE_ARG_HISTORY(FUNCNAME, 13); \
            SAVE_ARG_HISTORY(FUNCNAME, 14); \
            SAVE_ARG_HISTORY(FUNCNAME, 15); \
            SAVE_ARG_HISTORY(FUNCNAME, 16); \
            SAVE_ARG_HISTORY(FUNCNAME, 17); \
            SAVE_ARG_HISTORY(FUNCNAME, 18); \
        } \
        else{ \
            HISTORY_DROPPED(FUNCNAME); \
        } \
        INCREMENT_CALL_COUNT(FUNCNAME); \
        REGISTER_CALL(FUNCNAME); \
        if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */ \
            if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){ \
                va_list ap; \
                va_start(ap, arg18); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
            } \
            else{ \
                va_list ap; \
                va_start(ap, arg18); \
                RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
                SAVE_RET_HISTORY(FUNCNAME, ret); \
                va_end(ap); \
                return ret; \
                return FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
            } \
        } \
        if(FUNCNAME##_fake.custom_fake){ \
            RETURN_TYPE ret; \
            va_list ap; \
            va_start(ap, arg18); \
            ret = FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ap); \
            va_end(ap); \
            SAVE_RET_HISTORY(FUNCNAME, ret); \
            return ret; \
        } \
        RETURN_FAKE_RESULT(FUNCNAME) \
    } \
    DEFINE_RESET_FUNCTION(FUNCNAME) \

#define FAKE_VALUE_FUNC20_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    DECLARE_FAKE_VALUE_FUNC20_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \
    DEFINE_FAKE_VALUE_FUNC20_VARARG(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE, ARG9_TYPE, ARG10_TYPE, ARG11_TYPE, ARG12_TYPE, ARG13_TYPE, ARG14_TYPE, ARG15_TYPE, ARG16_TYPE, ARG17_TYPE, ARG18_TYPE, ...) \


#ifndef PP_NARG
#ifndef PP_UTIL
  #define EXPAND(x) x
  #define HEAD(FIRST, ...) FIRST
  #define TAIL(FIRST, ...) __VA_ARGS__
  #define CAT(A, B) _CAT(A,B)
  #define _CAT(A, B) A ## B
  #define PP_RSEQ_N() 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#endif //PP_UTIL

#ifndef PP_NARG
  #define PP_NARG(...)  EXPAND(PP_ARG_N(__VA_ARGS__, PP_RSEQ_N()))
  #define PP_ARG_N(...) EXPAND(_PP_ARG_N(__VA_ARGS__))
  #define _PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N
#endif //PP_NARG

#endif

#ifndef PP_NARG_MINUS1
  #define PP_NARG_MINUS1(...)  EXPAND(PP_ARG_MINUS1_N(__VA_ARGS__, PP_RSEQ_N()))
  #define PP_ARG_MINUS1_N(...) EXPAND(_PP_ARG_MINUS1_N(__VA_ARGS__))
  #define _PP_ARG_MINUS1_N(__0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N
#endif //PP_NARG_MINUS1


#ifndef PP_NARG_MINUS2
  #define PP_NARG_MINUS2(...)  EXPAND(PP_ARG_MINUS2_N(__VA_ARGS__, PP_RSEQ_N()))
  #define PP_ARG_MINUS2_N(...) EXPAND(_PP_ARG_MINUS2_N(__VA_ARGS__))
  #define _PP_ARG_MINUS2_N(__1, __0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N
#endif //PP_NARG_MINUS2


#ifndef PP_NARG_MINUS3
  #define PP_NARG_MINUS3(...)  EXPAND(PP_ARG_MINUS3_N(__VA_ARGS__, PP_RSEQ_N()))
  #define PP_ARG_MINUS3_N(...) EXPAND(_PP_ARG_MINUS3_N(__VA_ARGS__))
  #define _PP_ARG_MINUS3_N(__2, __1, __0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N
#endif //PP_NARG_MINUS3


#ifndef PP_1PAR_EACH_IDX
#ifndef PP_1PAR_EACH_IDX
  #define PP_1PAR_EACH_IDX(TF, P1, ...) _PP_1PAR_EACH_IDX(TF, P1, PP_NARG(__VA_ARGS__), __VA_ARGS__)
  #define _PP_1PAR_EACH_IDX(TF, P1, N, ...) __PP_1PAR_EACH_IDX(TF, P1, N, __VA_ARGS__)
  #define __PP_1PAR_EACH_IDX(TF, P1, N, ...) _PP_1PAR_IDX_##N(TF, P1, __VA_ARGS__)
  
  #define _PP_1PAR_IDX_0(TF, ARG, dummy)
  #define _PP_1PAR_IDX_1(TF, P1, _0) TF(P1, _0, 0)
  #define _PP_1PAR_IDX_2(TF, P1, _0, _1) TF(P1, _0, 0) TF(P1, _1, 1)
  #define _PP_1PAR_IDX_3(TF, P1, _0, _1, _2) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2)
  #define _PP_1PAR_IDX_4(TF, P1, _0, _1, _2, _3) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3)
  #define _PP_1PAR_IDX_5(TF, P1, _0, _1, _2, _3, _4) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4)
  #define _PP_1PAR_IDX_6(TF, P1, _0, _1, _2, _3, _4, _5) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5)
  #define _PP_1PAR_IDX_7(TF, P1, _0, _1, _2, _3, _4, _5, _6) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6)
  #define _PP_1PAR_IDX_8(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7)
  #define _PP_1PAR_IDX_9(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8)
  #define _PP_1PAR_IDX_10(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9)
  #define _PP_1PAR_IDX_11(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10)
  #define _PP_1PAR_IDX_12(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11)
  #define _PP_1PAR_IDX_13(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12)
  #define _PP_1PAR_IDX_14(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13)
  #define _PP_1PAR_IDX_15(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14)
  #define _PP_1PAR_IDX_16(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15)
  #define _PP_1PAR_IDX_17(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16)
  #define _PP_1PAR_IDX_18(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17)
  #define _PP_1PAR_IDX_19(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18)
  #define _PP_1PAR_IDX_20(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19)
  #define _PP_1PAR_IDX_21(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20)
  #define _PP_1PAR_IDX_22(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21)
  #define _PP_1PAR_IDX_23(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22)
  #define _PP_1PAR_IDX_24(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23)
  #define _PP_1PAR_IDX_25(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24)
  #define _PP_1PAR_IDX_26(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25)
  #define _PP_1PAR_IDX_27(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26)
  #define _PP_1PAR_IDX_28(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27)
  #define _PP_1PAR_IDX_29(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28)
  #define _PP_1PAR_IDX_30(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29)
  #define _PP_1PAR_IDX_31(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30)
  #define _PP_1PAR_IDX_32(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31)
  #define _PP_1PAR_IDX_33(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32)
  #define _PP_1PAR_IDX_34(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33)
  #define _PP_1PAR_IDX_35(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34)
  #define _PP_1PAR_IDX_36(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35)
  #define _PP_1PAR_IDX_37(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36)
  #define _PP_1PAR_IDX_38(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37)
  #define _PP_1PAR_IDX_39(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38)
  #define _PP_1PAR_IDX_40(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39)
  #define _PP_1PAR_IDX_41(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40)
  #define _PP_1PAR_IDX_42(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41)
  #define _PP_1PAR_IDX_43(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42)
  #define _PP_1PAR_IDX_44(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43)
  #define _PP_1PAR_IDX_45(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44)
  #define _PP_1PAR_IDX_46(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45)
  #define _PP_1PAR_IDX_47(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46)
  #define _PP_1PAR_IDX_48(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47)
  #define _PP_1PAR_IDX_49(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48)
  #define _PP_1PAR_IDX_50(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49)
  #define _PP_1PAR_IDX_51(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50)
  #define _PP_1PAR_IDX_52(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51)
  #define _PP_1PAR_IDX_53(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52)
  #define _PP_1PAR_IDX_54(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53)
  #define _PP_1PAR_IDX_55(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54)
  #define _PP_1PAR_IDX_56(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55)
  #define _PP_1PAR_IDX_57(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56)
  #define _PP_1PAR_IDX_58(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57)
  #define _PP_1PAR_IDX_59(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58)
  #define _PP_1PAR_IDX_60(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58) TF(P1, _59, 59)
  #define _PP_1PAR_IDX_61(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58) TF(P1, _59, 59) TF(P1, _60, 60)
  #define _PP_1PAR_IDX_62(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58) TF(P1, _59, 59) TF(P1, _60, 60) TF(P1, _61, 61)
  #define _PP_1PAR_IDX_63(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58) TF(P1, _59, 59) TF(P1, _60, 60) TF(P1, _61, 61) TF(P1, _62, 62)
  #define _PP_1PAR_IDX_64(TF, P1, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63) TF(P1, _0, 0) TF(P1, _1, 1) TF(P1, _2, 2) TF(P1, _3, 3) TF(P1, _4, 4) TF(P1, _5, 5) TF(P1, _6, 6) TF(P1, _7, 7) TF(P1, _8, 8) TF(P1, _9, 9) TF(P1, _10, 10) TF(P1, _11, 11) TF(P1, _12, 12) TF(P1, _13, 13) TF(P1, _14, 14) TF(P1, _15, 15) TF(P1, _16, 16) TF(P1, _17, 17) TF(P1, _18, 18) TF(P1, _19, 19) TF(P1, _20, 20) TF(P1, _21, 21) TF(P1, _22, 22) TF(P1, _23, 23) TF(P1, _24, 24) TF(P1, _25, 25) TF(P1, _26, 26) TF(P1, _27, 27) TF(P1, _28, 28) TF(P1, _29, 29) TF(P1, _30, 30) TF(P1, _31, 31) TF(P1, _32, 32) TF(P1, _33, 33) TF(P1, _34, 34) TF(P1, _35, 35) TF(P1, _36, 36) TF(P1, _37, 37) TF(P1, _38, 38) TF(P1, _39, 39) TF(P1, _40, 40) TF(P1, _41, 41) TF(P1, _42, 42) TF(P1, _43, 43) TF(P1, _44, 44) TF(P1, _45, 45) TF(P1, _46, 46) TF(P1, _47, 47) TF(P1, _48, 48) TF(P1, _49, 49) TF(P1, _50, 50) TF(P1, _51, 51) TF(P1, _52, 52) TF(P1, _53, 53) TF(P1, _54, 54) TF(P1, _55, 55) TF(P1, _56, 56) TF(P1, _57, 57) TF(P1, _58, 58) TF(P1, _59, 59) TF(P1, _60, 60) TF(P1, _61, 61) TF(P1, _62, 62) TF(P1, _63, 63)
#endif //PP_1PAR_EACH_IDX

#endif

#ifndef PP_2PAR_EACH_IDX
#ifndef PP_2PAR_EACH_IDX
  #define PP_2PAR_EACH_IDX(TF, P1, P2, ...) _PP_2PAR_EACH_IDX(TF, P1, P2, PP_NARG(__VA_ARGS__), __VA_ARGS__)
  #define _PP_2PAR_EACH_IDX(TF, P1, P2, N, ...) __PP_2PAR_EACH_IDX(TF, P1, P2, N, __VA_ARGS__)
  #define __PP_2PAR_EACH_IDX(TF, P1, P2, N, ...) _PP_2PAR_IDX_##N(TF, P1, P2, __VA_ARGS__)
  
  #define _PP_2PAR_IDX_0(TF, ARG, dummy)
  #define _PP_2PAR_IDX_1(TF, P1, P2, _0) TF(P1, P2, _0, 0)
  #define _PP_2PAR_IDX_2(TF, P1, P2, _0, _1) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1)
  #define _PP_2PAR_IDX_3(TF, P1, P2, _0, _1, _2) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2)
  #define _PP_2PAR_IDX_4(TF, P1, P2, _0, _1, _2, _3) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3)
  #define _PP_2PAR_IDX_5(TF, P1, P2, _0, _1, _2, _3, _4) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4)
  #define _PP_2PAR_IDX_6(TF, P1, P2, _0, _1, _2, _3, _4, _5) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5)
  #define _PP_2PAR_IDX_7(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6)
  #define _PP_2PAR_IDX_8(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7)
  #define _PP_2PAR_IDX_9(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8)
  #define _PP_2PAR_IDX_10(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9)
  #define _PP_2PAR_IDX_11(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10)
  #define _PP_2PAR_IDX_12(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11)
  #define _PP_2PAR_IDX_13(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12)
  #define _PP_2PAR_IDX_14(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13)
  #define _PP_2PAR_IDX_15(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14)
  #define _PP_2PAR_IDX_16(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15)
  #define _PP_2PAR_IDX_17(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16)
  #define _PP_2PAR_IDX_18(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17)
  #define _PP_2PAR_IDX_19(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18)
  #define _PP_2PAR_IDX_20(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19)
  #define _PP_2PAR_IDX_21(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20)
  #define _PP_2PAR_IDX_22(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21)
  #define _PP_2PAR_IDX_23(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22)
  #define _PP_2PAR_IDX_24(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23)
  #define _PP_2PAR_IDX_25(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24)
  #define _PP_2PAR_IDX_26(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25)
  #define _PP_2PAR_IDX_27(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26)
  #define _PP_2PAR_IDX_28(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27)
  #define _PP_2PAR_IDX_29(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28)
  #define _PP_2PAR_IDX_30(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29)
  #define _PP_2PAR_IDX_31(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30)
  #define _PP_2PAR_IDX_32(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31)
  #define _PP_2PAR_IDX_33(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32)
  #define _PP_2PAR_IDX_34(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33)
  #define _PP_2PAR_IDX_35(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34)
  #define _PP_2PAR_IDX_36(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35)
  #define _PP_2PAR_IDX_37(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36)
  #define _PP_2PAR_IDX_38(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37)
  #define _PP_2PAR_IDX_39(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38)
  #define _PP_2PAR_IDX_40(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39)
  #define _PP_2PAR_IDX_41(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40)
  #define _PP_2PAR_IDX_42(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41)
  #define _PP_2PAR_IDX_43(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42)
  #define _PP_2PAR_IDX_44(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43)
  #define _PP_2PAR_IDX_45(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44)
  #define _PP_2PAR_IDX_46(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45)
  #define _PP_2PAR_IDX_47(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46)
  #define _PP_2PAR_IDX_48(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47)
  #define _PP_2PAR_IDX_49(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48)
  #define _PP_2PAR_IDX_50(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49)
  #define _PP_2PAR_IDX_51(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50)
  #define _PP_2PAR_IDX_52(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51)
  #define _PP_2PAR_IDX_53(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52)
  #define _PP_2PAR_IDX_54(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53)
  #define _PP_2PAR_IDX_55(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54)
  #define _PP_2PAR_IDX_56(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55)
  #define _PP_2PAR_IDX_57(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56)
  #define _PP_2PAR_IDX_58(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57)
  #define _PP_2PAR_IDX_59(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58)
  #define _PP_2PAR_IDX_60(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58) TF(P1, P2, _59, 59)
  #define _PP_2PAR_IDX_61(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58) TF(P1, P2, _59, 59) TF(P1, P2, _60, 60)
  #define _PP_2PAR_IDX_62(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58) TF(P1, P2, _59, 59) TF(P1, P2, _60, 60) TF(P1, P2, _61, 61)
  #define _PP_2PAR_IDX_63(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58) TF(P1, P2, _59, 59) TF(P1, P2, _60, 60) TF(P1, P2, _61, 61) TF(P1, P2, _62, 62)
  #define _PP_2PAR_IDX_64(TF, P1, P2, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63) TF(P1, P2, _0, 0) TF(P1, P2, _1, 1) TF(P1, P2, _2, 2) TF(P1, P2, _3, 3) TF(P1, P2, _4, 4) TF(P1, P2, _5, 5) TF(P1, P2, _6, 6) TF(P1, P2, _7, 7) TF(P1, P2, _8, 8) TF(P1, P2, _9, 9) TF(P1, P2, _10, 10) TF(P1, P2, _11, 11) TF(P1, P2, _12, 12) TF(P1, P2, _13, 13) TF(P1, P2, _14, 14) TF(P1, P2, _15, 15) TF(P1, P2, _16, 16) TF(P1, P2, _17, 17) TF(P1, P2, _18, 18) TF(P1, P2, _19, 19) TF(P1, P2, _20, 20) TF(P1, P2, _21, 21) TF(P1, P2, _22, 22) TF(P1, P2, _23, 23) TF(P1, P2, _24, 24) TF(P1, P2, _25, 25) TF(P1, P2, _26, 26) TF(P1, P2, _27, 27) TF(P1, P2, _28, 28) TF(P1, P2, _29, 29) TF(P1, P2, _30, 30) TF(P1, P2, _31, 31) TF(P1, P2, _32, 32) TF(P1, P2, _33, 33) TF(P1, P2, _34, 34) TF(P1, P2, _35, 35) TF(P1, P2, _36, 36) TF(P1, P2, _37, 37) TF(P1, P2, _38, 38) TF(P1, P2, _39, 39) TF(P1, P2, _40, 40) TF(P1, P2, _41, 41) TF(P1, P2, _42, 42) TF(P1, P2, _43, 43) TF(P1, P2, _44, 44) TF(P1, P2, _45, 45) TF(P1, P2, _46, 46) TF(P1, P2, _47, 47) TF(P1, P2, _48, 48) TF(P1, P2, _49, 49) TF(P1, P2, _50, 50) TF(P1, P2, _51, 51) TF(P1, P2, _52, 52) TF(P1, P2, _53, 53) TF(P1, P2, _54, 54) TF(P1, P2, _55, 55) TF(P1, P2, _56, 56) TF(P1, P2, _57, 57) TF(P1, P2, _58, 58) TF(P1, P2, _59, 59) TF(P1, P2, _60, 60) TF(P1, P2, _61, 61) TF(P1, P2, _62, 62) TF(P1, P2, _63, 63)
#endif //PP_2PAR_EACH_IDX

#endif

/* DECLARE AND DEFINE FAKE FUNCTIONS - PLACE IN TEST FILES */

#define FAKE_VALUE_FUNC(...)     EXPAND(FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define FUNC_VALUE_(N,...)     EXPAND(FUNC_VALUE_N(N,__VA_ARGS__))

#define FUNC_VALUE_N(N,...)     EXPAND(FAKE_VALUE_FUNC ## N(__VA_ARGS__))


#define FAKE_VOID_FUNC(...)     EXPAND(FUNC_VOID_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define FUNC_VOID_(N,...)     EXPAND(FUNC_VOID_N(N,__VA_ARGS__))

#define FUNC_VOID_N(N,...)     EXPAND(FAKE_VOID_FUNC ## N(__VA_ARGS__))


#define FAKE_VALUE_FUNC_VARARG(...)     EXPAND(FUNC_VALUE_VARARG_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define FUNC_VALUE_VARARG_(N,...)     EXPAND(FUNC_VALUE_VARARG_N(N,__VA_ARGS__))

#define FUNC_VALUE_VARARG_N(N,...)     EXPAND(FAKE_VALUE_FUNC ## N ## _VARARG(__VA_ARGS__))


#define FAKE_VOID_FUNC_VARARG(...)     EXPAND(FUNC_VOID_VARARG_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define FUNC_VOID_VARARG_(N,...)     EXPAND(FUNC_VOID_VARARG_N(N,__VA_ARGS__))

#define FUNC_VOID_VARARG_N(N,...)     EXPAND(FAKE_VOID_FUNC ## N ## _VARARG(__VA_ARGS__))



/* DECLARE FAKE FUNCTIONS - PLACE IN HEADER FILES */

#define DECLARE_FAKE_VALUE_FUNC(...)     EXPAND(DECLARE_FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define DECLARE_FUNC_VALUE_(N,...)     EXPAND(DECLARE_FUNC_VALUE_N(N,__VA_ARGS__))

#define DECLARE_FUNC_VALUE_N(N,...)     EXPAND(DECLARE_FAKE_VALUE_FUNC ## N(__VA_ARGS__))


#define DECLARE_FAKE_VOID_FUNC(...)     EXPAND(DECLARE_FUNC_VOID_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define DECLARE_FUNC_VOID_(N,...)     EXPAND(DECLARE_FUNC_VOID_N(N,__VA_ARGS__))

#define DECLARE_FUNC_VOID_N(N,...)     EXPAND(DECLARE_FAKE_VOID_FUNC ## N(__VA_ARGS__))


#define DECLARE_FAKE_VALUE_FUNC_VARARG(...)     EXPAND(DECLARE_FUNC_VALUE_VARARG_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define DECLARE_FUNC_VALUE_VARARG_(N,...)     EXPAND(DECLARE_FUNC_VALUE_VARARG_N(N,__VA_ARGS__))

#define DECLARE_FUNC_VALUE_VARARG_N(N,...)     EXPAND(DECLARE_FAKE_VALUE_FUNC ## N ## _VARARG(__VA_ARGS__))


#define DECLARE_FAKE_VOID_FUNC_VARARG(...)     EXPAND(DECLARE_FUNC_VOID_VARARG_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define DECLARE_FUNC_VOID_VARARG_(N,...)     EXPAND(DECLARE_FUNC_VOID_VARARG_N(N,__VA_ARGS__))

#define DECLARE_FUNC_VOID_VARARG_N(N,...)     EXPAND(DECLARE_FAKE_VOID_FUNC ## N ## _VARARG(__VA_ARGS__))



/* DEFINE FAKE FUNCTIONS - PLACE IN SOURCE FILES */

#define DEFINE_FAKE_VALUE_FUNC(...)     EXPAND(DEFINE_FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define DEFINE_FUNC_VALUE_(N,...)     EXPAND(DEFINE_FUNC_VALUE_N(N,__VA_ARGS__))

#define DEFINE_FUNC_VALUE_N(N,...)     EXPAND(DEFINE_FAKE_VALUE_FUNC ## N(__VA_ARGS__))


#define DEFINE_FAKE_VOID_FUNC(...)     EXPAND(DEFINE_FUNC_VOID_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define DEFINE_FUNC_VOID_(N,...)     EXPAND(DEFINE_FUNC_VOID_N(N,__VA_ARGS__))

#define DEFINE_FUNC_VOID_N(N,...)     EXPAND(DEFINE_FAKE_VOID_FUNC ## N(__VA_ARGS__))


#define DEFINE_FAKE_VALUE_FUNC_VARARG(...)     EXPAND(DEFINE_FUNC_VALUE_VARARG_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__))

#define DEFINE_FUNC_VALUE_VARARG_(N,...)     EXPAND(DEFINE_FUNC_VALUE_VARARG_N(N,__VA_ARGS__))

#define DEFINE_FUNC_VALUE_VARARG_N(N,...)     EXPAND(DEFINE_FAKE_VALUE_FUNC ## N ## _VARARG(__VA_ARGS__))


#define DEFINE_FAKE_VOID_FUNC_VARARG(...)     EXPAND(DEFINE_FUNC_VOID_VARARG_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__))

#define DEFINE_FUNC_VOID_VARARG_(N,...)     EXPAND(DEFINE_FUNC_VOID_VARARG_N(N,__VA_ARGS__))

#define DEFINE_FUNC_VOID_VARARG_N(N,...)     EXPAND(DEFINE_FAKE_VOID_FUNC ## N ## _VARARG(__VA_ARGS__))



/* ASSERTIONS (Public API) */

#define FFF_ASSERT_CALLS(FN, COUNT) _FFF_ASSERT_EQ_MSG(COUNT, FFF_CALLS(FN), #FN " called incorrect number of times")

#define FFF_ASSERT_CALLED(FN)  FFF_ASSERT_CALLS(FN, 1);

#define FFF_ASSERT_NOT_CALLED(FN)  FFF_ASSERT_CALLS(FN, 0);

#define FFF_ASSERT(FN, ...) \
  FFF_ASSERT_CALLS(FN, 1); \
  PP_1PAR_EACH_IDX(_FFF_ASSERT_ARG, FN, __VA_ARGS__)

#define FFF_ASSERT_NTH(FN, CALL_IDX, ...) \
  _FFF_ASSERT_TRUE_MSG(CALL_IDX > 0, "Invalid call index -- expected >0, got " #CALL_IDX); \
  _FFF_ASSERT_TRUE_MSG(FFF_CALLS(FN) >= CALL_IDX, #FN " not called " #CALL_IDX " times"); \
  PP_2PAR_EACH_IDX(_FFF_ASSERT_HISTORICAL_ARG, FN, CALL_IDX, __VA_ARGS__)

#define FFF_ASSERT_LAST(FN, ...) FFF_ASSERT_NTH(FN, FFF_CALLS(FN), __VA_ARGS__)

#define FFF_ASSERT_ANY(FN, ...) _FFF_ASSERT_TRUE_MSG(_FFF_VERIFY_ANY_CALL(FN, __VA_ARGS__), "No calls made to '"#FN"("#__VA_ARGS__")'")

#define FFF_ASSERT_NONE(FN, ...)  _FFF_ASSERT_FALSE_MSG(_FFF_VERIFY_ANY_CALL(FN, __VA_ARGS__), "At least one call made to '"#FN"("#__VA_ARGS__")'")

/* ASSERTIONS (Internal) */

#ifndef _FFF_ASSERT_EQ_MSG
#include <assert.h>
#include <stdio.h>
#define _FFF_ASSERT_EQ_MSG(expected, actual, message) do { \
        if(expected!=actual) {printf(message);} \
        assert(expected==actual); \
} while(0)
#endif

#ifndef _FFF_ASSERT_EQ
#define _FFF_ASSERT_EQ(expected, actual) _FFF_ASSERT_EQ_MSG(expected, actual, "Expected " #expected " / Got " #actual)
#endif

#define _FFF_ASSERT_TRUE_MSG(COND, MSG) _FFF_ASSERT_EQ_MSG(((COND)>0), 1, MSG)
#define _FFF_ASSERT_FALSE_MSG(COND, MSG) _FFF_ASSERT_EQ_MSG((COND), 0, MSG)

#define _FFF_ASSERT_ARG(FN, VAL, ARG_IDX) _FFF_ASSERT_EQ_MSG(VAL, FFF_LAST_ARG_VAL(FN, ARG_IDX), #FN" parameter value mismatch at ARG"#ARG_IDX);

#define _FFF_ASSERT_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX) _FFF_ASSERT_EQ_MSG(VAL, FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX),  #FN", call "#CALL_IDX": value mismatch for arg"#ARG_IDX);

#endif /* FAKE_FUNCTIONS */
