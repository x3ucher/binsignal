
#define CATCH_CONFIG_MAIN // Просит Catch2 реализовать свой main, снимая эту задачу с разработчика

#include <sstream>
#include <catch2/catch.hpp>
#include "SignalState.h"
#include "BinarySignal.h"

TEST_CASE("SignalState Constructors") {
    SECTION("Default Constructor") {
        lab2::SignalState signal;
        REQUIRE(signal.getLevel() == 0);
        REQUIRE(signal.getTime() == 0);
    }

    SECTION("Parameterized Constructor") {
        lab2::SignalState signal(1, 5);
        REQUIRE(signal.getLevel() == 1);
        REQUIRE(signal.getTime() == 5);
    }

    SECTION("String Constructor") {
        lab2::SignalState signal("11110010");
        REQUIRE(signal.getLevel() == 1);
        REQUIRE(signal.getTime() == 4);
    }
    
    SECTION("Valid vector") {
        std::vector<int> signal_vec = {1, 1, 0, 0, 0, 1};
        lab2::SignalState state(signal_vec);

        REQUIRE(state.getLevel() == true);
        REQUIRE(state.getTime() == 2);
    }

    SECTION("Empty vector") {
        std::vector<int> signal_vec;
        REQUIRE_THROWS(lab2::SignalState(signal_vec));
    }

    SECTION("Test Exeption") {
        lab2::SignalState signal;
	REQUIRE_THROWS(lab2::SignalState(3, 5));
	REQUIRE_THROWS(lab2::SignalState(0, -6));
	REQUIRE_THROWS(lab2::SignalState("0002011"));
    }
}

TEST_CASE("SignalState copy constructor") {
    lab2::SignalState original(true, 3);
    lab2::SignalState copy(original);

    REQUIRE(copy.getLevel() == original.getLevel());
    REQUIRE(copy.getTime() == original.getTime());
}

TEST_CASE("SignalState copy assignment operator") {
    lab2::SignalState original(true, 3);
    lab2::SignalState copy;
    copy = original;

    REQUIRE(copy.getLevel() == original.getLevel());
    REQUIRE(copy.getTime() == original.getTime());
}

TEST_CASE("SignalState Methods") {
    lab2::SignalState signal(1, 5);

    SECTION("Invert Signal") {
        signal.invertSignal();
        REQUIRE(signal.getLevel() == false);
        REQUIRE(signal.getTime() == 5);
    }

    SECTION("Elongate Signal") {
        signal.elongateSignal(3);
        REQUIRE(signal.getLevel() == true);
        REQUIRE(signal.getTime() == 8);
    }

    SECTION("Truncate Signal") {
        signal.truncateSignal(2);
        REQUIRE(signal.getLevel() == true);
        REQUIRE(signal.getTime() == 3);
    }

    SECTION("Format Signal") {
        REQUIRE(signal.formatSignal() == "'''''");
    }

    SECTION("Test Exeption") {
	REQUIRE_THROWS_AS(signal.elongateSignal(-5), std::invalid_argument);
	REQUIRE_THROWS_AS(signal.truncateSignal(-5), std::invalid_argument);
	REQUIRE_THROWS_AS(signal.truncateSignal(8), std::invalid_argument);
    }
}

TEST_CASE("SignalState input operator >>") {
    lab2::SignalState signal;
    SECTION("Valid input"){
    	std::istringstream input("1 5");

    	input >> signal;
    		
	//REQUIRE(input.good());
	REQUIRE(signal.getLevel() == true);
    	REQUIRE(signal.getTime() == 5);
    }
    SECTION("Invalid input") {
        std::istringstream input("invalid_input");
        input >> signal;
        REQUIRE(input.fail());
    }

    SECTION("End of file input") {
        std::istringstream input;
        input >> signal;
        REQUIRE(input.eof());
    }

    SECTION("Input with bad state") {
        std::istringstream input("5 10");
        input.setstate(std::ios_base::failbit);
        input >> signal;
        REQUIRE(input.fail());
    }
}

TEST_CASE("SignalState output operator <<") {
    SECTION("Valid output") {
        lab2::SignalState signal(1, 5);
        std::ostringstream output;
        output << signal;
        REQUIRE(output.str() == "1 5");
    }
}

TEST_CASE("SignalState operator ~") {
    SECTION("Invert signal") {
        lab2::SignalState signal(1, 10);
        lab2::SignalState invertedSignal = ~signal;
        REQUIRE(invertedSignal.getLevel() == 0);
        REQUIRE(invertedSignal.getTime() == 10);
    }

    SECTION("Invert signal with zero level") {
        lab2::SignalState signal(0, 5);
        lab2::SignalState invertedSignal = ~signal;
        REQUIRE(invertedSignal.getLevel() == 1);
        REQUIRE(invertedSignal.getTime() == 5);
    }
}

TEST_CASE("BinarySignal constructor with level and time") {
    SECTION("Valid input") {
        REQUIRE_NOTHROW(lab2::BinarySignal(0, 5));
        REQUIRE_NOTHROW(lab2::BinarySignal(1, 3));
    }

    SECTION("Invalid input") {
        REQUIRE_THROWS_AS(lab2::BinarySignal(2, 5), std::invalid_argument);
        REQUIRE_THROWS_AS(lab2::BinarySignal(1, -1), std::invalid_argument);
    }
}

TEST_CASE("BinarySignal constructor with signal string") {
    SECTION("Valid input") {
        REQUIRE_NOTHROW(lab2::BinarySignal("010011"));
        REQUIRE_NOTHROW(lab2::BinarySignal("1"));
    }

    SECTION("Invalid input") {
        REQUIRE_THROWS_AS(lab2::BinarySignal("10201"), std::invalid_argument);
    }
}

TEST_CASE("BinarySignal multiplication operator *=") {
    SECTION("Valid input") {
        lab2::BinarySignal signal("0101");
	int len = signal.getCount();
        signal *= 3;
        REQUIRE(signal.getCount() == len * 3);
        REQUIRE(signal.toString() == "010101010101");
    }

    SECTION("Multiplying by 1 should not change the signal") {
        lab2::BinarySignal signal("1100");
        int len = signal.getCount();
	signal *= 1;
        REQUIRE(signal.getCount() == len * 1);
        REQUIRE(signal.toString() == "1100");
    }

    SECTION("Invalid input (negative factor)") {
        lab2::BinarySignal signal("1100");
        REQUIRE_THROWS_AS(signal *= -2, std::invalid_argument);
    }
}

TEST_CASE("BinarySignal multiplication operator *") {
    SECTION("Valid input") {
        lab2::BinarySignal signal("0011");
        lab2::BinarySignal result = signal * 2;
        REQUIRE(result.getCount() == signal.getCount() * 2);
	REQUIRE(result.toString() == "00110011");
    }

    SECTION("Multiplying by 1 should return a copy of the signal") {
        lab2::BinarySignal signal("111");
        lab2::BinarySignal result = signal * 1;
        REQUIRE(result.getCount() == signal.getCount() * 1);
        REQUIRE(result.toString() == "111");
    }

    SECTION("Invalid input (negative factor)") {
        lab2::BinarySignal signal("101");
        REQUIRE_THROWS_AS(signal * -3, std::invalid_argument);
    }
}

TEST_CASE("BinarySignal operator +=", "[BinarySignal]") {
    SECTION("BinarySignal") {
        lab2::BinarySignal signal1("0011");
        lab2::BinarySignal signal2("1100");
        signal1 += signal2;
        REQUIRE(signal1.toString() == "00111100");
    }
    SECTION("SignalState") {
        lab2::BinarySignal signal("1100");
        lab2::SignalState state(0, 2);
        signal += state;
        REQUIRE(signal.toString() == "110000");
    }
}

TEST_CASE("BinarySignal operator []", "[BinarySignal]") {
    lab2::BinarySignal signal("101010");
    REQUIRE(signal[0] == true);
    REQUIRE(signal[1] == false);
    REQUIRE(signal[2] == true);
    REQUIRE(signal[3] == false);
    REQUIRE(signal[4] == true);
    REQUIRE(signal[5] == false);
    REQUIRE_THROWS(signal[6]);
}

TEST_CASE("BinarySignal invertSignal", "[BinarySignal]") {
    lab2::BinarySignal signal("1100");
    signal.invertSignal();
    REQUIRE(signal.toString() == "0011");
}

TEST_CASE("BinarySignal operator ~", "[BinarySignal]") {
    lab2::BinarySignal signal("1010101");
    lab2::BinarySignal invertedSignal = ~signal;
    REQUIRE(invertedSignal.toString() == "0101010");
}

TEST_CASE("BinarySignal formatedSignal") {
  lab2::BinarySignal signal("10011100");
  REQUIRE(signal.formatedSignal() == "'\\../'''\\..");
}

TEST_CASE("BinarySignal insertSignal") {
  lab2::BinarySignal signal("000000");
  lab2::BinarySignal otherSignal("111");
  signal.insertSignal(otherSignal, 2);
  REQUIRE(signal.toString() == "001110000");
  REQUIRE_THROWS(signal.insertSignal(otherSignal, -1));
}

TEST_CASE("BinarySignal removeSignal", "[BinarySignal]") {
  lab2::BinarySignal signal("10011000");
  signal.removeSignal(2, 3);
  REQUIRE(signal.toString() == "10000");
  REQUIRE_THROWS(signal.removeSignal(3, 4));
}


TEST_CASE("BinarySignal input operator >>") {
    lab2::BinarySignal signal;
    SECTION("Valid input"){
        std::istringstream input("00011010");

        input >> signal;

        REQUIRE(signal.getCount() == 5);
        REQUIRE(signal.toString() == "00011010");
    }
}

TEST_CASE("BinarySignal output operator <<") {
    SECTION("Valid output") {
        lab2::BinarySignal signal("00011010");
        std::ostringstream output;
        output << signal;
        REQUIRE(output.str() == "00011010");
    }
}
