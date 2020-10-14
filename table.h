/*
 * Defines structures and prototypes, used in fw.c
 * CPE357, Assignement 2
 */

typedef struct Data
{
        int freq;
        char *word;
} Data;

typedef struct Table 
{
	long int cap;
	long int size;
	Data **hashArr;
} Table;


/* Table Implementation*/

void flagRealloc(Data **hashArr, int start, int end);
Table *createHashTable();
int isFull(Table *table);
void growTable(Table *table);
int horner_hash(Table *table, char *string);
int isPresent(Table *table, char *string);
int get_index(Table *table, char *string);
int get_freq(Table *table, int index);
void add(Table *table, char *string);
void addGrow(Table *table, Data *data);

/* Cleaning Table*/
void destroyTable(Table *table);
void destroyHashArr(Data **array, long int size);
void destroyWord(char *word);


/*Reading Words*/
char *read_long_word(FILE *file);

/* Sorting Part*/
void sortArray(Table *table);
int compareData(const void *data1, const void *data2);

/* Command Line Inputs*/
int typeOfCommand(int argc, char* argv[]);
