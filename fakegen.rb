
# fakegen.rb
# A simple code generator to create some C macros for defining test fake functions


$cpp_output = true
$MAX_ARGS = 10
$DEFAULT_ARG_HISTORY = 10
$MAX_CALL_HISTORY = 10

def output_macro(args, is_value_function)

  macro_name_preamble = is_value_function ? "FAKE_VALUE_FUNC" : "FAKE_VOID_FUNC";
  macro_name = "#{macro_name_preamble}#{args}"
  return_type = is_value_function ? "RETURN_TYPE" : ""

  output_macro_header(macro_name, args, return_type)

  extern_c {  # define argument capture variables
    output_variables(args, is_value_function)
    output_function_signature(args, is_value_function)

    puts "{ \\"
    output_function_body(args, is_value_function)
    puts "	} \\"
    output_reset_function(args, is_value_function)
  }
  puts "STATIC_INIT(FUNCNAME) \\" if $cpp_output
  puts ""
end

def output_cpp_reset_code
  puts <<-REGISTRATION
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

def output_macro_header(macro_name, args, return_type)
  puts ""

  output_macro_name(macro_name, args, return_type)

end

def output_macro_name(macro_name, args, return_type)
  parameter_list = return_type
  if return_type == ""
    puts "/* Defining a void function with #{args} parameters*/"
  else
    puts "/* Defining a function returning a value with #{args} parameters*/"
    parameter_list += ", "
  end
  parameter_list += "FUNCNAME"
  print "#define #{macro_name}(" + parameter_list

  args.times { |i| print ", ARG#{i}_TYPE" }

  puts ") \\"
end

def output_argument_capture_variables(i)
  # last argument
  puts "    static ARG#{i}_TYPE FUNCNAME##_arg#{i}_val; \\"
  # argument history array
  puts "    static ARG#{i}_TYPE FUNCNAME##_arg#{i}_history[#{$DEFAULT_ARG_HISTORY}];\\"
end

def output_variables(arg_count, is_value_function)
  arg_count.times { |i| output_argument_capture_variables(i) }
  puts "    static RETURN_TYPE FUNCNAME##_return_val; \\" unless not is_value_function
  puts "    static unsigned int FUNCNAME##_call_count = 0; \\"
  puts "    static unsigned int FUNCNAME##_arg_history_len = #{$DEFAULT_ARG_HISTORY};\\"
  puts "    static unsigned int FUNCNAME##_arg_histories_dropped = 0; \\"
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
  # capture arguments
  arg_count.times { |i| puts "        FUNCNAME##_arg#{i}_val = arg#{i}; \\" }
  # store in argument history
  arg_count.times { |i|
    puts "        if(FUNCNAME##_call_count < FUNCNAME##_arg_history_len){\\"
    puts "            FUNCNAME##_arg#{i}_history[FUNCNAME##_call_count] = arg#{i}; \\"
    puts "        }\\"
  }

  # update dropped argument history counts
  puts "        if(FUNCNAME##_call_count >= FUNCNAME##_arg_history_len){\\"
  puts "            FUNCNAME##_arg_histories_dropped++;\\"
  puts "        }\\"

  # update call count
  puts "        FUNCNAME##_call_count++; \\"
  #register call
  puts "        REGISTER_CALL(FUNCNAME); \\"
  # return something if value function
  puts "        return FUNCNAME##_return_val; \\" unless not is_value_function
end

def output_reset_function(arg_count, is_value_function)
  puts "    void FUNCNAME##_reset(){ \\"
  arg_count.times { |i|
    puts "        FUNCNAME##_arg#{i}_val = (ARG#{i}_TYPE) 0; \\"
    puts "        memset(FUNCNAME##_arg#{i}_history, 0, sizeof(FUNCNAME##_arg#{i}_history)); \\"
  }
  puts "        FUNCNAME##_call_count = 0; \\"
  puts "        FUNCNAME##_return_val = 0; \\" unless not is_value_function
  puts "    } \\"
end

def define_reset_fake
  puts ""
  puts "/* Defining a function to reset a fake function */"
  puts "#define RESET_FAKE(FUNCNAME) { \\"
  puts "    FUNCNAME##_reset(); \\"
  puts "} \\"
  puts ""
end

def define_call_history
  puts "#define MAX_CALL_HISTORY #{$MAX_CALL_HISTORY}u"
  puts "static void * call_history[MAX_CALL_HISTORY];"
  puts "static unsigned int call_history_idx;"
  puts "void RESET_HISTORY() { "
  puts "    call_history_idx = 0; "
  puts "}"

  puts "#define REGISTER_CALL(function) \\"
  puts "   if(call_history_idx < MAX_CALL_HISTORY) call_history[call_history_idx++] = (void *)function;"
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


def output_c_and_cpp

  include_guard {
    puts "#ifdef __cplusplus"
    $cpp_output = true
    yield

    puts "#else  /* ansi c */"

    $cpp_output = false
    yield
    puts "#endif  /* cpp/ansi c */"
  }
end


# lets generate!!
output_c_and_cpp{
  output_cpp_reset_code if $cpp_output
  output_cpp_static_initializer if $cpp_output
  10.times {|arg_count| output_macro(arg_count, false)}
  10.times {|arg_count| output_macro(arg_count, true)}
  define_reset_fake
  define_call_history
}