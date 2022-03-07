#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const double x_0 = 0.51;
const double epsilon = 1e-4;
const int num_lambda = 4;

double x_n(double x, double lambda){
    return 4*lambda*x*(1-x);
}

double derivada(double x, double lambda){
    return 4*lambda*(1-2*x);
}

void gera_arquivos(FILE **arr, int num_files, char *preffix){
    char suffix[10] = "";
    for(int i = 0; i < num_files; i++){
        char filename[15] = "";
        sprintf(suffix, "_%d.dat", i);       
        strcat(filename, preffix);
        strcat(filename, suffix);
    	arr[i] = fopen(filename, "w");
    }
}

int main(){
    FILE *n_xn[4];
    FILE *teia[4];
    FILE *recorrencia[4];

    FILE *bifurcacao = fopen("bifurcacao.dat", "w");
    FILE *lyapunov = fopen("lyapunov.dat", "w");

    gera_arquivos(n_xn, num_lambda, "n_xn");
    gera_arquivos(teia, num_lambda, "teia");
    gera_arquivos(recorrencia, num_lambda, "recor");

    for(int i=0; i<4; i++){
        fprintf(teia[i],"%f %f\n%f ", x_0, 0, x_0);
        fprintf(n_xn[i],"%d %f\n", 0, x_0);
    }
   
    for(double lambda = 0.70; lambda < 1.01; lambda = lambda + 0.001){
        double coeficiente = 0;
        double x = x_0;
        double x_arr[701];
        int l_flag = -1;

        for(int n = 1; n <= 700; n++){

            x = x_n(x, lambda);

            if(l_flag < 0){
                if(fabs(lambda - 0.70) <= epsilon) l_flag = 0;
                else if(fabs(lambda - 0.80) <= epsilon) l_flag = 1;
                else if(fabs(lambda - 0.88) <= epsilon) l_flag = 2;
                else if(fabs(lambda - 1.00) <= epsilon) l_flag = 3;
            }

            if(l_flag >= 0){
                fprintf(n_xn[l_flag], "%d %f\n", n, x);
                fprintf(teia[l_flag], "%f\n%f %f\n%f ", x, x, x, x);
                x_arr[n] = x;
            }
            
            if(n >= 300){
                fprintf(bifurcacao,"%f %f\n", lambda, x);
                coeficiente = coeficiente + log(fabs(derivada(x, lambda)));
            } 
        }

        fprintf(lyapunov, "%f %f\n", lambda, (coeficiente)/700);

        if(l_flag >= 0){
            for(int i = 0; i <= 700; i++){
                for(int j = 0; j <= 700; j++){
                    if(fabs(x_arr[i]-x_arr[j])<=epsilon){
                        fprintf(recorrencia[l_flag], "%d %d\n", i, j);
                    }
                }
            }
        }
    }
    return 0;
}