// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
// Refactored for SPEED: In-memory processing.
#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};

#define MAX_ACCOUNTS 100

// prototypes
unsigned int enterChoice(void);
void textFile(struct clientData accounts[]);
void updateRecord(struct clientData accounts[]);
void newRecord(struct clientData accounts[]);
void deleteRecord(struct clientData accounts[]);
void displayAllRecords(struct clientData accounts[]);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice
    struct clientData accounts[MAX_ACCOUNTS];

    // Initialize in-memory accounts array
    struct clientData blankClient = {0, "", "", 0.0};
    for (int i = 0; i < MAX_ACCOUNTS; ++i)
    {
        accounts[i] = blankClient;
    }

    // Load existing records from file into memory for faster processing
    if ((cfPtr = fopen("credit.dat", "rb")) != NULL)
    {
        fread(accounts, sizeof(struct clientData), MAX_ACCOUNTS, cfPtr);
        fclose(cfPtr);
    }
    else
    {
        printf("credit.dat not found. Starting with empty database.\n");
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 6)
    {
        switch (choice)
        {
        case 1:
            textFile(accounts);
            break;
        case 2:
            updateRecord(accounts);
            break;
        case 3:
            newRecord(accounts);
            break;
        case 4:
            deleteRecord(accounts);
            break;
        case 5:
            displayAllRecords(accounts);
            break;
        default:
            puts("Incorrect choice");
            break;
        } // end switch
    }     // end while

    // Save all records back to file upon exiting (Refactored for efficiency)
    if ((cfPtr = fopen("credit.dat", "wb")) == NULL)
    {
        printf("Error: Could not save data to credit.dat.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(accounts, sizeof(struct clientData), MAX_ACCOUNTS, cfPtr);
    fclose(cfPtr);
    
    printf("Data successfully saved. Exiting...\n");
    return 0;
} // end main

// create formatted text file for printing
void textFile(struct clientData accounts[])
{
    FILE *writePtr; // accounts.txt file pointer

    // fopen opens the file; exits if file cannot be opened
    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        // write all active records to text file
        for (int i = 0; i < MAX_ACCOUNTS; ++i)
        {
            if (accounts[i].acctNum != 0)
            {
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n", accounts[i].acctNum, 
                        accounts[i].lastName, accounts[i].firstName, accounts[i].balance);
            }
        }
        fclose(writePtr); // fclose closes the file
        printf("Accounts successfully written to accounts.txt\n");
    }
} // end function textFile

// update balance in record
void updateRecord(struct clientData accounts[])
{
    unsigned int account; // account number
    double transaction;   // transaction amount

    // obtain number of account to update
    do {
        printf("%s", "Enter account to update ( 1 - 100 ): ");
        scanf("%u", &account);
    } while (account < 1 || account > 100);

    // display error if account does not exist
    if (accounts[account - 1].acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    { // update record
        printf("%-6d%-16s%-11s%10.2f\n\n", accounts[account - 1].acctNum, 
               accounts[account - 1].lastName, accounts[account - 1].firstName, accounts[account - 1].balance);

        // request transaction amount from user
        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        scanf("%lf", &transaction);
        accounts[account - 1].balance += transaction; // update record balance

        printf("%-6d%-16s%-11s%10.2f\n", accounts[account - 1].acctNum, 
               accounts[account - 1].lastName, accounts[account - 1].firstName, accounts[account - 1].balance);
        printf("Account updated successfully.\n");
    }
} // end function updateRecord

// delete an existing record
void deleteRecord(struct clientData accounts[])
{
    struct clientData blankClient = {0, "", "", 0}; // blank client
    unsigned int accountNum;                        // account number

    // obtain number of account to delete
    do {
        printf("%s", "Enter account number to delete ( 1 - 100 ): ");
        scanf("%u", &accountNum);
    } while (accountNum < 1 || accountNum > 100);

    // display error if record does not exist
    if (accounts[accountNum - 1].acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    }
    else
    {
        // replace existing record with blank record
        accounts[accountNum - 1] = blankClient;
        printf("Account %d deleted successfully.\n", accountNum);
    }
} // end function deleteRecord

// create and insert record
void newRecord(struct clientData accounts[])
{
    unsigned int accountNum; // account number

    // obtain number of account to create
    do {
        printf("%s", "Enter new account number ( 1 - 100 ): ");
        scanf("%u", &accountNum);
    } while (accountNum < 1 || accountNum > 100);

    // display error if account already exists
    if (accounts[accountNum - 1].acctNum != 0)
    {
        printf("Account #%d already contains information.\n", accounts[accountNum - 1].acctNum);
    }
    else
    { // create record
        // user enters last name, first name and balance
        printf("%s", "Enter lastname, firstname, balance\n? ");
        scanf("%14s%9s%lf", accounts[accountNum - 1].lastName, 
              accounts[accountNum - 1].firstName, &accounts[accountNum - 1].balance);
        accounts[accountNum - 1].acctNum = accountNum;
        printf("Account created successfully.\n");
    }
} // end function newRecord

// display all active records to the console
void displayAllRecords(struct clientData accounts[])
{
    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
    
    for (int i = 0; i < MAX_ACCOUNTS; ++i)
    {
        if (accounts[i].acctNum != 0)
        {
            printf("%-6d%-16s%-11s%10.2f\n", accounts[i].acctNum, 
                   accounts[i].lastName, accounts[i].firstName, accounts[i].balance);
        }
    }
} // end function displayAllRecords

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice; // variable to store user's choice
    // display available options
    printf("%s", "\nEnter your choice\n"
                 "1 - store a formatted text file of accounts called\n"
                 "    \"accounts.txt\" for printing\n"
                 "2 - update an account\n"
                 "3 - add a new account\n"
                 "4 - delete an account\n"
                 "5 - list all accounts\n"
                 "6 - end program\n? ");

    scanf("%u", &menuChoice); // receive choice from user
    return menuChoice;
} // end function enterChoice