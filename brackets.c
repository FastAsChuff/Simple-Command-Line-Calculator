_Bool goodbracketing(char *string) {
  int64_t stringlen = strlen(string);
  int64_t expressions = 0;
  for (uint64_t i = 0; i<stringlen; i++) {
    if (string[i] == ')') expressions--;
    if (string[i] == '(') expressions++;
    if (expressions < 0) return false;
  }
  if (expressions == 0) return true;
  return false;
}

_Bool findfirstbracketedsubstring(char *string, char **leftbracket, char **rightbracket) {
  char *rightbracketlocal = strchr(string, ')');
  char *leftbracketlocal;
  if (rightbracketlocal == NULL) {
    return false;
  } else {
    if (rightbracketlocal == string) return false;
    for (leftbracketlocal = rightbracketlocal - 1; leftbracketlocal - string > 0; leftbracketlocal--) {
      if (*leftbracketlocal == '(') {
        *leftbracket = leftbracketlocal;
        *rightbracket = rightbracketlocal;
        return true;
      }
    }
    if (*leftbracketlocal == '(') {
      *leftbracket = leftbracketlocal;
      *rightbracket = rightbracketlocal;
      return true;
    }
    return false;
  }
}

char *extractbracketedsubstring(char *string, char *leftbracket, char *rightbracket) {
  // Make new null terminated string from characters between leftbracket and rightbracket.
  uint64_t chars = rightbracket - leftbracket - 1;
  char *res = malloc(chars+1);
  if (res == NULL) return NULL;
  memcpy(res, leftbracket+1, chars);
  res[chars] = 0;
  return res;
}

char *substitutebracketedsubstring(char *string, char *leftbracket, char *rightbracket, char *substitute) {
  // Make new null terminated string from characters before leftbracket, substitute string, and after rightbracket.
  int64_t stringlen = strlen(string);
  int64_t substitutelen = strlen(substitute);
  uint64_t chars = substitutelen + stringlen - (rightbracket - leftbracket + 1);
  char *res = malloc(chars+1);
  char *pos;
  memcpy(res, string, leftbracket-string);
  pos = res + (leftbracket - string);
  memcpy(pos, substitute, substitutelen);
  pos += substitutelen;
  memcpy(pos, rightbracket+1, stringlen - (rightbracket - string) - 1);
  res[chars] = 0;
  return res;
}

