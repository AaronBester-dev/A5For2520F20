#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/*Make and allocates memory for a new performance struct */
struct Performance * newPerformance(){
    struct Performance * perf = malloc(sizeof(struct Performance));
    perf->frees = 0;
    perf->mallocs = 0;
    perf->reads = 0;
    perf->writes = 0;
    return(perf);
}
/*Makes and allocates memory for a new hashtable structure */
struct HashTable *createTable(struct Performance * performance, unsigned int capacity, int(*hash)(void *, int ),int (*compar)(const void *, const void *)){
    struct HashTable * newHashTable = malloc(sizeof(struct HashTable));
    int i = 0;

    newHashTable->capacity = capacity;
    newHashTable->nel = 0;
    newHashTable->data = malloc(sizeof(void *) * capacity);

    for(i = 0; i < newHashTable->capacity; i++){
        newHashTable->data[i] = NULL;
    }

    newHashTable->hash = *hash;
    newHashTable->compar = *compar;
    performance->reads += capacity;
    performance->mallocs++;

    return(newHashTable);
}
/*Adds element to the necessary location in hashtable*/
void addElement(struct Performance * performance, struct HashTable * table, void * src){
    int index = table->hash(src,table->capacity);
    
    if(table->nel == table->capacity){
        fprintf(stderr,"ERROR- HashTable is full\n");
    }

    while(table->data[index] != NULL){
        performance->reads++;
        index++;
        if(index == table->capacity){
            index = 0;
        }
    }
    
    table->data[index] = src;
    table->nel++;
    performance->writes++;
}
/*Gets the id of whatever element the user wants. If element can't be found -1 is returned*/
int getIdx(struct Performance * performance, struct HashTable * table, void * src){
    int index = table->hash(src,table->capacity);
    int timesRun = 0;
    int comparResult = -1;
   
    while(timesRun < table->capacity){
        
        if(table->data[index] != NULL){
            comparResult = table->compar(src,table->data[index]);
            performance->reads++;
        }
        
        if(comparResult == 0 ){
            return(index); 
        }
        
        timesRun++;
        index++;
        if(index == table->capacity){
            index = 0;
        }
    }
    
    return(-1);
}
/*Frees the hashtable*/
void freeTable(struct Performance * performance, struct HashTable * table){
    free(table->data); 
    free(table);
    performance->frees++;
}
/*gets element from a hashtable. If element can't be found it returns null */
void * getElement (struct Performance * performance, struct HashTable * table, void * src){
    int index = getIdx(performance,table,src);
    
    if(index == -1){
        return NULL;
    }
    else{
        return(table->data[index]);
    }
}
/*Removes a element from the hashtable*/
void removeElement(struct Performance *performance, struct HashTable * table, void * target){
    int index = getIdx(performance,table,target);
    if(index != -1){
        
        table->data[index] = NULL;
        performance->writes++;
        table->nel--;
    }
    
}
/*Calculates the hashaccuracy of the hashtable*/
int hashAccuracy(struct HashTable * table){
    int i = 0;
    int hashIndex = 0;
    int totalDifference = 0;
    for(i = 0; i < table->capacity; i++ ){
        if(table->data[i] != NULL){
            hashIndex = table->hash(table->data[i],table->capacity);
            if(i < hashIndex){
                totalDifference += (table->capacity - hashIndex);
                totalDifference += i;
            }
            else{
                totalDifference += (i - hashIndex);
            }
        }
    }

    return(totalDifference);
}
/*Rehashes each element in order to improve hashaccuracy*/
void rehash(struct HashTable * table){
    int i = 0;
    int hashIndex = 0;
   
    for(i = 0; i < table->capacity; i++){
        if(table->data[i] != NULL){
            hashIndex = table->hash(table->data[i], table->capacity);
            if(i != hashIndex){
                while(hashIndex != i){
                    if(table->data[hashIndex] == NULL){
                        table->data[hashIndex] = table->data[i];
                        table->data[i] = NULL;
                        hashIndex = i;
                    }
                    else{
                        hashIndex++;
                        if(hashIndex == table->capacity){
                            hashIndex = 0;
                        }
                    }
                }
            }
        }
    }
}