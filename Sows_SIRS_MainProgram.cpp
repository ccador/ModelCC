#include <winsock.h>
#include <MYSQL/mysql.h>
#include "Parameters.h"
#include "Change_Table.h"
#include "Sows_SIRS_MainProgram.h"
#include "Piglets_MSIRS_Program.h"
#include "Usual_Fonctions.h"
#include "Corridor_Pression.h"
#include "Recuperation_Database.h"
#include "Culling_Sows.h"
#include "Farrowing_Room_Program.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;

/*Recovery of external variables*/

extern int nbInfGilts;
extern double **tabEvSows;

double Iteration(int loop, int old[18][3][batchRearing], int new_value[18][3][batchRearing])
{
    int n(7),i(0),j(0),k(0);
    double rate[18][3][batchRearing];
    double lambda[2]={0,0};
    double lambdaFarrow[batchRearing];
    double sumrates(0);
    double step(0),csum(0);
    double aleatoire;
    int coord1,coord2,coord3;
    for(i=0;i<18;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<batchRearing;k++)
            {
                rate[i][j][k]=0;
            }
        }
    }
    if(sum_any_old(0,old)>0)
    {
        lambda[0]=(((betaS/2*sum_old(1,0,old))+phi[0]*betaS/2*sum_old(10,0,old))/sum_any_old(0,old))*Nt;
        /*betaS/2, sows being housed in individual stalls in service room*/
    }
    if(sum_any_old(1,old)>0)
    {
        lambda[1]=(((betaS/2*sum_old(1,1,old))+phi[0]*betaS/2*sum_old(10,1,old))/sum_any_old(1,old))*Nt;
        /*lambda in gestating room*/
    }

    for(i=0;i<batchRearing;i++)
    {
        if(sum_any_old(0,old)>0)
        {
            rate[0][0][i]=old[0][0][i]*lambda[0];
            rate[9][0][i]=old[9][0][i]*lambda[0];
        }
        if(sum_any_old(1,old)>0)
        {
            rate[0][1][i]=old[0][1][i]*lambda[1];
            rate[9][1][i]=old[9][1][i]*lambda[1];
        }
        lambdaFarrow[i]=(betaS*old[1][2][i]+phi[0]*betaS*old[10][2][i])/Nt;
        rate[0][2][i]=old[0][2][i]*lambdaFarrow[i];
        rate[10][2][i]=old[10][2][i]*lambdaFarrow[i];
        for(j=0;j<3;j++)
        {
            rate[1][j][i]=gammaS*old[1][j][i];
            rate[10][j][i]=phi[1]*gammaS*old[10][j][i];
            for(k=2;k<9;k++)
            {
                rate[k][j][i]=sigmaS*n*old[k][j][i];
            }
            for(k=11;k<18;k++)
            {
                rate[k][j][i]=phi[2]*sigmaS*n*old[k][j][i];
            }
        }
        for(j=0;j<3;j++)
        {
            for(k=0;k<18;k++)
            {
                sumrates+=rate[k][j][i];
            }
        }
    }
    aleatoire=rand();
    if(sumrates!=0)
    {
        step=-log(aleatoire/RAND_MAX)/sumrates;
    }
    else
    {
        step=2.45;
    }
    aleatoire=rand();
    /*Find which event to do*/
    for(k=0;k<batchRearing;k++)
    {
        for(j=0;j<3;j++)
        {
            for(i=0;i<18;i++)
            {
                csum+=rate[i][j][k];
                if(csum>=(aleatoire/RAND_MAX)*sumrates)
                {
                    coord1=i;
                    coord2=j;
                    coord3=k;
                    goto new_value_label;
                }
            }
        }
    }
    new_value_label:

        for(i=0;i<18;i++)
        {
            for(j=0;j<3;j++)
            {
                for(k=0;k<batchRearing;k++)
                {
                    new_value[i][j][k]=old[i][j][k];
                }
            }
            if(sumrates!=0)
            {
                new_value[i][coord2][coord3]+=changeSows[coord1][i];
            }
        }
        cout << "Success" << endl;
        return step;
}

void Stoch_Iteration(MYSQL *database, double Time[2], double **tabEvSows, double **tabEvPigs, int Ps[2][18][3][batchRearing], double Ts[30000], int Pf[29], double Tf[2000], int Pp[11][2][nBatch], double Tp[10000])
{
    mysql_query(database, "DELETE FROM ps");
    mysql_query(database, "DELETE FROM pf");
    mysql_query(database, "DELETE FROM pp");
    mysql_query(database, "INSERT INTO ps VALUES(0,1,1,1,44)");
    cout << "Stoch Iteration" << endl;
    int old[18][3][batchRearing];
    int i,j,k;
    int new_value[18][3][batchRearing];
    Ts[0]=0;
    int indexpiglets(0),totalBatch(0),ind(0),NbBatch(1),Event(0),index(0);
    double randP1[100];
    double randP2[100];
    double randP14[100];
    double randP24[100];
    bool IsEvent[73 /*size of tabEvSows*/];
    bool any_IsEvent(false);
    bool antiredondance(true);
    double infectPress;
    double Transfert[2]={0,0};
    double temploop;
    double aleatoire;
    char requete1[500]="";
    char requete2[500]="";
    Ps[1][0][0][0]=Nt;
    for(i=0;i<18;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<batchRearing;k++)
            {
                old[i][j][k]=0;
                Ps[0][i][j][k]=Ps[1][i][j][k];
                old[i][j][k]=Ps[1][i][j][k];
            }
        }
    }
    int loop(0);
    double step(0);

    while(Ts[loop]<Time[1])
    {
        cout << "New loop " << endl;
        antiredondance=true;
        cout << "Begin step" << endl;
        step=Iteration(loop,old,new_value);

        cout << step << endl;
        cout << "End step" << endl;
        loop++;
        Ts[loop]=std::min((double)(Ts[loop-1]+step), Time[1]);
        cout << "Time " << Ts[loop] << endl;
        for(i=0;i<18;i++)
        {
            for(j=0;j<3;j++)
            {
                for(k=0;k<batchRearing;k++)
                {
                    Ps[1][i][j][k]=old[i][j][k];
                }
            }
        }
        if(Ts[loop]==Time[1])
        {
            loop_copy(Ps);
        }

        for(i=0; i<73 /*size of TabEvSows*/; i++)
        {
            IsEvent[i]=(Ts[loop-1]<tabEvSows[0][i] && Ts[loop]>tabEvSows[0][i]);
            any_IsEvent=false;
            if(Ts[loop-1]<tabEvSows[0][i] && Ts[loop]>tabEvSows[0][i])
            {
                any_IsEvent=true;
                ind=i;
                break;
            }
        }
        if(any_IsEvent)
        {
            NbBatch=tabEvSows[1][ind];
            cout << "NbBatch " << NbBatch << endl;
            Ts[loop]=tabEvSows[0][ind];
            cout << "Ts[loop] " << Ts[loop] << endl;
            Event=tabEvSows[2][ind];
            if(Event==1)
            {
                cout << "Event 1" << endl;
                loop_copy(Ps);
                Ps[1][0][0][NbBatch-1]=Nt;
                ps_to_old(Ps,old);
            }
            if(Event==2)
            {
                cout << "Event 2" << endl;
                loop_copy(Ps);
                for(i=0;i<18;i++)
                {
                    Ps[1][i][1][NbBatch-1]=Ps[0][i][0][NbBatch-1];
                    Ps[1][i][0][NbBatch-1]=0;
                }
                ps_to_old(Ps,old);
            }
            if(Event==3)
            {
                cout << "Event 3" << endl;
                loop_copy(Ps);
                infectPress=Corridor_Pression(database,NbBatch);
                /*Creation of the matrix "Transfert"*/
                /*Initialization*/
                Transfert[0]=0;
                Transfert[1]=0;
                /*sum(rand(Ps(loop-1,1,2,NbBatch),1)<=1-exp(-betair*infectPress))*/
                for(i=0;i<Ps[0][0][1][NbBatch-1];i++)
                {
                    aleatoire=rand();
                    randP1[i]=aleatoire/RAND_MAX;
                    if(randP1[i]<=(1-exp(-betAir*infectPress)))
                    {
                        Transfert[0]++;
                    }
                }
                /*Idem for Transfert[1]*/
                /*sum(rand(P(loop-1,1,2,NbBatch),1)<=1-exp(-betair*infectPress))*/
                for(i=0;i<Ps[0][9][1][NbBatch-1];i++)
                {
                    aleatoire=rand();
                    randP2[i]=aleatoire/RAND_MAX;
                    if(randP2[i]<=(1-exp(-betAir*infectPress)))
                    {
                        Transfert[1]++;
                    }
                }

                /*Corridor pressure from growing pigs on sows (Health states S1 and S2)
                when they pass through the corridor in front of the nursery rooms*/
                Ps[0][0][1][NbBatch-1]=Ps[0][0][1][NbBatch-1]-Transfert[0];
                Ps[0][9][1][NbBatch-1]=Ps[0][9][1][NbBatch-1]-Transfert[1];
                Ps[0][1][1][NbBatch-1]=Ps[0][1][1][NbBatch-1]+Transfert[0];
                Ps[0][10][1][NbBatch-1]=Ps[0][10][1][NbBatch-1]+Transfert[1];
                for(i=0;i<18;i++)
                {
                    Ps[1][i][2][NbBatch-1]=Ps[0][i][1][NbBatch-1];
                    Ps[1][i][1][NbBatch-1]=0;
                }
                ps_to_old(Ps,old);
            }
            if(Event==33)
            {
                cout << "Event 33" << endl;
                /*Piglet birth the week following the entrance in farrowing room*/
                indexpiglets++;
                loop_copy(Ps);
                Time[0]=Ts[loop]+0.00001;
                temploop=(double)(Ts[loop]+weaningAge);
                Time[1]=std::min(MaxTime, temploop);
                /*Swing to the Farrowing_Room_Program until the weaning of the piglets*/
                sol_FARROW(database, indexpiglets, NbBatch, Time, Ps);

                for(i=0;i<18;i++)
                {
                    Ps[1][i][2][NbBatch-1]=0;
                }
                ps_to_old(Ps,old);
            }
            if(Event==4)
            {
                cout << "Event 4" << endl;
                totalBatch++;
                sprintf(requete1, "SELECT batch FROM popfarrow WHERE NbBatch='%d' ORDER BY batch DESC LIMIT 0,1",NbBatch);
                sprintf(requete2, "SELECT COUNT(batch) FROM popfarrow WHERE NbBatch='%d' ORDER BY batch DESC LIMIT 0,1",NbBatch);
                index=Recup_int(database,requete1,requete2);
                /*Transfer of piglets in nursery room*/
                Time[0]=Ts[loop]+0.00001;
                Time[1]=std::min(MaxTime, (double)Ts[loop]+betweenBatch);

                /*Corridor pressure from growing pigs on sows (Health states S1 and S2)
                when they pass through the corridor in front of the nursery rooms*/
                infectPress=Corridor_Pression(database,totalBatch);

                /*Creation of the matrix "Transfert"*/
                /*Initialization*/
                Transfert[0]=0;
                Transfert[1]=0;
                /*sum(rand(P(loop-1,1,2,NbBatch),1)<=1-exp(-betair*infectPress))*/
                for(i=0;i<Ps[0][0][2][NbBatch-1];i++)
                {
                    aleatoire=rand();
                    randP14[i]=aleatoire/RAND_MAX;
                    if(randP14[i]<=(1-exp(-betAir*infectPress)))
                    {
                        Transfert[0]++;
                    }
                }
                /*Idem for Transfert[1]*/
                /*sum(rand(P(loop-1,1,2,NbBatch),1)<=1-exp(-betair*infectPress))*/
                for(i=0;i<Ps[0][9][2][NbBatch-1];i++)
                {
                    aleatoire=rand();
                    randP24[i]=aleatoire/RAND_MAX;
                    if(randP24[i]<=(1-exp(-betAir*infectPress)))
                    {
                        Transfert[1]++;
                    }
                }
                Ps[0][0][2][NbBatch-1]=Ps[0][0][2][NbBatch-1]-Transfert[0];
                Ps[0][9][2][NbBatch-1]=Ps[0][9][2][NbBatch-1]-Transfert[1];
                Ps[0][1][2][NbBatch-1]=Ps[0][1][2][NbBatch-1]+Transfert[0];
                Ps[0][10][2][NbBatch-1]=Ps[0][10][2][NbBatch-1]+Transfert[1];

                sprintf(requete1, "REPLACE INTO poppigs SELECT '%d',1,health_state,1,nb_animals,'%lf' FROM popfarrow WHERE health_state<12 AND iterations=(SELECT iterations FROM popfarrow ORDER BY iterations DESC LIMIT 0,1) AND batch='%d'", totalBatch, Ts[loop]+0.0000000001, totalBatch);
                mysql_query(database,requete1);
                sprintf(requete1, "REPLACE INTO poppigs SELECT '%d',1,health_state,2,0,'%lf' FROM popfarrow WHERE health_state>0 AND health_state<12 AND batch='%d'", totalBatch, Ts[loop]+0.0000000001, totalBatch);
                mysql_query(database,requete1);


                Piglets_MSIRS_Program(database, Time, totalBatch, Ps);


                /*Return of sows in service room + replacement process*/
                loop_copy(Ps);
                for(i=0;i<18;i++)
                {
                    sprintf(requete1, "SELECT nb_animals FROM popfarrow WHERE batch='%d' AND health_state='%d' AND iterations=(SELECT iterations FROM popfarrow ORDER BY iterations DESC LIMIT 0,1)",index,i+12);
                    sprintf(requete2, "SELECT COUNT(nb_animals) FROM popfarrow WHERE batch='%d' AND health_state='%d' AND iterations=(SELECT iterations FROM popfarrow ORDER BY iterations DESC LIMIT 0,1)",index,i+12);
                    Ps[1][i][2][NbBatch-1]=Recup_int(database, requete1,requete2);
                }

                Culling_Sows(Ps,NbBatch,Ts,loop);
                ps_to_old(Ps,old);
            }

            cout << "End switch" << endl;
        }
        else
        {
            for(i=0;i<18;i++)
            {
                for(j=0;j<3;j++)
                {
                    for(k=0;k<batchRearing;k++)
                    {
                        Ps[1][i][j][k]=new_value[i][j][k];
                        old[i][j][k]=new_value[i][j][k];
                    }
                }
            }
        }
        for(i=0;i<18;i++)
        {
            for(j=0;j<3;j++)
            {
                for(k=0;k<batchRearing;k++)
                {
                    if(Ps[1][i][j][k]!=Ps[0][i][j][k])
                    {
                        antiredondance=false;
                    }
                }
            }
        }
        if(antiredondance==false)
        {
            for(i=0;i<18;i++)
            {
                for(j=0;j<3;j++)
                {
                    for(k=0;k<batchRearing;k++)
                    {
                        if(Ps[1][i][j][k]!=0)
                        {
                            if(Ps[1][i][j][k]<0)
                            {
                                exit(666);
                            }
                            sprintf(requete1,"REPLACE INTO ps VALUES('%d','%d','%d','%d','%d','%lf')",loop,i+1,j+1,k+1,Ps[1][i][j][k],Ts[loop]);
                            mysql_query(database,requete1);
                        }
                        Ps[0][i][j][k]=Ps[1][i][j][k];
                    }
                }
            }
        }
    }
}

