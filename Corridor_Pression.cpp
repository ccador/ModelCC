#include <winsock.h>
#include <MYSQL/mysql.h>
#include "Recuperation_Database.h"
#include "Parameters.h"
#include "Usual_Fonctions.h"
#include <cstdlib>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>

double Corridor_Pression(MYSQL *database, int totalBatch)
{
    char requete1[500]="";
    char requete2[500]="";
    int sumPigletBatches(0); //Number of piglet batches in nursery and finishing room when a batch of piglets pass through the corridor
    double infectPress(0); //Number of infectious piglets in nursery and finishing room in the other batches
    sprintf(requete1,"SELECT COUNT(value) FROM solpigs_poppigs WHERE poppigs1=(SELECT poppigs1 FROM solpigs_poppigs ORDER BY poppigs1 DESC LIMIT 0,1)");
    sprintf(requete2,"SELECT COUNT(value) FROM solpigs_poppigs WHERE poppigs1=(SELECT poppigs1 FROM solpigs_poppigs ORDER BY poppigs1 DESC LIMIT 0,1)");
    sumPigletBatches=Recup_int(database,requete1,requete2);
    sprintf(requete1,"SELECT SUM(value)FROM solpigs_poppigs WHERE poppigs1=(SELECT poppigs1 FROM solpigs_poppigs ORDER BY poppigs1 DESC LIMIT 0,1) AND (poppigs2=8 OR poppigs2=10)");
    sprintf(requete2,"SELECT COUNT(value)FROM solpigs_poppigs WHERE poppigs1=(SELECT poppigs1 FROM solpigs_poppigs ORDER BY poppigs1 DESC LIMIT 0,1) AND (poppigs2=8 OR poppigs2=10)");
    infectPress=Recup_double(database,requete1,requete2);
    if(Np*sumPigletBatches!=0)
    {
        infectPress=infectPress/(sumPigletBatches*Np);
    }
    return infectPress;
}
