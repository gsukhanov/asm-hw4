#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
extern double func1(double x);
extern double func2(double x);
extern double func3(double x);

#ifdef HARD_BUILD
extern double _l;
extern double _r;
#else
double _l = -5.0;
double _r = -0.25;
#endif
double _eps = 0.00001;

static unsigned root_iterations = 0; // Variable to count the number of iterations required to find the roots
static unsigned integral_iterations = 0; // Same for integrals

typedef double afunc(double);

#ifndef CHORD // This defines a search method using middle-splitting of the search interval.
double root(afunc *f1, afunc *f2, double l, double r, double eps){
    double l_val = f1(l) - f2(l);
    double r_val = f1(r) - f2(r);
    double m = (l + r) / 2;
    double m_val = f1(m) - f2(m);
    while(r - l > eps) {
        root_iterations++;
        if (l_val > 0) {
            if (m_val > 0) {
                l = m;
                l_val = m_val;
            }
            else {
                r = m;
                r_val = m_val;
            }
        }
        else {
            if (m_val > 0) {
                r = m;
                r_val = m_val;
            }
            else {
                l = m;
                l_val = m_val;
            }
        }
        m = (l + r) / 2;
        m_val = f1(m) - f2(m);
    }
    return m;
}
#endif

#ifdef CHORD // This defines a search method using intersections of chords and the x-axis.
double root(afunc *f1, afunc *f2, double l, double r, double eps){
    double l_val = f1(l) - f2(l);
    double r_val = f1(r) - f2(r);
    double m = l - l_val * (r - l) / (r_val - l_val);
    double m_val = f1(m) - f2(m);
    double m_plus_eps_val = f1(m + eps) - f2(m + eps);
    double m_minus_eps_val = f1(m - eps) - f2(m - eps);
    while (1) {
        root_iterations++;
        if (root_iterations > 1000) { // It may happen that no root will be found but the cycle will still keep going,
            fprintf(stderr, "Error! No root found on the interval!\n"); // Therefore, it is forcefully stopped after 1000 iterations.
            exit(-1);
        }
        if (m_val * m_minus_eps_val < 0) return (2 * m - eps) / 2;
        if (m_val * m_plus_eps_val < 0) return (2 * m + eps) / 2;
        if (l_val > 0) {
            if (m_val > 0) {
                l = m;
                l_val = m_val;
            }
            else {
                r = m;
                r_val = m_val;
            }
        }
        else {
            if (m_val > 0) {
                r = m;
                r_val = m_val;
            }
            else {
                l = m;
                l_val = m_val;
            }
        }
        m = l - l_val * (r - l) / (r_val - l_val);
        m_val = f1(m) - f2(m);
        m_plus_eps_val = f1(m + eps) - f2(m + eps);
        m_minus_eps_val = f1(m - eps) - f2(m - eps);
    }
}
#endif

inline double fabs(double x) { // Did not want to include an extra header
    return x > 0? x: -x;
}

double integral(afunc *f, double a, double b, double eps) { // Integral is counted using the rectangle method
    double I = 0, I2 = 0;
    unsigned n = 10;
    double h = (b - a) / n;
    for (int i = 0; i < n; i++) I += f(a + (i + 0.5) * h);
    I *= h;
    n *= 2;
    h = (b - a) / n;
    for (int i = 0; i < n; i++) I2 += f(a + (i + 0.5) * h);
    I2 *= h;
    int iter = 0;
    integral_iterations++;
    while(fabs((I2 - I) / 3) > eps) {
        I = I2;
        I2 = 0;
        n *= 2;
        h = (b - a) / n;
        for (int i = 0; i < n; i++) I2 += f(a + (i + 0.5) * h);\
        I2 *= h;
        integral_iterations++;
    }
    return I2;
}

int main(int argc, char ** argv) {
#ifndef TEST_LAUNCH
// If you launch the programme in default mode, the TEST_LAUNCH flag will not be defined, and therefore you get this main:
    const struct option long_options[] = {
        {"help", 0, 0, 'h'}, 
        {"root", 0, 0, 'r'}, 
        {"iterations", 0, 0, 'i'}, 
        {"test-root", 1, 0, 'R'}, 
        {"test-integral", 1, 0, 'I'}
    };
    char flag = 1; // This flag is required to print the area between the curves without any command line options.
// Here is a cycle to work with multiple command line options
    while (1) {
        int c = getopt_long(argc, argv, "hriR:I:", long_options, 0);
        if (c == -1 && flag) {
            double root1 = root(func1, func2, _l, _r, _eps);
            double root2 = root(func2, func3, _l, _r, _eps);
            double root3 = root(func3, func1, _l, _r, _eps);
            printf("The area of the figure is: %lf\n", fabs(integral(func1, root1, root3, _eps) + integral(func3, root3, root2, _eps) + integral(func2, root2, root1, _eps)));
            break;
        }
        else if (c == -1) break;
        else flag = 0;
        int f1 = 0, f2 = 0;
        double A, B, E, R;
        switch(c) {
            case 'h':
                printf("Available command line options:\n");
                printf("--help -h: Display all available command line options.\n");
                printf("--root -r: Display intersection points of functions.\n");
                printf("--iterations -i: Display the number of iterations required to find intersection points and integrals.\n");
                printf("--test-root -R: Test the root function. Format: -R F1:F2:A:B:E:R, F1, F2 - function indexes, A, B - limits, E - prescision, R - expected result.\n");
                printf("--test-integral -I: Test the root function. Format: -I F:A:B:E:R, F, - function index, A, B - limits, E - prescision, R - expected result.\n");
                printf("If launched without any options, the programme will calculate the area limited by the three functions.\n");
                break;
            case 'r':
                printf("Roots: %lf %lf %lf\n", root(func1, func2, _l, _r, _eps), root(func1, func3, _l, _r, _eps), root(func2, func3, _l, _r, _eps));
                break;
            case 'i':
                printf("Number of iterations:");
                double root1 = root(func1, func2, _l, _r, _eps);
                printf("Roots: %u ", root_iterations);
                root_iterations = 0;
                double root2 = root(func1, func3, _l, _r, _eps);
                printf("%u ", root_iterations);
                root_iterations = 0;
                double root3 = root(func2, func3, _l, _r, _eps);
                printf("%u\n", root_iterations);
                root_iterations = 0;
                integral(func1, root1, root3, _eps);
                printf("Integrals: %u ", integral_iterations);
                integral_iterations = 0;
                integral(func3, root3, root2, _eps);
                printf("%u ", integral_iterations);
                integral_iterations = 0;
                integral(func2, root2, root1, _eps);
                printf("%u\n", integral_iterations);
                integral_iterations = 0;
                break;
            case 'R':
                sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &f1, &f2, &A, &B, &E, &R);
                if ((f1 == 1 && f2 == 2) || (f1 == 2 && f2 == 1)) {
                    double r = root(func1, func2, A, B, E);
                    printf("%lf %lf %lf\n", r, fabs(r - R), fabs((r - R)/ R));
                }
                else if ((f1 == 1 && f2 == 3) || (f1 == 3 && f2 == 1)) {
                    double r = root(func1, func3, A, B, E);
                    printf("%lf %lf %lf\n", r, fabs(r - R), fabs((r - R)/ R));
                }
                else if ((f1 == 2 && f2 == 3) || (f1 == 3 && f2 == 2)) {
                    double r = root(func2, func3, A, B, E);
                    printf("%lf %lf %lf\n", r, fabs(r - R), fabs((r - R)/ R));
                }
                else {
                    fprintf(stderr, "Error! Wrond function index! Function indexes are 1, 2, 3.\n");
                    exit(-1);
                }
                f1 = 0;
                break;
            case 'I':
                sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &f1, &A, &B, &E, &R);
                if (f1 == 1) {
                    double i = integral(func1, A, B, E);
                    printf("%lf %lf %lf\n", i, fabs(i - R), fabs((i - R)/ R));
                }
                else if (f1 == 2) {
                    double i = integral(func2, A, B, E);
                    printf("%lf %lf %lf\n", i, fabs(i - R), fabs((i - R)/ R));
                }
                else if (f1 == 3) {
                    double i = integral(func3, A, B, E);
                    printf("%lf %lf %lf\n", i, fabs(i - R), fabs((i - R)/ R));
                }
                else {
                    fprintf(stderr, "Error! Wrond function index! Function indexes are 1, 2, 3.\n");
                    exit(-1);
                }
                break;
            default:
                fprintf(stderr, "Error! Unknown argument! To see available arguments use -h.");
                exit(-1);
        }
    }
#else

// If the flag is not defined, you get this test mode main: functions are x^sinx, cosx and x - 6. 28.7053
// You can set limits and prescision to get varying results (two different figures exist in intersections of those).
    printf("Set left root search limit: ");
    scanf("%lf", &_l);
    printf("Set right root search limit: ");
    scanf("%lf", &_r);
    printf("Set prescision of root and integral search: ");
    scanf("%lf", &_eps);
    double root1 = root(func1, func2, _l, _r, _eps);
    double root2 = root(func2, func3, _l, _r, _eps);
    double root3 = root(func3, func1, _l, _r, _eps);
    printf("The intersection point of functions: %lf %lf %lf\n", root1, root2, root3);
    printf("The expected roots are either 2*pi, 6.84584, 8.91396 or 11.08681, 6.84584, 8.91396 depending ont the limits.\n");
    printf("The area of the figure is: %lf\n", fabs(integral(func1, root1, root3, _eps) + integral(func3, root3, root2, _eps) + integral(func2, root2, root1, _eps)));
    printf("Expected values of the figure are either 8.543606 or 10.51035 depending on the limiits.\n");
#endif

    return 0;
}