#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED

/*Initialisation of the constants*/

int const batchRearing(5); //Batch-rearing system: 4, 5, 7, 10 or 20 batches
double const betAir(0.25); //0.01, 0.25, 0.50, 0.75
double const sigmaS(1./75.); //Sigma1 in Table2: 60, 90, 120, 150
double const sigmaP(1./75.); //Sigma1 in Table2: 60, 90, 120, 150
double const phi[3]={0.65, 0.85, 1.5}; //0.25, 0.50, 0.75, 1
double const betaS(2.18); //Beta1 in Table2
double const betaP(2.18); //Beta1 in Table2
double const betaM(0.36);
double const gammaS(1./7.); //Gamma1 in Table2
double const gammaP(1./7.); //Gamma1 in Table2
double const gammaM(1./7.);
double const sigmaM(1./70.);
double const eps[8]={0.00062, 0.024, 0.13, 0.33, 0.55, 0.73, 0.86, 1};
/*double const betabP(0.01*2);
double const betabm(0.0008*2);
 On considère le même beta au sein de la salle de truies gestantes
double const betad(1.1); //0.1
double const betabd(0.005*2);
double const gammad(1./7.);*/
double const mu(0.385/2.35); // Sow culling rate after weaning
int const durServ=32;
int const durGest=82;
int const exitNursery=77; //Age of the piglets when they leave the nursery
int const exitFinishingRoom=182; //Age of the piglets when they leave the finishing room
double const litterSize(11.5); //Number of piglets per litter
double const MaxTime=600; // Duration of the simulation
int const weaningAge=21; //28 if batchRearing=7
int const Nt=44; /*depends on the batch-rearing system: Nt=33 if batchRearing=4, 44 if 5, 24 if 7, 37 if 10, 32 if 20*/
int const betweenBatch=28; /*depends on the batch-rearing system: Nt=35 if batchRearing=4, 28 if 5, 21 if 7, 14 if 10, 7 if 20*/
int const durCycle=140; //Duration of a reproduction cycle (147 if batchRearing=7
int const durFarrow=weaningAge+5;
int const Np=4; /*depends on the batch-rearing system: Np=3 if batchRearing=4, 4 if 5, 2 if 7, 3 if 10, 3 if 20*/
int const nBatch=17; /*depends on the batch-rearing system and the duration of the simulation:
for 600 days, nBatch=13 if batchRearing=4, 17 if 5, 22 if 7, 34 if 10, 60 if 20*/

#endif // PARAMETERS_H_INCLUDED
