#ifndef FARROWING_ROOM_PROGRAM_H_INCLUDED
#define FARROWING_ROOM_PROGRAM_H_INCLUDED

double Iterate_FARROW(MYSQL *database, int old_value_farrow[29], int new_value_farrow[29]);
void Stoch_Iteration_FARROW(MYSQL *database, double Time[2],int Ps[2][18][3][batchRearing],int Pf[29],double Tf[2000], int NbBatch);
void sol_FARROW(MYSQL *database, int indexpiglets, int NbBatch, double Time[2], int Ps[2][18][3][batchRearing]);

#endif // FARROWING_ROOM_PROGRAM_H_INCLUDED
