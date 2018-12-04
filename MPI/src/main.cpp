#include <utils.h>
#include <mpi.h>
#include <kmeansCluster.h>

using namespace std;

// kmeans PAR/SEQ REPS SIZE DATASET PAPI_OPT

int main(int argc, char *argv[])
{
    int repetitions;
    int size;
    int clusters;
    char const *filename = NULL;
    char const *mode = NULL;
    char const *papiOpt = NULL;
    double *xcomp = NULL;
    double *ycomp = NULL;
    int *sets = NULL;
    if (argc < 6)
    {
        fprintf(stderr, "Usage: ./bin/kmeans PAR|SEQ #REPETITIONS #CLUSTERS #SIZE DATASET_PATH \n");
        return -1;
    }

    mode = argv[1];
    repetitions = atoi(argv[2]);
    clusters = atoi(argv[3]);
    size = atoi(argv[4]);
    filename = argv[5];
    xcomp = (double *)malloc(sizeof(double) * size);
    ycomp = (double *)malloc(sizeof(double) * size);
    int utils_error = utils_read_dataset(filename, xcomp, ycomp);

    if (utils_error == -1)
    {
        return -1;
    }

    if (!strcmp(mode, PAR))
    {
        int myrank, nprocesses;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocesses);
        MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
        if (myrank == 0)
            cout << "start;initPhase1;initPhase2;initPhase2;endPhase2;initPhase3;step1Phase3;step2Phase3;step3Phase3;endPhase3;end" << endl;
        for (int i = 0; i < repetitions; i++)
        {
            utils_clear_cache();
            if (myrank == 0)
                printf("%lf;",MPI_Wtime());
            kmc_mpi(clusters, size, xcomp, ycomp, myrank, nprocesses, &sets);
            if (myrank == 0)
                printf("%lf\n",MPI_Wtime());
        }
        MPI_Finalize();
        if (myrank != 0)
        {
            return 0;
        }
    }
    else if (!strcmp(mode, SEQ))
    {
        utils_setup_papi(repetitions);
        for (int i = 0; i < repetitions; i++)
        {
            utils_clear_cache();
            utils_start_papi();
            utils_start_timer();
            sets = kmc_seq_initial(clusters, size, xcomp, ycomp);
            //sets = kmc_seq_final(clusters, size, xcomp, ycomp);

            utils_stop_timer();
            utils_stop_papi(i);
        }
    }

    utils_results();
    utils_save_results("bin/kmc_out.csv", xcomp, ycomp, sets, size);
    utils_clean_memory(xcomp, ycomp);
    return 0;
}
