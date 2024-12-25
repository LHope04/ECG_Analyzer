//
// Created by liuxi on 2023/12/13.
//

#ifndef QEA2_FIRFILTER_H
#define QEA2_FIRFILTER_H

#endif //QEA2_FIRFILTER_H
#include "main.h"
extern int16_t FIRResult;
extern void FIRInit(void);
extern void FIRFilter(int16_t);
int heartbeat_check(int value);
double calc_heartbeat_rate(int is_heartbeat);
