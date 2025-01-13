#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char num_set[] = "0123456789abcdef";

int vsnprintf(char *out, size_t n,const char *fmt,va_list ap) {
  size_t putlen = 0;
  char stk[1<<10];
  int stk_top = 0;
  while( *fmt != '\0' && putlen < n ) {
    if(*fmt == '%') {
      fmt++;
      switch (*fmt)
      {
        case 'd':{
          int32_t num = va_arg(ap,int32_t);
          if(num == 0) out[putlen++] = '0';
          if(num<0) {
            out[putlen++] = '-';
            num = -num;
          }
          stk_top = 0;
          while(num) {
            stk[stk_top++] = num%10 + '0';
            num = num / 10;
          }
          while(stk_top) {
            out[putlen++] = stk[stk_top-1];
            stk_top -= 1;
          }
          break;
        }
        case 'x':{
         uint32_t num = va_arg(ap,uint32_t);
          if(num == 0) out[putlen++] = '0';
          stk_top = 0;
          while(num) {
            stk[stk_top++] = num_set[num%16];
            num = num / 16;
          }
          while(stk_top) {
            out[putlen++] = stk[stk_top-1];
            stk_top -= 1;
          }
          break;
        }
        case 'u':{
          uint32_t num = va_arg(ap,uint32_t);
          if(num == 0) out[putlen++] = '0';
          stk_top = 0;
          while(num) {
            stk[stk_top++] = num%10 + '0';
            num = num / 10;
          }
          while(stk_top) {
            out[putlen++] = stk[stk_top-1];
            stk_top -= 1;
          }
          break;
        }        
        case 's':{
          char *str = va_arg(ap,char*);
          if(str==NULL) {
            str = "(null)";
          }
          for(int i=0;str[i]!='\0';i++) {
            out[putlen++] = str[i];
          }
          break;
        }       
        case 'c':{
          char c = va_arg(ap,int);
          out[putlen++] = c;
          break;
        }
      default:
        _putc(*fmt);
        assert(0&&"TO DO IN PRINTF");
        break;
      }
    } else {
      out[putlen] = *fmt;
      putlen++;
    }
    fmt++;
  }
  out[putlen] = '\0';
  return putlen;
}

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  char buf[(1<<10)+2];
  int n = vsprintf(buf,fmt,args);
  if(n>(1<<10)) {
    assert(0&&"PRINTF EXCEED STACK");
    return -1;
  }
  for(int i=0;i<n;i++) {
    _putc(buf[i]);
  }
  buf[n] = '\0';
  va_end(args);
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return vsnprintf(out,-1,fmt,ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  int n = vsprintf(out,fmt,args);
  va_end(args);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  int len = (int)vsnprintf(out,n,fmt,args);
  va_end(args);
  return len;
}

#endif
