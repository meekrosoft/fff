# fakegen.rb
# A simple code generator to create some C macros for defining test fake functions

require_relative './pp_iterators.rb'

$cpp_output = true
$MAX_ARGS = 20 # Max faked function argument count
$MAX_ITER = 64 # Max args for iteration and arg counting macros
$DEFAULT_ARG_HISTORY = 50
$MAX_CALL_HISTORY = 50

def include_dependencies
  putd "#include <stdarg.h>"
  putd "#include <string.h> /* For memset and memcpy */"
  puts
end

def output_constants
  putd "#define FFF_MAX_ARGS (#{$MAX_ARGS}u)"
  putd "#ifndef FFF_ARG_HISTORY_LEN"
  indent {
    putd "#define FFF_ARG_HISTORY_LEN (#{$DEFAULT_ARG_HISTORY}u)"
  }
  putd "#endif"
  putd "#ifndef FFF_CALL_HISTORY_LEN"
  indent {
    putd "#define FFF_CALL_HISTORY_LEN (#{$MAX_CALL_HISTORY}u)"
  }
  putd "#endif"
  putd "#ifndef FFF_GCC_FUNCTION_ATTRIBUTES"
  indent {
     putd "#define FFF_GCC_FUNCTION_ATTRIBUTES"
  }
  putd "#endif"

end


# ------  Helper macros to use internally ------ #
def output_internal_helper_macros
  putd "/* -- INTERNAL HELPER MACROS -- */"

  define_return_sequence_helpers
  define_custom_fake_sequence_helper
  define_reset_fake_macro
  define_declare_arg_helper
  define_declare_all_func_common_helper
  define_declare_return_value_history
  define_save_arg_helper
  define_room_for_more_history
  define_save_ret_history_helper
  define_save_arg_history_helper
  define_history_dropped_helper
  define_value_function_variables_helper
  define_custom_fake_seq_variables_helper
  define_increment_call_count_helper
  define_return_fake_result_helper
  define_extern_c_helper
  define_reset_fake_helper
  define_fake_helpers
  define_call_verification_helpers

  putd "/* -- END INTERNAL HELPER MACROS -- */"
  puts
end

def define_return_sequence_helpers
  putd_backslash "#define SET_RETURN_SEQ(FUNCNAME, ARRAY_POINTER, ARRAY_LEN)"
  indent {
    putd_backslash "FUNCNAME##_fake.return_val_seq = ARRAY_POINTER;"
    putd "FUNCNAME##_fake.return_val_seq_len = ARRAY_LEN;"
  }
  # The elaborate construction of SET_RETURN allows it to be prefixed with `static`
  # in case you want to initialise a sequence outside the test body (e.g. in a fixture
  # setup method or whatever)
  putd <<-'EOT'

/* FFF_RETURN (begin) */
#define FFF_RETURN(...)                                             \
  _FFF_SEQ_DEF(__VA_ARGS__); if(PP_NARG(__VA_ARGS__) == 2) {FFF_RETURN_VAL(__VA_ARGS__, 0);} else {_FFF_RETURN_SEQ(HEAD(__VA_ARGS__));}
#ifdef _MSC_VER
//MSVC decltype ~= typeof, but only in c++ mode....
#define _FFF_RET_TYPE(FN) decltype(FFF_RET(FN))
#else
#define _FFF_RET_TYPE(FN) __typeof__(FFF_RET(FN))
#endif
#define _FFF_SEQ(FN) CAT(FN, __LINE__)
#define _FFF_SEQ_DEF(FN, ...) _FFF_RET_TYPE(FN) _FFF_SEQ(FN)[] = {__VA_ARGS__}
#define _FFF_SEQ_LEN(FN) (sizeof(_FFF_SEQ(FN))/sizeof(FFF_RET(FN)))
#define FFF_RETURN_VAL(FN, ...) FFF_RET(FN) = HEAD(__VA_ARGS__)
#define _FFF_RETURN_SEQ(FN) SET_RETURN_SEQ(FN, _FFF_SEQ(FN), _FFF_SEQ_LEN(FN)); \
    _FFF_ASSERT_TRUE_MSG(FFF(FN).return_val_seq_len>0, "No return values specified")
/* FFF_RETURN (end) */

EOT
end

def define_custom_fake_sequence_helper
  putd_backslash "#define SET_CUSTOM_FAKE_SEQ(FUNCNAME, ARRAY_POINTER, ARRAY_LEN)"
  indent {
    putd_backslash "FUNCNAME##_fake.custom_fake_seq = ARRAY_POINTER;"
    putd "FUNCNAME##_fake.custom_fake_seq_len = ARRAY_LEN;"
  }
end

def define_reset_fake_macro
  puts
  putd "/* Defining a function to reset a fake function */"
  putd_backslash "#define RESET_FAKE(FUNCNAME) {"
  indent {
    putd_backslash "FUNCNAME##_reset();"
  }
  putd_backslash "}"
  puts
end

def define_declare_arg_helper
  puts
  putd_backslash "#define DECLARE_ARG(type, n, FUNCNAME)"
  indent {
    putd_backslash "type arg##n##_val;"
    putd "type arg##n##_history[FFF_ARG_HISTORY_LEN];"
  }
end

def define_declare_all_func_common_helper
  puts
  putd_backslash "#define DECLARE_ALL_FUNC_COMMON"
  indent {
    putd_backslash "unsigned int call_count;"
    putd_backslash "unsigned int arg_history_len;"
    putd_backslash "unsigned int arg_histories_dropped;"
  }
end

def define_declare_return_value_history
  putd ""
  putd_backslash "#define DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE)"
  indent {
    putd "RETURN_TYPE return_val_history[FFF_ARG_HISTORY_LEN];"
  }
end

def define_save_arg_helper
  puts
  putd_backslash "#define SAVE_ARG(FUNCNAME, n)"
  indent {
    putd "memcpy((void*)&FUNCNAME##_fake.arg##n##_val, (void*)&arg##n, sizeof(arg##n));"
  }
end

def define_save_ret_history_helper
  putd ""
  putd_backslash "#define SAVE_RET_HISTORY(FUNCNAME, RETVAL)"
  indent {
    putd_backslash "if ((FUNCNAME##_fake.call_count - 1) < FFF_ARG_HISTORY_LEN)"
    indent {
      putd_backslash "memcpy((void *)&FUNCNAME##_fake.return_val_history[FUNCNAME##_fake.call_count - 1], (const void *) &RETVAL, sizeof(RETVAL));"
    }
  }
end

def define_room_for_more_history
  puts
  putd_backslash "#define ROOM_FOR_MORE_HISTORY(FUNCNAME)"
  indent {
    putd "FUNCNAME##_fake.call_count < FFF_ARG_HISTORY_LEN"
  }
end

def define_save_arg_history_helper
  puts
  putd_backslash "#define SAVE_ARG_HISTORY(FUNCNAME, ARGN)"
  indent {
    putd "memcpy((void*)&FUNCNAME##_fake.arg##ARGN##_history[FUNCNAME##_fake.call_count], (void*)&arg##ARGN, sizeof(arg##ARGN));"
  }
end

def define_history_dropped_helper
  puts
  putd_backslash "#define HISTORY_DROPPED(FUNCNAME)"
  indent {
    putd "FUNCNAME##_fake.arg_histories_dropped++"
  }
end

def define_value_function_variables_helper
  puts
  putd_backslash "#define DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE)"
  indent {
    putd_backslash "RETURN_TYPE return_val;"
    putd_backslash "int return_val_seq_len;"
    putd_backslash "int return_val_seq_idx;"
    putd_backslash "RETURN_TYPE * return_val_seq;"
  }
end

def define_custom_fake_seq_variables_helper
  puts
  putd_backslash "#define DECLARE_CUSTOM_FAKE_SEQ_VARIABLES"
  indent {
    putd_backslash "int custom_fake_seq_len;"
    putd_backslash "int custom_fake_seq_idx;"
  }
end

def define_increment_call_count_helper
  puts
  putd_backslash "#define INCREMENT_CALL_COUNT(FUNCNAME)"
  indent {
    putd "FUNCNAME##_fake.call_count++"
  }
end

def define_return_fake_result_helper
  puts
  putd_backslash "#define RETURN_FAKE_RESULT(FUNCNAME)"
  indent {
    putd_backslash "if (FUNCNAME##_fake.return_val_seq_len){ /* then its a sequence */"
    indent {
      putd_backslash "if(FUNCNAME##_fake.return_val_seq_idx < FUNCNAME##_fake.return_val_seq_len) {"
      indent {
        putd_backslash "SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx])"
        putd_backslash "return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx++];"
      }
      putd_backslash "}"
      putd_backslash "SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1])"
      putd_backslash "return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1]; /* return last element */"
    }
    putd_backslash "}"
    putd_backslash "SAVE_RET_HISTORY(FUNCNAME, FUNCNAME##_fake.return_val)"
    putd_backslash "return FUNCNAME##_fake.return_val;"
  }
end

def define_extern_c_helper
  puts
  putd "#ifdef __cplusplus"
  indent {
    putd "#define FFF_EXTERN_C extern \"C\"{"
    putd "#define FFF_END_EXTERN_C } "
  }
  putd "#else  /* ansi c */"
  indent {
    putd "#define FFF_EXTERN_C "
    putd "#define FFF_END_EXTERN_C "
  }
  putd "#endif  /* cpp/ansi c */"
end

def define_reset_fake_helper
  puts
  putd_backslash "#define DEFINE_RESET_FUNCTION(FUNCNAME)"
  indent {
    putd_backslash "void FUNCNAME##_reset(void){"
    indent {
      putd_backslash "memset(&FUNCNAME##_fake, 0, sizeof(FUNCNAME##_fake));"
      putd_backslash "FUNCNAME##_fake.arg_history_len = FFF_ARG_HISTORY_LEN;"
    }
    putd "}"
  }
end

def define_fake_helpers
  putd <<-EOH
#define FFF(FN) FN##_fake
#define FFF_CALLS(FN) FFF(FN).call_count
#define FFF_LAST_ARG_VAL(FN, ARG_IDX) FFF(FN).arg##ARG_IDX##_val
#define FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX) FFF(FN).arg##ARG_IDX##_history[CALL_IDX-1]
#define FFF_RET(FN) FFF(FN).return_val
  EOH
end

def define_call_verification_helpers
  putd <<-EOH
#define _FFF_VERIFY_ARG(FN, VAL, ARG_IDX) VAL == FFF_LAST_ARG_VAL(FN, ARG_IDX)
#define _FFF_VERIFY_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX) (VAL == FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX))
#define _FFF_AND_VERIFY_NTH_CALL_ARG(FN, CALL_IDX, VAL, ARG_IDX) && _FFF_VERIFY_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX)
#define _FFF_VERIFY_NTH_CALL(FN, CALL_IDX, ...)                         \
  ( (CALL_IDX > 0) && (CALL_IDX <= FFF_CALLS(FN)) PP_2PAR_EACH_IDX(_FFF_AND_VERIFY_NTH_CALL_ARG, FN, CALL_IDX, __VA_ARGS__) )

//This uses GCC compound statement expression:
//https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
#define _FFF_VERIFY_ANY_CALL(FN, ...) ({                              \
      int verified = 0;                                               \
      int call_idx = FFF_CALLS(FN);                                  \
      while(call_idx && !verified) {                                  \
        verified |= _FFF_VERIFY_NTH_CALL(FN, call_idx, __VA_ARGS__);  \
        call_idx--;                                                   \
      }                                                               \
      verified;                                                       \
    })

  EOH
end
# ------  End Helper macros ------ #

#fakegen helpers to print at levels of indentation
$current_depth = 0
def putd(str)
  $current_depth.times {|not_used| print " "}
  puts str
end

#multiline putd which adds a \ at the end of the generated macro
def putd_backslash str
  putd(str + " \\")
end

def pushd
  $current_depth = $current_depth + 4
end

def popd
  $current_depth = $current_depth - 4
end

def indent
  pushd
    yield
  popd
end

def output_macro(arg_count, has_varargs, has_calling_conventions, is_value_function)

  vararg_name = has_varargs ? "_VARARG" : ""
  fake_macro_name = is_value_function ? "FAKE_VALUE_FUNC#{arg_count}#{vararg_name}" : "FAKE_VOID_FUNC#{arg_count}#{vararg_name}"
  declare_macro_name = "DECLARE_#{fake_macro_name}"
  define_macro_name = "DEFINE_#{fake_macro_name}"
  saved_arg_count = arg_count - (has_varargs ? 1 : 0)
  return_type = is_value_function ? "RETURN_TYPE" : ""

  puts
  output_macro_header(declare_macro_name, saved_arg_count, has_varargs, has_calling_conventions, return_type)
  indent {
    output_variables(saved_arg_count, has_varargs, has_calling_conventions, is_value_function)
  }

  puts
  output_macro_header(define_macro_name, saved_arg_count, has_varargs, has_calling_conventions, return_type)
  indent {
    putd_backslash "FUNCNAME##_Fake FUNCNAME##_fake;"
    putd_backslash function_signature(saved_arg_count, has_varargs, has_calling_conventions, is_value_function) + "{"
    indent {
      output_function_body(saved_arg_count, has_varargs, is_value_function)
    }
    putd_backslash "}"
    putd_backslash "DEFINE_RESET_FUNCTION(FUNCNAME)"
  }

  puts

  output_macro_header(fake_macro_name, saved_arg_count, has_varargs, has_calling_conventions, return_type)
  indent {
    putd macro_signature_for(declare_macro_name, saved_arg_count, has_varargs, has_calling_conventions, return_type)
    putd macro_signature_for(define_macro_name, saved_arg_count, has_varargs, has_calling_conventions, return_type)
    puts
  }
end

def output_macro_header(macro_name, arg_count, has_varargs, has_calling_conventions, return_type)
  output_macro_name(macro_name, arg_count, has_varargs, has_calling_conventions, return_type)
end

# #define #macro_name(RETURN_TYPE, FUNCNAME, ARG0,...)
def output_macro_name(macro_name, arg_count, has_varargs, has_calling_conventions, return_type)
  putd "#define " + macro_signature_for(macro_name, arg_count, has_varargs, has_calling_conventions, return_type)
end

# #macro_name(RETURN_TYPE, FUNCNAME, ARG0,...) \
def macro_signature_for(macro_name, arg_count, has_varargs, has_calling_conventions, return_type)
  parameter_list = "#{macro_name}("
  if return_type != ""
    parameter_list += return_type
    parameter_list += ", "
  end
  parameter_list += "CALLING_CONVENTION, " if (has_calling_conventions)
  parameter_list += "FUNCNAME"

  arg_count.times { |i| parameter_list += ", ARG#{i}_TYPE" }

  parameter_list += ", ..." if has_varargs

  parameter_list +=  ") \\"

  parameter_list
end

def output_variables(arg_count, has_varargs, has_calling_conventions, is_value_function)
  in_struct{
    arg_count.times { |argN|
      putd_backslash "DECLARE_ARG(ARG#{argN}_TYPE, #{argN}, FUNCNAME)"
    }
    putd_backslash "DECLARE_ALL_FUNC_COMMON"
    putd_backslash "DECLARE_VALUE_FUNCTION_VARIABLES(RETURN_TYPE)" unless not is_value_function
    putd_backslash "DECLARE_RETURN_VALUE_HISTORY(RETURN_TYPE)" unless not is_value_function
    putd_backslash "DECLARE_CUSTOM_FAKE_SEQ_VARIABLES"
    output_custom_function_signature(arg_count, has_varargs, has_calling_conventions, is_value_function)
    output_custom_function_array(arg_count, has_varargs, has_calling_conventions, is_value_function)
  }
  putd_backslash "extern FUNCNAME##_Fake FUNCNAME##_fake;"
  putd_backslash "void FUNCNAME##_reset(void);"
  putd_backslash function_signature(arg_count, has_varargs, has_calling_conventions, is_value_function) + ";"
end

#example: ARG0_TYPE arg0, ARG1_TYPE arg1
def arg_val_list(args_count)
  return "void" if (args_count == 0)
  arguments = []
  args_count.times { |i| arguments << "ARG#{i}_TYPE arg#{i}" }
  arguments.join(", ")
end

#example: arg0, arg1
def arg_list(args_count)
  arguments = []
  args_count.times { |i| arguments << "arg#{i}" }
  arguments.join(", ")
end

# RETURN_TYPE (*custom_fake)(ARG0_TYPE arg0);\
# OR
# RETURN_TYPE (CALLING_CONVENTION *custom_fake)(ARG0_TYPE arg0);\
#
# void (*custom_fake)(ARG0_TYPE arg0, ARG1_TYPE arg1, ARG2_TYPE arg2);\
def output_custom_function_signature(arg_count, has_varargs, has_calling_conventions, is_value_function)
  return_type = is_value_function ? "RETURN_TYPE" : "void"
  ap_list = has_varargs ? ", va_list ap" : ""
  signature = has_calling_conventions ? "(CALLING_CONVENTION *custom_fake)" : "(*custom_fake)"
  signature += "(#{arg_val_list(arg_count)}#{ap_list});"
  putd_backslash return_type + signature
end

def output_custom_function_array(arg_count, has_varargs, has_calling_conventions, is_value_function)
  return_type = is_value_function ? "RETURN_TYPE" : "void"
  ap_list = has_varargs ? ", va_list ap" : ""
  custom_array = has_calling_conventions ? "(CALLING_CONVENTION **custom_fake_seq)" : "(**custom_fake_seq)"
  custom_array += "(#{arg_val_list(arg_count)}#{ap_list});"
  putd_backslash return_type + custom_array
end

# example: RETURN_TYPE FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1)
# OR
# RETURN_TYPE CALLING_CONVENTION FUNCNAME(ARG0_TYPE arg0, ARG1_TYPE arg1)
def function_signature(arg_count, has_varargs, has_calling_conventions, is_value_function)
  return_type = is_value_function ? "RETURN_TYPE" : "void"
  varargs = has_varargs ? ", ..." : ""
  calling_conventions = has_calling_conventions ?
    "#{return_type} FFF_GCC_FUNCTION_ATTRIBUTES CALLING_CONVENTION FUNCNAME(#{arg_val_list(arg_count)}#{varargs})" :
    "#{return_type} FFF_GCC_FUNCTION_ATTRIBUTES FUNCNAME(#{arg_val_list(arg_count)}#{varargs})"
end

def output_function_body(arg_count, has_varargs, is_value_function)
  arg_count.times { |i| putd_backslash "SAVE_ARG(FUNCNAME, #{i});" }
  putd_backslash "if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){"
  indent {
    arg_count.times { |i| putd_backslash "SAVE_ARG_HISTORY(FUNCNAME, #{i});" }
  }
  putd_backslash "}"
  putd_backslash "else{"
  indent {
    putd_backslash "HISTORY_DROPPED(FUNCNAME);"
  }
  putd_backslash "}"
  putd_backslash "INCREMENT_CALL_COUNT(FUNCNAME);"
  putd_backslash "REGISTER_CALL(FUNCNAME);"

  if has_varargs
    return_type = is_value_function ? "return " : ""
    putd_backslash "if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */"
    indent {
      putd_backslash "if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){"
      indent {
      putd_backslash "va_list ap;"
      putd_backslash "va_start(ap, arg#{arg_count-1});"
      putd_backslash "RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](#{arg_list(arg_count)}, ap);" unless not is_value_function
      putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
      putd_backslash "va_end(ap);"  unless not is_value_function
      putd_backslash "return ret;" unless not is_value_function
      putd_backslash "#{return_type}FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](#{arg_list(arg_count)}, ap);" unless is_value_function
      putd_backslash "va_end(ap);" unless is_value_function
      }
      putd_backslash "}"
      putd_backslash "else{"
      indent {
      putd_backslash "va_list ap;"
      putd_backslash "va_start(ap, arg#{arg_count-1});"
      putd_backslash "RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](#{arg_list(arg_count)}, ap);" unless not is_value_function
      putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
      putd_backslash "va_end(ap);"  unless not is_value_function
      putd_backslash "return ret;" unless not is_value_function
      putd_backslash "#{return_type}FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](#{arg_list(arg_count)}, ap);"
      putd_backslash "va_end(ap);" unless is_value_function
      }
      putd_backslash "}"
    }
    putd_backslash "}"
    putd_backslash "if(FUNCNAME##_fake.custom_fake){"
    indent {
      putd_backslash "RETURN_TYPE ret;" if is_value_function
      putd_backslash "va_list ap;"
      putd_backslash "va_start(ap, arg#{arg_count-1});"
    }
    custom_fake_call = "FUNCNAME##_fake.custom_fake(#{arg_list(arg_count)}, ap);"
    indent {
      if is_value_function
        putd_backslash "ret = #{custom_fake_call}"
      else
        putd_backslash "#{custom_fake_call}"
      end
      putd_backslash "va_end(ap);"
      putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
      putd_backslash "return ret;" if is_value_function
    }
    putd_backslash "}"
  else
    return_type = is_value_function ? "return " : ""
    putd_backslash "if (FUNCNAME##_fake.custom_fake_seq_len){ /* a sequence of custom fakes */"
    indent {
      putd_backslash "if (FUNCNAME##_fake.custom_fake_seq_idx < FUNCNAME##_fake.custom_fake_seq_len){"
      indent {
        putd_backslash "RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](#{arg_list(arg_count)});" unless not is_value_function
        putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
        putd_backslash "return ret;" unless not is_value_function
        putd_backslash "#{return_type}FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_idx++](#{arg_list(arg_count)});" unless is_value_function
      }
      putd_backslash "}"
      putd_backslash "else{"
      indent {
        putd_backslash "RETURN_TYPE ret = FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](#{arg_list(arg_count)});" unless not is_value_function
        putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
        putd_backslash "return ret;" unless not is_value_function
        putd_backslash "#{return_type}FUNCNAME##_fake.custom_fake_seq[FUNCNAME##_fake.custom_fake_seq_len-1](#{arg_list(arg_count)});"
      }
      putd_backslash "}"
    }
    putd_backslash "}"
    putd_backslash "if (FUNCNAME##_fake.custom_fake){ "
    indent {
        putd_backslash "RETURN_TYPE ret = FUNCNAME##_fake.custom_fake(#{arg_list(arg_count)});" unless not is_value_function
        putd_backslash "SAVE_RET_HISTORY(FUNCNAME, ret);" unless not is_value_function
        putd_backslash "return ret;" unless not is_value_function
        putd_backslash "#{return_type}FUNCNAME##_fake.custom_fake(#{arg_list(arg_count)});"
    }
    putd_backslash "}"
  end

  putd_backslash "RETURN_FAKE_RESULT(FUNCNAME)" if is_value_function
end

def define_fff_globals
  putd "typedef void (*fff_function_t)(void);"
  putd "typedef struct { "
  indent {
    putd "fff_function_t call_history[FFF_CALL_HISTORY_LEN];"
    putd "unsigned int call_history_idx;"
  }
  putd "} fff_globals_t;"
  puts
  putd "FFF_EXTERN_C"
  putd "extern fff_globals_t fff;"
  putd "FFF_END_EXTERN_C"
  puts
  putd_backslash "#define DEFINE_FFF_GLOBALS"
  indent {
    putd_backslash "FFF_EXTERN_C"
    indent {
      putd_backslash "fff_globals_t fff;"
    }
    putd "FFF_END_EXTERN_C"
  }
  puts
  putd_backslash "#define FFF_RESET_HISTORY()"
  indent {
    putd_backslash "fff.call_history_idx = 0;"
    putd "memset(fff.call_history, 0, sizeof(fff.call_history));"
  }
  puts
  putd_backslash "#define REGISTER_CALL(function)"
  indent {
    putd_backslash "if(fff.call_history_idx < FFF_CALL_HISTORY_LEN)"
    indent {
                putd "fff.call_history[fff.call_history_idx++] = (fff_function_t)function;"
    }
  }
end

def in_struct
  putd_backslash "typedef struct FUNCNAME##_Fake {"
  indent {
    yield
  }
  putd_backslash "} FUNCNAME##_Fake;"
end

def include_guard
  putd "#ifndef FAKE_FUNCTIONS"
  putd "#define FAKE_FUNCTIONS"
  puts

  yield

  puts
  putd "#endif /* FAKE_FUNCTIONS */"
end

def msvc_expand_macro_fix
  putd "/* MSVC expand macro fix */"
  putd "#define EXPAND(x) x"
end

def generate_arg_sequence(args, prefix, do_reverse, joinstr, first_idx = 0)
 fmap = (first_idx..args).flat_map {|i| [prefix + i.to_s]}
 if do_reverse then fmap.reverse.join(joinstr) else fmap.join(", ") end
end

def counting_macro_instance(type, has_calling_conventions, vararg = :non_vararg, prefix = "")
  appendix = (vararg == :vararg) ? "_VARARG" : ""
  if has_calling_conventions
    minus_count = (type == :VOID) ? 2 : 3
  else
    minus_count = (type == :VOID) ? 1 : 2
  end

  <<-MACRO_COUNTING_INSTANCE
#define #{prefix}FAKE_#{type.to_s}_FUNC#{appendix}(...) \
    EXPAND(#{prefix}FUNC_#{type.to_s}#{appendix}_(PP_NARG_MINUS#{minus_count}(__VA_ARGS__), __VA_ARGS__))

#define #{prefix}FUNC_#{type.to_s}#{appendix}_(N,...) \
    EXPAND(#{prefix}FUNC_#{type.to_s}#{appendix}_N(N,__VA_ARGS__))

#define #{prefix}FUNC_#{type.to_s}#{appendix}_N(N,...) \
    EXPAND(#{prefix}FAKE_#{type.to_s}_FUNC ## N#{" ## _VARARG" if vararg == :vararg}(__VA_ARGS__))

  MACRO_COUNTING_INSTANCE
end

def output_macro_counting_shortcuts(has_calling_conventions)

  ppiter = PPIterators.new(recursive: false)
  putd <<-MACRO_COUNTING

#{ppiter.embed_macros}

#{ppiter.narg_minus(1)}

#{ppiter.narg_minus(2)}

#{ppiter.narg_minus(3)}

#{ppiter.parameterised_each_with_index}

#ifndef PP_1PAR_EACH_IDX
#{ppiter.parameterised_each_with_index_n(1)}
#endif

#ifndef PP_2PAR_EACH_IDX
#{ppiter.parameterised_each_with_index_n(2)}
#endif

/* DECLARE AND DEFINE FAKE FUNCTIONS - PLACE IN TEST FILES */

#{counting_macro_instance(:VALUE, has_calling_conventions)}
#{counting_macro_instance(:VOID,  has_calling_conventions)}
#{counting_macro_instance(:VALUE, has_calling_conventions, :vararg)}
#{counting_macro_instance(:VOID, has_calling_conventions, :vararg)}

/* DECLARE FAKE FUNCTIONS - PLACE IN HEADER FILES */

#{counting_macro_instance(:VALUE, has_calling_conventions, :non_vararg, "DECLARE_")}
#{counting_macro_instance(:VOID, has_calling_conventions, :non_vararg, "DECLARE_")}
#{counting_macro_instance(:VALUE, has_calling_conventions, :vararg, "DECLARE_")}
#{counting_macro_instance(:VOID, has_calling_conventions, :vararg, "DECLARE_")}

/* DEFINE FAKE FUNCTIONS - PLACE IN SOURCE FILES */

#{counting_macro_instance(:VALUE, has_calling_conventions, :non_vararg, "DEFINE_")}
#{counting_macro_instance(:VOID, has_calling_conventions, :non_vararg, "DEFINE_")}
#{counting_macro_instance(:VALUE, has_calling_conventions, :vararg, "DEFINE_")}
#{counting_macro_instance(:VOID, has_calling_conventions, :vararg, "DEFINE_")}

  MACRO_COUNTING
end

def output_assertions
  putd <<-'EOA'
/* ASSERTIONS (Public API) */

#define FFF_ASSERT_CALLS(FN, COUNT) _FFF_ASSERT_EQ_MSG(COUNT, FFF_CALLS(FN), #FN " called incorrect number of times")

#define FFF_ASSERT_CALLED(FN)  FFF_ASSERT_CALLS(FN, 1);

#define FFF_ASSERT_NOT_CALLED(FN)  FFF_ASSERT_CALLS(FN, 0);

#define FFF_ASSERT(FN, ...) \
  FFF_ASSERT_CALLS(FN, 1); \
  PP_1PAR_EACH_IDX(_FFF_ASSERT_ARG, FN, __VA_ARGS__)

#define FFF_ASSERT_NTH(FN, CALL_IDX, ...) \
  _FFF_ASSERT_TRUE_MSG(CALL_IDX>0, "Invalid call index -- expected >0, got " #CALL_IDX); \
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

#define _FFF_ASSERT_TRUE_MSG(COND, MSG) _FFF_ASSERT_EQ_MSG((COND), true, MSG)
#define _FFF_ASSERT_FALSE_MSG(COND, MSG) _FFF_ASSERT_EQ_MSG((COND), false, MSG)

#define _FFF_ASSERT_ARG(FN, VAL, ARG_IDX) _FFF_ASSERT_EQ_MSG(VAL, FFF_LAST_ARG_VAL(FN, ARG_IDX), #FN" parameter value mismatch at ARG"#ARG_IDX);

#define _FFF_ASSERT_HISTORICAL_ARG(FN, CALL_IDX, VAL, ARG_IDX) _FFF_ASSERT_EQ_MSG(VAL, FFF_NTH_ARG_VAL(FN, CALL_IDX, ARG_IDX),  #FN", call "#CALL_IDX": value mismatch for arg"#ARG_IDX);
EOA
end

def output_c_and_cpp(has_calling_conventions)
  include_guard {
    include_dependencies
    output_constants
    output_internal_helper_macros
    yield
    output_macro_counting_shortcuts(has_calling_conventions)
    output_assertions
  }
end

def help
  # Check if we should generate _with_ support for specifying calling conventions
  if (ARGV[0] == "--help" or ARGV[0] == "-h")
    puts "Usage: fakegen.rb [options]
    -h, --help                        Show this help message
    -wcc, --with-calling-conventions  Support specifying calling conventions"
    exit
  end
  yield
end

help {
  # Determine if we should generate with support for calling conventions
  has_calling_conventions = true if (ARGV[0] == "--with-calling-conventions" or ARGV[0] == "-wcc")
  # lets generate!!
  output_c_and_cpp(has_calling_conventions) {
    define_fff_globals
    # Create fake generators for 0..MAX_ARGS
    num_fake_generators = $MAX_ARGS + 1
    num_fake_generators.times {|arg_count| output_macro(arg_count, false, has_calling_conventions, false)}
    num_fake_generators.times {|arg_count| output_macro(arg_count, false, has_calling_conventions, true)}
    # generate the varargs variants
    (2..$MAX_ARGS).each {|arg_count| output_macro(arg_count, true, has_calling_conventions, false)}
    (2..$MAX_ARGS).each {|arg_count| output_macro(arg_count, true, has_calling_conventions, true)}
  }
}
