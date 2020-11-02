#include <iostream>
#include "Blockchain.hpp"
#include "random_string_and_number_utility.hpp"

void makeRandomTransactionsPeriodically(std::int64_t secondsForTransactionPushMin = 2, std::int64_t secondsForTransactionPushMax = 4)
{
    
    std::string randomBlockchainFileName;// = getRandomString(10);
    std::cout << "Enter blockchain filename: ";
    std::getline(std::cin, randomBlockchainFileName);
    Blockchain blockchain(randomBlockchainFileName); 

    //std::cout << "Blockchain file name: " << randomBlockchainFileName << std::endl;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(getRandomInt(secondsForTransactionPushMin, secondsForTransactionPushMax)));
        blockchain.push_transaction({getRandomDouble(1.0,5.0), getRandomString(5), getRandomString(5)});
        blockchain.save(randomBlockchainFileName);
        std::cout << "Blockchain saved with filename: " << randomBlockchainFileName << std::endl;
    }
}

int main()
{
    //Blockchain blockchain(3, 2); 
    //Create blockchain with difficulty level 3 and 2 transactions per block max
    
    makeRandomTransactionsPeriodically(2, 5);
    return 10;
    //add transactions
    //the block is mined when the transaction count reaches maximum value
    /*blockchain.push_transaction({1.0, "Joe", "Paul"});
    blockchain.push_transaction({3.14, "Paul", "Ruth"});
    blockchain.push_transaction({1.32, "Joe", "Ruth"});
    blockchain.push_transaction({5.52, "Steve", "Joe"});
    blockchain.push_transaction({6.7, "Ruth", "Steve"});
    blockchain.push_transaction({7.62, "Joe", "Steve"});*/
/*
    //prints blockchain
    std::cout << blockchain.dump() << std::endl;

    //saves blockchain do disk
    blockchain.save();

    //validates the chain
    std::cout << "Is blockchain valid? " << blockchain.validate() << std::endl;

    //automatically load blockchain from disk
    std::cout << "Sleeping for 1000 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1000));
    //main();
    //Blockchain alternative;*/
}
