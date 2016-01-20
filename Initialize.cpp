#include <iostream>
#include "Parameters.h"
#include "Initialize.h"

using namespace std;

void Initialize_Tabs(int Ps[2][18][3][batchRearing], double Ts[30000], int Pf[29], double Tf[2000], int Pp[11][2][nBatch], double Tp[10000])
{
    int i,j,k;
    for(i=0;i<18;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<batchRearing;k++)
            {
                Ps[1][i][j][k]=0;
            }
        }
    }
    for(i=0;i<11;i++)
    {
        for(j=0;j<2;j++)
        {
            for(k=0;k<nBatch;k++)
            {
                Pp[i][j][k]=0;
            }
        }
    }
    for(i=0;i<29;i++)
    {
        Pf[i]=0;
    }
    for(i=0;i<30000;i++)
    {
        Ts[i]=0;
    }
    for(i=0;i<2000;i++)
    {
        Tf[i]=0;
    }
    for(i=0;i<1000;i++)
    {
        Tp[i]=0;
    }
}
