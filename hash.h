#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

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
        // Add your code here
        unsigned long long blocks[5] = {0,0,0,0,0};
        const size_t numBlocks = 5;
        const size_t blockSpan = 6;
        const size_t maxNumChars = numBlocks * blockSpan; // = 30
        const size_t kPadding = maxNumChars - k.size();
        const size_t firstBlock = (maxNumChars - k.size()) / blockSpan;
        const size_t firstInBlock = (maxNumChars - k.size()) % blockSpan;
        
        for (size_t indexOfBlock = firstBlock; indexOfBlock < numBlocks; indexOfBlock ++) {
            size_t startIndex = 0;
            if (indexOfBlock == firstBlock) startIndex = firstInBlock;
            unsigned long long blockValue = 0;
            for (size_t indexInBlock = startIndex; indexInBlock < blockSpan; indexInBlock ++) {
                const size_t indexOfLetter = blockSpan * indexOfBlock + indexInBlock - kPadding;
                blockValue += letterDigitToNumber(k[indexOfLetter]);
                // Increasing the exponent of this and all previous digits, except for the last digit.
                if (indexInBlock < blockSpan - 1) blockValue *= 36;
            }
            blocks[indexOfBlock] = blockValue;
        }
        unsigned long long sum = 0;
        for (size_t i = 0; i < numBlocks; i ++) {
            sum += blocks[i] * rValues[i];
            // std::cout << "blocks[" << i << "]=" << blocks[i] << '\n';
            // std::cout << "rValues[" << i << "]=" << rValues[i] << '\n';
            // std::cout << "(blocks * rValues)[" << i << "]=" << blocks[i] * rValues[i] << '\n';
        }

        return sum;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if (letter >= 97) return letter - 97;
        if (letter >= 65) return letter - 65;
        return letter - 22;
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
