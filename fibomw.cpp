#include <iostream> // std::cout
#include <cstdint> // uint64_t
#include <vector> // std::vector
#include <thread> // std::thread
#include <future> // std::promise/future
using namespace std;
 template <typename value_t, typename index_t>
 void fibo(
 value_t n,
 std::promise<value_t> && result) { // <- pass promise

	 value_t a_0 = 0;
	 value_t a_1 = 1;

	 for (index_t index = 0; index < n; index++) {
	 	const value_t tmp = a_0; 
	 	a_0 = a_1; 
	 	a_1 += tmp;
	 }

	 result.set_value(a_0); // <- fulfill promise
 }

 int main(int argc, char * argv[]) {

	 const uint64_t num_threads = 32;
	 std::vector<std::thread> threads;

	 // storage for futures
	 std::vector<std::future<uint64_t>> results;
	 // for each thread
	 for (uint64_t id = 0; id < num_threads; id++) {

		 // define a promise and store the associated future
		 std::promise<uint64_t> promise;
		 results.emplace_back(promise.get_future());

		 // emplace the spawned thread
		 threads.emplace_back(
		 // move the promise to the spawned thread
		 // note that promise is now moved elsewhere
		 // and cannot be accessed safely anymore
		 fibo<uint64_t, uint64_t>, id, std::move(promise)
		 );
	 }

	 // read the futures resulting in synchronization of threads
	 // up to the point where promises are fulfilled
	 for (auto& result: results)
	 std::cout << result.get() << std::endl;

	 // this is mandatory since threads have to be either
	 // joined or detached at the end of our program
	 for (auto& thread: threads)
	 thread.join();
 }