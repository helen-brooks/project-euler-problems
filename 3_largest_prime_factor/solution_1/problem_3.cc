#include <iostream>
#include <vector>
#include <stdexcept>

/// Return if a is divisble by b
bool is_divisible(long int a, long int b){
  return (a % b == 0);
}

class PrimeGenerator {

public:

  PrimeGenerator(){
    prime_cache.push_back(2);
  };
  ~PrimeGenerator(){};

  /// Add a new prime to the list of known primes
  long int generate_new_prime();

  /// Return the next prime number which exceeds the input value
  /// Also add to the list of known primes
  long int next_prime(long int value);

  void print_all_known_primes(){
    std::cout<<"Known primes: "<<std::endl;
    for( const auto & prime : prime_cache ){
      std::cout<<prime<<" ";
    }
    std::cout<<std::endl;
  };

private:

  /// Store known primes
  std::vector<long int> prime_cache;

};

long int largest_factor(long int number){

  PrimeGenerator pgen;

  long int start_prime = 2;
  std::vector<long int> prime_factors;

  long int current_prime = start_prime;
  long int current_factor = number;
  long int largest_so_far = start_prime;

  while( current_prime <= current_factor){

    if ( is_divisible(current_factor,current_prime) ){
      // Found a new prime factor!
      long int new_factor = current_factor / current_prime;
      prime_factors.push_back(current_prime);

      // Save if this was the largest so far
      if( current_prime > largest_so_far){
        largest_so_far = current_prime;
      }

      // Repeat process on the new factor
      current_prime = start_prime;
      current_factor = new_factor;

    }
    else{
      // Generate the next prime number
      current_prime = pgen.next_prime(current_prime);
    }
  }

  // Sanity check
  long int check_result =1;
  for( const auto& factor : prime_factors){
    check_result *= factor;
  }
  if ( check_result != number){
    throw std::logic_error("Failed to find prime factors");
  }

  pgen.print_all_known_primes();

  return largest_so_far;

}

int main(int argc, char *argv[]) {

  long int target = 600851475143;
  long int result = 0;
  try{
    result = largest_factor(target);
  }
  catch( const std::exception& e){
    std::cerr<<"Failed to return largest factor"<<std::endl;
    std::cerr<<e.what()<<std::endl;;
    return -1;
  }

  std::cout<<"The largest prime factor of "<< target <<" is: ";
  std::cout<<result<<std::endl;
  return 0;
}

/// Add a new prime to the list
long int PrimeGenerator::generate_new_prime(){

  long int candidate = prime_cache.back()+1;
  bool candidate_is_prime = false;

  while(!candidate_is_prime){

    // Loop over all known primes and see if this is a factor.
    bool found_factor=false;
    for( size_t i_prime=0;  i_prime<prime_cache.size(); i_prime++ ){
       long int current_prime = prime_cache.at(i_prime);
       if ( is_divisible(candidate,current_prime) ){
         found_factor=true;
         break;
       }
    }

    // No factors means this is a new prime
    if(!found_factor){
      candidate_is_prime = true;
      prime_cache.push_back(candidate);
    }
    else{
      // New candidate
      candidate++;
    }

  }

  return candidate;
}


long int PrimeGenerator::next_prime(long int value){

  long int max_known_prime = prime_cache.back();

  // If less than max known prime, just return first prime that is larger
  if(value<max_known_prime){
    for(size_t i_prime=0; i_prime<prime_cache.size(); i_prime++){
      long int current_prime = prime_cache.at(i_prime);
      if( current_prime > value){
        return current_prime;
      }
    }
  }

  // Keep generating new primes until we exceed this value
  while(max_known_prime<=value){
    max_known_prime = generate_new_prime();
  }

  return max_known_prime;
}
