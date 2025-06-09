#include <stdio.h>
#include <getopt.h>
extern double func1(double x);
extern double func2(double x);
extern double func3(double x);

double l = -5.0;
double r = -0.25;

double root(double (*f1)(double), double (*f2)(double), double l, double r, double eps);

#ifdef SPLIT // поиск корня методом разделения отрезка пополам
// по сути своей обычный бинпоиск
double root(double (*f1)(double), double (*f2)(double), double l, double r, double eps){
    if (r - l < eps) return (l + r)/2;
    double l_val = f1(l) - f2(l);
    double r_val = f1(r) - f2(r);
    double m_val = f1((l + r) / 2) - f2((l + r) / 2);
    if (l_val > 0) {
        if (m_val > 0) return root(f1, f2, (l + r) / 2, r, eps);
        else return root(f1, f2, l, (l + r) / 2, eps);
    }
    else {
        if (m_val > 0) return root(f1, f2, l, (l + r) / 2, eps);
        else return root(f1, f2, (l + r) / 2, r, eps);
    }
}
#endif

#ifdef CHORD // поиск корня методом хорд
//использует формулу точки пересечения прямой, проходящей  через две данные точки, и оси x
double root(double (*f1)(double), double (*f2)(double), double l, double r, double eps){
    double l_val = f1(l) - f2(l);
    double r_val = f1(r) - f2(r);

}
#endif

#ifdef TANGENT
double root(double (*f1)(double), double (*f2)(double), double l, double r, double eps){

}
#endif

#ifdef MIX
double root(double (*f1)(double), double (*f2)(double), double l, double r, double eps){

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
    return 0;
}