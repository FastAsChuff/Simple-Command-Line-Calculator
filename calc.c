#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include </home/simon/brackets.c>

// gcc calc.c -o calc.bin -O3 -march=native -lm -Wall

char* doubletostring(double num) {
  char buffer[1000];
  int bufsize = sizeof(buffer)/sizeof(buffer[0]);
  snprintf(buffer, bufsize, " %.20f ", num);
  uint64_t chars = strlen(buffer);
  char *res = malloc(chars+1);
  memcpy(res, buffer, chars);
  res[chars] = 0;
  return res;  
}

char* errorstring() {
  static char* errorstring = "Error";
  char *res = malloc(6);
  memcpy(res, errorstring, 5);
  res[5] = 0;
  return res;  
}

char* strcatnew(char *string1, char *string2) {
  // Creates new string which is the concatenation of string1 and string2.
  int64_t string1len = strlen(string1);
  int64_t string2len = strlen(string2);
  uint64_t chars = string1len + string2len;
  char *res = malloc(chars+1);
  memcpy(res, string1, string1len);
  memcpy(res + string1len, string2, string2len);
  res[chars] = 0;
  return res;  
}

const double log2const = 1.0/logl(2);
const double log10const = 1.0/logl(10);
const double pi = 3.14159265358979323846264338;
const double e = 2.718281828459045235360287;

char* evalstring(char *string) {
  double left, right;
  uint64_t leftu64, rightu64;
  char operator, extra, extra2;
  if (sscanf(string, "%lf %c %c", &left, &extra, &extra2) == 2) {
    if (extra == '!') return doubletostring(expl(lgamma(left+1)));
  }
  if (strcmp(string, "e") == 0) return doubletostring(e);
  if (strcmp(string, "pi") == 0) return doubletostring(pi);
  if (sscanf(string, "degrees %lf %c", &left, &extra) == 1) return doubletostring(180*left/pi);
  if (sscanf(string, "radians %lf %c", &left, &extra) == 1) return doubletostring(pi*left/180);
  if (sscanf(string, "sin %lf %c", &left, &extra) == 1) return doubletostring(sinl(left));
  if (sscanf(string, "cos %lf %c", &left, &extra) == 1) return doubletostring(cosl(left));
  if (sscanf(string, "tan %lf %c", &left, &extra) == 1) return doubletostring(tanl(left));
  if (sscanf(string, "asin %lf %c", &left, &extra) == 1) return doubletostring(asinl(left));
  if (sscanf(string, "acos %lf %c", &left, &extra) == 1) return doubletostring(acosl(left));
  if (sscanf(string, "atan %lf %c", &left, &extra) == 1) return doubletostring(atanl(left));
  if (sscanf(string, "sqrt %lf %c", &left, &extra) == 1) return doubletostring(sqrt(left));
  if (sscanf(string, "ln %lf %c", &left, &extra) == 1) return doubletostring(logl(left));
  if (sscanf(string, "log2 %lf %c", &left, &extra) == 1) return doubletostring(log2const*logl(left));
  if (sscanf(string, "log10 %lf %c", &left, &extra) == 1) return doubletostring(log10const*logl(left));
  if (sscanf(string, " %lf %c", &left, &extra) == 1) return doubletostring(left);
  if (sscanf(string, " %lf %c %lf %c", &left, &operator, &right, &extra) != 3) return errorstring();
  if (operator == '+') return doubletostring(left + right);
  if (operator == '-') return doubletostring(left - right);
  if (operator == '*') return doubletostring(left * right);
  if (operator == '/') return doubletostring(left / right);
  if (operator == '^') return doubletostring(pow(left, right));
  if (operator == '%') {
    leftu64 = floor(left);
    if (leftu64 != left) return errorstring();
    rightu64 = floor(right);
    if (rightu64 != right) return errorstring();
    return doubletostring(leftu64 % rightu64);
  }
  if ((operator == '&') || (operator == '|')) {
    leftu64 = floor(left);
    if (leftu64 != left) return errorstring();
    rightu64 = floor(right);
    if (rightu64 != right) return errorstring();
    if (operator == '&') return doubletostring(leftu64 & rightu64);
    if (operator == '|') return doubletostring(leftu64 | rightu64);
  }
  return errorstring();
}

int main(int argc, char**argv) {
  //char *result = evalstring(argv[1]);
  //printf("%s\n", result);
  //exit(0);
  char *leftbracket;
  char *rightbracket;
  if (!goodbracketing(argv[1])) {
    printf("Bad Bracketing!\n");
    exit(1);
  }
  char *unbracketed, *replacementstring, *newstring, *string = strdup(argv[1]);
  while (findfirstbracketedsubstring(string, &leftbracket, &rightbracket)) {
    unbracketed = extractbracketedsubstring(string, leftbracket, rightbracket);
    replacementstring = evalstring(unbracketed);
    newstring = substitutebracketedsubstring(string, leftbracket, rightbracket, replacementstring);
    free(string);
    free(unbracketed);
    free(replacementstring);
    string = newstring;
  }
  newstring = evalstring(string);
  printf("%s\n", newstring);
  free(string);
  free(newstring);  
}
