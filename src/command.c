#include "command.h"
#include "coder.h"

int encode_file(char *in_file_name, char *out_file_name) { //Кодирование текстового файла в 
    uint32_t code_point;                                   // бинарный, реализация команды encode.
    CodeUnits Cod;
    CodeUnits *code_units = &Cod;
    
    FILE *in = fopen(in_file_name,"r");
    FILE *out = fopen(out_file_name,"wb");

    if (in == NULL) {                     // 0 в случае успеха, -1 в случае ошибки
        printf("ERROR: encode_file\n");
        return -1;
    } 
    
    if (out == NULL) {
        printf("ERROR: encode_file\n");
        return -1;    
    } 
    while(fscanf(in, "%" SCNx32, &code_point) != EOF) {
        if (encode(code_point, code_units) == 0) {
            write_code_unit(out, code_units);
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}

int decode_file(char *in_file_name, char *out_file_name) { //Декодирование бинарного файла, 
    FILE *in = fopen(in_file_name,"rb");                   // реализация команды decode.
    FILE *out = fopen(out_file_name,"w");
    CodeUnits Cod;
    CodeUnits *code_units = &Cod;

    if (in == NULL) {                      // 0 в случае успеха, -1 в случае ошибки
        printf("ERROR: decode_file\n");
        return -1;
    }   
    if (out == NULL) {
        printf("ERROR: decode_file\n");
        return -1;    
    }
    while(read_next_code_unit(in, code_units) == 0 ) {
        fprintf(out, "%x", decode(code_units));
    }
    fclose(in);
    fclose(out);
    
    return 0;
}