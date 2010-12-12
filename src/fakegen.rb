
# fakegen.rb
# A simple code generator to create some C macros for defining test fake functions


$cpp_output = true

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
    output_reset_function(args)
  }
  puts "STATIC_INIT(FUNCNAME) \\"
  puts ""
end

def output_reset_code
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

def output_static_initializer
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

def output_variables(args, is_value_function)
  args.times { |i| puts "    static ARG#{i}_TYPE FUNCNAME##_arg#{i}_val; \\" }
  puts "    static RETURN_TYPE FUNCNAME##_return_val; \\" unless not is_value_function
  puts "    static int FUNCNAME##_call_count = 0; \\"
end

def output_function_signature(args, is_value_function)
  if is_value_function
    print "    RETURN_TYPE FUNCNAME("
  else
    print "    void FUNCNAME("
  end

  args.times { |i| print "ARG#{i}_TYPE arg#{i}" }

  print ")"
end

def output_function_body(args, is_value_function)
  # capture arguments
  args.times { |i| puts "        FUNCNAME##_arg#{i}_val = arg#{i}; \\" }
  # update call count
  puts "        FUNCNAME##_call_count++; \\"
  # return something if value function
  puts "        return FUNCNAME##_return_val; \\" unless not is_value_function
end

def output_reset_function(args)
  puts "    void FUNCNAME##_reset(){ \\"
  args.times { |i|
    puts "        FUNCNAME##_arg#{i}_val = (ARG#{i}_TYPE) 0; \\"
  }
  puts "        FUNCNAME##_call_count = 0; \\"
  puts "    } \\"
end

def define_reset_fake
  puts ""
  puts "/* Defining a function to reset a fake function */"
  puts "#define RESET_FAKE(FUNCNAME) { \\"
  puts "    FUNCNAME##_reset(); \\"
  puts "} \\"
end

def include_guard
  puts "#ifndef FAKE_FUNCTIONS"
  puts "#define FAKE_FUNCTIONS"
  puts ""

  yield

  puts ""
  puts "#endif // FAKE_FUNCTIONS"
end

def extern_c

  puts "extern \"C\"{ \\" unless !$cpp_output
  yield
  puts "} \\" unless !$cpp_output
end

# lets generate!!
include_guard {
  output_reset_code if $cpp_output
  output_static_initializer if $cpp_output
  10.times {|arg_count| output_macro(arg_count, false)}
  10.times {|arg_count| output_macro(arg_count, true)}
  define_reset_fake
}
