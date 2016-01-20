#ifndef PIGLETS_MSIRS_PROGRAM_H_INCLUDED
#define PIGLETS_MSIRS_PROGRAM_H_INCLUDED

void lambda_FOI(MYSQL *database, int totalBatch, double lambda[2][batchRearing]);
double Iterate_PIGLETS(MYSQL *database, int totalBatch, int old_value_piglets[11][2][100],int new_value_piglets[11][2][100]);
void Stoch_Iteration_PIGLETS(MYSQL *database, double Time[2], int totalBatch, int Ps[2][18][3][batchRearing], int Pp[2][11][2][100], double Tp[10000]);
void Piglets_MSIRS_Program(MYSQL *database, double Time[2], int totalBatch, int Ps[2][18][3][batchRearing]);

#endif // PIGLETS_MSIRS_PROGRAM_H_INCLUDED
