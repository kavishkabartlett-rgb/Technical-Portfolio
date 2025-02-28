#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Building {
    char name[63];
    int footage;
    float KwH;
    float efficiency;
    struct Building* next;
} building;

void sortList(struct Building **head) {
    int is_sorted = 0;
    while(!is_sorted) {
        struct Building* current = *head; 
        struct Building* previous = NULL;
        is_sorted = 1;

        while (current != NULL) {
            struct Building* temp = current->next;

            if (temp != NULL && current->efficiency < temp->efficiency) {


                is_sorted = 0;
                current->next = temp->next;
                temp->next = current;
                    if (previous != NULL)
                        previous->next = temp;
                    else 
                        *head = temp;

                current = temp;
                temp = current->next;

            }

            else if (temp != NULL && current->efficiency == temp->efficiency && strcmp(current->name, temp->name) > 0) {

                current->next = temp->next;
                temp->next = current;
                    if (previous != NULL)
                        previous->next = temp;
                    else 
                        *head = temp;

                current = temp;
                temp = current->next;
            }

            previous = current;
            current = temp;
        }
    }
}

//struct Building* loadBuildingsFromFile(const char* buildingFile){
//}

void freeMemoryFromBuildings(struct Building **head) { //Since we allocated memeory for each Building structure, we need to free it at the end of the program
    struct Building* toDelete = *head;
    while (toDelete != NULL) {
        struct Building* currentDelete = toDelete; //Creates a temporary pointer that we can free (if we freed currentDelete we would not be able to progress to next)
        toDelete = toDelete->next;
        free(currentDelete);
    }

}

int main(int argc, char *argv[]) {
    if (argc != 2) //Not really needed since there aren't going to be incorrect enters, but putting it here for practice
        return EXIT_SUCCESS;//Can put exit_failure here since its expecting int
    //struct Building* buildingList = loadBuildingsFromFile(argv[1]); - Used to have seperate method but it didnt work

    FILE* file;
    file = fopen(argv[1], "r");
    char temp[64];
    /*if (fgets(temp, sizeof(temp), file) == NULL || strcmp(temp,"DONE") == 0) {
        printf("BUILDING FILE IS EMPTY");
        return EXIT_SUCCESS; 
    }*/
    int counter = 0; //Check to see if this is the first time reading anything in the file
    int squareFoot; //Use to check if its 0 for the code for calculting efficiency
    float KwHTotal;
    char name[256]; // Will hold whatever is on the current line of the file
    char squareFootStr[256];
    char KwHTotalStr[256];
    struct Building* head = NULL;
    while (fgets(name, sizeof(name), file)) {//Make sure there is something to be read, again, not really needed for this case
        struct Building* newBuilding = (building*)malloc(sizeof(building));
        name[strcspn(name, "\n")] = 0;
        if ((strcmp(name, "DONE") == 0) && (counter == 0)) { //First time reading file
            free(newBuilding);
            printf("BUILDING FILE IS EMPTY");
            return EXIT_SUCCESS;
        }
        else if ((strcmp(name, "DONE") == 0)) {
            free(newBuilding);
            break;
        }
        counter++;  
            //if (placeHolder != NULL)
              //  placeHolder->next = newBuilding; //placeHolder is equal to the last building that was created, thus, setting this newBuilding equal to ->next makes it linked list
            //else
            //    head = newBuilding; //This will make head the newBuilding since this is the first building being created
        strcpy(newBuilding->name, name); //copies the string from the line read from the file into the name part of building
        fgets(squareFootStr, sizeof(squareFootStr), file);
        squareFootStr[strcspn(squareFootStr, "\n")] = 0;
        newBuilding->footage = atoi(squareFootStr);
        squareFoot = atoi(squareFootStr);
        fgets(KwHTotalStr, sizeof(KwHTotalStr), file);
            newBuilding->KwH = KwHTotal;
            KwHTotal = atof(KwHTotalStr);
            float efficiency;
            if (newBuilding->footage == 0) 
                efficiency = 0;
            else
                efficiency = (KwHTotal) / (newBuilding->footage);
            newBuilding->efficiency = efficiency;
            newBuilding->next = head; //Makes the newBuilding equal to a placeHolder
            head = newBuilding;
    }
    fclose(file); //Close file

    sortList(&head);
    //return head;
    if (head != NULL) {
        struct Building* currentPrint = head;
        while (currentPrint != NULL) {
            printf("%s %f\n", currentPrint->name, currentPrint->efficiency);
            currentPrint = currentPrint->next;
        }
    }
    freeMemoryFromBuildings(&head); //Now that I've printed the information, I can delete everything
    return EXIT_SUCCESS;
}