#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        unsigned long long w[5] = {0}; // Initialize w array to 0
        int groupIdx = 4; // Start filling w from the last group

        // Process the string in reverse, 6 characters at a time
        for (int i = k.size() - 1; i >= 0; i -= 6) {
            unsigned long long groupValue = 0;
            unsigned long long base36Multiplier = 1;

            // Process up to 6 characters in the current group
            for (int j = 0; j < 6 && i - j >= 0; ++j) {
                char c = k[i - j];

                // Convert to lowercase if it's an uppercase letter
                if (65 <= c && c <= 90) {
                    c += 32; // Convert to lowercase
                }

                // Compute the base-36 value for the current character
                groupValue += letterDigitToNumber(c) * base36Multiplier;
                base36Multiplier *= 36; // Update the multiplier for the next character
            }

            w[groupIdx] = groupValue; // Store the computed group value
            groupIdx--; // Move to the next group
            if (groupIdx < 0) break; // Stop if all groups are filled
        }

        // // Debug output for w values
        // for (int i = 0; i < 5; ++i) {
        //     std::cout << "w[" << i << "] = " << w[i] << std::endl;
        // }

        // Compute the final hash value using the rValues
        unsigned long long hashValue = 0;
        for (int i = 0; i < 5; ++i) {
            hashValue += rValues[i] * w[i];
        }

        return hashValue;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        HASH_INDEX_T conv = static_cast<size_t>(letter);

        if (48 <= conv && conv <= 57) conv -= 48 - 26 ; // for 0-9
        else if (97 <= conv && conv <= 122) conv -= 97; // for a-z

        return conv;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
