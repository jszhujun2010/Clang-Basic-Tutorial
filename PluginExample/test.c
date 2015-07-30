//The header is commented because handling header needs additional effort.
//#include <stdio.h>

void do_math(int *x) {
  *x += 5;
}

int main(void) {
  int result = -1, val = 4;
  do_math(&val);
  return result;
}
