#!/usr/bin/env ruby

# Generator logic for a family of macros for C metaprogramming
#
# See these blog posts in relation to VA_EACH:
# - http://saadahmad.ca/cc-preprocessor-metaprogramming-2/
# - http://ptspts.blogspot.ie/2013/11/how-to-apply-macro-to-all-arguments-of.html
# - https://codecraft.co/2014/11/25/variadic-macros-tricks/ (Similar, but recursive)
# IS_EMPTY implementation adapted from here:
# - https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/

require 'date'

class PPIterators

  class CFile
    INDENT = 2
    class << self

      def indent(content)
        indentation = " " * INDENT
        indentation + content.gsub("\n","\n#{indentation}").gsub("\n#{indentation}\n", "\n\n").sub(/\s+\Z/,"\n")
      end

      def include_guard(name, content)
        content.prepend("#define #{name}\n") unless content =~ /#define #{name}/

        "#ifndef #{name}\n" + indent(content) + "#endif //#{name}\n"
      end

      def define_macro(m)
        m.empty? ? m : "\#define #{m}\n"
      end

      def define_macros(macros)
        macros.map{ |m| define_macro(m) }.join
      end

      def define_macro_set(guard_name, macros)
        include_guard(guard_name, define_macros(macros))
      end
    end
  end

  MAX_ARG_COUNT_DEFAULT = 256
  MAX_ARG_COUNT_DEFAULT_NON_RECURSIVE = 64
  DEFER_LEVELS_DEFAULT = 6
  attr_reader :nargs_max, :recursive, :gcc, :defer_levels
  def initialize(recursive: true,
                 use_gcc_extensions: false,
                 defer_levels: DEFER_LEVELS_DEFAULT)
    @gcc = use_gcc_extensions
    @recursive = recursive
    @nargs_max = @recursive ? MAX_ARG_COUNT_DEFAULT : MAX_ARG_COUNT_DEFAULT_NON_RECURSIVE
    @defer_levels = defer_levels
  end


  def arg_seq(reverse: false, first: 0, last: @nargs_max, prefix: '', sep:', ')
    seq = (first..last).map{ |i| "#{prefix}#{i}"}
    seq.reverse! if reverse
    seq.join(sep)
  end

  # This complex implementation of a concatenation macro is tuple-aware
  # i.e. is valid for braced tuples during pattern matching (see matcher macros later)
  # This is required for recursive for_each on an array of tuples, each PP_EACH(TF, (1, 2), (3, 4))
  # As the list empty macros will break down otherwise
  # See saadahmad's posts for further info
  def concat
    CFile::include_guard('PP_CAT', <<-EOH
// Defer the call to the CAT so that we get the updated parameters first
#define CAT(a, b) _CAT_EVAL ( _CAT_EXPAND_PARAMETERS(a, b) )
#define _CAT_EVAL(...) _CAT_HELPER __VA_ARGS__

// Find the result of testing whether a macro is enclosed or not
#define _CAT_EXPAND_PARAMETERS(a, b) (a, _CAT_PAREN_CHECK b, DEFAULT, b )
#define _CAT_PAREN_CHECK(...) EXPANDED, ENCLOSED, (__VA_ARGS__) ) EAT (

// Pattern match the result of testing if it is enclose or not
#define _CAT_HELPER(a, _, f, b) _CAT_HELPER_ ## f (a, b)
#define _CAT_HELPER_ENCLOSED(a, b) a b
#define _CAT_HELPER_DEFAULT(a, b) a ## b

EOH
                        )
  end

  def eval_set(suffix: '', prefix: '')
    level_count = Math.log2(@nargs_max).ceil
    eroot = "#{prefix}EVAL#{suffix}"
    CFile::define_macro_set(eroot,
                            [
                              "#{eroot}(...) _#{eroot}_#{level_count}(__VA_ARGS__)",
                              "_#{eroot}_1(...) __VA_ARGS__"
                            ] + (2..level_count).map{ |l| "_#{eroot}_#{l}(...) _#{eroot}_#{l-1}(_#{eroot}_#{l-1}(__VA_ARGS__))"}
                           )
  end

  #Support two levels of eval nesting
  def eval
    [eval_set, eval_set(:suffix => '_')].join("\n")
  end

  def defer
    CFile::define_macro_set('DEFER',
                            [
                              'PP_NOP()',
                              'DEFER(...) __VA_ARGS__ PP_NOP()',
                              'DEFER2(...) __VA_ARGS__ DEFER(PP_NOP) ()',
                            ] + (3..@defer_levels).map{ |l| "DEFER#{l}(...) __VA_ARGS__ DEFER#{l-1}(PP_NOP) ()"}
                           )
  end

  # See http://saadahmad.ca/cc-preprocessor-metaprogramming-basic-pattern-matching-macros-and-conditionals/
  def logical
    CFile::include_guard('PP_LOGIC', <<-EOH
//Usage: IF(<condition>) (<then>, <else>)
#define  IF(value) CAT(_IF_, value)
#define _IF_1(true, ...) true
#define _IF_0(true, ...) __VA_ARGS__

#define NOT(x) PP_MATCHER ( CAT(NOT_, x), 0 )
#define NOT_0 EXISTS(1)

//== Building blocks

//PP_MATCHER
#define PP_MATCHER(value, ...) \
  IF ( _MATCH(_MATCH_FIND(value)) )\
       ( _MATCH_EXTRACT(value), __VA_ARGS__ )

#define _MATCH(x) CAT(__MATCH_, x)
#define __MATCH_EXISTS(...) 1
#define __MATCH_UNMATCHED 0

#define _MATCH_FIND(x) __MATCH_RESULT_EXTRACT (  __MATCH_RESULT_FIND(x) )

/**
 * Extract 2nd element of a match result in the format:
 * (IGNORED, EXISTS(DEFINED_VALUE)) or (IGNORED, UNMATCHED)
 * This is appended to __MATCH_ to convert result to a boolean,
 */
#define __MATCH_RESULT_EXTRACT(x) __MATCH_RESULT_EXTRACT_HELPER  x
#define __MATCH_RESULT_EXTRACT_HELPER(match, return_value) return_value

/**
 * __MATCH_RESULT_FIND( EXISTS(bla) ) returns ( EXPANDED, EXISTS(bla) )
 * __MATCH_RESULT_FIND( bla ) returns ( TEST_bla, UNMATCHED)
 */
#define __MATCH_RESULT_FIND(x) ( CAT(__MATCH_RESULT_EXPAND_, x),  UNMATCHED )
#define __MATCH_RESULT_EXPAND_EXISTS(...) EXPANDED, EXISTS(__VA_ARGS__) ) EAT (
#define EAT(...)

//_MATCH_EXTRACT / __MATCH_EXTRACT_EXISTS
#define _MATCH_EXTRACT(value) CAT(__MATCH_EXTRACT_, value)
#define __MATCH_EXTRACT_EXISTS(...) __VA_ARGS__

EOH
)
  end

  def is_empty
      <<-EOH
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
  #define IS_EMPTY(...)  NOT(PP_NARG(__VA_ARGS__))
#else
  #define IS_EMPTY(...)                                                   \
  _ISEMPTY(                                                               \
            /* test if there is just one argument, eventually an empty    \
               one */                                                     \
            HAS_COMMA(__VA_ARGS__),                                       \
            /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
               adds a comma */                                            \
            HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
            /* test if the argument together with a parenthesis           \
               adds a comma */                                            \
            HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
            /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
               parenthesis adds a comma */                                \
            HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
            )

  #define _ISEMPTY(_0, _1, _2, _3)                                               \
    HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))

  #define HAS_COMMA(...)                                                         \
    PP_ARG_N(__VA_ARGS__, #{'1, '*(@nargs_max-1)} 0)

  #define _TRIGGER_PARENTHESIS_(...) ,

  #define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
  #define _IS_EMPTY_CASE_0001 ,
#endif
  EOH
  end

  def lists
    CFile::include_guard('PP_LISTS', <<-EOH
#define HEAD(FIRST, ...) FIRST
#define TAIL(FIRST, ...) __VA_ARGS__

#define TEST_LAST EXISTS(1)
#define NOT_EMPTY(...) NOT(IS_EMPTY(__VA_ARGS__))
#{is_empty}
EOH
                        )
  end

  def tuples
    CFile::include_guard('PP_TUPLES', <<-EOH
#define PAREN(...) ( __VA_ARGS__ )
#define DEPAREN(...) DEPAREN_ __VA_ARGS__
#define DEPAREN_(...) __VA_ARGS__

#define IS_ENCLOSED(x, ...) PP_MATCHER ( IS_ENCLOSED_TEST x, 0 )
#define IS_ENCLOSED_TEST(...) EXISTS(1)

#define IF_ENCLOSED(...) CAT(_IF_ENCLOSED_, IS_ENCLOSED(__VA_ARGS__))
#define _IF_ENCLOSED_0(true, ...) __VA_ARGS__
#define _IF_ENCLOSED_1(true, ...) true
// This function will optionally remove parentheses around its arguments
// if there are any. Otherwise it will return normally
#define OPT_DEPAREN(...) \
  IF_ENCLOSED (__VA_ARGS__) ( DEPAREN(__VA_ARGS__), __VA_ARGS__ )
EOH
                        )
  end

  def narg_common
    CFile::define_macro_set('PP_UTIL',
                            [
                              # Fix for MSVC expansion order (nicked from fff project)
                              "EXPAND(x) x",
                              "PP_SEQ_N() #{arg_seq(last: @nargs_max)}",
                              "PP_RSEQ_N() #{arg_seq(last: @nargs_max, reverse: true)}"
                            ]
                           )
  end

  def arg_n_seq(delta)
    seq = arg_seq(first: 1, last: @gcc ? @nargs_max+1 : @nargs_max, prefix: '_')
    delta==0 ? seq :[arg_seq(first: 0, last: delta-1, prefix: '__', reverse: true), seq].join(', ')
  end

  def narg
    narg_minus(0)
  end

  def narg_minus(m)
    suffix = m>0 ? "_MINUS#{m}" : ''
    CFile::define_macro_set("PP_NARG#{suffix}",
                            [
                              "PP_NARG#{suffix}(...)  EXPAND(PP_ARG#{suffix}_N(#{'_0, ##' if @gcc}__VA_ARGS__, PP_RSEQ_N()))",
                              "PP_ARG#{suffix}_N(...) EXPAND(_PP_ARG#{suffix}_N(__VA_ARGS__))",
                              "_PP_ARG#{suffix}_N(#{arg_n_seq(m)}, N, ...) N",
                            ]
                           )
  end

  def pp_each
    <<-'EOH'
#define PP_EACH(TF, ...) \
  EVAL(_PP_EACH_DEFER(TF, __VA_ARGS__))

#define _PP_EACH_DEFER(TF, ...) \
  IF ( NOT_EMPTY( __VA_ARGS__ )  ) \
  ( \
    DEFER(TF) (OPT_DEPAREN(HEAD(__VA_ARGS__))) \
    DEFER2 ( __PP_EACH_DEFER ) () (TF, TAIL(__VA_ARGS__)) \
  )

//This indirection along with the DEFER2 and EVAL macros allows the recursive implementation of _PP_EACH_DEFER
#define __PP_EACH_DEFER() _PP_EACH_DEFER
EOH
  end

  def pp_each_non_recursive
    CFile::define_macros([
                         "PP_EACH(TF, ...) _PP_EACH(TF, PP_NARG(__VA_ARGS__), __VA_ARGS__)",
                         "_PP_EACH(TF, N, ...) __PP_EACH(TF, N, __VA_ARGS__)",
                         "__PP_EACH(TF, N, ...) _PP_EACH_##N(TF, __VA_ARGS__)",
                         "",
                         "_PP_EACH_0(TF, ...)",
                         "_PP_EACH_1(TF, next_arg) TF(next_arg)",
                       ] + (2..@nargs_max).map { |arg_count| "_PP_EACH_#{arg_count}(TF, next_arg, ...) TF(next_arg) _PP_EACH_#{arg_count-1}(TF, __VA_ARGS__)" }
                      )
  end

  def each_idx_non_recursive
    CFile::define_macros(
      [
        "PP_EACH_IDX(TF, ...) _PP_EACH_IDX(TF, PP_NARG(__VA_ARGS__), __VA_ARGS__)",
        "_PP_EACH_IDX(TF, N, ...) __PP_EACH_IDX(TF, N, __VA_ARGS__)",
        "__PP_EACH_IDX(TF, N, ...) _PP_EACH_IDX_##N(TF, __VA_ARGS__)",
        "",
        "_PP_EACH_IDX_0(TF, dummy)"
      ] + (1..@nargs_max).map do |arg_count|
        arg_indices = (0..arg_count-1)
        arg_ids = arg_indices.map{ |aidx| "_#{aidx}"}
        macro_signature = "_PP_EACH_IDX_#{arg_count}(TF, #{arg_ids.join(', ')})"
        macro_body = arg_indices.map { |aidx| "TF(_#{aidx}, #{aidx})" }.join(' ')
        "#{macro_signature} #{macro_body}"
      end
    )
  end

  def pp_each_idx
    <<-'EOH'
#define PP_EACH_IDX(TF, ...) EVAL(_PP_EACH_IDX_DEFER(TF, (PP_SEQ_N()), __VA_ARGS__))

#define _PP_EACH_IDX_DEFER(TF, VA_INDICES, ...) \
    IF ( NOT_EMPTY( __VA_ARGS__ )  ) \
    ( \
      DEFER2(TF) (OPT_DEPAREN(HEAD(__VA_ARGS__)), DEFER(HEAD)(DEPAREN(VA_INDICES))) \
      DEFER2 ( __PP_EACH_IDX_DEFER ) () (TF, (TAIL VA_INDICES), TAIL(__VA_ARGS__)) \
    )

  #define __PP_EACH_IDX_DEFER() _PP_EACH_IDX_DEFER
EOH
  end

  def pp_par_each_idx
    <<-'EOH'
#define PP_PAR_EACH_IDX(TF, FARGS, ...) EVAL(_PP_PAR_EACH_IDX_DEFER(TF, FARGS, (PP_SEQ_N()), __VA_ARGS__))

#define _PP_PAR_EACH_IDX_DEFER(TF, FARGS, VA_INDICES, ...)                         \
  IF ( NOT_EMPTY( __VA_ARGS__ )  )                                      \
  (                                                                     \
    DEFER2(TF) (OPT_DEPAREN(FARGS), OPT_DEPAREN(HEAD(__VA_ARGS__)), DEFER(HEAD)(DEPAREN(VA_INDICES))) \
    DEFER2 ( __PP_PAR_EACH_IDX_DEFER ) () (TF, FARGS, (TAIL VA_INDICES), TAIL(__VA_ARGS__)) \
  )

#define __PP_PAR_EACH_IDX_DEFER() _PP_PAR_EACH_IDX_DEFER
EOH
  end

  def pp_par_each_idx_non_recursive
    CFile::define_macros(
      [
        "PP_PAR_EACH_IDX(TF, FARGS, ...) _PP_PAR_EACH_IDX(TF, FARGS, PP_NARG(__VA_ARGS__), __VA_ARGS__)",
        "_PP_PAR_EACH_IDX(TF, FARGS, N, ...) __PP_PAR_EACH_IDX(TF, FARGS, N, __VA_ARGS__)",
        "__PP_PAR_EACH_IDX(TF, FARGS, N, ...) _PP_PAR_IDX_##N(TF, FARGS, __VA_ARGS__)",
        "_PP_APPLY(TF, FARGS, VARG, IDX) DEFER(TF) (DEPAREN(FARGS), VARG, IDX)",
        "",
        "_PP_PAR_IDX_0(TF, FARGS, dummy)",
      ] + (1..@nargs_max).map do |arg_count|
        arg_indices = (0..arg_count-1)
        arg_ids = arg_indices.map{ |aidx| "_#{aidx}"}
        macro_signature = "_PP_PAR_IDX_#{arg_count}(TF, FARGS, #{arg_ids.join(', ')})"
        macro_body = arg_indices.map { |aidx| "_PP_APPLY(TF, FARGS, _#{aidx}, #{aidx})" }.join(' ')
        "#{macro_signature} EVAL(#{macro_body})"
      end
    )
  end

  def each
    CFile::include_guard('PP_EACH', @recursive ? pp_each : pp_each_non_recursive)
  end

  def each_with_index
    CFile::include_guard('PP_EACH_IDX', @recursive ? pp_each_idx : each_idx_non_recursive)
  end

  def parameterised_each_with_index
    CFile::include_guard("PP_PAR_EACH_IDX", @recursive ? pp_par_each_idx : pp_par_each_idx_non_recursive)
  end

  def parameterised_each_with_index_n(n)
    fargs = (1..n).map { |aidx| "P#{aidx}"}.join(", ")
    CFile::define_macro("PP_#{n}PAR_EACH_IDX(TF, #{fargs}, ...) PP_PAR_EACH_IDX(TF, (#{fargs}), __VA_ARGS__)")
  end

  def generate_header
    <<-EOH
/**
 * @file
 *
 * pp_iter.h
 * Some useful c preprocessor extensions for dealing with variadic macros
 *
 * @author Cormac Cannon (cormacc.public@gmail.com)
 *
 * This is auto-generated code. The generator script and further background/usage info may be found here:
 * https://github.com/cormacc/va_args_iterators
 *
 * Autogenerated on #{DateTime.now.strftime("%d/%m/%Y %H:%M")}
 * - Script:             #{$0}
 * - Max argument count: #{@nargs_max}
 *
 * I initially encountered the variadic macro counting logic in this post by Laurent Deniau:
 * https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
 * Refined by arpad. and zhangj to handle the no-argument case
 *
 * The (preferred) recursive implementations of PP_EACH, PP_EACH_IDX and PP_PAR_EACH_IDX are based on an excellent series of posts by Saad Ahmad
 * http://saadahmad.ca/cc-preprocessor-metaprogramming-2/
 *
 * The non- (or semi-) recursive PP_EACH implementation is based on this blog post by Daniel Hardman:
 * https://codecraft.co/2014/11/25/variadic-macros-tricks/
 *
 * The non-recursive PP_EACH_IDX and PP_PAR_EACH_IDX macro implementations extend the non-recursive PP_EACH implementation
 * described in this (anonymous) blog post:
 * http://ptspts.blogspot.ie/2013/11/how-to-apply-macro-to-all-arguments-of.html
 *
 * The MSVC macro expansion fix was lifted from the excellent fake function framework:
 * https://github.com/meekrosoft/fff
 */

#ifndef PP_ITER_H
#  define PP_ITER_H

#  ifdef  __cplusplus
extern "C" {
#  endif

#{embed_macros}

#  ifdef  __cplusplus
}
#  endif

#endif  /* PP_ITER_H */
EOH
  end

  def embed_macros
    <<-EOH

//Defer / evaluate macros
#{defer}
#{eval}

//Token concatenation (tuple-aware)
#{concat}

//Logical operations
#{logical}

//Lists (HEAD, TAIL, ISEMPTY etc.)
#{lists}

//Tuples
#{tuples}

//Argument counting
#{narg_common}
#{narg}

//PP_EACH
#{each}

//PP_EACH_IDX
#{each_with_index}

//PP_PAR_EACH_IDX
#{parameterised_each_with_index}

//PP_xPAR_EACH_IDX (Wrappers for deprecated macros)
#{parameterised_each_with_index_n(1)}
#{parameterised_each_with_index_n(2)}

EOH
  end

end

# Generate a header if run standalone rather than required as a dependency
if __FILE__==$0
  ppi = ARGV.empty? ? PPIterators.new() : PPIterators.new(ARGV[0].to_i)
  puts ppi.generate_header
end
