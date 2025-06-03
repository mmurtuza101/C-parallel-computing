#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MAX_VALUE 999
#define ARRAY_SIZE 1000

void count_sort(int *local_data, int local_size, int *local_count) {
    for (int i = 0; i < local_size; i++) {
        local_count[local_data[i]]++;
    }
}

int main(int argc, char **argv) {
    int rank, size, i;
    int *data, *local_data, *local_count, *global_count;
    int local_size, total_size;
    FILE *file;
    int count;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <file_name>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }
    if (rank == 0) {
        file = fopen(argv[1], "rb");
        if (!file) {
            printf("Error opening file\n");
            MPI_Finalize();
            return 1;
        }

        fseek(file, 0, SEEK_END);
        total_size = ftell(file) / sizeof(int);
        fseek(file, 0, SEEK_SET);

        data = (int *)malloc(total_size * sizeof(int));
        fread(data, sizeof(int), total_size, file);
        fclose(file);
    }

    MPI_Bcast(&total_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_size = total_size / size;
    local_data = (int *)malloc(local_size * sizeof(int));
    local_count = (int *)calloc(ARRAY_SIZE, sizeof(int));
    MPI_Scatter(data, local_size, MPI_INT, local_data, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    count_sort(local_data, local_size, local_count);
    global_count = (int *)calloc(ARRAY_SIZE, sizeof(int));
    MPI_Reduce(local_count, global_count, ARRAY_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        file = fopen(argv[1], "wb");
        if (!file) {
            printf("Error opening file for writing\n");
            MPI_Finalize();
            return 1;
        }
        for (i = 0; i < ARRAY_SIZE; i++) {
            for (int j = 0; j < global_count[i]; j++) {
                fwrite(&i, sizeof(int), 1, file);
            }
        }
        fclose(file);
        free(data);
    }
    free(local_data);
    free(local_count);
    free(global_count);
    MPI_Finalize();
    return 0;
}

