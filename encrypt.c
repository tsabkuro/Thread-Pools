#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>

int main(int argc, char *argv[]) {
  struct crypt_data data;
  puts(crypt_r(argv[1], argv[2], &data));
  puts(crypt_r("99999999", "aaGDb/tGmFYl.", &data));
  return 0;
}

//   // struct crypt_data data;
//   //puts(crypt_r("1234", "$5$12345", &data));    same thing
//   //puts(crypt_r("1234", "$5$12345$aBMyVPBFcg0A2cuP8s6ESkeqSpRL6VI8G6BOXUE8JA2", &data));  same thing


//   // puts(crypt_r("", "$5$12345", &data));
//   // puts(crypt_r("\0", "$5$12345$aBMyVPBFcg0A2cuP8s6ESkeqSpRL6VI8G6BOXUE8JA2", &data));
//   // printf("data %s\n", data.output);
