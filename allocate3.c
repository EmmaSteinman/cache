//========================================================
// Matt Kretchmar
// April 25, 2019
// allocate3.c
// This program illustrates malloc multi-dimensional records
//
// Suppose we have a company with several divisions.  Each
// has several employees.   Each employee has an ID number
// and a salary.  
// We want to use malloc to dynamically allocate memory
// for this organization.
//========================================================

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int     ID;
    float   salary;
} EmployeeType;

typedef struct
{
    EmployeeType    *workers;
    int             numWorkers;
} DivisionType;

typedef struct
{
    int             numDivisions;
    DivisionType    *divisions;
} OrganizationType;

EmployeeType    ReadEmployee    ( void );
void            PrintEmployee   ( EmployeeType );

//========================================================
//========================================================
int main ( void )
{
    OrganizationType    org;
    
    printf("How many divisions? ");
    scanf("%d",&(org.numDivisions));
    
    org.divisions = (DivisionType *)
        malloc(org.numDivisions * sizeof (DivisionType));
    
    for ( int i = 0; i < org.numDivisions; i++ )
    {
        printf("For Division %d:\n",i+1);
        printf("\tHow many workers? ");
        int n;
        scanf("%d",&n);
        org.divisions[i].numWorkers = n;
        org.divisions[i].workers = (EmployeeType *)
            malloc(n * sizeof(EmployeeType));
        
        for ( int j = 0; j < n; j++ )
        {
            printf("\tFor Employee %d\n",j+1);
            org.divisions[i].workers[j] = ReadEmployee();
        }        
    }
    
    // Now print out a table of all employees for the organization
    printf("\n\n\n================ ORGANIZATION ===================\n");
    int totalE = 0;     // total employees
    float totalF = 0;   // total salaray
    for ( int i = 0; i < org.numDivisions; i++ )
    {
        printf("\n");
        printf("Division %d\n",i+1);
        for ( int j = 0; j < org.divisions[i].numWorkers; j++ )
        {
            PrintEmployee(org.divisions[i].workers[j]);
            totalE++;
            totalF += org.divisions[i].workers[j].salary;
        }
    }
    
    printf("\nTotal Employees: %d\n",totalE);
    printf("Total Salary: $%8.2f\n",totalF);
    
    
    // Deallocate the memory
    for ( int i = 0; i < org.numDivisions; i++ )
    {
        free(org.divisions[i].workers);
    }
    free(org.divisions);
    
	return 0;
}

//========================================================
// ReadEmployee
//========================================================

EmployeeType ReadEmployee ( void )
{
    EmployeeType e;
    int ID;
    float salary;
    
    printf("\t\tEnter ID: ");
    scanf("%d",&ID);
    printf("\t\tEnter salary: ");
    scanf("%f",&salary);
    e.ID = ID;
    e.salary = salary;
    
    return e;
}
//========================================================
// PrintEmployee
//========================================================

void PrintEmployee ( EmployeeType e )
{
    printf("\tID:       %03d\n",e.ID);
    printf("\tSalary:  %8.2f\n",e.salary); 
}