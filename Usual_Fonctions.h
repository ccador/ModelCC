#ifndef USUAL_FONCTIONS_H_INCLUDED
#define USUAL_FONCTIONS_H_INCLUDED

int sum_old(int etat, int stade_phy, int old[18][3][batchRearing]);
int sum_any_old(int stade_phy, int old[18][3][batchRearing]);
void loop_copy(int Ps[2][18][3][batchRearing]);
void ps_to_old(int Ps[2][18][3][batchRearing], int old[18][3][batchRearing]);
double sum_lambda_FOI(int old_value_piglets[11][2][100],int i, int nombre1, int nombre2, int totalBatch);





#endif // USUAL_FONCTIONS_H_INCLUDED
