#include <winsock.h>
#include <MYSQL/mysql.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Parameters.h"
#include "Sows_SIRS_MainProgram.h"
#include "Event_Table.h"
#include "Change_Table.h"
#include "Initialize.h"
#include "Usual_Fonctions.h"
#include <cmath>

using namespace std;

extern double **tabEvSows;
extern double **tabEvPigs;


int main()
{
    /*Initialisation of the object "database"*/
    MYSQL *database;
    database=mysql_init(NULL);
    mysql_options(database, MYSQL_READ_DEFAULT_GROUP, "option");

    /*Connexion to the database*/
    if(mysql_real_connect(database, "localhost" /*server name*/, "root" /*user*/, "z4fvSDp$89" /*password (if needed)*/, "pig_project", 0, NULL, 0))
    {
        cout << "Connexion to database succeed" << endl;
        mysql_query(database, "DELETE FROM popfarrow");
        mysql_query(database, "DELETE FROM poppigs");

        /*Initialization of the Tables used in the different programs*/
        int Ps[2][18][3][batchRearing];
        int i,j;
        int Pf[29];
        int Pp[11][2][nBatch];
        double Ts[30000];
        double Tf[2000];
        double Tp[10000];
        double Time[2]={0,MaxTime};
        Initialize_Tabs(Ps,Ts,Pf,Tf,Pp,Tp);
        srand(time(NULL));
        cout << "Stoch Iteration" << endl;
        Stoch_Iteration(database,Time,tabEvSows,tabEvPigs,Ps,Ts,Pf,Tf,Pp,Tp);
    }
    else
    {
        cout << "ERROR: DATABASE CONNEXION" << endl;
    }

    mysql_close(database);
    return 0;
}
