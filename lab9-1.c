#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// Fill out this structure
struct HashType
{
	struct RecordType **records; // Array of pointers to records
	int *counts;				 // Array to store counts of records at each index
	int size;					 // Size of the hash table
};

// Compute the hash function
int hash(int x)
{
	int hashTableSize = 23; // Assuming a hash table size of 100 for demonstration
	return x % hashTableSize;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
	{
		if (pHashArray->counts[i] > 0)
		{ // Check if the index is occupied
			printf("index %d ->", i);
			for (int j = 0; j < pHashArray->counts[i]; ++j)
			{
				if (j > 0)
				{
					printf(" ->"); // Add '->' only between records, not at the beginning or the end.
				}
				struct RecordType *rec = &(pHashArray->records[i][j]);
				printf(" %d, %c, %d", rec->id, rec->name, rec->order);
			}
			printf("\n"); // Ends the line after all records are printed for a given index.
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Hash table implementation
	struct HashType hashTable;
	hashTable.size = 100; // Use a size suitable for your needs
	hashTable.records = (struct RecordType **)calloc(hashTable.size, sizeof(struct RecordType *));
	hashTable.counts = (int *)calloc(hashTable.size, sizeof(int));

	// Hashing records into the hash table
	for (int i = 0; i < recordSz; i++)
	{
		int index = hash(pRecords[i].id);
		if (hashTable.counts[index] == 0)
		{
			hashTable.records[index] = (struct RecordType *)malloc(sizeof(struct RecordType));
		}
		else
		{
			hashTable.records[index] = (struct RecordType *)realloc(hashTable.records[index], (hashTable.counts[index] + 1) * sizeof(struct RecordType));
		}
		hashTable.records[index][hashTable.counts[index]++] = pRecords[i];
	}

	// Displaying records in the hash structure
	displayRecordsInHash(&hashTable, hashTable.size);

	// Freeing memory
	for (int i = 0; i < hashTable.size; i++)
	{
		if (hashTable.counts[i] > 0)
		{
			free(hashTable.records[i]);
		}
	}

	return 0;
}