#include <iostream>

#include "SignalState.h"

namespace lab2{

  //SignalState::SignalState() : level(0), time(0) {}
/**
 * @brief Constructor for the SignalState class.
 *
 * This constructor creates a SignalState object with the given signal level and time.
 *
 * @param level The signal level (0 or 1).
 * @param time The time during which the signal level remains constant.
 * @throw std::invalid_argument if invalid values are provided.
 */
  SignalState::SignalState(int level, int time){
    if (!(level < 0 || level > 1) && time > 0){
      this->level = level;
      this->time = time;
    }
    else{
      throw std::invalid_argument("error: invalid signal state");
    }
  }

/**
 * @brief Constructor for the SignalState class based on a signal string.
 *
 * This constructor creates a SignalState object based on a string representation of a signal.
 *
 * @param signal The signal string consisting of '0' and '1' characters.
 * @throw std::invalid_argument if the string contains invalid characters.
 */
  SignalState::SignalState(const std::string &signal){
    if (signal.find_first_not_of("01") != std::string::npos){
      throw std::invalid_argument("error: invalid characters in string");
    }
    else{
      int i;
      for (i = 0; i < (int)signal.length(); i++) {
        if (signal[i] != signal[0]) {
          break;
        }
      }
      this->level = signal[0];
      this->time = i;
    }
  }

/**
 * @brief Constructor for the SignalState class based on a vector of integers.
 *
 * This constructor creates a SignalState object based on a vector of integers representing a signal.
 *
 * @param signal The vector of integers representing the signal (0 and 1).
 * @throw std::invalid_argument if the vector is empty or contains invalid values.
 */
  SignalState::SignalState(std::vector<int> &signal) {
    if (signal.empty()) {
      throw std::invalid_argument("error: empty signal vector");
    }
    int count = 0;
    this->level = (signal[0] != 0);
    for (int i = 0; i < (int)signal.size(); i++){
      if (signal[i] != this->level){
        break;
      }
      count++;
    }
    this->time = count;
  }

/**
 * @brief Copy constructor for the SignalState class.
 *
 * This constructor creates a copy of a SignalState object based on another SignalState object.
 *
 * @param other A reference to the SignalState object to be copied.
 */
  SignalState::SignalState(const SignalState &other){
    this->level = other.level;
    this->time = other.time;
  }

/**
 * @brief Assignment operator for the SignalState class.
 *
 * This operator assigns the level and time values from another SignalState object.
 *
 * @param other A reference to the SignalState object whose values will be copied.
 * @return A reference to the current SignalState object.
 */
  SignalState &SignalState::operator =(const SignalState &other){
    this->level = other.level;
    this->time = other.time;
    return *this;
  }
  

/**
 * @brief Get the signal level.
 *
 * @return true if the signal level is 1, false if the signal level is 0.
 */
  bool SignalState::getLevel() const{
    return level;
  }

/**
 * @brief Get the signal time.
 *
 * @return The time at which the signal level remains constant.
 */
  int SignalState::getTime() const{
    return time;
  }


/**
 * @brief Set the signal level.
 *
 * @param level The signal level (true or false).
 */
  void SignalState::setLevel(bool level){
    this->level = level;
  }

/**
 * @brief Set the signal time.
 *
 * @param time The signal time.
 * @throw std::invalid_argument if the time is less than or equal to 0.
 */
  void SignalState::setTime(int time){
    if (time >= 0){
      this->time = time;
    }
    else{
      throw std::invalid_argument("error: time must be positive");
    }
  }
  

/**
 * @brief Inverts the signal level.
 * 
 * This method changes the signal level to its logical NOT.
 */
  void SignalState::invertSignal(){
    level = !level;
  } 

/**
 * @brief Bitwise NOT operator for SignalState.
 * 
 * This operator inverts the signal level and returns a new SignalState object.
 *
 * @return A new SignalState object with the inverted signal level.
 */
  SignalState SignalState::operator ~(){
    SignalState result = *this;
    result.invertSignal();
    return result;
  }

/**
 * @brief Elongates the signal duration.
 * 
 * This method increases the signal duration by the specified amount.
 *
 * @param duration The duration to add to the signal.
 * @throw std::invalid_argument if an invalid duration is provided (non-positive).
 */
  void SignalState::elongateSignal(int duration){
    if (duration > 0){
      time += duration;
    }
    else{
      throw std::invalid_argument("error: invalid time value");
    }
  }

/**
 * @brief Truncates the signal duration.
 * 
 * This method decreases the signal duration by the specified amount.
 *
 * @param duration The duration to remove from the signal.
 * @throw std::invalid_argument if an invalid duration is provided (non-positive or greater than the current time).
 */
  void SignalState::truncateSignal(int duration){
    if (duration > 0 && duration <= time){
      time -= duration;
    }
    else{
      throw std::invalid_argument("error: invalid time value");
    }
  }

/**
 * @brief Formats the signal as a string.
 * 
 * This method returns a string representation of the signal, where '.' represents the low signal level
 * and ''' (single quote) represents the high signal level. 'x' is used for time durations of 0.
 *
 * @return A string representing the signal.
 */
  std::string SignalState::formatSignal() const{
    if (time == 0){ return "x"; }
    std::string chart;
    char symbol = (level ? '\'' : '.'); 
    for (int i = 0; i < time; i++){
      chart += symbol;
    }
    return chart;
  }

/**
 * @brief Constructs a SignalState with the provided level and time values.
 *
 * @param level The signal level (true for high, false for low).
 * @param time The duration of the signal.
 * @throw std::invalid_argument if the provided values are invalid.
 */

/**
 * @brief Outputs the SignalState to the given output stream.
 *
 * This global operator overloads the output stream operator (<<) for SignalState.
 * It formats the SignalState object as "level time" and writes it to the output stream.
 *
 * @param output The output stream to write to.
 * @param signal The SignalState object to be written.
 * @return The output stream after writing the SignalState.
 */
  std::ostream &operator <<(std::ostream &output, const SignalState &signal){
    char ch_lvl = signal.level ? '1' : '0';
    output << ch_lvl << " " << signal.time;
    return output;
  }

/**
 * @brief Overloaded input stream operator.
 * 
 * This operator reads the SignalState from the input stream.
 *
 * @param input The input stream.
 * @param signal The SignalState object to read from the input stream.
 * @return The input stream.
 */
  std::istream &operator >>(std::istream &input, SignalState &signal) {
    int lvl, time;
    if (input >> lvl >> time) {
      try {
        signal = SignalState(lvl, time);
      } 
      catch (...) {
        input.setstate(std::ios_base::failbit);
      }
    } 
    return input;
  }

}

