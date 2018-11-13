#include <kmeansCluster.h>
using namespace std;

int * kmc_seq_final(int clusters, int size, double *xcomp, double *ycomp)
{
    //Random
    std::mt19937 rng;
    uint32_t seed_val;
    rng.seed(seed_val);

    int*    sets                      = (int *) calloc(size,sizeof(int));
    int     point_set_idx             = 0;
    int*    sets_counter              = (int *) calloc(clusters,sizeof(int));
    double  max                       = -DBL_MAX;
    double  norm                      = 0.0;
    double  error                     = DBL_MAX;
    double  random_real               = 0.0;


    double centroid_x[clusters];
    double centroid_y[clusters];
    double* centroid_old              = (double*) malloc(sizeof(double)*clusters*2);
    double  centroid_point_distance   = 0.0;
    //double reduction + sets = 0
    for(size_t i = 0; i < size; i++)
    {
        if(max < xcomp[i])
            max = xcomp[i];
        if(max < ycomp[i])
            max = ycomp[i];  
    }
    //sets_counter = 0
    uniform_real_distribution<double> urd_g(0,max); 
    for(int i = 0 ; i < clusters; i++){
        centroid_x[i] = urd_g(rng);
        centroid_y[i] = urd_g(rng);
        //sets_counter[i] = 0;
    }

    while( error != 0.0)
    {


        //"ASSIGNMENT STEP"
        for (int point_idx = 0; point_idx < size ;  point_idx++)
        {
            int current_point_cluster_idx = -1;
            double minimun_distance = DBL_MAX;

            for (int cluster_idx = 0; cluster_idx < clusters; cluster_idx++)
            {
                double dx = centroid_x[cluster_idx] - xcomp[point_idx];
                double dy = centroid_y[cluster_idx] - ycomp[point_idx];
                double centroid_point_distance = dx*dx + dy*dy;
                if(minimun_distance > centroid_point_distance)
                {
                    minimun_distance = centroid_point_distance; 
                    current_point_cluster_idx = cluster_idx;
                }
            }

            sets_counter[current_point_cluster_idx]++;
            sets[point_idx] = current_point_cluster_idx;
            
        }





        //DEEP COPY 
        for(int cluster_idx = 0; cluster_idx < clusters ; cluster_idx++)
        {
            norm = norm - centroid_y[cluster_idx] - centroid_x[cluster_idx];
            centroid_x[cluster_idx] = 0.0;
            centroid_y[cluster_idx] = 0.0;
            sets_counter[cluster_idx] = 1/sets_counter[cluster_idx];
        }


        //"UPDATE STEP"
        for(int i = 0; i < size; i++)
        {
            int point_set_idx = sets[i];
            int set_size = sets_counter[point_set_idx];
            centroid_x[point_set_idx] += xcomp[i]*set_size;
            centroid_y[point_set_idx] += ycomp[i]*set_size;
            
        }

        for(int k = 0; k < clusters; k++)
        {
            sets_counter[k] = 0;
            norm += centroid_x[k];
            norm += centroid_y[k];
        }
        error = norm;
        norm = 0.0;
    }
    
    return sets;
}

int * kmc_seq_initial(int clusters, int size, double *xcomp, double *ycomp)
{
    //Random
    
    double timer, timer1, timer2, timer3, timer4, timer5, timer6, timer7 ;
    timer1=timer2=timer3=timer4=timer5=timer6=timer7= 0.0;
    utils_start_section_timer();
    std::mt19937 rng;
    uint32_t seed_val;
    rng.seed(seed_val);
    int*    sets                      = (int *) calloc(size,sizeof(int));
    int     point_set_idx             = 0;
    double* sets_counter              = (double *) calloc(clusters,sizeof(double));
    double  max                       = -DBL_MAX;
    double  error                     = DBL_MAX;
    double  c_error = DBL_MAX;
    double  random_real               = 0.0;

    double centroid_x[clusters];
    double centroid_y[clusters];
    timer1 = utils_stop_section_timer()/(double) 1000;



    utils_start_section_timer();
    //double reduction + sets = 0
    for(size_t i = 0; i < size; i++)
    {
        if(max < xcomp[i])
            max = xcomp[i];
        if(max < ycomp[i])
            max = ycomp[i];  
    }
    timer2 = utils_stop_section_timer()/(double) 1000;
    //sets_counter = 0
    uniform_real_distribution<double> urd_g(0,max); 
    utils_start_section_timer();
    for(int i = 0 ; i < clusters; i++){
        centroid_x[i] = urd_g(rng);
        centroid_y[i] = urd_g(rng);
    }
    timer3 = utils_stop_section_timer()/(double) 1000;


    

    do
    {
        c_error = error;
        error = 0.0;
        //"ASSIGNMENT STEP"
        utils_start_section_timer();
        for (int point_idx = 0; point_idx < size ;  point_idx++)
        {
            int current_point_cluster_idx = -1;
            double minimun_distance = DBL_MAX;

            for (int cluster_idx = 0; cluster_idx < clusters; cluster_idx++)
            {
                double dx = centroid_x[cluster_idx] - xcomp[point_idx];
                double dy = centroid_y[cluster_idx] - ycomp[point_idx];
                double centroid_point_distance = dx*dx + dy*dy;
                if(minimun_distance > centroid_point_distance)
                {
                    minimun_distance = centroid_point_distance; 
                    current_point_cluster_idx = cluster_idx;
                }
            }

            sets_counter[current_point_cluster_idx] += 1.0;
            sets[point_idx] = current_point_cluster_idx;
        }
        timer4 += utils_stop_section_timer()/(double) 1000;




        utils_start_section_timer();
        for(int cluster_idx = 0; cluster_idx < clusters ; cluster_idx++)
        {
            error = error - centroid_y[cluster_idx] - centroid_x[cluster_idx];
            centroid_x[cluster_idx] = 0.0;
            centroid_y[cluster_idx] = 0.0;
            sets_counter[cluster_idx] = 1/sets_counter[cluster_idx];
        }
        timer5 += utils_stop_section_timer()/(double) 1000;
        utils_start_section_timer();

        for(int i = 0; i < size; i++)
        {
            int point_set_idx = sets[i]*2;
            centroid[point_set_idx] += xcomp[i];
            centroid[point_set_idx+1] += ycomp[i];
            
        }
        timer6 += utils_stop_section_timer()/(double) 1000;
        utils_start_section_timer(); 
        for(int k = 0; k < clusters; k++)
        {
            sets_counter[k] = 0;
            error = error + centroid_x[k] + centroid_y[k];
        }
        timer7 += utils_stop_section_timer()/(double) 1000;
    }while(error != c_error);
    cout << timer1 << ","<< timer2 << "," << timer3 << "," << timer4 << "," << timer5 << "," << timer6 << "," << timer7 << endl; 
    return sets;
}
void kmc_par(){}
