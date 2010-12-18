#ifndef FAKE_FUNCTIONS
#define FAKE_FUNCTIONS

#ifdef __cplusplus
#include <vector>
typedef void (*void_fptr)();
std::vector<void_fptr> reset_functions;
void RESET_FAKES()
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
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(){ \
        FUNCNAME##_call_count++; \
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
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 2 parameters*/
#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 3 parameters*/
#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 4 parameters*/
#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 5 parameters*/
#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 6 parameters*/
#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 7 parameters*/
#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 8 parameters*/
#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a void function with 9 parameters*/
#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static int FUNCNAME##_call_count = 0; \
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
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 0 parameters*/
#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
extern "C"{ \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(){ \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 1 parameters*/
#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 2 parameters*/
#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 3 parameters*/
#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 4 parameters*/
#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 5 parameters*/
#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 6 parameters*/
#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 7 parameters*/
#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 8 parameters*/
#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function returning a value with 9 parameters*/
#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
extern "C"{ \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
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
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \
} \
STATIC_INIT(FUNCNAME) \


/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \

#else  /* ansi c */

/* Defining a void function with 0 parameters*/
#define FAKE_VOID_FUNC0(FUNCNAME) \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(){ \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 1 parameters*/
#define FAKE_VOID_FUNC1(FUNCNAME, ARG0_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 2 parameters*/
#define FAKE_VOID_FUNC2(FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 3 parameters*/
#define FAKE_VOID_FUNC3(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 4 parameters*/
#define FAKE_VOID_FUNC4(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 5 parameters*/
#define FAKE_VOID_FUNC5(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 6 parameters*/
#define FAKE_VOID_FUNC6(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 7 parameters*/
#define FAKE_VOID_FUNC7(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 8 parameters*/
#define FAKE_VOID_FUNC8(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static int FUNCNAME##_call_count = 0; \
    void FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a void function with 9 parameters*/
#define FAKE_VOID_FUNC9(FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static int FUNCNAME##_call_count = 0; \
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
        FUNCNAME##_call_count++; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 0 parameters*/
#define FAKE_VALUE_FUNC0(RETURN_TYPE, FUNCNAME) \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(){ \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 1 parameters*/
#define FAKE_VALUE_FUNC1(RETURN_TYPE, FUNCNAME, ARG0_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 2 parameters*/
#define FAKE_VALUE_FUNC2(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 3 parameters*/
#define FAKE_VALUE_FUNC3(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 4 parameters*/
#define FAKE_VALUE_FUNC4(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 5 parameters*/
#define FAKE_VALUE_FUNC5(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 6 parameters*/
#define FAKE_VALUE_FUNC6(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 7 parameters*/
#define FAKE_VALUE_FUNC7(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 8 parameters*/
#define FAKE_VALUE_FUNC8(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
    RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2, ARG3_TYPE arg3, ARG4_TYPE arg4, ARG5_TYPE arg5, ARG6_TYPE arg6, ARG7_TYPE arg7){ \
        FUNCNAME##_arg0_val = arg0; \
        FUNCNAME##_arg1_val = arg1; \
        FUNCNAME##_arg2_val = arg2; \
        FUNCNAME##_arg3_val = arg3; \
        FUNCNAME##_arg4_val = arg4; \
        FUNCNAME##_arg5_val = arg5; \
        FUNCNAME##_arg6_val = arg6; \
        FUNCNAME##_arg7_val = arg7; \
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function returning a value with 9 parameters*/
#define FAKE_VALUE_FUNC9(RETURN_TYPE, FUNCNAME, ARG0_TYPE, ARG1_TYPE, ARG2_TYPE, ARG3_TYPE, ARG4_TYPE, ARG5_TYPE, ARG6_TYPE, ARG7_TYPE, ARG8_TYPE) \
    static ARG0_TYPE FUNCNAME##_arg0_val; \
    static ARG1_TYPE FUNCNAME##_arg1_val; \
    static ARG2_TYPE FUNCNAME##_arg2_val; \
    static ARG3_TYPE FUNCNAME##_arg3_val; \
    static ARG4_TYPE FUNCNAME##_arg4_val; \
    static ARG5_TYPE FUNCNAME##_arg5_val; \
    static ARG6_TYPE FUNCNAME##_arg6_val; \
    static ARG7_TYPE FUNCNAME##_arg7_val; \
    static ARG8_TYPE FUNCNAME##_arg8_val; \
    static RETURN_TYPE FUNCNAME##_return_val; \
    static int FUNCNAME##_call_count = 0; \
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
        FUNCNAME##_call_count++; \
        return FUNCNAME##_return_val; \
	} \
    void FUNCNAME##_reset(){ \
        FUNCNAME##_arg0_val = (ARG0_TYPE) 0; \
        FUNCNAME##_arg1_val = (ARG1_TYPE) 0; \
        FUNCNAME##_arg2_val = (ARG2_TYPE) 0; \
        FUNCNAME##_arg3_val = (ARG3_TYPE) 0; \
        FUNCNAME##_arg4_val = (ARG4_TYPE) 0; \
        FUNCNAME##_arg5_val = (ARG5_TYPE) 0; \
        FUNCNAME##_arg6_val = (ARG6_TYPE) 0; \
        FUNCNAME##_arg7_val = (ARG7_TYPE) 0; \
        FUNCNAME##_arg8_val = (ARG8_TYPE) 0; \
        FUNCNAME##_call_count = 0; \
    } \


/* Defining a function to reset a fake function */
#define RESET_FAKE(FUNCNAME) { \
    FUNCNAME##_reset(); \
} \

#endif  /* cpp/ansi c */

#endif // FAKE_FUNCTIONS
