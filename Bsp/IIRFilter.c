#include "IIRFilter.h"
#include "main.h"
#include "stdio.h"

//IIR滤波器相关参数
float a = 0.991;
int last_input = 0;
int last_output = 0;
uint16_t cur_input = 0;
int cur_output = 0;
int16_t IIR_Result;

void IIRFilter(uint16_t rawData){
    cur_input = rawData;
    cur_output = cur_input - last_input + a * last_output;
    IIR_Result = (int16_t)cur_output;
    //printf("%d\n",IIR_Result);
    last_output = cur_output;
    last_input = cur_input;
}
