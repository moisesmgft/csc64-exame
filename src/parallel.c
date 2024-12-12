#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "buffer.h"
#include "trie.h"

#define NUM_RECORDS 30000

#define THRESHOLD_CA_MIN 0.25f
#define THRESHOLD_CB_MAX 0.75f
#define EPS 1e-7f

#define RECORD_A_FILEPATH "output/db/B.txt"
#define RECORD_B_FILEPATH "output/db/A.txt"
#define ID_FILEPATH "output/db/ids.txt"

typedef struct
{
    char id[6];
    float value;
} Record;

int comparator(const void *left, const void *right)
{
    return ((const Record *)left)->value > ((const Record *)right)->value;
}

FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }
    return file;
}

void load_records(const char *value_filename, const char *id_filename, Record *records)
{
    FILE *value_file = open_file(value_filename, "r");
    FILE *id_file = open_file(id_filename, "r");
    for (int i = 0; i < NUM_RECORDS; i++)
    {
        fscanf(value_file, "%f", &records[i].value);
        fscanf(id_file, "%s", records[i].id);
    }
    fclose(value_file);
    fclose(id_file);
}

size_t lower_bound(Record *records, size_t size, float x)
{
    size_t lo = 0, hi = size;
    while (lo + 1 < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (records[mid].value < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return records[lo].value < x ? hi : lo;
}

void combine_ids(const char *id1, const char *id2, char *result)
{
    result[0] = id1[0];
    result[1] = id2[1];
    result[2] = id1[2];
    result[3] = id2[3];
    result[4] = id1[4];
    result[5] = '\0';
}

int main(int argc, char *argv[])
{

    if (argc != 1)
    {
        printf("Requer numero de threads.\n");
        exit(1);
    }
    int num_threads;
#ifdef _OPENMP
#pragma omp parallel
    {
        num_threads = omp_get_num_threads();
    }
#else
    num_threads = 1;
#endif

    clock_t start = clock();

    char output_filepath[256];
    snprintf(output_filepath, sizeof(output_filepath), "output/output_%zu.csv", num_threads);
    FILE *output = open_file(output_filepath, "w");
    Record *recordsA = (Record *)malloc(NUM_RECORDS * sizeof(Record));
    Record *recordsB = (Record *)malloc(NUM_RECORDS * sizeof(Record));

    if (!recordsA || !recordsB)
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }

    fprintf(output, "ID_a_m,ID_b_M,ID',a_m,b_M,f\n");
    load_records(RECORD_A_FILEPATH, ID_FILEPATH, recordsA);
    load_records(RECORD_B_FILEPATH, ID_FILEPATH, recordsB);

    TrieNode *trie = create_node();
    for (size_t i = 0; i < NUM_RECORDS; i++)
    {
        assert(strcmp(recordsA[i].id, recordsB[i].id) == 0);
        insert(trie, recordsA[i].id, recordsA[i].value * recordsB[i].value);
    }

    qsort(recordsA, NUM_RECORDS, sizeof(Record), comparator);
    qsort(recordsB, NUM_RECORDS, sizeof(Record), comparator);

    size_t beginA = lower_bound(recordsA, NUM_RECORDS, THRESHOLD_CA_MIN);
    size_t endB = lower_bound(recordsB, NUM_RECORDS, THRESHOLD_CB_MAX);

    Buffer *buffers = (Buffer *)calloc(num_threads, sizeof(Buffer));

    for (size_t i = 0; i < num_threads; i++)
    {
        init_buffer(&buffers[i]);
    }

#pragma omp parallel for
    for (size_t idx_a = beginA; idx_a < NUM_RECORDS; idx_a++)
    {
        for (size_t idx_b = 0; idx_b < endB; idx_b++)
        {
            if (strcmp(recordsA[idx_a].id, recordsB[idx_b].id) == 0)
                continue;

            char combined_id[6];
            combine_ids(recordsA[idx_a].id, recordsB[idx_b].id, combined_id);
            float p;

            if (find(trie, combined_id, &p) == 0)
                continue;

            p *= recordsA[idx_a].value * recordsB[idx_b].value;

            char entry[128];
            snprintf(entry, sizeof(entry), "%s,%s,%s,%f,%f,%f\n",
                     recordsA[idx_a].id, recordsB[idx_b].id,
                     combined_id, recordsA[idx_a].value, recordsB[idx_b].value, p);

            size_t thread_num = omp_get_thread_num();
            append_to_buffer(&buffers[thread_num], entry);
        }
    }

    for (size_t i = 0; i < num_threads; i++)
    {
        flush_buffer(output, &buffers[i]);
        free(buffers[i].buffer);
    }

    fclose(output);

    char command[256];
    snprintf(command, sizeof(command),
             "sort -t, -k6 -n %s -o output/sorted_%zu.csv",
             output_filepath, num_threads);
    system(command);
    snprintf(command, sizeof(command),
             "rm %s", output_filepath);
    system(command);

    free(recordsA);
    free(recordsB);
    free(buffers);
    free_trie(trie);

    clock_t end = clock();
    float duration = ((float)(end - start)) / CLOCKS_PER_SEC;
    printf("Threads = %zu\n", num_threads);
    printf("Tempo de execucao = %f segundos\n", duration);
}
