#include <winsock.h>
#include <MYSQL/mysql.h>
#include "Recuperation_Database.h"
#include "Parameters.h"
#include "Usual_Fonctions.h"
#include "Change_Table.h"
#include "Piglets_MSIRS_Program.h"
#include "Corridor_Pression.h"
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

void lambda_FOI(MYSQL *database, int totalBatch, double lambda[2][100], int old_value_piglets[11][2][100])
{
    int i,j,compteur(0);
    for(i=0;i<2;i++)
    {
        for(j=0;j<100;j++)
        {
            lambda[i][j]=0;
        }
    }
    int Npigs[2];
    for(i=0;i<11;i++)
    {
        for(j=0;j<totalBatch;j++)
        {
            if(old_value_piglets[i][0][j]!=0)
            {
                compteur++;
            }
        }
    }
    Npigs[0]=compteur;
    compteur=0;
    for(i=0;i<11;i++)
    {
        for(j=0;j<totalBatch;j++)
        {
            if(old_value_piglets[i][1][j]!=0)
            {
                compteur++;
            }
        }
    }
    Npigs[1]=compteur;
    if(Npigs[1]==Np || Npigs[0]==Np)
    {
        for(i=0;i<totalBatch;i++)
        {
            lambda[0][i]=0;
            lambda[1][i]=0;
        }
    }
    else
    {
        for(i=0;i<totalBatch;i++)
        {
            lambda[0][i]=(betaM*old_value_piglets[7][0][i]+betaP*old_value_piglets[9][0][i])/Np+
                (betAir*sum_lambda_FOI(old_value_piglets,i,7,0,totalBatch)+sum_lambda_FOI(old_value_piglets,i,9,0,totalBatch))/(Npigs[0]-Np);
            lambda[1][i]=(betaM*old_value_piglets[7][1][i]+betaP*old_value_piglets[9][1][i])/Np+
                (betAir*sum_lambda_FOI(old_value_piglets,i,7,1,totalBatch)+sum_lambda_FOI(old_value_piglets,i,9,1,totalBatch))/(Npigs[1]-Np);
        }
    }
}

double Iterate_PIGLETS(MYSQL *database, int totalBatch, int old_value_piglets[11][2][100],int new_value_piglets[11][2][100])
{
    double rate[18][2][100];
    double lambda[2][100];
    double sumrates(0);
    int sum_old_value_piglets(0);
    int i(0),j(0),k(0);
    int n(7);
    for(i=0;i<18;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<100;k++)
            {
                rate[i][j][k]=0;
                lambda[j][k]=0;
            }
        }
    }
    lambda_FOI(database, totalBatch, lambda, old_value_piglets);
    for(i=0;i<7;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<totalBatch;k++)
            {
                rate[i][j][k]=sigmaM*n*old_value_piglets[i][j][k];
            }
        }
    }
    for(i=0;i<11;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<totalBatch;k++)
            {
                sum_old_value_piglets+=old_value_piglets[i][j][k];
            }
        }
    }
    for(i=7;i<14;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<totalBatch;k++)
            {
                rate[i][j][k]=old_value_piglets[i-7][j][k]*eps[i-7]*lambda[j][k]*sum_old_value_piglets;
            }
        }
    }
    for(j=0;j<2;j++)
    {
        for(k=0;k<totalBatch;k++)
        {
            rate[14][j][k]=old_value_piglets[8][j][k]*eps[i-7]*lambda[j][k]*sum_old_value_piglets;
        }
    }
    for(j=0;j<2;j++)
    {
        for(k=0;k<totalBatch;k++)
        {
            rate[15][j][k]=old_value_piglets[7][j][k]*gammaM;
            rate[16][j][k]=gammaP*old_value_piglets[9][j][k];
            rate[17][j][k]=sigmaP*old_value_piglets[10][j][k];
        }
    }
    for(i=0;i<18;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<totalBatch;k++)
            {
                sumrates+=rate[i][j][k];
            }
        }
    }
    double aleatoire=rand();
    double step;
    cout << sumrates << endl;
    if(sumrates==0)
    {
        step=10;
    }
    else
    {
        step=(-log(aleatoire/RAND_MAX))/sumrates;
    }
    cout << step << endl;

    aleatoire=rand();
    double csum(0);
    int coord1(0),coord2(0),coord3(0);
    /*Find which event to do*/
    for(k=0;k<totalBatch;k++)
    {
        for(j=0;j<2;j++)
        {
            for(i=0;i<18;i++)
            {
                csum+=rate[i][j][k];
                if(csum>=(aleatoire/RAND_MAX)*sumrates)
                {
                    coord1=i;
                    coord2=j;
                    coord3=k;
                    goto new_value_piglets_label;
                }
            }
        }
    }
    new_value_piglets_label:

        for(i=0;i<11;i++)
        {
            for(j=0;j<2;j++)
            {
                for(k=0;k<totalBatch;k++)
                {
                    new_value_piglets[i][j][k]=old_value_piglets[i][j][k];

                }
            }
            if(sumrates!=0)
            {
                new_value_piglets[i][coord2][coord3]+=changePiglets[coord1][i];
            }
        }
        cout << "Coord1: " << coord1 << endl;
        cout << "Coord2: " << coord2 << endl;
        cout << "Coord3: " << coord3 << endl;
        return step;
}

void Stoch_Iteration_PIGLETS(MYSQL *database, double Time[2], int totalBatch, int Ps[2][18][3][batchRearing], int Pp[2][11][2][100], double Tp[10000])
{
    Tp[0]=Time[0];
    char requete1[500]="";
    char requete2[500]="";
    int ind(0),batch(0),Event(0);
    bool IsEvent[34 /*Size of tabEvPigs*/];
    bool anyIsEvent(false);
    bool antiredondance=true;
    double step,infectPress;
    double aleatoire;
    int old_value_piglets[11][2][100];
    int new_value_piglets[11][2][100];
    int b,i,j,k;
    int testnan,transfert;
    for(b=0;b<totalBatch;b++)
    {
        for(i=0;i<11;i++)
        {
            for(j=0;j<2;j++)
            {
                sprintf(requete1, "SELECT nb_pigs FROM poppigs WHERE batch='%d' AND iterations=(SELECT iterations-1 FROM poppigs ORDER BY iterations DESC LIMIT 0,1) AND health_state='%d' AND physio_stage='%d'",b+1,i+1,j+1);
                sprintf(requete2, "SELECT COUNT(nb_pigs) FROM poppigs WHERE batch='%d' AND iterations=(SELECT iterations-1 FROM poppigs ORDER BY iterations DESC LIMIT 0,1) AND health_state='%d' AND physio_stage='%d'",b+1,i+1,j+1);

                Pp[1][i][j][b]=Recup_int(database,requete1,requete2);
            }
        }
    }
    for(i=0;i<11;i++)
    {
        for(j=0;j<2;j++)
        {
            sprintf(requete1, "SELECT nb_pigs FROM poppigs WHERE batch='%d' AND iterations=1 AND health_state='%d' AND physio_stage='%d'",totalBatch,i+1,j+1);
            sprintf(requete2, "SELECT COUNT(nb_pigs) FROM poppigs WHERE batch='%d' AND iterations=1 AND health_state='%d' AND physio_stage='%d'",totalBatch,i+1,j+1);
            Pp[1][i][j][totalBatch-1]=Recup_int(database,requete1,requete2);
        }
    }
    for(i=0;i<11;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<totalBatch;k++)
            {
                old_value_piglets[i][j][k]=Pp[1][i][j][k];
                Pp[0][i][j][k]=Pp[1][i][j][k];
            }
        }
    }

    int loop(0);
    while(Tp[loop]<Time[1])
    {
        antiredondance=true;
        cout << tabEvPigs[0][0] << endl;
        cout << "Begin Step" << endl;
        step=Iterate_PIGLETS(database,totalBatch,old_value_piglets,new_value_piglets);
        cout << "End Step" << endl;
        cout << step << endl;
        loop++;
        Tp[loop]=std::min((double)(Tp[loop-1]+step), Time[1]);
        cout << "Time " << endl;
        cout << Tp[loop]<< endl;
        cout << "  " << endl;

        for(i=0;i<11;i++)
        {
            for(j=0;j<2;j++)
            {
                for(k=0;k<totalBatch;k++)
                {
                    Pp[1][i][j][k]=old_value_piglets[i][j][k];
                }
            }
        }
        if(Tp[loop]==Time[1])
        {
            for(i=0;i<11;i++)
            {
                for(j=0;j<2;j++)
                {
                    for(k=0;k<totalBatch;k++)
                    {
                        Pp[1][i][j][k]=Pp[0][i][j][k];
                    }
                }
            }
        }
        for(i=0; i<34 /*Size of tabEvPigs*/; i++)
        {
            IsEvent[i]=(Tp[loop-1]<tabEvPigs[0][i] && Tp[loop]>tabEvPigs[0][i]);
            anyIsEvent=false;
            if(Tp[loop-1]<tabEvPigs[0][i] && Tp[loop]>tabEvPigs[0][i])
            {
                anyIsEvent=true;
                ind=i;
                break;
            }
        }
        if(anyIsEvent)
        {
            batch=tabEvPigs[1][ind];
            Tp[loop]=tabEvPigs[0][ind+1];
            Event=tabEvPigs[2][ind];
            if(Event==1)
            {
                cout << "Event 1 " << endl;
                /*Transfer of the batch called "batch" in finishing room*/
                /*Corridor pressure*/
                infectPress=Corridor_Pression(database,totalBatch);
                sprintf(requete1,"SELECT nb_pigs FROM pp WHERE iterations='%d' AND health_state=9 AND physio_stage=1 AND batch='%d'",loop,batch);
                sprintf(requete2,"SELECT COUNT(nb_pigs) FROM pp WHERE iterations='%d' AND health_state=9 AND physio_stage=1 AND batch='%d'",loop,batch);
                testnan=Recup_int(database,requete1,requete2);
                if(testnan==0)
                {
                    transfert=0;
                }
                else
                {
                    aleatoire=rand();
                    if(aleatoire/RAND_MAX<=(1-exp(-betAir*infectPress)))
                    {
                        transfert=1;
                    }
                    else
                    {
                        transfert=0;
                    }
                    sprintf(requete1, "UPDATE pp SET nb_pigs=nb_pigs-'%d' WHERE iterations='%d' AND health_state=9 AND physio_stage=1 AND batch='%d'",transfert,loop,batch);
                    mysql_query(database,requete1);
                    sprintf(requete1, "UPDATE pp SET nb_pigs=nb_pigs+'%d' WHERE iterations='%d' AND health_state=10 AND physio_stage=1 AND batch='%d'",transfert,loop,batch);
                    mysql_query(database,requete1);
                }
                for(i=0;i<11;i++)
                {
                    for(j=0;j<2;j++)
                    {
                        for(k=0;k<100;k++)
                        {
                            Pp[1][i][j][k]=Pp[0][i][j][k];
                        }
                    }
                }
                for(i=0;i<11;i++)
                {
                    Pp[1][i][1][batch]=Pp[0][i][0][batch];
                    Pp[1][i][0][batch]=0;
                }
                for(i=0;i<11;i++)
                {
                    for(j=0;j<2;j++)
                    {
                        for(k=0;k<totalBatch;k++)
                        {
                            old_value_piglets[i][j][k]=Pp[1][i][j][k];
                        }
                    }
                }
            }
            if(Event==2)
            {
                cout << "Event 2 " << endl;
                /*Exit from finishing room for "batch"*/
                for(i=0;i<11;i++)
                {
                    for(j=0;j<2;j++)
                    {
                        for(k=0;k<100;k++)
                        {
                            Pp[1][i][j][k]=Pp[0][i][j][k];
                        }
                    }
                }
                for(i=0;i<11;i++)
                {
                    Pp[1][i][1][batch]=0;
                    old_value_piglets[i][1][batch]=0;
                }
                for(i=0;i<11;i++)
                {
                    for(j=0;j<2;j++)
                    {
                        for(k=0;k<totalBatch;k++)
                        {
                            old_value_piglets[i][j][k]=Pp[1][i][j][k];
                        }
                    }
                }
            }
        }
        else
        {
            for(i=0;i<11;i++)
            {
                for(j=0;j<2;j++)
                {
                    for(k=0;k<totalBatch;k++)
                    {
                        Pp[1][i][j][k]=new_value_piglets[i][j][k];
                        old_value_piglets[i][j][k]=new_value_piglets[i][j][k];
                    }
                }
            }
        }
        for(i=0;i<11;i++)
        {
            for(j=0;j<2;j++)
            {
                for(k=0;k<totalBatch;k++)
                {
                    if(Pp[1][i][j][k]!=Pp[0][i][j][k])
                    {
                        antiredondance=false;
                    }
                }
            }
        }
        if(antiredondance==false)
        {
            for(i=0;i<11;i++)
            {
                for(j=0;j<2;j++)
                {
                    for(k=0;k<totalBatch;k++)
                    {
                        if(Pp[1][i][j][k]!=0)
                        {
                            sprintf(requete1,"REPLACE INTO pp VALUES('%d','%d','%d','%d','%d')",loop,i+1,j+1,k+1,Pp[1][i][j][k]);
                            mysql_query(database,requete1);
                            sprintf(requete1,"REPLACE INTO tp VALUES('%d','%lf')",loop,Tp[loop]);
                            mysql_query(database,requete1);
                        }
                        Pp[0][i][j][k]=Pp[1][i][j][k];
                    }
                }
            }
        }
    }
}

void Piglets_MSIRS_Program(MYSQL *database, double Time[2], int totalBatch, int Ps[2][18][3][batchRearing])
{
    int Pp[2][11][2][100];
    double Tp[10000];
    Stoch_Iteration_PIGLETS(database,Time,totalBatch,Ps,Pp,Tp);
    cout << "End Stoch Iter" << endl;
    char requete1[500]="";
    char requete2[500]="";
    int i(0);
    for(i=0;i<totalBatch;i++)
    {
        sprintf(requete1,"REPLACE INTO poppigs SELECT batch,iterations,health_state,physio_stage,nb_pigs,tp.value FROM pp LEFT JOIN tp ON (tp.iterations=pp.iterations) WHERE batch='%d'",i+1);
        mysql_query(database,requete1);
    }
    cout << "Retour Event 4" << endl;
}
