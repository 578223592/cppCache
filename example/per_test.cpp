//
// Created by swx on 23-12-5.
//
#include <string>
#include "LRUCache.h"
#include "LFUCache.h"
#include "ARCCache.h"
#include "Cache.h"
#include <iostream>
#include <cstring>

// represents expected command line arguments
typedef struct {
    int c;        // cache size in number of pages
    char fname[1024];
    FILE *trace;  // trace file
} input;
// represents a line in a trace file
// fields are self explanatory, see description.pdf
// for more info.
typedef struct {
    int starting_block;
    int number_of_blocks;
    int ignore;
    int request_number;
} trace_line;

// display usage info
int usage (char *bin) {
    printf(
            "USAGE: %s <c> <trace>\n"
            "  c - cache size in number of pages\n"
            "  trace - location of trace file\n"
            , bin);
    return 2;
}

// parse command line arguments
//   argc - argument count
//   argv - array of string arguments
// return 0 if successful
int parse_args (int argc, char **argv, input* in) {

    // check correct number of parameters are passed
    if (argc != 3) {
        printf("Error: not enough parameters passed\n");
        return usage(argv[0]);
    }

    // check first parameter is an integer
    if (sscanf(argv[1], "%d", &in->c) != 1) {
        printf("Error: c in not an integer\n");
        return usage(argv[0]);
    }

    // check trace file by opening it
    in->trace = fopen(argv[2], "r");
    if (in->trace == nullptr) {
        printf("Error: \"%s\" is not a valid path\n", argv[2]);
        return usage(argv[0]);
    }
    std::strcpy(in->fname, argv[2]);

    // made it this far, congrats!
    return 0;
}

// parse out the next line of the trace file
//   f - open trace file pointer
//   l - line struct pointer to populate line entries
// return 0 when done
// NOTE: assuming the trace file is similar to
// the description specification
int next_line (FILE *f, trace_line *l) {

    if (fscanf(f,
               "%d %d %d %d\n",
               &l->starting_block,
               &l->number_of_blocks,
               &l->ignore,
               &l->request_number
    ) == EOF) {
        return 0;
    }

    return 1;
}

void cache_get(Cache<std::string,std::string > *cacheAlg, int key){
    cacheAlg->set(std::to_string(key),std::to_string(key));
}
void cache_print_stats(Cache<std::string,std::string > *c) {
    printf(
            "requests: %8d, "
            "hits: %8d, "
            "ratio: %5.2f\n",
            c->totalCount_,
            c->hitCount_,
            (double)c->hitCount_/(double)c->totalCount_ *100
    );
}
int main(const int argc,char  *argv[]){

    std::cout<<argc<<"\n";
    for(int i = 0;i<argc;++i){
        printf("%s   ", argv[i]);
    }
    std::cout<<"\n";
    std::cout<<"----------------------------"<<"\n";
    input in;
    if (parse_args(argc, argv, &in)) {
        return 1;
    }


    trace_line  l;
    int lines = 0;

    Cache<std::string,std::string > *pLruCache = new LRUCache<std::string , std::string>(in.c);
    Cache<std::string,std::string > *pLfuCache = new LFUCache<std::string , std::string>(in.c);

    Cache<std::string,std::string > *pARCCache = new ARCCache<std::string , std::string>(in.c);


    while (next_line(in.trace, &l)) {
        int n = l.starting_block + l.number_of_blocks;
        for (int i = l.starting_block; i < n; i++) {
            cache_get(pLruCache, i);
            cache_get(pLfuCache, i);
            cache_get(pARCCache, i);
        }
        lines++;
        if (lines % 100000 == 0) {
            fprintf(stderr, "%s(c=%d): processed %d lines\r\n", in.fname, in.c, lines);
            fflush(stderr);
        }
    }

    printf("file: %17s, capacity: %6d, algo: lru, ", in.fname, in.c);
    cache_print_stats(pLruCache);
    printf("file: %17s, capacity: %6d, algo: lfu, ", in.fname, in.c);
    cache_print_stats(pLfuCache);
    printf("file: %17s, capacity: %6d, algo: arc, ", in.fname, in.c);
    cache_print_stats(pARCCache);


    delete pLruCache;
    delete pLfuCache;
    delete pARCCache;
    fclose(in.trace);
    return 0;
}

