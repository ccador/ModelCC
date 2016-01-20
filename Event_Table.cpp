#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Parameters.h"
#include "Event_Table.h"

using namespace std;


int *tIntroduction_Initialize();
void free_tIntroduction(int **tIntroduction);
int *batchNumberINTRO_Initialize(int tIntroduction[batchRearing]);
void free_batchNumberINTRO(int *batchNumberINTRO);
int **INTRO_Initialize(int tIntroduction[batchRearing], int batchNumberINTRO[batchRearing]);
void free_INTRO(int **INTRO);

int *tGestatingRoom_Initialize();
void free_tGestatingRoom(int *tGestatingRoom);
int *batchNumberGEST_Initialize(int tGestatingRoom[nBatch]);
void free_batchNumberGEST(int *batchNumberGEST);
int **GEST_Initialize(int tGestatingRoom[nBatch], int batchNumberGEST[nBatch]);
void free_GEST(int **GEST);

int *tFarrowingRoom_Initialize();
void free_FarrowingRoom(int *FarrowingRoom);
int *batchNumberFARROW_Initialize(int FarrowingRoom[nBatch]);
void free_batchNumberFARROW(int *batchNumberFARROW);
int **FARROW_Initialize(int  FarrowingRoom[nBatch], int batchNumberFARROW[nBatch]);
void free_FARROW(int **FARROW);

int *tServiceRoom_Initialize();
void free_tServiceRoom(int *tServiceRoom);
int *batchNumberSERV_Initialize(int tServiceRoom[nBatch]);
void free_batchNumberSERV(int *batchNumberSERV);
int **SERV_Initialize(int  tServiceRoom[nBatch], int batchNumberSERV[nBatch]);
void free_RVERR(int **SERV);

int *tBirth_Initialize();
void free_tBirth(int *tBirth);
int *batchNumberBIRTH_Initialize(int tBirth[nBatch]);
void free_batchNumberBIRTH(int *batchNumberBIRTH);
double **BIRTH_Initialize(int  tBirth[nBatch], int batchNumberBIRTH[nBatch]);
void free_BIRTH(double **BIRTH);

double **tabEvSows_Initialize(int **INTRO, int **GEST, int **FARROW, double **BIRTH, int **SERV);
void free_tabEvSows(double **tabEvSows);


int **tNursery_Initialize(int tBirth[nBatch]);
void free_tNursery(int **tNursery);
double **tFinishingRoom_Initialize(double **BIRTH);
void free_tFinishingRoom(double **tFinishingRoom);
double **tabEvPigs_Initialize(int **tNursery, double **tFinishingRoom);
void free_tabEvPigs(double **tabEvPigs);


/*Evt 1: entrance in service room*/

int *tIntroduction=tIntroduction_Initialize();
int *batchNumberINTRO=batchNumberINTRO_Initialize(tIntroduction);
int **INTRO=INTRO_Initialize(tIntroduction, batchNumberINTRO);

/*Evt 2: entrance in gestating room*/

int *tGestatingRoom=tGestatingRoom_Initialize();
int *batchNumberGEST=batchNumberGEST_Initialize(tGestatingRoom);
int **GEST=GEST_Initialize(tGestatingRoom, batchNumberGEST);

/*Evt 3: entrance in farrowing room*/

int *tFarrowingRoom=tFarrowingRoom_Initialize();
int *batchNumberFARROW=batchNumberFARROW_Initialize(tFarrowingRoom);
int **FARROW=FARROW_Initialize(tFarrowingRoom, batchNumberFARROW);

/*Evt 4: Return in service room*/

int *tServiceRoom=tServiceRoom_Initialize();
int *batchNumberSERV=batchNumberSERV_Initialize(tServiceRoom);
int **SERV=SERV_Initialize(tServiceRoom, batchNumberSERV);

/*Evt 33: Piglet's birth*/

int *tBirth=tBirth_Initialize();
int *batchNumberBIRTH=batchNumberBIRTH_Initialize(tBirth);
double **BIRTH=BIRTH_Initialize(tBirth, batchNumberBIRTH);

/*Creation of the event table*/

double **tabEvSows=tabEvSows_Initialize(INTRO, GEST, FARROW, BIRTH, SERV);

/*Initialization functions*/

int *tIntroduction_Initialize()
{
    int *tIntroduction=(int*)malloc(batchRearing*sizeof(int));
    int i;
    for(i=0; i<batchRearing; i++)
    {
        tIntroduction[i]=i*betweenBatch;
    }
    return tIntroduction;
}

void free_tIntroduction(int *tIntroduction)
{
    free(tIntroduction);
}

int *batchNumberINTRO_Initialize(int tIntroduction[batchRearing])
{
    int *batchNumberINTRO=(int*)malloc(batchRearing*sizeof(int));
    int i;
    for(i=0; i<batchRearing; i++)
    {
        batchNumberINTRO[i]=(tIntroduction[i]%durCycle)/betweenBatch+1;
    }
    return batchNumberINTRO;
}

void free_batchNumberINTRO(int *batchNumberINTRO)
{
    free(batchNumberINTRO);
}

int **INTRO_Initialize(int tIntroduction[batchRearing], int batchNumberINTRO[batchRearing])
{
    int **INTRO=(int**)malloc(3*sizeof(int));
    int i;
    for(i=0; i<3; i++)
    {
        INTRO[i]=(int*)malloc(batchRearing*sizeof(int));
    }
    for(i=0; i<batchRearing; i++)
    {
        INTRO[0][i]=tIntroduction[i];
        INTRO[1][i]=batchNumberINTRO[i];
        INTRO[2][i]=1;
    }
    return INTRO;
}

void free_INTRO(int **INTRO)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(INTRO[i]);
    }
    free(INTRO);
}


int *tGestatingRoom_Initialize()
{
    int *tGestatingRoom=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        tGestatingRoom[i]=durServ+i*betweenBatch;
    }
    return tGestatingRoom;
}

void free_tGestatingRoom(int *tGestatingRoom)
{
    free(tGestatingRoom);
}

int *batchNumberGEST_Initialize(int tGestatingRoom[nBatch])
{
    int *batchNumberGEST=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        batchNumberGEST[i]=((tGestatingRoom[i]-durServ)%durCycle)/betweenBatch+1;
    }
    return batchNumberGEST;
}

void free_batchNumberGEST(int *batchNumberGEST)
{
    free(batchNumberGEST);
}

int **GEST_Initialize(int tGestatingRoom[nBatch], int batchNumberGEST[nBatch])
{
    int **GEST=(int**)malloc(3*sizeof(int));
    int i;
    for(i=0; i<3; i++)
    {
        GEST[i]=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    }
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        GEST[0][i]=tGestatingRoom[i];
        GEST[1][i]=batchNumberGEST[i];
        GEST[2][i]=2;
    }
    return GEST;
}

void free_GEST(int **GEST)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(GEST[i]);
    }
    free(GEST);
}


int *tFarrowingRoom_Initialize()
{
    int *tFarrowingRoom=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        tFarrowingRoom[i]=durGest+durServ+i*betweenBatch;
    }
    return tFarrowingRoom;
}

void free_tFarrowingRoom(int *tFarrowingRoom)
{
    free(tFarrowingRoom);
}

int *batchNumberFARROW_Initialize(int tFarrowingRoom[nBatch])
{
    int *batchNumberFARROW=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        batchNumberFARROW[i]=((tFarrowingRoom[i]-durServ-durGest)%durCycle)/betweenBatch+1;
    }
    return batchNumberFARROW;
}

void free_batchNumberFARROW(int *batchNumberFARROW)
{
    free(batchNumberFARROW);
}

int **FARROW_Initialize(int  tFarrowingRoom[nBatch], int batchNumberFARROW[nBatch])
{
    int **FARROW=(int**)malloc(3*sizeof(int));
    int i;
    for(i=0; i<3; i++)
    {
        FARROW[i]=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    }
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        FARROW[0][i]=tFarrowingRoom[i];
        FARROW[1][i]=batchNumberFARROW[i];
        FARROW[2][i]=3;
    }
    return FARROW;
}

void free_FARROW(int **FARROW)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(FARROW[i]);
    }
    free(FARROW);
}


int *tServiceRoom_Initialize()
{
    int *tServiceRoom=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        tServiceRoom[i]=durFarrow+durGest+durServ+i*betweenBatch;
    }
    return tServiceRoom;
}

void free_tServiceRoom(int *tServiceRoom)
{
    free(tServiceRoom);
}

int *batchNumberSERV_Initialize(int tServiceRoom[nBatch])
{
    int *batchNumberSERV=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        batchNumberSERV[i]=((tServiceRoom[i]-durFarrow-durServ-durGest)%durCycle)/betweenBatch+1;
    }
    return batchNumberSERV;
}

void free_batchNumberSERV(int *batchNumberSERV)
{
    free(batchNumberSERV);
}

int **SERV_Initialize(int  tServiceRoom[nBatch], int batchNumberSERV[nBatch])
{
    int **SERV=(int**)malloc(3*sizeof(int));
    int i;
    for(i=0; i<3; i++)
    {
        SERV[i]=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    }
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        SERV[0][i]=tServiceRoom[i];
        SERV[1][i]=batchNumberSERV[i];
        SERV[2][i]=4;
    }
    return SERV;
}

void free_SERV(int **SERV)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(SERV[i]);
    }
    free(SERV);
}


int *tBirth_Initialize()
{
    int *tBirth=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        tBirth[i]=durFarrow+durGest+durServ-weaningAge+i*betweenBatch;
    }
    return tBirth;
}

void free_tBirth(int *tBirth)
{
    free(tBirth);
}

int *batchNumberBIRTH_Initialize(int tBirth[nBatch])
{
    int *batchNumberBIRTH=(int*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(int));
    int i;
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        batchNumberBIRTH[i]=((tServiceRoom[i]-durFarrow-durServ-durGest+weaningAge)%durCycle)/betweenBatch+1;
    }
    return batchNumberBIRTH;
}

void free_batchNumberBIRTH(int *batchNumberBIRTH)
{
    free(batchNumberBIRTH);
}

double **BIRTH_Initialize(int  tBirth[nBatch], int batchNumberBIRTH[nBatch])
{
    double **BIRTH=(double**)malloc(3*sizeof(double));
    int i;
    for(i=0; i<3; i++)
    {
        BIRTH[i]=(double*)malloc(floor((MaxTime-119)/betweenBatch)*sizeof(double));
    }
    for(i=0; i<floor((MaxTime-119)/betweenBatch); i++)
    {
        BIRTH[0][i]=tBirth[i]+0.5;
        BIRTH[1][i]=batchNumberBIRTH[i];
        BIRTH[2][i]=33.;
    }
    return BIRTH;
}

void free_BIRTH(double **BIRTH)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(BIRTH[i]);
    }
    free(BIRTH);
}


double **tabEvSows_Initialize(int **INTRO, int **GEST, int **FARROW, double **BIRTH, int **SERV)
{
    double **tabEvSows=(double**)malloc(3*sizeof(double));
    int i;
    double j;
    for(i=0; i<3; i++)
    {
        tabEvSows[i]=(double*)malloc((4*floor((MaxTime-119)/betweenBatch)+5)*sizeof(double));
    }
    int compteurINTRO(0), compteurGEST(0), compteurFARROW(0), compteurBIRTH(0), compteurSERV(0), compteurtabEvSows(0);
    for(j=0; j<MaxTime; j+=0.5)
    {
        if(j==INTRO[0][compteurINTRO])
        {
            tabEvSows[0][compteurtabEvSows]=j;
            tabEvSows[1][compteurtabEvSows]=INTRO[1][compteurINTRO];
            tabEvSows[2][compteurtabEvSows]=INTRO[2][compteurINTRO];
            compteurtabEvSows++;
            compteurINTRO++;
        }
        else if (j==GEST[0][compteurGEST])
        {
            tabEvSows[0][compteurtabEvSows]=j;
            tabEvSows[1][compteurtabEvSows]=GEST[1][compteurGEST];
            tabEvSows[2][compteurtabEvSows]=GEST[2][compteurGEST];
            compteurtabEvSows++;
            compteurGEST++;
        }
        else if (j==FARROW[0][compteurFARROW])
        {
            tabEvSows[0][compteurtabEvSows]=j;
            tabEvSows[1][compteurtabEvSows]=FARROW[1][compteurFARROW];
            tabEvSows[2][compteurtabEvSows]=FARROW[2][compteurFARROW];
            compteurtabEvSows++;
            compteurFARROW++;
        }
        else if (j==BIRTH[0][compteurBIRTH])
        {
            tabEvSows[0][compteurtabEvSows]=j;
            tabEvSows[1][compteurtabEvSows]=BIRTH[1][compteurBIRTH];
            tabEvSows[2][compteurtabEvSows]=BIRTH[2][compteurBIRTH];
            compteurtabEvSows++;
            compteurBIRTH++;
        }
       else if (j==SERV[0][compteurSERV])
        {
            tabEvSows[0][compteurtabEvSows]=j;
            tabEvSows[1][compteurtabEvSows]=SERV[1][compteurSERV];
            tabEvSows[2][compteurtabEvSows]=SERV[2][compteurSERV];
            compteurtabEvSows++;
            compteurSERV++;
        }
    }
    return tabEvSows;
}

void free_tabEvSows(double **tabEvSows)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(tabEvSows[i]);
    }
    free(tabEvSows);
}



/*Piglets Event Table*/

int **tNursery=tNursery_Initialize(tBirth);
double **tFinishingRoom=tFinishingRoom_Initialize(BIRTH);
double **tabEvPigs=tabEvPigs_Initialize(tNursery, tFinishingRoom);

/*Event 1: Exit from nursery*/

int **tNursery_Initialize(int tBirth[nBatch])
{
    int **tNursery=(int**)malloc(3*sizeof(int));
    int i;
    for(i=0; i<3; i++)
    {
        tNursery[i]=(int*)malloc(nBatch*sizeof(int));
    }
    for(i=0; i<nBatch; i++)
    {
        tNursery[0][i]=tBirth[i]+exitNursery;
        tNursery[1][i]=i;
        tNursery[2][i]=1;
    }
    return tNursery;
}

void free_tNursery(int **tNursery)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(tNursery[i]);
    }
    free(tNursery);
}

/*Event 2: Exit from finishing room*/

double **tFinishingRoom_Initialize(double **BIRTH)
{
    double **tFinishingRoom=(double**)malloc(3*sizeof(double));
    int i;
    for(i=0; i<3; i++)
    {
        tFinishingRoom[i]=(double*)malloc(nBatch*sizeof(double));
    }
    for(i=0; i<nBatch; i++)
    {
        tFinishingRoom[0][i]=BIRTH[0][i]+exitFinishingRoom;
        tFinishingRoom[1][i]=i;
        tFinishingRoom[2][i]=2;
    }
    return tFinishingRoom;
}

void free_tFinishingRoom(double **tFinishingRoom)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(tFinishingRoom[i]);
    }
    free(tFinishingRoom);
}

/*tabEvPigs building*/

double **tabEvPigs_Initialize(int **tNursery, double **tFinishingRoom)
{
    double **tabEvPigs=(double**)malloc(3*sizeof(double));
    int i;
    double j;
    for(i=0; i<3; i++)
    {
        tabEvPigs[i]=(double*)malloc((4*floor((MaxTime-119)/betweenBatch)+5)*sizeof(double));
    }
    int compteurtNursery(0), compteurtFinishingRoom(0), compteurtabEvPigs(0);
    for(j=0; j<MaxTime; j+=0.5)
    {
        if(j==tNursery[0][compteurtNursery])
        {
            tabEvPigs[0][compteurtabEvPigs]=j;
            tabEvPigs[1][compteurtabEvPigs]=tNursery[1][compteurtNursery];
            tabEvPigs[2][compteurtabEvPigs]=tNursery[2][compteurtNursery];
            compteurtabEvPigs++;
            compteurtNursery++;
        }
        else if (j==tFinishingRoom[0][compteurtFinishingRoom])
        {
            tabEvPigs[0][compteurtabEvPigs]=j;
            tabEvPigs[1][compteurtabEvPigs]=tFinishingRoom[1][compteurtFinishingRoom];
            tabEvPigs[2][compteurtabEvPigs]=tFinishingRoom[2][compteurtFinishingRoom];
            compteurtabEvPigs++;
            compteurtFinishingRoom++;
        }
    }
    return tabEvPigs;
}

void free_tabEvPigs(double **tabEvPigs)
{
    int i;
    for(i=0; i<3; i++)
    {
        free(tabEvPigs[i]);
    }
    free(tabEvPigs);
}
