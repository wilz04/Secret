#ifndef Crypto_cpp
#define Crypto_cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long int hex;

char *bin(char *text) {
  char *newS = (char*) malloc(strlen(text)*8);
  newS[0] = '\0';
  for (int i=0; i<strlen(text); i++) {
    char *newC = (char*) malloc(64);
    ltoa(text[i], newC, 2);
    newS = strcat(newS, newC);
  }
  return newS;
}

char *pad(char *text) {
  int i = strlen(text);
  while ((i == 0) || (i%16 != 0)) {
    text[i] = '0';
    text[++i] = '\0';
  }
  return text;
}

bool boolean(char c) {
  if (c == '0') {
    return false;
  } else {
    return true;
  }
}

bool f(int x, int y, int z) {
  x = boolean(x);
  y = boolean(y);
  z = boolean(z);
  return (x & y) | (!x & z);
}

bool g(int x, int y, int z) {
  x = boolean(x);
  y = boolean(y);
  z = boolean(z);
  return (x & z) | (y & !z);
}

bool h(int x, int y, int z) {
  x = boolean(x);
  y = boolean(y);
  z = boolean(z);
  return x ^ y ^ z;
}

bool i(int x, int y, int z) {
  x = boolean(x);
  y = boolean(y);
  z = boolean(z);
  return (x | !z) ^ y;
}

hex ff(hex w, hex x, hex y, hex z, int k) {
  w += f(x, y, z) + boolean(k);
  w <<= 5;
  w += x;
  return w;
}

hex gg(hex w, hex x, hex y, hex z, int k) {
  w += g(x, y, z) + boolean(k);
  w <<= 10;
  w += x;
  return w;
}

hex hh(hex w, hex x, hex y, hex z, int k) {
  w += h(x, y, z) + boolean(k);
  w <<= 15;
  w += x;
  return w;
}

hex ii(hex w, hex x, hex y, hex z, int k) {
  w += i(x, y, z) + boolean(k);
  w <<= 20;
  w += x;
  return w;
}

char *crypto(char *text) {
  text = bin(text);
  text = pad(text);
  
  hex a = 0x01234567;
  hex b = 0x89ABCDEF;
  hex c = 0xFEDCBA98;
  hex d = 0x76543210;
  
  hex oldA;
  hex oldB;
  hex oldC;
  hex oldD;

  for(int i=0; i<strlen(text); i+=16) {
    oldA = a;
    oldB = b;
    oldC = c;
    oldD = d;
    
    a = ff(a, b, c, d, text[i]);
    b = ff(b, a, b, c, text[i+1]);
    c = ff(c, b, a, d, text[i+2]);
    d = ff(d, c, b, a, text[i+3]);
    a = ff(a, d, c, b, text[i+4]);
    b = ff(b, a, d, c, text[i+5]);
    c = ff(c, b, a, d, text[i+6]);
    d = ff(d, c, b, a, text[i+7]);
    a = ff(a, d, c, b, text[i+8]);
    b = ff(b, a, d, c, text[i+9]);
    c = ff(c, b, a, d, text[i+10]);
    d = ff(d, c, b, a, text[i+11]);
    a = ff(a, d, c, b, text[i+12]);
    b = ff(b, a, d, c, text[i+13]);
    c = ff(c, b, a, d, text[i+14]);
    d = ff(d, c, b, a, text[i+15]);
    
    a = gg(a, b, c, d, text[i]);
    b = gg(b, a, b, c, text[i+1]);
    c = gg(c, b, a, d, text[i+2]);
    d = gg(d, c, b, a, text[i+3]);
    a = gg(a, d, c, b, text[i+4]);
    b = gg(b, a, d, c, text[i+5]);
    c = gg(c, b, a, d, text[i+6]);
    d = gg(d, c, b, a, text[i+7]);
    a = gg(a, d, c, b, text[i+8]);
    b = gg(b, a, d, c, text[i+9]);
    c = gg(c, b, a, d, text[i+10]);
    d = gg(d, c, b, a, text[i+11]);
    a = gg(a, d, c, b, text[i+12]);
    b = gg(b, a, d, c, text[i+13]);
    c = gg(c, b, a, d, text[i+14]);
    d = gg(d, c, b, a, text[i+15]);
    
    a = hh(a, b, c, d, text[i]);
    b = hh(b, a, b, c, text[i+1]);
    c = hh(c, b, a, d, text[i+2]);
    d = hh(d, c, b, a, text[i+3]);
    a = hh(a, d, c, b, text[i+4]);
    b = hh(b, a, d, c, text[i+5]);
    c = hh(c, b, a, d, text[i+6]);
    d = hh(d, c, b, a, text[i+7]);
    a = hh(a, d, c, b, text[i+8]);
    b = hh(b, a, d, c, text[i+9]);
    c = hh(c, b, a, d, text[i+10]);
    d = hh(d, c, b, a, text[i+11]);
    a = hh(a, d, c, b, text[i+12]);
    b = hh(b, a, d, c, text[i+13]);
    c = hh(c, b, a, d, text[i+14]);
    d = hh(d, c, b, a, text[i+15]);
    
    a = ii(a, b, c, d, text[i]);
    b = ii(b, a, b, c, text[i+1]);
    c = ii(c, b, a, d, text[i+2]);
    d = ii(d, c, b, a, text[i+3]);
    a = ii(a, d, c, b, text[i+4]);
    b = ii(b, a, d, c, text[i+5]);
    c = ii(c, b, a, d, text[i+6]);
    d = ii(d, c, b, a, text[i+7]);
    a = ii(a, d, c, b, text[i+8]);
    b = ii(b, a, d, c, text[i+9]);
    c = ii(c, b, a, d, text[i+10]);
    d = ii(d, c, b, a, text[i+11]);
    a = ii(a, d, c, b, text[i+12]);
    b = ii(b, a, d, c, text[i+13]);
    c = ii(c, b, a, d, text[i+14]);
    d = ii(d, c, b, a, text[i+15]);
    
    a += oldA;
    b += oldB;
    c += oldC;
    d += oldD;
  }
  
  char *result = (char*) malloc(256);
  result[0] = '\0';
  
  char *temp = (char*) malloc(64);
  
  ltoa(a, temp, 16);
  strcat(result, temp);
  ltoa(b, temp, 16);
  strcat(result, temp);
  ltoa(c, temp, 16);
  strcat(result, temp);
  ltoa(d, temp, 16);
  strcat(result, temp);
  
  return result;
}

#endif

