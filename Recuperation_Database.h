#ifndef RECUPERATION_DATABASE_H_INCLUDED
#define RECUPERATION_DATABASE_H_INCLUDED

int Recup_int(MYSQL *database, char requete1[500], char requete2[500]);
double Recup_double(MYSQL *database, char requete1[500], char requete2[500]);

#endif // RECUPERATION_DATABASE_H_INCLUDED
