#ifndef QEA2_IIRFILTER_H
#define QEA2_IIRFILTER_H



#include "main.h"
extern int16_t IIR_Result;
void IIRFilter(uint16_t rawData);


#include <stdint.h>

// ??????????????
#define FILTER_ORDER 2

// ????
static const double NUM[FILTER_ORDER + 1] = {
    0.9805274676617, -1.961054935168, 0.9805274676617
};

// ????
static const double DEN[FILTER_ORDER + 1] = {
    1.0, -1.961054935168, 0.9610549353233
};

// ?????
typedef struct {
    double x[FILTER_ORDER + 1]; // ?????
    double y[FILTER_ORDER + 1]; // ?????
} IIRFilterState;

// ????????
void IIRFilter_Init(IIRFilterState *state);

// ?????
double IIRFilter_Process(IIRFilterState *state, double input);

#endif //QEA2_IIRFILTER_H
