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


void IIRFilter_Init(IIRFilterState *state) {
    // 初始化缓冲区为 0
    for (int i = 0; i <= FILTER_ORDER; i++) {
        state->x[i] = 0.0;
        state->y[i] = 0.0;
    }
}

double IIRFilter_Process(IIRFilterState *state, double input) {
    // 更新输入缓冲区
    for (int i = FILTER_ORDER; i > 0; i--) {
        state->x[i] = state->x[i - 1];
    }
    state->x[0] = input;

    // 计算输出
    double output = 0.0;

    // 分子部分
    for (int i = 0; i <= FILTER_ORDER; i++) {
        output += NUM[i] * state->x[i];
    }

    // 分母部分（跳过 a[0] = 1）
    for (int i = 1; i <= FILTER_ORDER; i++) {
        output -= DEN[i] * state->y[i - 1];
    }

    // 更新输出缓冲区
    for (int i = FILTER_ORDER; i > 0; i--) {
        state->y[i] = state->y[i - 1];
    }
    state->y[0] = output;

    return output;
}
