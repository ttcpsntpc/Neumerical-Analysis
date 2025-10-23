#include <stdio.h>
#include <math.h>

int point[11][11];

double f(double x, double y)
{
    return x * x / 9 + y * y / 9 - 1;
}
double g(double x, double y)
{
    return x * x - 3 * x * y + y * y - 9;
}
double fdx(double x, double y)
{
    return 2 * x / 9;
}
double fdy(double x, double y)
{
    return 2 * y / 9;
}
double gdx(double x, double y)
{
    return 2 * x - 3 * y;
}
double gdy(double x, double y)
{
    return 2 * y - 3 * x;
}
int newton(FILE *ph, double x0, double y0)
{
    double h, k, x[100], y[100];
    int n = 0;

    x[0] = x0;
    y[0] = y0;
    h = 100;
    k = 100;
    
    while (sqrt(h * h + k * k) > 0.000001)
    {
        double jacobian_coef = fdy(x[n], y[n]) * gdx(x[n], y[n]) - gdy(x[n], y[n]) * fdx(x[n], y[n]);
        if (fabs(jacobian_coef) < 0.00001)
        {
            n = 31;
            return n;
        }
        h = 1 / jacobian_coef * (gdy(x[n], y[n]) * f(x[n], y[n]) - fdy(x[n], y[n]) * g(x[n], y[n]));
        k = 1 / jacobian_coef * (-gdx(x[n], y[n]) * f(x[n], y[n]) + fdx(x[n], y[n]) * g(x[n], y[n]));
        x[n + 1] = x[n] + h;
        y[n + 1] = y[n] + k;
        n++;
        if (n > 30)
            return n;
    }

    if (n <= 30)
    {
        point[(int)(x0 + 5)][(int)(y0 + 5)] = 0;
        printf("(x, y) = (%.1lf, %.1lf) -- (%d times) --> converge in (%lf, %lf)\n", x0, y0, n, x[n], y[n]);
    }
    
    for (int k = 0; k <= n; k++)
    {
        // 輸入座標資料
        fprintf(ph, "%lf %lf\n", x[k], y[k]);
    }
    return n;
}

int main()
{
    // first part: test data
    {
        double h, k, x[100], y[100], norm;
        int n = 0;

        x[0] = 2.0;
        y[0] = 1.0;
        h = 100;
        k = 100;
        norm = sqrt(h * h + k * k);

        printf("n\tx\t\ty\t\te\n");
        printf("%d\t%lf\t%lf\t---\n", n, x[0], y[0]);

        while (norm > 0.000001)
        {
            double jacobian_coef = 1 / (fdy(x[n], y[n]) * gdx(x[n], y[n]) - gdy(x[n], y[n]) * fdx(x[n], y[n]));
            h = jacobian_coef * (gdy(x[n], y[n]) * f(x[n], y[n]) - fdy(x[n], y[n]) * g(x[n], y[n]));
            k = jacobian_coef * (-gdx(x[n], y[n]) * f(x[n], y[n]) + fdx(x[n], y[n]) * g(x[n], y[n]));
            x[n + 1] = x[n] + h;
            y[n + 1] = y[n] + k;
            norm = sqrt(h * h + k * k);
            n++;
            printf("%d\t%lf\t%lf\t%lf\n", n, x[n], y[n], norm);
        }
    }

    FILE *ph = popen("gnuplot -persist", "w");
    fprintf(ph, "set terminal gif animate delay 100 optimize size 800,800\n");
    fprintf(ph, "set output 'result.gif'\n");
    fprintf(ph, "set title \"Newton's method\"\n");
    fprintf(ph, "set parametric\n");
    fprintf(ph, "set xrange [-10:10]\n");
    fprintf(ph, "set yrange [-10:10]\n");

    // second part: testing convergence or divergence in each grid point
    // for (int i = -4; i < 4; i++)
    // {
    //     for (int j = -4; j < 4; j++)
    //     {
    //         fprintf(ph, ", '-' with linespoints title 'Path(%d, %d)'", i, j);
    //     }
    // }
    fprintf(ph, "\n");
    
    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            fprintf(ph, "a = 1.0/sqrt(5.0)\nx(t) = 3*(sinh(t) + a*cosh(t))\ny(t) = 3*(sinh(t) - a*cosh(t))\n");
            fprintf(ph, "plot x(t), y(t) title 'x^2 - 3xy + y^2 - 9 = 0', -x(t), -y(t) title 'x^2 - 3xy + y^2 - 9 = 0', 3*cos(t), 3*sin(t) title 'x^2 + y^2 - 9 = 0'");
            //fprintf(ph, ", '-' with points pt 7 ps 1 lc rgb 'red' title 'Convergence', '-' with points pt 7 ps 1 lc rgb 'green' title 'Convergence after modifying', '-' with points pt 7 ps 1 lc rgb 'blue' title 'Divergence'");
            fprintf(ph, ", '-' with linespoints title 'Path(%d, %d)'", i, j);
            fprintf(ph, "\n");

            int n;
            n = newton(ph, i, j);
            
            if(n > 30)
            {
                n = newton(ph, i + 0.001, j);
                if(n > 30)
                {
                    point[i + 5][j + 5] = 2;
                    printf("(x, y) = (%d, %d) -> diverge\n", i, j);
                }
                else
                {
                    point[i + 5][j + 5] = 1;
                    printf("the modified initial guess sucess!\n");
                } 
            }

            fprintf(ph, "e\n"); // 'e' 表示結束資料輸入
        }
    }

    // for(int i = 0; i < 3; i++)
    // {
    //     for(int j = 0; j < 11; j++)
    //     {
    //         for(int k = 0; k < 11; k++)
    //         {
    //             if(point[j][k] == i)
    //                 fprintf(ph, "%d %d\n", j - 5, k - 5);
    //         }
    //     }
    //     fprintf(ph, "e\n");
    // }

    pclose(ph);
    return 0;
}