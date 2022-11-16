/*
 * lookup2 : local file ; setup an in-memory index of words
 *             and pointers into the file. resource is file name
 *             use qsort & bsearch
 */

#include <stdlib.h>
#include "dict.h"

typedef struct {
    char word[WORD];  /* The word to be looked up */
    long off;         /* Offset into file for word definition */
} Index;

/*
 * This ugly little function can be used by qsort & bsearch
 * It compares the word part of two structures of type Index
 */

int dict_cmp(const void *a,const void *b) {
  return strcmp(((Index *)a)->word,((Index *)b)->word);
}

int lookup(Dictrec * sought, const char * resource) {
  static Index * table;
  Dictrec dr;
  static int numrec;
  int i;
  Index * found,tmp;
  static FILE * in;
  static int first_time = 1;

  if (first_time) { /* set up index */
    first_time = 0;

    /* Open file. */
    if ( (in = fopen(resource, "r")) == NULL)
       return UNAVAIL; 

    /* Get number records in file by dividing ending file offset by recsize. */
    fseek(in,0L,SEEK_END);
    numrec = ftell(in) / sizeof(Dictrec); 

    /* Go to the beginning of the file. */
    fseek(in,0L,SEEK_SET);
    table = calloc(sizeof(Index),numrec);

    /* Read the file into the just-allocated array in memory. */
    for(i=0;fread(&dr,sizeof(dr),1,in) == 1;++i){
       strcpy(table[i].word,dr.word);
       table[i].off = i * sizeof(dr);
    }

    /* Sort the table of entry/offset Index structures. */
    qsort(table,numrec,sizeof(Index),dict_cmp);

  } /* end of first-time initialization */
    
  /* use bsearch to find word in the table; seek & read from file if found. */
  strcpy(tmp.word,sought->word);
  found = bsearch(&tmp,table,numrec,sizeof(Index),dict_cmp);

  /* If found, go to that place in the file, and read the record into the
   * caller-supplied space.
   */
  if (found) {
    fseek(in,found->off,SEEK_SET);
    fread(sought,sizeof(dr),1,in);   
    return FOUND;
  }
  return NOTFOUND;
}
