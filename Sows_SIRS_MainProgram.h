#ifndef SOWS_SIRS_MAINPROGRAM_H_INCLUDED
#define SOWS_SIRS_MAINPROGRAM_H_INCLUDED

double Iteration(int loop, int old[18][3][batchRearing], int new_value[18][3][batchRearing]);
void Stoch_Iteration(MYSQL *database, double Time[2], double **tabEvSows, double **tabEvPigs, int Ps[2][18][3][batchRearing], double Ts[30000], int Pf[29], double Tf[2000], int Pp[11][2][nBatch], double Tp[10000]);

#endif // SOWS_SIRS_MAINPROGRAM_H_INCLUDED
