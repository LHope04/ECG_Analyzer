#ifndef QEA2_QUEUE_H
#define QEA2_QUEUE_H

#endif 

extern struct queue testQ;
extern void queueInit(struct queue*, int [], int);
extern void queuePush(struct queue* q, int element);
extern int queuePop(struct queue* q);
extern void queuePrint(struct queue* q);

struct queue {
    int count;
    int* array;
    int front;
    int rear;
    int maxSize;
};
