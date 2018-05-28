#include <math.h>
#include "coder.h"

int encode(uint32_t code_point, CodeUnits *code_units) { //сode_point – число, необходимо закодировать
    uint32_t temp1 = pow(2, 7);                          //code_units – результат кодирования
    if (code_point < temp1) {
        code_units->length = 1;
        (code_units->code)[0] = code_point;
        (code_units->code)[1] = 0;
        (code_units->code)[2] = 0;
        (code_units->code)[3] = 0;
        return 0;                                        //	0, если кодирование успешно, иначе -1
    }
    temp1 = pow(2, 11);
    if (code_point < temp1) {
        code_units->length = 2;
        (code_units->code)[0] = 192 | (code_point >> 6);
        (code_units->code)[1] = 128 | (63 & code_point);
        (code_units->code)[2] = 0;
        (code_units->code)[3] = 0;
        return 0;                                         // 0, если кодирование успешно, иначе -1
    }
    temp1 = pow(2, 16);
    if (code_point < temp1) {
        code_units->length = 3;
        (code_units->code)[0] = 224 | (code_point >> 12);
        (code_units->code)[1] = 128 | ((code_point & 4095) >> 6);
        (code_units->code)[2] = 128 | (code_point & 63);
        (code_units->code)[3] = 0;
        return 0;                                          // 0, если кодирование успешно, иначе -1
    }
    temp1 = pow(2, 21);
    if (code_point < temp1) {
        code_units->length = 4;
        (code_units->code)[0] = 240 | (code_point >> 18);
        (code_units->code)[1] = 128 | ((code_point & 262143) >> 12);
        (code_units->code)[2] = 128 | ((code_point & 4095) >> 6);
        (code_units->code)[3] = 128 | (code_point & 63);
        return 0;  //	0, если кодирование успешно, иначе -1
    }
    return -1;  
}


uint32_t decode(CodeUnits *code_units) { //Допущение code_unit - корректный код, 
    uint32_t result = 0;        //полученный с помощью функции read_next_code_unit.
    uint32_t part1 = 0;
    uint32_t part2 = 0;
    uint32_t part3 = 0;
    uint32_t part4 = 0;

    if(code_units->length == 1) {  //code_unit – закодированное представление числа
        result = (code_units->code)[0];

        return result;  
    } 
    if(code_units->length == 2) {
        part1 = ((code_units->code)[0] & 31) << 6;
        part2 = (code_units->code)[1] & 63;

        result = part1 | part2;
        return result;  
    }
    if(code_units->length == 3) {
        part1 = ((code_units->code)[0] & 15) << 12;
        part2 = ((code_units->code)[1] & 63) << 6;
        part3 = (code_units->code)[2] & 63;

        result = part1 | part2 | part3;
        return result;  
   }
    if(code_units->length == 4) {
        part1 = ((code_units->code)[0] & 7) << 18;
        part2 = ((code_units->code)[1] & 63) << 12;
        part3 = ((code_units->code)[2] & 63) << 6;
        part4 = (code_units->code)[3] & 63;

        result = part1 | part2 | part3 | part4;
        return result;
    }
    return -1;  
}

uint8_t mask1 = 128, mask2 = 192, mask3 = 224, mask4 = 240;

int read_next_code_unit(FILE *in, CodeUnits *code_units) {  // Считывает последовательность code_units (Структура CodeUnits и указатель *code_inits) 
    code_units->length = 0;                      //обращение к длине числа, кол-ву байт     // 
    uint8_t * value = malloc(sizeof(uint8_t));   //выделение памяти                         // из потока in.
    int need = MaxCodeLength + 1;                //присваивание need MaxCodeLength + 1;     // если считываемый code_unit битый, то 
                                                                                            // функция пропускает байты до тех пор, 
    while(need != (code_units->length)) {        //пока need не равна длине числа           // пока не найдет корректный лидирующий байт.
    
        size_t rez = fread(value, sizeof(uint8_t), 1, in);  // считываем число из in
        if (rez != 1) {                                     // сравнение rez с 1
            return -1;                                      // возврат -1 в случае ошибки или EOF
        }
        if (*value <= 127) {                                // Пока указатель меньше или равен 127
            code_units->code[0] = *value;                   // обращаемся к нулевому эллементу поля code структуры code_units и присваиваем ему *value
            code_units->length = 1;                         // обращаемся к полю length структуры code_units и присваиваем 1

            return 0;                                       // 0 в случае успеха
        }   
        if (~(*value) & mask1>>1) {                              // пока обратный битовый указатель *value и mask1 сдвигаются на 1 
            if (code_units->length) {                            // пока обращаемся к полю length структуры code_units  
                code_units->code[code_units->length] = *value;   // обращаемся к полю code структуры code_units при этом обращаемся к полю length структуры code_units
                code_units->length += 1;                         // обращаемся к полю length структуры code_units и добавляем 1

                continue;
            } else {                                             // иначе
                free(value);                                     // освобождаем память
                read_next_code_unit(in, code_units);             // вызываем функцию для чтения полученного числа в файл in
            }
        }               
        if (~(*value) & mask2>>1) {                              // пока обратный битовый указатель *value и mask2 сдвигаются на 1
            need = 2;                                            // присваивание need значение 2
            code_units->code[0] = *value;                        // обращаемся к нулевому эллементу поля code структуры code_units и присваиваем ему *value
            code_units->length = 1;                              // обращаемся к полю length структуры code_units и присваиваем 1

            continue;
        }             
        if (~(*value) & mask3>>1) {                              // пока обратный битовый указатель *value и mask3 сдвигаются на 1
            need = 3;                                            // присваивание need значение 3
            code_units->code[0] = *value;                        // обращаемся к нулевому эллементу поля code структуры code_units и присваиваем ему *value
            code_units->length = 1;                              // обращаемся к полю length структуры code_units и добавляем 1

            continue;
        }
        if (~(*value) & mask4>>1) {                              // пока обратный битовый указатель *value и mask4 сдвигаются на 1
            need = 4;                                            // присваивание need значение 4
            code_units->code[0] = *value;                        // обращаемся к нулевому эллементу поля code структуры code_units и присваиваем ему *value
            code_units->length = 1;                              // обращаемся к полю length структуры code_units и добавляем 1

            continue;
        }
    }
    free(value);                                                 // Освобождение памяти
    return 0;                                                    // 0 в случае успеха
}

int write_code_unit(FILE *out, CodeUnits *code_units) {
    size_t result = fwrite(code_units->code , 1 , code_units->length, out);
    if(result != code_units->length) {
        printf("ERROR: write_code_unit\n");
        return -1;
    }
    return 0;                                         //0 в случае успеха, -1 в случае ошибки
}
