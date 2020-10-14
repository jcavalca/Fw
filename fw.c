# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <ctype.h>
# include <unistd.h>
# include "table.h"


/* This part of the program is responsible
 *  * for storing and organizing data into 
 *   * the structures defined in table.h, mainly
 *    * the hash table*/


/*This function is responsible for initializing
 *  * the hash table whenever we want to grow it 
 *   * or when first we create it. */
void flagRealloc(Data **hashArr, int start, int end){
	int count;
	for(count = start; count < end; count ++)
	{
		Data *data = malloc(sizeof(Data));
      	        data -> word = NULL;
       	 	data -> freq = 0;
		hashArr[count] = data;
	}

}

void myRealloc(Table *table, long int new_cap){
	int count;
	int eleCount = 0;
	Data **occupiedElements = malloc( (table -> size) * sizeof(Data*) );
	Data **array = table -> hashArr;
	for (count = 0; count < table -> cap; count++){
		if (table -> hashArr[count] -> word != NULL){
			 occupiedElements[eleCount] = table -> hashArr[count];
			 eleCount++;
		}
	}

	/*Changing Table*/
	table -> cap = new_cap;
	table -> hashArr =  malloc( new_cap * sizeof(Data *));
	flagRealloc(table -> hashArr, 0, new_cap);

	for (count = 0; count < table -> size; count++){
		addGrow(table, occupiedElements[count]);
	}
	free(occupiedElements);  
	 free(array);
}


/* This function is only used when growing the 
 * hashArr, we are passing table and a Data* and returning
 * void. */
void addGrow(Table *table, Data *data){
	int index = horner_hash(table, data -> word);
	int new_index = index;
	Data **array = table -> hashArr;	
        int count = 0;
        while (1 > 0){
                /*Found empty spot*/
                if (array[new_index] -> word == NULL){
                         free(array[new_index]);
			 array[new_index] = data;
			 return;}
		new_index = (index + count) % (table -> cap);		
                count++;
        }

}


/*Only grow if necessary*/
void growTable(Table *table){
        long int old_cap, new_cap;
        if (isFull(table) == 1){
                old_cap = table -> cap;
                new_cap = 2*old_cap + 1;
                myRealloc(table, new_cap);
        }
}


/* Initializes table structure with decently large 
 *  * capacity for the hash array. */
Table *createHashTable(){
	Table* table = malloc(2*sizeof(long int) + sizeof(Data **));


	table -> cap =  31091; /*Very large arbitrary number*/
        table -> size = 0;
        table -> hashArr = malloc( 31091 * sizeof(Data *));
        flagRealloc(table -> hashArr, 0, 31091);

/*
 This was before, I figured it out resizing :)
 	table -> cap =  500000*2 + 1;
        table -> size = 0;
        table -> hashArr = malloc( (500000*2 + 1) * sizeof(Data *));
        flagRealloc(table -> hashArr, 0, 500000*2 + 1);*/


        return table;

}

/*Returns 0 if we don't need to grow table
 *  * and 1 if we do, the chosen load factor 
 *   * was 0.5*/
int isFull(Table *table){
	if ( ((double) table -> size / (double) table -> cap) > 0.5)
		return 1;
	else{
		return 0;
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
 *  * this takes into account quad. 
 *   * collisions. Returns 1 for presence
 *    * and 0 for absence. */
int isPresent(Table *table, char *string){
	int index = horner_hash(table, string);
	int new_index = index;
	int count = 0;
	Data **array = table -> hashArr;
	/*Seemingly infinite loop ends because inevitably
 *  * 	reaches one of the following two conditions*/
	while (1>0){
		/*If absent, returns false*/
		if(array[new_index] -> word == NULL){
			return 0;

		/*If present, returns true*/
		} else if(strcmp(array[new_index] -> word, string) == 0){
			return 1;
		}else{
/*		new_index = (index + (int) pow(2, count)) % (table -> cap);*/
		new_index = (index + count) % (table -> cap);
		count++;
		}
	}
}

/* Calculates the index of a word inside the hash
 *  * table. If present, it outputs the actual index. 
 *   * Conversely, if absent, if defaults the index to
 *    * -1.*/

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
                 
               /* new_index = (index + (int) pow(2, count)) % (table -> cap);*/
		new_index = (index + count) % (table -> cap);
        	count++;     
	   }
	}
}

/* Find the frequency of a word if present.
 *  * If not, outputs 0. */
int get_freq(Table *table, int index){
	Data **array = table -> hashArr;
	Data *data = array[index];
	
	return data -> freq;
}

/* This function adds an element to the table
 *  * in case it was not present already, incrementing
 *   * the table size. Conversely, if the word is already 
 *    * present, it just increments its frequency*/

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
	}
	array = table -> hashArr;
	while (1 > 0){
		/*New word in*/
		if (array[new_index] -> word == NULL){
			Data *new_word = array[new_index];
			new_word -> freq = 1;
			new_word -> word = string;
			growTable(table);
			return;
		}else if (strcmp(array[new_index] -> word, string) == 0){
			
			old_freq = array[new_index] -> freq;
			array[new_index] -> freq = old_freq + 1;
			return;}		
		/*new_index = (index + (int) pow(2, count) ) % (table -> cap);*/
		new_index = (index + count) % (table -> cap);
		count++;
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
	free(array);	

}

void destroyWord(char *word){
	free(word);
}



/* This part of the program will be responsible 
 *  * for reading words from an input file. */

char *read_long_word(FILE *file){
        int currentChar;
        int wordLimit = 100; /* Arbitrary, grows as needed*/
        int charCount = 0;
        char *word = malloc(wordLimit * sizeof(char));
        currentChar = fgetc(file);
	
	/* Checking if first character is not part of a word, 
 *  	 * for example, many white spaces.	         */
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
 *  * the words in descending order. Then, words with higher
 *   * frequency will be first, and ties will be decided based
 *    * on reverse alphabetical order.*/


/* This functions tells qsort how to compare two Data pointers.
 *  * Words with higher frequency will come first and ties will 
 *   * be decided upon reverse alphabetical order (descending order). */
int compareData(const void *p1, const void *p2){
	
	Data *data1 = * (Data * const *) p1;
	Data *data2 = * (Data * const *) p2 ;

	char *word1 = data1 -> word;	
	char *word2 = data2 -> word;	
		
	int freq1 = data1 -> freq;
	int freq2 = data2 -> freq;	

	/* Higher frequency will be first in array*/
	if ( freq1 != freq2){
		return - (freq1 - freq2);
	
	/* Both words are NULL*/
	}else if(word1 == word2){		
		return 0;
	/* Both elements have words*/
	}else{
		return - strcmp(word1, word2);
	}
}



/*This part of the program decides type of command line input
 *  *
 *   * TYPE 1: User uses -n and specifies how many words. 
 *    * TYPE 2: User doesn't use -n flag, program defaults k to 10.
 *     * */



int typeOfCommand(int argc, char* argv[]){
	if (argc == 1){
		return 3;
	}
	if( argv[1][0] != '/' && strcmp(argv[1], "-n") ==  0){

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
		
		return 1;
	}
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
	int stdinCheck = 0;
	int end = argc;

	/*Checking for stdin*/
	if(!isatty(0)){
		stdinCheck = 1;
	}


	/*Storing words in Hash Table*/
	if (commandLine == 1){
		k = atoi(argv[2]);	 
		start = 3;
	}else{
		k = 10;
	}
	if (stdinCheck){
		end = 2;	
	}

	/* Iterating over all files*/
	for (count = start; count < end; count++ ){
		FILE *currentFile = fopen(argv[count], "r");
		if (stdinCheck){
			currentFile = stdin;
		}
		if ( currentFile != NULL){

			char* currentWord = read_long_word(currentFile);
			while (currentWord != NULL){
				add(table, currentWord);
				currentWord = read_long_word(currentFile);
			} 
		fclose(currentFile);
		}else{
		perror(argv[count]);
		}
	}
	

	qsort(table -> hashArr, table -> cap, sizeof(Data *), compareData);


	printf("The top %d words (out of %d) are:\n", k, (int) table -> size);


	for (count = 0; count < k; count++)
	{	
		char* word = (table -> hashArr)[count] -> word;
	 	long int freq = (table -> hashArr)[count] -> freq;
	
		/*k is bigger than word count*/
		if (word == NULL){
			break;	
		}
		printf("%9d %s\n", (int) freq, word);
	

	} 	
	
	destroyTable(table);     
	return 0;
}
        int count = 0;
