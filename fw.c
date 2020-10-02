# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
#include <ctype.h>

# include "table.h"


/* This part of the program is responsible
 * for storing and organizing data into 
 * the structures defined in table.h, mainly
 * the hash table*/


/*This function is responsible for initializing
 * the hash table whenever we want to grow it 
 * or when first we create it. */
void flagRealloc(Data **hashArr, int start, int end){
	int count;
	for(count = start; count < end; count ++)
	{
		Data * data = malloc(sizeof(Data));
       	        data -> word = NULL;
       	 	data -> freq = 0;
		hashArr[count] = data;
	}

}

/* Initializes table structure with decently large 
 * capacity for the hash array. */
Table *createHashTable(){
	Table* table = malloc(2*sizeof(long int) + sizeof(Data **));
	table -> cap = 31091;
	table -> size = 0;
	table -> hashArr = malloc( 31091 * sizeof(Data *));
	flagRealloc(table -> hashArr, 0, 31091);
	return table;
}

/*Returns 0 if we don't need to grow table
 * and 1 if we do, the chosen load factor 
 * was 0.5*/
int isFull(Table *table){
	if (table -> size / table -> cap > 0.5)
		return 1;
	else{
		return 0;
	}
}


/*Only grow if necessary*/
void growTable(Table *table){
	long int old_cap;
	if (isFull(table) == 1){
		old_cap = table -> cap;
		table -> cap = 2*old_cap + 1;
		table -> hashArr= realloc(table, table -> cap);
		flagRealloc(table -> hashArr, old_cap, table -> cap);
	}
}


/*The hash function is Horner's Rule*/
int horner_hash(Table *table, char *string)
{
	long int h = 0;
	int n;
	int count = 0;
	long int add = 0;
	if (strlen(string) > 8){
		n = 8;
	}else{ 
		n = strlen(string);
	}
	for (count = 0; count < n; count++){
		add =  (  (int) string[count] ) * ( (int) pow(7, n - 1 - count));
		h = h + add;
	}
	return h % (table -> cap);
}

/* Checking for Presence of Word, 
 * this takes into account quad. 
 * collisions. Returns 1 for presence
 * and 0 for absence. */
int isPresent(Table *table, char *string){
	int index = horner_hash(table, string);
	int new_index = index;
	int count = 0;
	Data **array = table -> hashArr;
	/*Seemingly infinite loop ends because inevitably
 * 	reaches one of the following two conditions*/
	while (1>0){
  		
		/*If absent, returns false*/
		if(array[new_index] -> word == NULL){
			return 0;

		/*If present, returns true*/
		} else if(strcmp(array[new_index] -> word, string) == 0){
			return 1;
		}else{
			count++;
		new_index = (index + (int) pow(count, 2)) % (table -> cap);
		}
	}
}

/* Calculates the index of a word inside the hash
 * table. If present, it outputs the actual index. 
 * Conversely, if absent, if defaults the index to
 * -1.*/

int get_index(Table *table, char *string)
{
	int index = horner_hash(table, string);
        int new_index = index;
        int count = 0;
        Data **array = table -> hashArr;
	while (1>0){
                if(array[new_index] -> word == NULL){
                        return -1;

                } else if(strcmp(array[new_index] -> word, string) == 0){
                        return new_index;
                }else{
                        count++;
                new_index = (index + (int) pow(count, 2)) % (table -> cap);
                }
	}
}

/* Find the frequency of a word if present.
 * If not, outputs 0. */
int get_freq(Table *table, int index){
	Data **array = table -> hashArr;
	Data *data = array[index];
	
	return data -> freq;
}

/* This function adds an element to the table
 * in case it was not present already, incrementing
 * the table size. Conversely, if the word is already 
 * present, it just increments its frequency*/

void add(Table *table, char *string)
{	
	int index = horner_hash(table, string);	
	int new_index = index;
	int count = 0;
	Data **array = table -> hashArr;
	int old_freq;
	/*Checking for absence and growing table if necessary*/ 	
	if (isPresent(table, string) == 0){
		table -> size = (table -> size) + 1;
		growTable(table);
	}
	while (1 > 0){
		
		/*New word in*/
		if (array[new_index] -> word == NULL){
			Data *new_word = array[new_index];
			new_word -> freq = 1;
			new_word -> word = string;
			return;
		}else if (strcmp(array[new_index] -> word, string) == 0){
			old_freq = array[new_index] -> freq;
			array[new_index] -> freq = old_freq + 1;
			return;		
		}else{
			count++;
		new_index = (index + (int) pow(count, 2)) % (table -> cap);
		}
	}
}


/* This part is responsible for cleaning*/

void destroyTable(Table *table)
{
	destroyHashArr(table -> hashArr, table -> cap);
	free(table);

}

void destroyHashArr(Data **array, long int size)
{
	long int count;
	for (count = 0; count < size; count++){
		destroyWord(array[count] -> word);
		free(array[count]);
	}	

}

void destroyWord(char *word){
	free(word);
}



/* This part of the program will be responsible 
 * for reading words from an input file. */

char *read_long_word(FILE *file){
        int currentChar;
        int wordLimit = 100; /* Arbitrary, grows as needed*/
        int charCount = 0;
        char *word = malloc(wordLimit * sizeof(char));
        currentChar = fgetc(file);
	
	/* Checking if first character is not part of a word, 
 	 * for example, many white spaces.	         */
	while ( isalpha(currentChar) == 0){
		currentChar = fgetc(file);
		if (currentChar == EOF)
			break;
	}
        while (isalpha(currentChar) && currentChar != EOF){

                /* Growing my word if needed*/
                if (charCount > wordLimit - 5){
                        wordLimit *= 2;
                        word = realloc(word, wordLimit);
                }

                word[charCount] = (char) tolower(currentChar);
                charCount++;
                currentChar = fgetc(file);

                /*Getting the last word*/
                if (currentChar == EOF){
                        word[charCount] = '\0';
                        return word;}
        }

        if (currentChar == EOF){
                return NULL;}
        else
                word[charCount] = '\0';
                return word;

}

/* This part of the program is responsible for sorting 
 * the words in descending order. Then, words with higher
 * frequency will be first, and ties will be decided based
 * on reverse alphabetical order.*/


/*void sortArray(Data **hashArr){}


int compareTo(Data *data1, Data *data2){}*/



/*This part of the program decides type of command line input
 *
 * TYPE 1: User uses -n and specifies how many words. 
 * TYPE 2: User doesn't use -n flag, program defaults k to 10.
 * */



int typeOfCommand(int argc, char* argv[]){
	if(strcmp(argv[1], "-n") == 0){

		/*Used -n flag, but didn't give more inputs*/
		if ( ! (argc > 2)){
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			exit(1);
		}		
		/*Used -n flag, but didn't give a proper number k*/	
		if ( !isdigit(argv[2][0]) ){
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			exit(1);
			}
		}
		return 1;
	/* we still have to deal with -n[number] ... */
	
	/* No -n flag*/
	
	return 2;
}



int main(int argc, char *argv[]){

	Table *table = createHashTable();
	int commandLine = typeOfCommand(argc, argv);
	int k; 
	int count;
	int start = 1;
/*Storing words in Hash Table*/
	if (commandLine != 2){
		k = atoi(argv[2]);	 
		start = 3;
	}else{
		k = 10;
	}
	/* Iterating over all files*/
	for (count = start; count < argc; count++ ){
	
		FILE *currentFile = fopen(argv[count], "r");
	
		if ( currentFile != NULL){

			char* currentWord = read_long_word(currentFile);
			while (currentWord != NULL){
				add(table, currentWord);
				currentWord = read_long_word(currentFile);
			} 
		fclose(currentFile);
		}else{
		perror("fopen");
		}
	}

	/*Missing sorting and printing*/

	printf("How many words in the testFile: %d \n", (int) table -> size);

	
	destroyTable(table);     
	return 0;
}
