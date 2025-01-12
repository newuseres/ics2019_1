#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int i = 0;
  while(s[i]){
    i++;
  }
  return i;
}

char *strcpy(char* dst,const char* src) {
  int i = 0;
  for(;;i++) {
    dst[i] = src[i];
    if(!src[i]) break;
  }
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  int i = 0;
  for(;i<n;i++) {
    dst[i] = src[i];
    if(!src[i]) break;
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  int len = strlen(dst);
  int i=0;
  for(;;i++){
    dst[len+i] = src[i];
    if(src[i]==0) break;  
  }
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  int i=0;
  for(;;i++) {
    if(s1[i]==0&&s2[i]==0) return 0;
    else if(s1[i]!=s2[i]) {
      break;
    }
  }
  return (s1[i] > s2[i] ? 1:-1 );
}

int strncmp(const char* s1, const char* s2, size_t n) {
  int i=0;
  for(;i<n;i++) {
    if(s1[i]==0&&s2[i]==0) return 0;
    else if(s1[i]!=s2[i]) {
      return (s1[i] > s2[i] ? 1:-1 );
    }
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  for(int i=0;i<n;i++) {
      *((unsigned char*)v+i) = (unsigned char)c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  for(int i=0;i<n;i++) {
      *((unsigned char*)out+i) = *((unsigned char*)in+i);
  }
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  for(int i=0;i<n;i++) {
    if(*((unsigned char*)s1+i) != *((unsigned char*)s2+i)) {
      *((unsigned char*)s1+i) > *((unsigned char*)s2+i);
    }
  }
  return 0;
}

#endif
