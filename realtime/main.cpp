#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>


void run_realtime(std::string path, auto callback) {
    std::string line;
    std::ifstream input_f(path);

    std::getline(input_f, line);
    unsigned long long int previous_time_ms = std::stoull(line);

    while (std::getline(input_f, line)) {
        unsigned long long int current_time_ms = std::stoull(line);
        unsigned long long int time_elapsed_ms = current_time_ms - previous_time_ms;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_elapsed_ms));
        
        std::thread(callback, time_elapsed_ms).detach();

        previous_time_ms = current_time_ms;
    }

    input_f.close();
}


void generate_timeseries_ms(std::string path) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(50, 5);

    auto now = std::chrono::system_clock::now();
    std::ofstream output_f(path);

    for (int i=0; i<1000; i++) {
        double random_milliseconds = distribution(gen);
        std::chrono::milliseconds random_duration(static_cast<long>(random_milliseconds));
        now += random_duration;

        auto current_time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
        output_f << current_time_ms << '\n';
    }

    output_f.close();
}


std::vector<unsigned long long int> elapsed_ms(std::string path) {
    std::string line;
    std::ifstream input_f(path);
    std::vector<unsigned long long int> time_elapsed_data;

    std::getline(input_f, line);
    unsigned long long int previous_time_ms = std::stoull(line);

    while (std::getline(input_f, line)) {
        unsigned long long int current_time_ms = std::stoull(line);
        unsigned long long int time_elapsed_ms = current_time_ms - previous_time_ms;
        time_elapsed_data.push_back(time_elapsed_ms);
        previous_time_ms = current_time_ms;
    }

    input_f.close();
    return time_elapsed_data;
}


void validate_realtime(std::vector<unsigned long long int> elapsed_vector) {
    auto previous_time_ms =  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();

    for (int i=0; i<elapsed_vector.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(elapsed_vector[i]));
        auto current_time_ms =  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();

        std::cout << elapsed_vector[i] << " : " << (current_time_ms - previous_time_ms) << std::endl;
        previous_time_ms = current_time_ms;
    }
}



int main() {
    // generate timed data
    generate_timeseries_ms("/home/poseidon/realtime/build/time_ms.txt");

    // run realtime
    auto callback = [](unsigned long long int time_elapsed_ms) -> void { std::cout << "Elapsed: " << time_elapsed_ms << '\n'; };
    run_realtime("./time_ms.txt", callback);

    // validation
    auto time_elapsed_data = elapsed_ms("./time_ms.txt");
    validate_realtime(time_elapsed_data);

    return 0;
}