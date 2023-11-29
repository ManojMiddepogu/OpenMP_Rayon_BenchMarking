# C-and-Rust-BenchMarking

## Profiling Experiments:
### Rust:
Path: /rayon/ <br>
Compile: cargo build --release <br>
Run: cargo run --release

/Config/profile.json : consists of the profile parameters num_threads and runs.

### C++:
Path: /open_mp/ <br>
Compile: g++ -O3 -fopenmp -march=native -std=c++11 template.cpp ../utils/utils.cpp -o template
Run: ./template

## Performance Experiments:
### Rust:
Path: /Rayon/ <br>
Compile: cargo build --release <br>
Run: cargo run --release -- num_threads matrix_size_height algo <br>
Algo = 0 : full parallel <br>
Algo = 1 : block parallel <br>
Algo = 2 : Strassen parallel <br>
Algo = 3 : Linear parallel <br>
Choose matrix_size_height a multiple of 16

#### Perf
stat: perf stat -e page-faults,cache-misses,cache-references cargo run --release -- num_threads matrix_size_height algo <br>
record: perf record -e page-faults,cache-misses,cache-references cargo run --release -- num_threads matrix_size_height algo <br>
perf report <br>

### C++:
Path: /open_mp/ <br>
Compile: g++ -O3 -fopenmp -march=native -std=c++11 matrix_multiply_plain.cpp ../utils/utils.cpp -o matrix_multiply <br>
Run: ./matrix_multiply

/Config/config_matrix_multiply_cpp.json : consists of the Algo parameters num_threads and runs. <br>
Set any one algo to 1.
#### Perf
stat: perf stat -e page-faults,cache-misses,cache-references ./matrix_multiply <br>
record: perf record -e page-faults,cache-misses,cache-references ./matrix_multiply <br>
perf report <br>








