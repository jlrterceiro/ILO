#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>

#define MAX_DEP 40
#define EPS 0.01

using namespace std;
//pegar area de intersecao entre dois depts
//usando coordenadas dos cantos de cada um
//sendo x1e - o valor de x da esquerda do dept 1
//	x2d - o valor de x da direita do dept 2...
double get_area_inter(double x1e, double y1e, double x1d, double y1d,
                  double x2e, double y2e, double x2d, double y2d);
struct Retangulo {
    double x1e, y1e, x1d, y1d;
    Retangulo(double _x1e=0, double _y1e=0, double _x1d=0, double _y1d=0) {
        x1e=_x1e; y1e=_y1e; x1d=_x1d; y1d=_y1d;
    }

    ~Retangulo() {

    }
};
//ponto do centro do departamento
//e seu tipo de forma
//se flag_forma = 1 - normal (original)
//se flag_forma = 2 - alterada (para horizontal ou vertical)
struct PosicaoDepartamento {
    double x, y;
    int flag_forma;
};

//Dados de entrada
double comp_fab, larg_fab;
double comp_dep[MAX_DEP], larg_dep[MAX_DEP];
double fluxo[MAX_DEP][MAX_DEP];
vector<Retangulo> lugs_permitidos[MAX_DEP];
int quant_dep;

// solucao
PosicaoDepartamento pos_dep[MAX_DEP];
double dist_dep[MAX_DEP][MAX_DEP]; //
double area_inter[MAX_DEP][MAX_DEP];
double valor_func;
double peso_inter=1;
//distancia entre dois depts
double calcula_distancia(double x1, double x2, double y1, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
//gerar "cantos" aleatórios
void gerarCantos(double x, double y, int flag_forma, double comprimento, double largura,
                 double &xe, double &ye, double &xd, double &yd) {
    xe = x;
    ye = y;
    if (flag_forma==0) {
        xd = x + comprimento;
        yd = y - largura;
    }
    else {
        xd = x + largura;
        yd = y - comprimento;
    }
}
//coordenadas do centro
void gerarCentro(double x, double y, int flag_forma, double comprimento, double largura,
                 double &centro_x, double &centro_y) {
    if (flag_forma == 0) {
        centro_x = x + (comprimento / 2);
        centro_y = y - (largura / 2);
    }
    if (flag_forma == 1) {
        centro_x = x + (largura / 2);
        centro_y = y - (comprimento / 2);
    }
}

void gerar_valor_func() {
    int i, j;
    double x[quant_dep], y[quant_dep];
    for(i = 0; i < quant_dep; i++){
        gerarCentro(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], x[i], y[i]);
    }

    for(i = 0; i < quant_dep; i++){
        for(j = 0; j < quant_dep; j++){
            dist_dep[i][j] = calcula_distancia(x[i], x[j], y[i], y[j]);
        }
    }
//    for(i = 0; i < quant_dep; i++){
//            printf("\n");
//        for(j = 0; j < quant_dep; j++){
//            printf(" %.0lf ", dist_dep[i][j]);
//        }
//    }
    double x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d;

    for(i = 0; i < quant_dep; i++){
        for(j = 0; j < quant_dep; j++){
            if (i!=j) {
                gerarCantos(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], x1e, y1e, x1d, y1d);
                gerarCantos(pos_dep[j].x, pos_dep[j].y, pos_dep[j].flag_forma, comp_dep[j], larg_dep[j], x2e, y2e, x2d, y2d);
                area_inter[i][j] = get_area_inter(x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d);
            }
            else {
                area_inter[i][j]=0;
            }
        }
    }
//funcao objetivo = SUM((Flux(i,j) * Dist(i,j) + Peso_inter * Area_Inter(i,j));
    valor_func=0;
    for(i = 0; i < quant_dep; i++){
        for(j = 0; j < quant_dep; j++){
            valor_func += fluxo[i][j] * dist_dep[i][j] + peso_inter * area_inter[i][j];
        }
    }
    printf("Valor func: %lf", valor_func);
}
//
double get_valor_mudanca(int indice, PosicaoDepartamento novo_pos) {
    double retorno=valor_func;
    int i;
    for (i=0; i<quant_dep; i++) {
        if (i!=indice) {
            retorno-=dist_dep[i][indice];
            retorno-=area_inter[i][indice];
        }
    }
    double x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d;
    double centro_x1, centro_y1, centro_x2, centro_y2;
    gerarCentro(pos_dep[indice].x, pos_dep[indice].y, pos_dep[indice].flag_forma, comp_dep[indice], larg_dep[indice], centro_x1, centro_y1);
    gerarCantos(pos_dep[indice].x, pos_dep[indice].y, pos_dep[indice].flag_forma, comp_dep[indice], larg_dep[indice], x1e, y1e, x1d, y1d);
    for (i=0; i<quant_dep; i++) {
        if (i!=indice) {
            gerarCentro(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], centro_x2, centro_y2);
            retorno+=calcula_distancia(centro_x1, centro_y1, centro_x2, centro_y2);
            gerarCantos(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], x2e, y2e, x2d, y2d);
            retorno+=get_area_inter(x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d) * peso_inter;
        }
    }
    return retorno;
}

void fazer_mudanca(int indice, PosicaoDepartamento novo_pos) {
    int i;
    for (i=0; i<quant_dep; i++) {
        if (i!=indice) {
            valor_func-=dist_dep[i][indice];
            valor_func-=area_inter[i][indice];
        }
    }
     double x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d;

    double centro_x1, centro_y1, centro_x2, centro_y2;
    gerarCentro(pos_dep[indice].x, pos_dep[indice].y, pos_dep[indice].flag_forma, comp_dep[indice], larg_dep[indice], centro_x1, centro_y1);
    gerarCantos(pos_dep[indice].x, pos_dep[indice].y, pos_dep[indice].flag_forma, comp_dep[indice], larg_dep[indice], x1e, y1e, x1d, y1d);
    pos_dep[indice] = novo_pos;//troquei
    for (i=0; i<quant_dep; i++) {
            if(i != indice){
                gerarCentro(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], centro_x2, centro_y2);
                dist_dep[i][indice]=calcula_distancia(centro_x1, centro_y1, centro_x2, centro_y2);
                dist_dep[indice][i] = dist_dep[i][indice];
                valor_func+=dist_dep[i][indice];
                gerarCantos(pos_dep[i].x, pos_dep[i].y, pos_dep[i].flag_forma, comp_dep[i], larg_dep[i], x2e, y2e, x2d, y2d);
            }
//            retorno+=get_area_inter(x1e, y1e, x1d, y1d, x2e, y2e, x2d, y2d) * peso_inter;
    }

    valor_func+=get_valor_mudanca(indice, novo_pos);

}

void gerarSolucaoInicial() {
    int i;
    double x1_aux, y1_aux;
    for (i=0; i<quant_dep; i++) {

        x1_aux = lugs_permitidos[i][0].x1d-comp_dep[i];
        y1_aux = lugs_permitidos[i][0].y1d+larg_dep[i];
        double dif_x, dif_y;
        dif_x=x1_aux-lugs_permitidos[i][0].x1e;
        dif_y=lugs_permitidos[i][0].y1e-y1_aux;
        printf("mmmmmmmmm %d\n", i);
        printf("%lf %lf %lf %lf\n", x1_aux, y1_aux, dif_x, dif_y);
        if (dif_x>-EPS && dif_x<EPS && dif_y>-EPS && dif_y<EPS) {
             printf("olah\n");
            pos_dep[i].x=lugs_permitidos[i][0].x1e;
            pos_dep[i].y=lugs_permitidos[i][0].y1e;
            pos_dep[i].flag_forma=0;
        }
        else {
                 printf("eita\n");
            double sort_x, sort_y;
             printf("eita\n");
            sort_x=(rand()%((int) (dif_x*100)))/100.0;
            sort_y=(rand()%((int) (dif_y*100)))/100.0;
            printf("eita\n");
            pos_dep[i].x=lugs_permitidos[i][0].x1e+sort_x;
            pos_dep[i].y=lugs_permitidos[i][0].y1e-sort_y;
            pos_dep[i].flag_forma=0;
        }

    }
}



void imprimirSolucao() {
    int i;
    for (i=0; i<quant_dep; i++) {
        printf("%d: %lf %lf %lf %lf %d\n", i, pos_dep[i].x, pos_dep[i].y, pos_dep[i].x+comp_dep[i], pos_dep[i].y-larg_dep[i], pos_dep[i].flag_forma);
    }
}

void entrada_de_dados() {
    int i, j, k;
    freopen("entrada.txt", "r", stdin);
    scanf("%lf %lf %d", &comp_fab, &larg_fab, & quant_dep);
    for(i = 0; i < quant_dep; i++){
        scanf("%lf %lf", &comp_dep[i], &larg_dep[i]);
    }
    for(i = 0; i < quant_dep; i++){
        for(j = 0; j <= i; j++ ){
            scanf("%lf", &fluxo[i][j]);
        }
    }
    for (i=0; i<quant_dep; i++) {
        scanf("%d", &j);
        printf("++++ %d %d\n", i, j);
        for (k=0; k<j; k++) {
            Retangulo ret;
            scanf("%lf %lf %lf %lf", &ret.x1e, &ret.y1e, &ret.x1d, &ret.y1d);
            lugs_permitidos[i].push_back(ret);
        }
        //for (k=0; k<lugs_permitidos.size(); k++) {
            //printf(" gfgd", lugs_permitidos[i][k]);
        //}
    }
}

void saida(){
    int i, j;
//    freopen("saidaaaaaaaa.txt", "w", stdout);

    printf("%lf %lf\n", comp_fab, larg_fab);
    printf("%d\n", quant_dep);

    for(i = 0; i < quant_dep; i++){
        printf("%lf %lf\n", comp_dep[i], larg_dep[i]);
    }
    for(i = 0; i < quant_dep; i++){

        for(j = 0; j <= i; j++ ){
            printf("%lf ", fluxo[i][j]);
        }
         printf("\n");
    }
    for (i=0; i<quant_dep; i++) {
        printf("** %d %d\n", i, quant_dep);
        Retangulo ret=lugs_permitidos[i][0];
        printf("%lf %lf %lf %lf\n", ret.x1e, ret.y1e, ret.x1d, ret.y1d);
    }
}


void gerarPosicoes() {
    int n=26;
    freopen("centros.txt", "r", stdin);
    double x[n], y[n];
    int i;
    for (i=0; i<n; i++) {
        scanf("%lf %lf", &x[i], &y[i]);
    }
    freopen("dados_area.txt", "r", stdin);
    double comp[n], alt[n], fixo[n];
    double a, b;
    scanf("%lf %lf", &a, &b);
    for (i=0; i<26; i++) {
        scanf("%lf %lf %lf", &comp[i], &alt[i], &fixo[i]);
    }
    freopen("saida.txt", "w", stdout);
    for (i=0; i<26; i++) {
        printf("%d\n", 1);
        if (fixo[i]==1) {
            printf("%lf %lf %lf %lf\n", x[i]-comp[i]/2, y[i]+alt[i]/2, x[i]+comp[i]/2, y[i]-alt[i]/2);
        }
        else {
            printf("1\n0 50.881 140.830 0");
        }
    }
}

double get_area_inter(double x1e, double y1e, double x1d, double y1d,
                  double x2e, double y2e, double x2d, double y2d) {

    double left = max(x1e, x2e);
    double right = min(x1d, x2d);
    double bottom = max(y1d, y2d);
    double top = min(y1e, y2e);
    double area;
    if (!(left < right && bottom < top)) {
        area = 0;
    } else {
        area = (right - left) * (top - bottom);
    }
//    printf("** %lf %lf %lf %lf\n", left, right, bottom, top);
    return area;

}


int main(int argc, char * * argv) {
    //printf("+++ %lf\n", area_inter(3, 7, 10, 4, -1, 5, 6, 1));
    //printf("+++ %lf\n", area_inter(1, 10, 15, 3, 5, 5, 9, 1));
    //printf("+++ %lf\n", area_inter(1, 10, 15, 3, 5, 30, 9, 20));
    srand(time(NULL));
    entrada_de_dados();
    printf("oi\n");
    saida();
    gerarSolucaoInicial();
    printf("oi\n");
    imprimirSolucao();
    gerar_valor_func();

    //gerarPosicoes();
    return 0;
}
