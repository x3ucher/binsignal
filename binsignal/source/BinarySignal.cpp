
#include <iostream>

#include "BinarySignal.h"

namespace lab2{

  //BinarySignal::BinarySignal() : count(1), signal(new SignalState[this->count]) {}

/**
 * @brief Constructs a BinarySignal with the specified initial level and time.
 * 
 * This constructor initializes a BinarySignal with the given level and time.
 *
 * @param level The initial level (0 or 1) for the BinarySignal.
 * @param time The initial time (duration) for the BinarySignal.
 * @throw std::invalid_argument if the provided signal state is invalid.
 */
  BinarySignal::BinarySignal(int level, int time): count(1), signal(new SignalState[this->count]) {
    if (!(level < 0 || level > 1) && time > 0){
      signal[0].time = time;
      signal[0].level = level;
    }
    else{
      delete [] this->signal;
      throw std::invalid_argument("error: invalid signal state");
    }
  }

/**
 * @brief Constructs a BinarySignal from a string representation.
 * 
 * This constructor creates a BinarySignal based on a string representation of the signal, where '0' and '1' represent level changes.
 *
 * @param signal_str A string containing '0' and '1' characters to represent the signal.
 * @throw std::invalid_argument if the provided string contains invalid characters or has an invalid format.
 */
  BinarySignal::BinarySignal(std::string signal_str){
    if (signal_str.find_first_not_of("01") != std::string::npos){
      throw std::invalid_argument("error: invalid characters in string");
    }
    else {
      count = 0;
      signal = nullptr;

      for (int i = 0; i < (int)signal_str.length(); i++) {
        char current_level = signal_str[i];
        int current_time = 1;
        while (i < (int)signal_str.length() - 1 && signal_str[i + 1] == current_level) {
          current_time++;
          i++;
        }
        *this += SignalState(bool(current_level - '0'), current_time);
      }
    }
  }

/**
 * @brief Constructs a BinarySignal by copying the content of another BinarySignal.
 * 
 * This constructor creates a BinarySignal that is an exact copy of another BinarySignal.
 *
 * @param other The BinarySignal to copy.
 */
  BinarySignal::BinarySignal(const BinarySignal& other) : count(other.count), signal(new SignalState[other.count]) {
    std::copy(other.signal, other.signal + count, this->signal);
  }

/**
 * @brief Move constructor for BinarySignal.
 * 
 * This constructor transfers ownership of the signal data from the source BinarySignal to the new BinarySignal.
 *
 * @param other The BinarySignal to move from.
 */
  BinarySignal::BinarySignal(BinarySignal&& other) noexcept : count(other.count), signal(other.signal) {
    other.count = 0;
    other.signal = nullptr;
  }

/**
 * @brief Get the count of SignalState elements in the BinarySignal.
 * 
 * @return The number of SignalState elements in the BinarySignal.
 */
  int BinarySignal::getCount() const {
    return count;
  }

/**
 * @brief Convert the BinarySignal to a string representation.
 * 
 * This function converts the BinarySignal to a string of '0' and '1' characters based on its SignalState elements.
 *
 * @return A string representation of the BinarySignal.
 */
  std::string BinarySignal::toString() const {
    std::string result;
    for (int i = 0; i < this->count; i++){
      result += std::string(this->signal[i].getTime(), (this->signal[i].getLevel() ? '1' : '0'));
    }
    return result;
  }

/**
 * @brief Move assignment operator for BinarySignal.
 * 
 * This operator moves the content of another BinarySignal to this BinarySignal.
 * It releases the resources owned by this BinarySignal, if any.
 *
 * @param other The BinarySignal to move from.
 * @return A reference to the modified BinarySignal.
 */
  BinarySignal& BinarySignal::operator =(BinarySignal&& other) noexcept  {
    if (this != &other) {
      if (count != 0 && signal){ delete[] signal; }
      this->count = other.count;
      this->signal = other.signal;
      other.count = 0;
      other.signal = nullptr;
    }
    return *this;
  }

/**
 * @brief Copy assignment operator for BinarySignal.
 * 
 * This operator assigns the content of another BinarySignal to this BinarySignal, creating a deep copy.
 * It releases the resources owned by this BinarySignal, if any.
 *
 * @param other The BinarySignal to copy from.
 * @return A reference to the modified BinarySignal.
 */
  BinarySignal &BinarySignal::operator =(const BinarySignal &other){
    if (this == &other) {
      return *this;
    }
    if (count != 0){ delete[] signal; }
    this->count = other.count;
    if (other.count != 0) {
      this->signal = new SignalState[count];
    }
    std::copy(other.signal, other.signal + count, this->signal);
    return *this;
  }

/**
 * @brief In-place multiplication operator for BinarySignal.
 * 
 * This operator multiplies the BinarySignal by an integer n.
 * The signal is repeated n times. If n is less than or equal to 0, an exception is thrown.
 *
 * @param n The positive integer to multiply the BinarySignal by.
 * @return A reference to the modified BinarySignal.
 */
  BinarySignal &BinarySignal::operator *=(int n){
    if (n <= 0){ 
      throw std::invalid_argument("error: not positive number");
    }  
    else if (count == 0){
      return *this;
    }
    else{
      SignalState *result = new SignalState[n * count];
      int k = 0;
      for (int i = 0; i < n; i++){
        for (int j = 0; j < count; j++){
          result[k] = signal[j];
          k++;
        }
      }
      delete[] signal;
      this->signal = result;
      this->count = count * n;
    }
    return *this;
  }

/**
 * @brief Multiplication operator for BinarySignal.
 * 
 * This operator creates and returns a new BinarySignal that is the result of multiplying the original BinarySignal by an integer n.
 * The signal is repeated n times. If n is less than or equal to 0, an exception is thrown.
 *
 * @param n The positive integer to multiply the BinarySignal by.
 * @return A new BinarySignal representing the result of the multiplication.
 */
  BinarySignal BinarySignal::operator *(int n) const{
    if (n <= 0) {
      throw std::invalid_argument("error: not positive number");
    }
    BinarySignal result(*this);
    result *= n;
    return result;
  }

/**
 * @brief In-place addition operator for BinarySignal.
 * 
 * This operator appends the BinarySignal with another BinarySignal. 
 * If the current BinarySignal is empty, it's replaced with the provided BinarySignal.
 * If the current BinarySignal consists of a single empty SignalState (time = 0), it's replaced with the provided BinarySignal.
 *
 * @param other The BinarySignal to append to the current BinarySignal.
 * @return A reference to the modified BinarySignal.
 */
  BinarySignal &BinarySignal::operator +=(const BinarySignal &other){
    if (count == 0 || (this->count == 1 && this->signal[0].time == 0)){
      *this = other;
    }
    if (other.count != 0){
      SignalState *result = new SignalState[count + other.count];
      int j = 0;
      for (int i = 0; i < count; i++){
        result[j] = signal[i];
        j++;
      }
      for (int i = 0; i < other.count; i++){
        result[j] = other.signal[i];
        j++;
      }
      delete[] signal;
      this->signal = result;
      this->count = count + other.count;
    }
    return *this;
  }

/**
 * @brief In-place addition operator for BinarySignal with SignalState.
 * 
 * This operator appends the BinarySignal with a single SignalState.
 * If the current BinarySignal is empty, it's replaced with the provided SignalState.
 * If the current BinarySignal consists of a single empty SignalState (time = 0), it's replaced with the provided SignalState.
 *
 * @param other The SignalState to append to the current BinarySignal.
 * @return A reference to the modified BinarySignal.
 */
  BinarySignal &BinarySignal::operator +=(const SignalState &other){
    if (count == 0){
      signal = new SignalState[1];
      signal[0] = other;
      count = 1;
      return *this;
    }
    if (this->count == 1 && this->signal[0].time == 0){
      this->signal[0] = other;
    }
    if (other.time != 0){
      SignalState *result = new SignalState[count + 1];
      for (int i = 0; i < count; i++){
        result[i] = signal[i];
      }
      result[count] = other;
      delete[] signal;
      this->signal = result;
      this->count = count + 1;
    }
    return *this;
  }

/**
 * @brief Accesses the signal level at a specified time.
 * 
 * This operator returns the signal level (true for high, false for low) at the given time
 * by accumulating the time durations of individual SignalStates in the BinarySignal.
 * 
 * @param time The time at which to determine the signal level.
 * @return The signal level (true for high, false for low) at the specified time.
 * @throw std::invalid_argument if an invalid time is provided.
 */
  bool BinarySignal::operator [](int time){
    int sum_time = 0;
    for (int i = 0; i < count; i++){
      sum_time += signal[i].time;
      if (sum_time > time){
        return signal[i].level;
      }
    }
    throw std::invalid_argument("error: invalid time");
  }

/**
 * @brief Inverts the signal by changing the level of each SignalState.
 * 
 * This function inverts the signal represented by the BinarySignal
 * by changing the level (high to low and low to high) of each SignalState.
 */
  void BinarySignal::invertSignal(){
    for (int i = 0; i < count; i++){
      signal[i].invertSignal();
    }
  }

/**
 * @brief Bitwise NOT operator for BinarySignal.
 * 
 * This operator inverts the signal by creating a new BinarySignal
 * with inverted SignalStates.
 * 
 * @return A new BinarySignal with inverted SignalStates.
 */
  BinarySignal BinarySignal::operator ~(){
    BinarySignal result = *this;
    for (int i = 0; i < count; i++){
      result.signal[i].invertSignal();
    }
    return result;
  }

/**
 * @brief Calculate the total time duration of the signal.
 * 
 * This function calculates and returns the total time duration
 * of the signal by summing the time durations of individual SignalStates.
 * 
 * @return The total time duration of the signal.
 */
  int BinarySignal::totalTime(){
    int sum_time = 0;
    for (int i = 0; i < count; i++){
      sum_time += signal[i].time;
    }
    return sum_time;
  }

/**
 * @brief Formats the BinarySignal as a string with specific encoding.
 * 
 * This function formats the BinarySignal as a string with specific encoding,
 * where consecutive identical signal levels are encoded as a single character
 * followed by a number indicating the duration of that level. Level transitions
 * are marked with either '/' (low to high) or '\' (high to low).
 * 
 * Example: ".../''''\./'\.." represents a BinarySignal with alternating 0s and 1s.
 * 
 * @return The formatted string representation of the BinarySignal.
 */
  std::string BinarySignal::formatedSignal() const{
    std::string formated_signal;
    for (int i = 0; i < count; i++){
      formated_signal += signal[i].formatSignal();
    }
    for (int i = 1; i < (int)formated_signal.length(); i++){
      if (formated_signal[i-1] < formated_signal[i]){
        formated_signal.insert(i, 1, '\\');
        i++;
      }
      else if (formated_signal[i-1] > formated_signal[i]){
        formated_signal.insert(i, 1, '/');
        i++;
      }
    }
    return formated_signal;
  }

/**
 * @brief Inserts another BinarySignal at the specified time into the current BinarySignal.
 *
 * This function inserts the signal from another BinarySignal at the specified time
 * into the current BinarySignal. The signal is inserted without overlapping or
 * affecting the original signal, preserving its integrity. 
 *
 * @param other The BinarySignal to be inserted.
 * @param time The time at which to insert the other BinarySignal.
 *
 * @return A reference to the modified current BinarySignal.
 *
 * @throw std::invalid_argument If the insertion time is negative or greater than the
 * total duration of the current BinarySignal.
 */
  BinarySignal &BinarySignal::insertSignal(const BinarySignal &other, int time) {
    int total_time = this->totalTime();
    if (time < 0 || total_time < time) {
      throw std::invalid_argument("error: invalid insertion time");
    }
    if (total_time == time){
      *this += other;
      return *this;
    }
    int start_time = time;

    if (start_time == 0){
      BinarySignal result(other);
      result += *this;
      delete [] signal;
      *this = result;
      return *this;
    }

    BinarySignal before_interval;
    BinarySignal after_interval;

    int sum_time = 0;

    for (int i = 0; i < count; i++) {
      sum_time += signal[i].time;
      if (sum_time < time) {
        before_interval += signal[i];
      } 
      else if (sum_time == time) {
        before_interval += signal[i];
        before_interval += other;
      } 
      else if (sum_time - signal[i].time < time) {
        before_interval += SignalState(signal[i].level, start_time - sum_time + signal[i].time);
        before_interval += other;
        after_interval += SignalState(signal[i].level, sum_time - start_time);
      }
      if (sum_time - signal[i].time > time) {
        after_interval += signal[i];
      }
    }

    before_interval += after_interval;
    *this = before_interval;
    return *this;
  }

/**
 * @brief Removes a signal segment from the BinarySignal based on time and duration.
 *
 * This function removes a segment of the signal from the BinarySignal based on the
 * specified time and duration. If the time and duration parameters are valid, this
 * function modifies the current BinarySignal by removing the specified segment.
 *
 * @param time The starting time of the segment to be removed.
 * @param duration The duration of the segment to be removed.
 *
 * @return A reference to the modified current BinarySignal.
 *
 * @throw std::invalid_argument If the provided time is negative or if the sum of the
 * provided time and duration exceeds the total duration of the current BinarySignal.
 */
  BinarySignal &BinarySignal::removeSignal(int time, int duration) {
    int total_time = this->totalTime();
    if (time < 0 || total_time < time + duration) {
      throw std::invalid_argument("error: invalid time");
    }

    int start_time = time;
    int end_time = time + duration - 1;

    BinarySignal before_interval;
    BinarySignal after_interval;

    int sum_time = 0;
    for (int i = 0; i < count; i++) {
      sum_time += signal[i].time;
      if (sum_time <= start_time) {
        before_interval += signal[i];
      } 
      else if (sum_time - signal[i].time > end_time) {
          after_interval += signal[i];
      } 
      else {
        if (sum_time - signal[i].time < start_time) {
          before_interval += SignalState(signal[i].level, start_time - sum_time + signal[i].time);
        }
	if (1 < sum_time - end_time && sum_time - end_time <= signal[i].time) {
          after_interval += SignalState(signal[i].level, sum_time - end_time - 1);
        }
      }
    }

    before_interval += after_interval;
    *this = before_interval;

    return *this;
  }

/**
 * @brief Reads a BinarySignal from standard input based on the specified format.
 *
 * This function reads a BinarySignal from standard input according to the specified
 * input format. The supported formats are NUMBER_FORMAT and STRING_FORMAT.
 *
 * @param input_format The input format (NUMBER_FORMAT or STRING_FORMAT).
 *
 * @throw std::invalid_argument If an invalid input format is provided.
 * @throw std::runtime_error If the end of the input stream is reached (EOF).
 */
  void BinarySignal::input(int input_format){
    switch(input_format){
      case NUMBER_FORMAT:{
        bool lvl = getNum(0, 1);
        int time = getNum(1);
        *this = BinarySignal(lvl, time);
        break;
      }
      case STRING_FORMAT:{
        std::string signal_str;
        std::cin >> signal_str;
        *this = BinarySignal(signal_str);
        break;
      }
      default:{
       throw std::invalid_argument("error: invalid input format"); 
      }
    }
    if (std::cin.eof()){
      throw std::runtime_error("find eof");
    }
  }

/**
 * @brief Writes the BinarySignal to standard output as a formatted signal.
 *
 * This function writes the BinarySignal to standard output as a formatted signal.
 * Each segment of the signal is represented by a sequence of '0' or '1' characters.
 * The signal segments are printed consecutively.
 */
  void BinarySignal::output() const{
    for (int i = 0; i < count; i++) {
      char ch_lvl = signal[i].level ? '1' : '0';
      std::cout << std::string(signal[i].time, ch_lvl);
    }
    std::cout << std::endl;
  }

/**
 * @brief Overload for the output stream operator (<<) to print a BinarySignal to an output stream.
 *
 * This overload allows a BinarySignal to be printed to an output stream, such as std::cout, by
 * converting it into a string representation. The BinarySignal is converted into a string where
 * each segment of the signal is represented by a sequence of '0' or '1' characters.
 *
 * @param output The output stream where the BinarySignal will be printed.
 * @param state The BinarySignal to be printed.
 * @return The output stream after printing the BinarySignal.
 */
  std::ostream &operator <<(std::ostream &output, const BinarySignal &state){
    std::string signal_str;
    for (int i = 0; i < state.count; i++){
      signal_str += std::string(state.signal[i].getTime(), (state.signal[i].getLevel() ? '1' : '0'));
    }
    output << signal_str;
    return output;
  }

  /*std::istream &operator >>(std::istream& input, BinarySignal& state) {
    std::string signal_str;
    input >> signal_str;
    if (input.fail()) {
      input.setstate(std::ios_base::failbit);
    } 
    else if (input.eof()) {
      input.setstate(std::ios_base::eofbit);
    } 
    else if (input.bad()) {
      input.setstate(std::ios_base::badbit);
    } 
    else {
      try{
        state = BinarySignal(signal_str);
      }
      catch (...){
        input.setstate(std::ios_base::failbit);
      }
    }
    return input;
  }*/

/**
 * @brief Overload for the input stream operator (>>) to read a BinarySignal from an input stream.
 *
 * This overload allows a BinarySignal to be read from an input stream, such as std::cin. It reads a
 * string from the input stream and attempts to construct a BinarySignal from that string.
 *
 * @param input The input stream from which the BinarySignal will be read.
 * @param signal The BinarySignal that will be constructed from the input.
 * @return The input stream after reading the BinarySignal.
 */
  std::istream &operator >>(std::istream& input, BinarySignal& signal) {
    std::string signal_str;
    if (input >> signal_str) {
      try {
        signal = BinarySignal(signal_str);
      } 
      catch (...) {
        input.setstate(std::ios_base::failbit);
      }
    } 
    return input;
  }

  
}
 

// BinarySignal a = BinarySignal("0001010");
