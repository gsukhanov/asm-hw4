#include <stdio.h>
#include <getopt.h>
extern double func1(double x);
extern double func2(double x);
extern double func3(double x);

extern double l, r;

double calculate(double (*function)(double), double l, double r);

#ifdef SPLIT
double calculate(double (*function)(double), double l, double r){
    
}
#endif

#ifdef CHORD
double calculate(double (*function)(double), double l, double r){

}
#endif

int main(int argc, char ** argv) {
    const struct option long_options[] = {
        {"help", 0, 0, 'h'}, 
        {"root", 0, 0, 'r'}, 
        {"iterations", 0, 0, 'i'}, 
        {"test-root", 1, 0, 'R'}, 
        {"-test-integral", 1, 0, 'I'}
    };
    while (getopt_long(argc, argv, "hriR:I:", long_options, 0) != -1) {

    }
    double x;
    scanf("%lf", &x);
    printf("Which function?\n");
    int a;
    scanf("%d", &a);
    if (a == 1) printf("%lf\n", func1(x));
    else if (a == 2) printf("%lf\n", func2(x));
    else if (a == 3) printf("%lf\n", func3(x));
    else printf("No such function!\n");
    printf("%lf %lf\n", l, r);
    return 0;
}