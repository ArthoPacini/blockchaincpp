#include <iostream>

#include "Blockchain.hpp"

int main()
{
    //Create blockchain with difficulty level 3 and 2 transactions per block max
    Blockchain blockchain(3, 2); 

    //add transactions
    //the block is mined when the transaction count reaches maximum value
    blockchain.push_transaction({1.0, "Joe", "Paul"});
    blockchain.push_transaction({3.14, "Paul", "Ruth"});
    blockchain.push_transaction({1.32, "Joe", "Ruth"});
    blockchain.push_transaction({5.52, "Steve", "Joe"});
    blockchain.push_transaction({6.7, "Ruth", "Steve"});
    blockchain.push_transaction({7.62, "Joe", "Steve"});

    //prints blockchain
    std::cout << blockchain.dump() << std::endl;

    //saves blockchain do disk
    blockchain.save();

    //validates the chain
    std::cout << "Is blockchain valid? " << blockchain.validate() << std::endl;

    //load blockchain from disk
    Blockchain alternative;
}
