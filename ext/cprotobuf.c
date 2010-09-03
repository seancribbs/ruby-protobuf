#include <ruby.h>

/* Ensure compatibility with early releases of Ruby 1.8.5 */
#ifndef RSTRING_PTR
#  define RSTRING_PTR(v) RSTRING(v)->ptr
#endif

#ifndef RSTRING_LEN
#  define RSTRING_LEN(v) RSTRING(v)->len
#endif

#ifndef RARRAY_PTR
#  define RARRAY_PTR(v) RARRAY(v)->ptr
#endif

#ifndef RARRAY_LEN
#  define RARRAY_LEN(v) RARRAY(v)->len
#endif

#if HAVE_RUBY_ST_H
#include "ruby/st.h"
#endif
#if HAVE_ST_H
#include "st.h"
#endif

#define LONGBITS (sizeof(unsigned long) * 8)

static VALUE cVarintField;

// Thanks Eric Hodel and mongo-ruby-driver
static VALUE pack_chars(VALUE self)
{
  VALUE res;
  long i;
  char c;

  res = rb_str_buf_new(0);

  for (i = 0; i < RARRAY_LEN(self); i++) {
    c = FIX2LONG(RARRAY_PTR(self)[i]);
    rb_str_buf_cat(res, &c, sizeof(char));
  }

  return res;
}

static VALUE varint_field_encode_fixnum(VALUE self, VALUE value){
  unsigned long iValue;
  unsigned long currbyte;
  VALUE bytes;

  if(FIX2LONG(value) < 0)
    rb_raise(rb_eRangeError, "Varint value cannot be negative");

  iValue = FIX2ULONG(value);
  bytes = rb_ary_new();

  do {
    if(iValue < 128)
      currbyte = iValue;
    else
      currbyte = 0x80 | (iValue & 0x7f);
    rb_ary_push(bytes, INT2FIX(currbyte));
    iValue = (iValue >> 7);
  } while(iValue);

  return pack_chars(bytes);
}

/* static VALUE varint_field_encode_bignum(VALUE self, VALUE value){ */
/*   VALUE bytes, bignum, tempnum; */

/*   if(RBIGNUM_NEGATIVE_P(value)) */
/*     rb_raise(eRangeError, "Varint value cannot be negative"); */

/*   bytes = rb_ary_new(); */
/*   bignum = rb_big_clone(value); */
/*   while(!rb_bigzero_p(bignum)){ */
/*     if(FIX2INT(rb_big_cmp(bignum,rb_uint2big(128))) < 0) */
/*       tempnum = bignum; */
/*     else */
/*       tempnum = rb_big_or(rb_big_and(bignum, rb_uint2big(0x7F)), rb_uint2big(0x80)); */
/*     rb_ary_push(bytes, INT2FIX(rb_big2ulong(tempnum))); */
/*     bignum = rb_big_rshift(bignum, INT2FIX(7)); */
/*   } */

/*   return pack_chars(bytes); */
/* } */

static VALUE varint_field_class_encode(VALUE self, VALUE value){
  switch(TYPE(value)){
  case T_FIXNUM:
    return varint_field_encode_fixnum(self, value);
    break;
  case T_BIGNUM:
    return rb_funcall(self, rb_intern("_encode"), 1, value);
    break;
  default:
    rb_raise(rb_eArgError, "Cannot encode a non-integer type.");
  }
  return Qnil;
}

void Init_cprotobuf() {
  VALUE mProtobuf;
  VALUE mProtobuf_Field;

  rb_define_method(rb_cArray, "pack_chars", pack_chars, 0);

  rb_require("protobuf/message/field");
  mProtobuf = rb_const_get(rb_cObject, rb_intern("Protobuf"));
  mProtobuf_Field = rb_const_get(mProtobuf, rb_intern("Field"));
  cVarintField = rb_singleton_class(rb_const_get(mProtobuf_Field, rb_intern("VarintField")));

  rb_alias(cVarintField, rb_intern("_encode"), rb_intern("encode"));
  rb_define_method(cVarintField, "encode", varint_field_class_encode, 1);
}
