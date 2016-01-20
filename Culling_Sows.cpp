#include <winsock.h>
#include <MYSQL/mysql.h>
#include "Recuperation_Database.h"
#include "Parameters.h"
#include "Usual_Fonctions.h"
#include <cstdlib>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;

void Culling_Sows(int Ps[2][18][3][batchRearing], int NbBatch, double Ts[30000], int loop)
{
    int Cull[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int j,k;
    double aleatoire;
    int sumcull(0);
    for(k=0;k<18;k++)
    {
        for(j=0;j<Ps[1][k][2][NbBatch-1];j++)
        {
            aleatoire=rand();
            if((aleatoire/RAND_MAX)<=mu)
            {
                Cull[k]++;
            }
        }
        Ps[1][k][0][NbBatch-1]=Ps[1][k][2][NbBatch-1]-Cull[k];
        Ps[1][k][2][NbBatch-1]=0;
        sumcull+=Cull[k];
    }
    if(Ts[loop]==durCycle && NbBatch==1) /*Introduction of an infected gilt during the first replacement process*/
    {
        Ps[1][0][0][NbBatch-1]=Ps[1][0][0][NbBatch-1]+sumcull-1;
        Ps[1][1][0][NbBatch-1]=Ps[1][1][0][NbBatch-1]+1;
    }
    else
    {
        Ps[1][0][0][NbBatch-1]=Ps[1][0][0][NbBatch-1]+sumcull;
    }
 }
