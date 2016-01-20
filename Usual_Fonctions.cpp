#include <winsock.h>
#include "MYSQL/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Parameters.h"
#include "Usual_Fonctions.h"

using namespace std;

extern int nbInfGilts;
extern double **tabEvSows;
extern int **changeFarrowing;

int sum_old(int etat, int stade_phy, int old[18][3][batchRearing])
{
    int i,sum_old(0);
    for(i=0;i<batchRearing;i++)
    {
        sum_old+=old[etat][stade_phy][i];
    }
    return sum_old;
}

int sum_any_old(int stade_phy, int old[18][3][batchRearing])
{
    int i,j,sum_old(0),compteur(0);
    for(i=0;i<batchRearing;i++)
    {
        for(j=0;j<18;j++)
        {
            sum_old+=old[j][stade_phy][i];
        }
        if(sum_old!=0)
        {
            compteur++;
        }
        sum_old=0;
    }
    return compteur;
}

void loop_copy(int Ps[2][18][3][batchRearing])
{
    int i,j,k;
    for(i=0;i<18;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<batchRearing;k++)
            {
                Ps[1][i][j][k]=Ps[0][i][j][k];
            }
        }
    }
}

void ps_to_old(int Ps[2][18][3][batchRearing], int old[18][3][batchRearing])
{
    int i,j,k;
    for(i=0;i<18;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<batchRearing;k++)
            {
                old[i][j][k]=Ps[1][i][j][k];
            }
        }
    }
}

double sum_lambda_FOI(int old_value_piglets[11][2][100],int i, int nombre1, int nombre2, int totalBatch)
{
    double sum_lambda(0);
    int j;
    for(j=0;j<totalBatch;j++)
    {
        if(j!=i)
        {
            sum_lambda+=old_value_piglets[nombre1][nombre2][j];
        }
    }
    return sum_lambda;
}
