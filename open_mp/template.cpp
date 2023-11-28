#include <iostream>
#include <string>
#include <map>
#include <omp.h>
#include <unistd.h>
#include <random>

#include "../utils/utils.h"
using namespace std;

int main() {
    // Read JSON from file
    string config_file_pth = "../Config/config_matrix_multiply_cpp.json";
    string output_folder = "../output_csvs/";

    // Parse JSON and create a map
    map<string, int> args = readJsonFile(config_file_pth);

    int total_runs = args["runs"];
    int num_threads = args["num_threads"];
    
    vector<vector<double>> thread_creation_times, thread_execution_times, thread_total_times;
    int runs = 0;
    while(runs < total_runs){
        double creation_times[num_threads], execution_times[num_threads], total_time[1];
        double start_time, end_time, end_time1, end_time2;
        start_time = omp_get_wtime();

        omp_set_num_threads(num_threads);
        #pragma omp parallel 
        {
            int thread_no = omp_get_thread_num();

            // Thread creation time calculations
            end_time1 = omp_get_wtime();
            double thread_creation_time = (end_time1 - start_time);
            creation_times[thread_no] = thread_creation_time;
            printf("Run %d, Thread %d creation time: %f seconds\n", runs, thread_no, thread_creation_time);

            // Work goes here
            sleep(1);

            //Thread execution times
            end_time2 = omp_get_wtime();
            double thread_execution_time = (end_time2 - end_time1);
            execution_times[thread_no] = thread_execution_time;
            printf("Run %d, Thread %d creation time: %f seconds\n", runs, thread_no, thread_execution_time);
        }

        // Total exceution time for 
        end_time = omp_get_wtime();
        total_time[0] = end_time - start_time;
        printf("Runs %d, Threads %d, total time: %f seconds\n", runs, num_threads, total_time[0]);

        // Group times
        thread_creation_times.push_back(arr_to_vec(creation_times, num_threads));
        thread_execution_times.push_back(arr_to_vec(execution_times, num_threads));
        thread_total_times.push_back(arr_to_vec(total_time, 1));

        runs++;
    }

    // Pushing data to csv
    const string thread_creation_filename = output_folder + "thread_creation" + args_to_str(args) + ".csv";
    const string thread_execution_filename = output_folder + "thread_execution" + args_to_str(args) + ".csv";
    const string thread_totaltime_filename = output_folder + "thread_total_times" + args_to_str(args) + ".csv";
    writeCSV(thread_creation_filename, thread_creation_times);
    writeCSV(thread_execution_filename, thread_execution_times);
    writeCSV(thread_totaltime_filename, thread_total_times);

    return 0;
}