# compile
```
gcc -c *.c
```
or 
```
gcc *.c
```
# run 
```
./a.out
```


# split file and run
1. you create numberOf file 
    - example `sum.c , sum.h , main.c`

2. you switch directory to file in `sum.c , sum.h , main.c`

3. you do compile file command
```
gcc -c sum.c && gcc -c main.c  
```

4. bind and run
```
gcc -o 1102 sum.o main.o && ./1102
```

## sample

### main.c
```
#include "sum.h"

int main(void) {
    int n1 = 4,n2=5;
    double x=3.657,y=9.3445;
    printf("sum: %f \n",sum(x,y));
    printf("avg: %f \n" , avg(x,y));
    printf("test \n");
}
```
### sum.c
```
#include "sum.h"

double sum(double x,double y){
    return x + y;
} 

double avg(double x, double y){
    return (x+y)/2;
}
```

### sum.h
```
#include <stdio.h>
#ifndef _SUM_H_
#define _SUM_H_

// protType 
double sum(double,double);
double avg(double,double);

#endif // _SUM_H_
```