#include <iostream>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

int Recup_int(MYSQL *database, char requete1[500], char requete2[500])
{
    int output(-1);
    MYSQL_RES *result=NULL;
    MYSQL_ROW row=NULL;
    mysql_query(database, requete2);
    result=mysql_store_result(database);
    row=mysql_fetch_row(result);
    if(atoi(row[0])==0)
    {
        output=0;
        mysql_free_result(result);
    }
    else
    {
        mysql_free_result(result);
        result=NULL;
        row=NULL;
        mysql_query(database, requete1);
        result=mysql_store_result(database);
        row=mysql_fetch_row(result);
        output=atoi(row[0]);
        mysql_free_result(result);
    }
    if(output==-1)
    {
        cout << "ERROR: Value Recuperation" << endl;
        exit(101);
    }
    else
    {
        return output;
    }
}

double Recup_double(MYSQL *database, char requete1[500], char requete2[500])
{
    double output(-1);
    MYSQL_RES *result=NULL;
    MYSQL_ROW row=NULL;
    mysql_query(database, requete2);
    result=mysql_store_result(database);
    row=mysql_fetch_row(result);
    if(atoi(row[0])==0)
    {
        output=0;
        mysql_free_result(result);
    }
    else
    {
        mysql_free_result(result);
        result=NULL;
        row=NULL;
        mysql_query(database, requete1);
        result=mysql_store_result(database);
        row=mysql_fetch_row(result);
        output=atof(row[0]);
        mysql_free_result(result);
    }
    if(output==-1)
    {
        cout << "ERROR: Value Recuperation" << endl;
        exit(101);
    }
    else
    {
        return output;
    }
}
