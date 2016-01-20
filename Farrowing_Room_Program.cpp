#include <winsock.h>
#include <MYSQL/mysql.h>
#include "Parameters.h"
#include "Recuperation_Database.h"
#include "Change_Table.h"
#include "Farrowing_Room_Program.h"
#include <cstdlib>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;

extern double **tabEvSows;
extern double **tabEvPigs;

double Iterate_FARROW(MYSQL *database, int old_value_farrow[29], int new_value_farrow[29])
{
    double rate[36];
    double sumrates(0),aleatoire,step,csum(0);
    int i(0),n(7),coord1;
    for(i=0;i<36;i++)
    {
        rate[i]=0;
    }
    double lambda;
    lambda=betaM*old_value_farrow[7]/Np+betaP*old_value_farrow[9]/Np+
        betaS*old_value_farrow[12]/Nt+phi[0]*betaS*old_value_farrow[21]/Nt;
    for(i=0;i<7;i++)
    {
        rate[i]=sigmaM*n*old_value_farrow[i];
    }
    for(i=7;i<14;i++)
    {
        rate[i]=lambda*old_value_farrow[i-7]*eps[i-7];
    }
    rate[14]=lambda*old_value_farrow[8]*eps[7];
    rate[15]=gammaM*old_value_farrow[7];
    rate[16]=gammaP*old_value_farrow[9];
    rate[17]=sigmaP*old_value_farrow[10];
    rate[18]=lambda*old_value_farrow[11];
    rate[19]=gammaS*old_value_farrow[12];
    for(i=20;i<27;i++)
    {
        rate[i]=sigmaS*n*old_value_farrow[i-7];
    }
    rate[27]=old_value_farrow[20]*lambda;
    rate[28]=phi[1]*gammaS*old_value_farrow[21];
    for(i=29;i<36;i++)
    {
        rate[i]=phi[2]*sigmaS*n*old_value_farrow[i-7];
    }
    for(i=0;i<36;i++)
    {
        sumrates+=rate[i];
    }
    aleatoire=rand();
    if(sumrates==0)
    {
        step=0.45;
    }
    else
    {
        step=-log(aleatoire/RAND_MAX)/sumrates;
    }
    aleatoire=rand();
    /*Find which event to do*/
    for(i=0;i<36;i++)
    {
        csum+=rate[i];
        if(csum>=(aleatoire/RAND_MAX)*sumrates)
        {
            coord1=i;
            goto new_value_farrow_label;
        }
    }
    new_value_farrow_label:
        for(i=0;i<29;i++)
        {
            new_value_farrow[i]=old_value_farrow[i];
        }
        if(sumrates!=0)
        {
            for(i=0;i<29;i++)
            {
                new_value_farrow[i]=new_value_farrow[i]+changeFarrowing[coord1][i];
            }
        }
        return step;
}

void Stoch_Iteration_FARROW(MYSQL *database, double Time[2],int Ps[2][18][3][batchRearing],int Pf[2][29],double Tf[2000],int NbBatch)
{
    Tf[0]=Time[0];
    char requete1[500]="";
    bool antiredondance=true;
    int sum_number_pigs(0);
    int old_value_farrow[29];
    int new_value_farrow[29];
    int loop(0);
    double step;
    int i(0);
    /*For the piglets*/
    int initPiglets[11]={0,0,0,0,0,0,0,0,0,0,0};
    for(i=2;i<9;i++)
    {
        sum_number_pigs+=Ps[1][i][2][NbBatch-1];
    }
    initPiglets[0]=round(sum_number_pigs*litterSize);
    sum_number_pigs=Ps[1][0][2][NbBatch-1]+Ps[1][1][2][NbBatch-1];
    initPiglets[8]=round(sum_number_pigs*litterSize);
    /*For the sows*/
    for(i=0;i<11;i++)
    {
        Pf[1][i]=initPiglets[i];
    }
    for(i=11;i<29;i++)
    {
        Pf[1][i]=Ps[1][i-11][2][NbBatch-1];
    }
    for(i=0;i<29;i++)
    {
        old_value_farrow[i]=Pf[1][i];
    }
    for(i=0;i<29;i++)
    {
        Pf[0][i]=Pf[1][i];
        sprintf(requete1, "REPLACE INTO pf VALUES(0,'%d','%d')",i+1,Pf[1][i]);
        mysql_query(database,requete1);
    }
    while(Tf[loop]<Time[1])
    {
        antiredondance=true;
        step=Iterate_FARROW(database,old_value_farrow,new_value_farrow);
        cout << "Step " << step << endl;
        for(i=0;i<29;i++)
        {
            Pf[1][i]=old_value_farrow[i];
        }
        loop++;
        Tf[loop]=std::min(Tf[loop-1]+step, Time[1]);
        cout << "Tf[loop] " << Tf[loop] << endl;
        if(Tf[loop]==Time[1])
        {
            for(i=0;i<29;i++)
            {
                Pf[1][i]=Pf[0][i];
            }
        }
        else
        {
            for(i=0;i<29;i++)
            {
                Pf[1][i]=new_value_farrow[i];
                old_value_farrow[i]=new_value_farrow[i];
            }
        }
        for(i=0;i<29;i++)
        {
            if(Pf[0][i]!=Pf[1][i])
            {
                antiredondance=false;
            }
        }
        if(antiredondance==false)
        {
            for(i=0;i<29;i++)
            {
                if(Pf[1][i]!=0)
                {
                    sprintf(requete1,"REPLACE INTO pf VALUES('%d','%d','%d')",loop,i+1,Pf[1][i]);
                    mysql_query(database,requete1);
                    sprintf(requete1,"REPLACE INTO tf VALUES('%d','%f')",loop,Tf[loop]);
                    mysql_query(database,requete1);
                }
                if(Pf[1][i]<0)
                {
                    exit(666);
                }
                Pf[0][i]=Pf[1][i];
            }
        }
    }
}

void sol_FARROW(MYSQL *database, int indexpiglets, int NbBatch, double Time[2], int Ps[2][18][3][batchRearing])
{
    char requete1[500]="";
    int Pf[2][29];
    double Tf[2000];
    Stoch_Iteration_FARROW(database, Time, Ps, Pf, Tf, NbBatch);
    sprintf(requete1, "REPLACE INTO popfarrow SELECT '%d',pf.iterations,health_state,nb_animals,'%d',tf.value FROM pf LEFT JOIN tf ON (tf.iterations=pf.iterations)",indexpiglets,NbBatch);
    mysql_query(database,requete1);
}
