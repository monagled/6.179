//This is a game of thrones characters management system, my project for 6.179. 
//author: Daniel Monagle


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  //not sure if i can do this so I'll just make a struct down below for bools<----RESOLVED

typedef struct character
{
	char first[20]; //first name
	char last[20]; //family name
	int status; //alive (1) or dead (0)
	int trustworthy; //yes (1) or no (0)
	int badass; //how badass is this character on an integer scale from 1 to 10

} character;

/*typedef int bool
{
	enum {false, true};
}*/

//make "fight character" function here

character fight(character a, character b) //function that returns the character who wins based on the badass parameter
{
	if (a.badass > b.badass)
	{
		return a;
	}
	else if (b.badass > a.badass)
	{
		return b;
	}
	else
	{
		printf("TIE! (we automatically return the first character)");
		return a;
	}
}

void flush() //custom flush function taken from stack overflow, input on add character doesn't seem to work right without it
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		; //"pass"
}

int main()
{
	FILE *fp;
	//file pointer

	struct character c; 

	char firstname[20]; //will use this later in the "edit(3)" section



	fp = fopen("GOT.DATA", "r+"); //if the file exists, it will open here
	if (fp == NULL)
	{
		//the file doesnt exist, create a new copy (wb vs rb)
		fp = fopen("GOT.DATA", "w+");

	}

	int user_choice;
	

	while(1)
	{
		printf("Add character(1)\n");
		printf("Show characters(2)\n");
		printf("Edit character(3)\n"); //Game of Thrones database really needs this option considering how often characters are killed, betrayed, etc
		printf("Exit(4)\n");
		printf("Enter the integer representing what you would like to do\n");
		fflush(stdin); //clears the stdin buffer before reading user input <-RESOLVED

		//now let's scan in that input
		scanf("%d", &user_choice);

		int more = 1; 

		if (user_choice == 1)
		{
			fseek(fp, 0, SEEK_END); //we want to enter a new character at the end of our file

			while (more == 1) //the user wants to continue entering characters
			{
				flush();
				printf("\nEnter charcter first name: ");
				fgets(c.first, 20, stdin);
				strtok(c.first, "\n");
				printf("\nEnter charcter last name: ");
				fgets(c.last, 20, stdin);
				strtok(c.first, "\n");
				printf("\nEnter 1 if the character is alive, 0 otherwise: ");
				scanf("%d", &c.status);
				printf("\nEnter 1 if the character is trustworthy, 0 otherwise: ");
				scanf("%d", &c.trustworthy);
				printf("\nEnter an integer rating (1-10) on how badass this character is: ");
				scanf("%d", &c.badass);

				//now we want to write the character into the file

				fwrite(&c, sizeof(c), 1, fp);

				//and ask the user if they want to continue entering characters 

				printf("Would you like to continue entering in characters? Enter 1 for yes, 0 for no\n");
				scanf("%d", &more);

			}

		}
		else if (user_choice == 2)
		{
			//we want to show the characters in the file
			//the rewind function will move the file cursor back to the start of the file
			rewind(fp);
			while (fread(&c, sizeof(c), 1, fp)==1) //will be 0 when there are no more files to read
			{

				printf("\n%s %s %d %d %d", c.first, c.last, c.status, c.trustworthy, c.badass);
				printf("\n");
			}
		}
		else if (user_choice == 3)
		{
			more =1;
			while (more == 1)
			{
				printf("What is the first name of the character you would like to modify?\n");
				scanf("%s", firstname);
				//lets get all the characters in the file again to see if the name matches
				rewind(fp);
				while (fread(&c, sizeof(c), 1, fp)==1)
				{
					//printf("Name read: '%s'", c.first);
					if (strcmp(c.first, firstname) != 0) //we don't have a match yet
					{
						; //"pass"
					}
					else
					{
						printf("Enter this character's new first name, last name, status, trustworthiness, and badassness\n");
						scanf("%s %s %d %d %d", &c.first, &c.last, &c.status, &c.trustworthy, &c.badass);
						fseek(fp, -1*sizeof(c), SEEK_CUR); //this will move the curson back 1 position, so that we can rewrite the correct character
						fwrite(&c, sizeof(c), 1, fp);
						break;
					}
				}
				printf("\nWould you like to edit another character? Enter 1 for yes, 0 for no\n");
				//flush here? <-RESOlVED
				fflush(stdin);
				scanf("%d", &more);
			}
		}
		else if (user_choice == 4)
		{
			fclose(fp); //close the file
			exit(0);
		}




	}
	return 0;
}