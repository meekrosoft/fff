#ifndef FAKE_FUNCTIONS
#define FAKE_FUNCTIONS

#define FFF_MAX_ARGS (10u)
#ifndef FFF_ARG_HISTORY_LEN
    #define FFF_ARG_HISTORY_LEN (50u)
#endif
#ifndef FFF_CALL_HISTORY_LEN
  #define FFF_CALL_HISTORY_LEN (50u)
#endif
/* -- INTERNAL HELPER MACROS -- */
#define SET_RETURN_SEQ(FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \
                        FUNCNAME##_fake.return_val_seq = ARRAY_POINTER; \
                        FUNCNAME##_fake.return_val_seq_len = ARRAY_LEN;

/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \


#define DECLARE_ARG(type, n, FUNCNAME) \
    type arg##n##_val; \
    type arg##n##_history[FFF_ARG_HISTORY_LEN];

#define DECLARE_ALL_FUNC_COMMON \
    unsigned int call_count; \
    unsigned int arg_history_len;\
    unsigned int arg_histories_dropped; \

#define SAVE_ARG(FUNCNAME, n) \
    FUNCNAME##_fake.arg##n##_val = arg##n

#define ROOM_FOR_MORE_HISTORY(FUNCNAME) \
  FUNCNAME##_fake.call_count < FFF_ARG_HISTORY_LEN

#define SAVE_ARG_HISTORY(FUNCNAME, ARGN) \
    FUNCNAME##_fake.arg##ARGN##_history[FUNCNAME##_fake.call_count] = arg##ARGN

#define HISTORY_DROPPED(FUNCNAME) \
    FUNCNAME##_fake.arg_histories_dropped++

#define DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
    RETURN_TYPE return_val; \
    int return_val_seq_len; \
    int return_val_seq_idx; \
    RETURN_TYPE * return_val_seq; \

#define INCREMENT_CALL_COUNT(FUNCNAME) \
    FUNCNAME##_fake.call_count++

#define RETURN_FAKE_RESULT(FUNCNAME) \
    if (FUNCNAME##_fake.return_val_seq_len){ /* then its a sequence */ \
        if(FUNCNAME##_fake.return_val_seq_idx < FUNCNAME##_fake.return_val_seq_len) { \
            return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx++]; \
        } \
        return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1]; /* return last element */ \
    } \
    return FUNCNAME##_fake.return_val; \

#ifdef __cplusplus
    #define EXTERN_C extern "C"{
    #define END_EXTERN_C } 
#else  /* ansi c */
    #define EXTERN_C 
    #define END_EXTERN_C 
#endif  /* cpp/ansi c */

#define DEFINE_RESET_FUNCTION(FUNCNAME) \
    void FUNCNAME##_reset(){ \
        memset(&FUNCNAME##_fake, 0, sizeof(FUNCNAME##_fake)); \
        FUNCNAME##_fake.arg_history_len = FFF_ARG_HISTORY_LEN;\
    }
/* -- END INTERNAL HELPER MACROS -- */

typedef struct { 
    void * call_history[FFF_CALL_HISTORY_LEN];
    unsigned int call_history_idx;
} fff_globals_t;

EXTERN_C \
extern fff_globals_t fff;
END_EXTERN_C \

#define DEFINE_FFF_GLOBALS \
    EXTERN_C \
        fff_globals_t fff; \
    END_EXTERN_C

#define FFF_RESET_HISTORY() fff.call_history_idx = 0;

#define REGISTER_CALL(function) \
   if(fff.call_history_idx < FFF_CALL_HISTORY_LEN) \
       fff.call_history[fff.call_history_idx++] = (void *)function;

#define DECLARE_FAKE_VOID_FUNC0(FUNCNAME) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC0(FUNCNAME) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(){ \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC0(FUNCNAME) \
    DECLARE_FAKE_VOID_FUNC0(FUNCNAME) \
    DEFINE_FAKE_VOID_FUNC0(FUNCNAME) \
    

#define DECLARE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0){ \
            SAVE_ARG(FUNCNAME, 0); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    DECLARE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    DEFINE_FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DECLARE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DEFINE_FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DECLARE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DEFINE_FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DECLARE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DEFINE_FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DECLARE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DEFINE_FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
            DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DECLARE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DEFINE_FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
            DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
            DECLARE_ARG(ARG6_TYPE, 6, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DECLARE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DEFINE_FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    EXTERN_C \
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
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            SAVE_ARG(FUNCNAME, 7); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
                SAVE_ARG_HISTORY(FUNCNAME, 7); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DECLARE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DEFINE_FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    

#define DECLARE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    EXTERN_C \
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
            void(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            SAVE_ARG(FUNCNAME, 7); \
            SAVE_ARG(FUNCNAME, 8); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
                SAVE_ARG_HISTORY(FUNCNAME, 7); \
                SAVE_ARG_HISTORY(FUNCNAME, 8); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake)  FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DECLARE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DEFINE_FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(){ \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    DECLARE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    DEFINE_FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    

#define DECLARE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0){ \
            SAVE_ARG(FUNCNAME, 0); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    DECLARE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    DEFINE_FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DECLARE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    DEFINE_FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DECLARE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    DEFINE_FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DECLARE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    DEFINE_FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DECLARE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    DEFINE_FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    EXTERN_C \
        typedef struct FUNCNAME##_Fake { \
            DECLARE_ARG(ARG0_TYPE, 0, FUNCNAME) \
            DECLARE_ARG(ARG1_TYPE, 1, FUNCNAME) \
            DECLARE_ARG(ARG2_TYPE, 2, FUNCNAME) \
            DECLARE_ARG(ARG3_TYPE, 3, FUNCNAME) \
            DECLARE_ARG(ARG4_TYPE, 4, FUNCNAME) \
            DECLARE_ARG(ARG5_TYPE, 5, FUNCNAME) \
            DECLARE_ALL_FUNC_COMMON \
            DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE) \
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DECLARE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    DEFINE_FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    EXTERN_C \
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
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DECLARE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    DEFINE_FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    EXTERN_C \
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
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            SAVE_ARG(FUNCNAME, 7); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
                SAVE_ARG_HISTORY(FUNCNAME, 7); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DECLARE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    DEFINE_FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    

#define DECLARE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    EXTERN_C \
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
            RETURN_TYPE(*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8); \
        } FUNCNAME##_Fake;\
        extern FUNCNAME##_Fake FUNCNAME##_fake;\
        void FUNCNAME##_reset(); \
    END_EXTERN_C \

#define DEFINE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    EXTERN_C \
        FUNCNAME##_Fake FUNCNAME##_fake;\
        RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7, ARG8_TYPE arg8){ \
            SAVE_ARG(FUNCNAME, 0); \
            SAVE_ARG(FUNCNAME, 1); \
            SAVE_ARG(FUNCNAME, 2); \
            SAVE_ARG(FUNCNAME, 3); \
            SAVE_ARG(FUNCNAME, 4); \
            SAVE_ARG(FUNCNAME, 5); \
            SAVE_ARG(FUNCNAME, 6); \
            SAVE_ARG(FUNCNAME, 7); \
            SAVE_ARG(FUNCNAME, 8); \
            if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\
                SAVE_ARG_HISTORY(FUNCNAME, 0); \
                SAVE_ARG_HISTORY(FUNCNAME, 1); \
                SAVE_ARG_HISTORY(FUNCNAME, 2); \
                SAVE_ARG_HISTORY(FUNCNAME, 3); \
                SAVE_ARG_HISTORY(FUNCNAME, 4); \
                SAVE_ARG_HISTORY(FUNCNAME, 5); \
                SAVE_ARG_HISTORY(FUNCNAME, 6); \
                SAVE_ARG_HISTORY(FUNCNAME, 7); \
                SAVE_ARG_HISTORY(FUNCNAME, 8); \
            }\
            else{\
                HISTORY_DROPPED(FUNCNAME);\
            }\
            INCREMENT_CALL_COUNT(FUNCNAME); \
            REGISTER_CALL(FUNCNAME); \
            if (FUNCNAME##_fake.custom_fake) return FUNCNAME##_fake.custom_fake(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
            RETURN_FAKE_RESULT(FUNCNAME)  \
        } \
        DEFINE_RESET_FUNCTION(FUNCNAME) \
    END_EXTERN_C \

#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DECLARE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    DEFINE_FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    

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
