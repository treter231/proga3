#ifndef CODER
#define CODER

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

enum {
    MaxCodeLength = 4
};
typedef struct {
    uint8_t code[MaxCodeLength]; // code — закодированные байты, в порядке от старшего к младшему.
    size_t length;               // length — количество байт в закодированном представлении.
} CodeUnits;

int encode(uint32_t code_point, CodeUnits *code_units); //работает
uint32_t decode(CodeUnits *code_units); //работает
int read_next_code_unit(FILE *in, CodeUnits *code_units); //работает
int write_code_unit(FILE *out, CodeUnits *code_units); //работает

#endif