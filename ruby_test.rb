$MAX_ARGS = 20


#{ $MAX_ARGS.times {|arg_count| '_' + arg_count.to_s }
def generate_reverse_arg_sequence(args)
  (0..args).flat_map {|i| ['_' + i.to_s]}.join(", ")
end

def generate_arg_sequence(args, prefix, do_reverse) 
 fmap = (0..args).flat_map {|i| [prefix + i.to_s]}
 if do_reverse then fmap.reverse.join(", ") else fmap.join(", ") end
end


def output_macro_counting_shortcuts
  puts <<-MACRO_COUNTING

#define PP_NARG_MINUS2(...) \
    PP_NARG_MINUS2_(__VA_ARGS__, PP_RSEQ_N_MINUS2())

#define PP_NARG_MINUS2_(...) \
    PP_ARG_MINUS2_N(__VA_ARGS__)

#define PP_ARG_MINUS2_N(returnVal, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, N, ...)   N
#define PP_ARG_MINUS2_N(returnVal, #{ generate_arg_sequence($MAX_ARGS - 1, '_', false) }, N, ...)   N

#define PP_RSEQ_N_MINUS2() \
    19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define PP_RSEQ_N_MINUS2() \
    #{ generate_arg_sequence($MAX_ARGS - 1, '', true) }


#define FAKE_VALUE_FUNC(...) \
    FUNC_VALUE_(PP_NARG_MINUS2(__VA_ARGS__), __VA_ARGS__)

#define FUNC_VALUE_(N,...) \
    FUNC_VALUE_N(N,__VA_ARGS__)

#define FUNC_VALUE_N(N,...) \
    FAKE_VALUE_FUNC ## N(__VA_ARGS__)

  MACRO_COUNTING
end

puts generate_arg_sequence($MAX_ARGS, '', false)
puts generate_arg_sequence($MAX_ARGS, '_', false)
puts generate_arg_sequence($MAX_ARGS, '', true)
puts generate_arg_sequence($MAX_ARGS, '_', true)

output_macro_counting_shortcuts