#include "file.h"

int main(){
    file* f = NULL;

    printf("==========================================================\n");
    printf("=                    Test d'insertion                    =\n");
    printf("==========================================================\n");
    printf("Insertion de (5,1)\n");
    inserer(&f, 5,1);
    printFile(f);
    printf("Insertion de (5,2)\n");
    inserer(&f, 5,2);    
    printFile(f);
    printf("Insertion de (1,3)\n");
    inserer(&f, 1,3);    
    printFile(f);
    printf("Insertion de (1,1)\n");
    inserer(&f, 1,1);    
    printFile(f);
    printf("Insertion de (5,5)\n");
    inserer(&f, 5,5);    
    printFile(f);

    printf("==========================================================\n");
    printf("=                    Test de primaité                    =\n");
    printf("==========================================================\n");
    printf("Est ce que (1,1) est premier ? %d\n", verifierSiPremier(f,1,1));
    printf("Est ce que (1,5) est premier ? %d\n", verifierSiPremier(f,1,5));

    printf("==========================================================\n");
    printf("=                    Test de suppression                 =\n");
    printf("==========================================================\n");
    printf("Suppression de (5,2)\n");
    retirer(&f, 5,2);
    printFile(f);
    printf("Suppression de (1,1)\n");
    retirer(&f, 1,1);
    printFile(f);
    printf("Suppression de (5,3)\n");
    retirer(&f, 5,3);
    printFile(f);
    printf("Suppression de (5,5)\n");
    retirer(&f, 5,5);
    printFile(f);
    printf("Suppression de (1,2)\n");
    retirer(&f, 1,2);
    printFile(f);
}
