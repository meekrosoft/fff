
# fakegen.rb
# A simple code generator to create some C macros for defining test fake functions


$cpp_output = true
$MAX_ARGS = 10
$DEFAULT_ARG_HISTORY = 50
$MAX_CALL_HISTORY = 50

def output_constants
  puts "#define FFF_MAX_ARGS (#{$MAX_ARGS}u)"
  puts "#ifndef FFF_ARG_HISTORY_LEN"
  puts "    #define FFF_ARG_HISTORY_LEN (#{$DEFAULT_ARG_HISTORY}u)"
  puts "#endif"
  puts "#ifndef FFF_CALL_HISTORY_LEN"
  puts "  #define FFF_CALL_HISTORY_LEN (#{$MAX_CALL_HISTORY}u)"
  puts "#endif"
end





# ------  Helper macros to use internally ------ #
def output_internal_helper_macros
  puts "/* -- INTERNAL HELPER MACROS -- */"
  
  define_return_sequence_helper
  define_reset_fake_helper
  define_declare_arg_helper
  define_declare_all_func_common_helper
  define_save_arg_helper
  define_room_for_more_history
  define_save_arg_history_helper
  define_history_dropped_helper
  define_value_function_variables_helper
  define_increment_call_count_helper
  define_return_fake_result_helper
  
  puts "/* -- END INTERNAL HELPER MACROS -- */"
  puts ""
end

def define_return_sequence_helper
  puts "#define SET_RETURN_SEQ( FUNCNAME, ARRAY_POINTER, ARRAY_LEN) \\"
  puts "                        FUNCNAME##_fake.return_val_seq = ARRAY_POINTER; \\"
  puts "                        FUNCNAME##_fake.return_val_seq_len = ARRAY_LEN;"
end

def define_reset_fake_helper
  puts ""
  puts "/* Defining a function to reset a fake function */"
  puts "#define RESET_FAKE(FUNCNAME) { \\"
  puts "    FUNCNAME##_reset(); \\"
  puts "} \\"
  puts ""
end

def define_declare_arg_helper
  puts ""
  puts "#define DECLARE_ARG(type, n, FUNCNAME) \\"
  puts "    type arg##n##_val; \\"
  puts "    type arg##n##_history[FFF_ARG_HISTORY_LEN];"
end

def define_declare_all_func_common_helper
  puts ""
  puts "#define DECLARE_ALL_FUNC_COMMON(FUNCNAME) \\"
  puts "    unsigned int call_count; \\"
  puts "    unsigned int arg_history_len;\\"
  puts "    unsigned int arg_histories_dropped;"
end

def define_save_arg_helper
  puts ""
  puts "#define SAVE_ARG(FUNCNAME, n) \\"
  puts "    FUNCNAME##_fake.arg##n##_val = arg##n"
end

def define_room_for_more_history
  puts ""
  puts "#define ROOM_FOR_MORE_HISTORY(FUNCNAME) \\"
  puts "  FUNCNAME##_fake.call_count < FFF_ARG_HISTORY_LEN"
end

def define_save_arg_history_helper
  puts ""
  puts "#define SAVE_ARG_HISTORY(FUNCNAME, ARGN) \\"
  puts "    FUNCNAME##_fake.arg##ARGN##_history[FUNCNAME##_fake.call_count] = arg##ARGN"
end

def define_history_dropped_helper
  puts ""
  puts "#define HISTORY_DROPPED(FUNCNAME) \\"
  puts "    FUNCNAME##_fake.arg_histories_dropped++"
end

def define_value_function_variables_helper
  puts ""
  puts "#define DECLARE_VALUE_FUNCTION_VARIABLES(FUNCNAME, RETURN_TYPE) \\"
  puts "    RETURN_TYPE return_val; \\" 
  puts "    int return_val_seq_len; \\" 
  puts "    int return_val_seq_idx; \\" 
  puts "    RETURN_TYPE * return_val_seq; \\" 
end

def define_increment_call_count_helper
  puts ""
  puts "#define INCREMENT_CALL_COUNT(FUNCNAME) \\"
  puts "    FUNCNAME##_fake.call_count++"
end

def define_return_fake_result_helper
  puts ""
  puts "#define RETURN_FAKE_RESULT(FUNCNAME) \\"
  puts "    if (FUNCNAME##_fake.return_val_seq_len){ /* then its a sequence */ \\"
  puts "        if(FUNCNAME##_fake.return_val_seq_idx < FUNCNAME##_fake.return_val_seq_len) { \\"
  puts "            return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_idx++]; \\"
  puts "        } \\"
  puts "        return FUNCNAME##_fake.return_val_seq[FUNCNAME##_fake.return_val_seq_len-1]; /* return last element */ \\"
  puts "    } \\"
  puts "    return FUNCNAME##_fake.return_val; \\"
end

# ------  End Helper macros ------ #





def output_macro(arg_count, is_value_function)

  macro_name_preamble = is_value_function ? "FAKE_VALUE_FUNC" : "FAKE_VOID_FUNC";
  macro_name = "#{macro_name_preamble}#{arg_count}"
  return_type = is_value_function ? "RETURN_TYPE" : ""

  output_macro_header(macro_name, arg_count, return_type)

  extern_c {  # define argument capture variables
    output_variables(arg_count, is_value_function)
    output_function_signature(arg_count, is_value_function)

    puts "{ \\"
    output_function_body(arg_count, is_value_function)
    puts "	} \\"
    output_reset_function(arg_count, is_value_function)
  }
  puts "STATIC_INIT(FUNCNAME) \\" if $cpp_output
  puts ""
end

def output_cpp_reset_code
  puts <<-REGISTRATION
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
  REGISTRATION
end

def output_cpp_static_initializer
  puts <<-MY_STATIC_INITIALIZER
#define STATIC_INIT(FUNCNAME) \\
class StaticInitializer_##FUNCNAME \\
{ \\
public: \\
	StaticInitializer_##FUNCNAME() \\
	{ \\
		reset_functions.push_back(FUNCNAME##_reset); \\
	} \\
}; \\
static StaticInitializer_##FUNCNAME staticInitializer_##FUNCNAME; \\

  MY_STATIC_INITIALIZER
end

def output_macro_header(macro_name, arg_count, return_type)
  puts ""

  output_macro_name(macro_name, arg_count, return_type)

end

def output_macro_name(macro_name, arg_count, return_type)
  parameter_list = return_type
  if return_type == ""
    puts "/* Defining a void function with #{arg_count} parameters*/"
  else
    puts "/* Defining a function returning a value with #{arg_count} parameters*/"
    parameter_list += ", "
  end
  parameter_list += "FUNCNAME"
  print "#define #{macro_name}(" + parameter_list

  arg_count.times { |i| print ", ARG#{i}_TYPE" }

  puts ") \\"
end

def output_argument_capture_variables(argN)
  puts "    DECLARE_ARG(ARG#{argN}_TYPE, #{argN}, FUNCNAME) \\"
end


def in_struct
  puts "typedef struct FUNCNAME##_Fake { \\"
  yield
  puts "} FUNCNAME##_Fake;\\"
end

def output_variables(arg_count, is_value_function)
  in_struct{
    arg_count.times { |argN| 
      puts "    DECLARE_ARG(ARG#{argN}_TYPE, #{argN}, FUNCNAME) \\"
    }
    puts "    DECLARE_ALL_FUNC_COMMON(FUNCNAME) \\"
    puts "    DECLARE_VALUE_FUNCTION_VARIABLES(FUNCNAME, RETURN_TYPE) \\" unless not is_value_function
  }
  puts "FUNCNAME##_Fake FUNCNAME##_fake;\\"
end

def output_function_signature(args_count, is_value_function)
  if is_value_function
    print "    RETURN_TYPE FUNCNAME("
  else
    print "    void FUNCNAME("
  end

  arguments = []
  args_count.times { |i| arguments << "ARG#{i}_TYPE arg#{i}" }
  print arguments.join(", ")

  print ")"
end

def output_function_body(arg_count, is_value_function)
  arg_count.times { |i| puts "        SAVE_ARG(FUNCNAME, #{i}); \\" }
  puts "        if(ROOM_FOR_MORE_HISTORY(FUNCNAME)){\\"
  arg_count.times { |i| puts "            SAVE_ARG_HISTORY(FUNCNAME, #{i}); \\" }
  puts "        }\\"
  puts "        else{\\"
  puts "            HISTORY_DROPPED(FUNCNAME);\\"
  puts "        }\\"
  puts "        INCREMENT_CALL_COUNT(FUNCNAME); \\"
  puts "        REGISTER_CALL(FUNCNAME); \\"
  puts "        RETURN_FAKE_RESULT(FUNCNAME)  \\" if is_value_function
end

def output_reset_function(arg_count, is_value_function)
  puts "    void FUNCNAME##_reset(){ \\"
  puts "        memset(&FUNCNAME##_fake, 0, sizeof(FUNCNAME##_fake)); \\"
  puts "        FUNCNAME##_fake.arg_history_len = FFF_ARG_HISTORY_LEN;\\"
  puts "    } \\"
end



def define_call_history
  puts "static void * call_history[FFF_CALL_HISTORY_LEN];"
  puts "static unsigned int call_history_idx;"
  puts "static void RESET_HISTORY() { "
  puts "    call_history_idx = 0; "
  puts "}"

  puts "#define REGISTER_CALL(function) \\"
  puts "   if(call_history_idx < FFF_CALL_HISTORY_LEN) call_history[call_history_idx++] = (void *)function;"
end



def extern_c
  puts "extern \"C\"{ \\" unless !$cpp_output
  yield
  puts "} \\" unless !$cpp_output
end

def include_guard
  puts "#ifndef FAKE_FUNCTIONS"
  puts "#define FAKE_FUNCTIONS"
  puts ""

  yield

  puts ""
  puts "#endif // FAKE_FUNCTIONS"
end

def output_macro_counting_shortcuts
  puts <<-MACRO_COUNTING

#define PP_NARG_MINUS2(...) \
    PP_NARG_MINUS2_(__VA_ARGS__, PP_RSEQ_N_MINUS2())

#define PP_NARG_MINUS2_(...) \
    PP_ARG_MINUS2_N(__VA_ARGS__)

#define PP_ARG_MINUS2_N(returnVal, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...)   N

#define PP_RSEQ_N_MINUS2() \
    9,8,7,6,5,4,3,2,1,0


#define FAKE_VALUE_FUNC(...) \
    FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__)

#define FUNC_VALUE_(N,...) \
    FUNC_VALUE_N(N,__VA_ARGS__)

#define FUNC_VALUE_N(N,...) \
    FAKE_VALUE_FUNC ## N(__VA_ARGS__)



#define PP_NARG_MINUS1(...) \
    PP_NARG_MINUS1_(__VA_ARGS__, PP_RSEQ_N_MINUS1())

#define PP_NARG_MINUS1_(...) \
    PP_ARG_MINUS1_N(__VA_ARGS__)

#define PP_ARG_MINUS1_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...)   N

#define PP_RSEQ_N_MINUS1() \
    9,8,7,6,5,4,3,2,1,0

#define FAKE_VOID_FUNC(...) \
    FUNC_VOID_(PP_NARG_MINUS1(__VA_ARGS__), __VA_ARGS__)

#define FUNC_VOID_(N,...) \
    FUNC_VOID_N(N,__VA_ARGS__)

#define FUNC_VOID_N(N,...) \
    FAKE_VOID_FUNC ## N(__VA_ARGS__)

  MACRO_COUNTING
end

def output_c_and_cpp

  include_guard {
    output_constants
    output_internal_helper_macros
    
    puts "#ifdef __cplusplus"
    $cpp_output = true
    yield

    puts "#else  /* ansi c */"

    $cpp_output = false
    yield
    puts "#endif  /* cpp/ansi c */"

    output_macro_counting_shortcuts
  }
end

# lets generate!!
output_c_and_cpp{
  define_call_history
  
  output_cpp_reset_code if $cpp_output
  output_cpp_static_initializer if $cpp_output
  $MAX_ARGS.times {|arg_count| output_macro(arg_count, false)}
  $MAX_ARGS.times {|arg_count| output_macro(arg_count, true)}
}