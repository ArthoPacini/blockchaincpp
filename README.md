# blockchaincpp
Simple Blockchain written in C++

just download it and run make
https://www.gitpod.io/#https://github.com/ArthoPacini/blockchaincpp

```cpp
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

    //automatically load blockchain from disk
    Blockchain alternative;
}

```
Output:
```console
Starting mining of block 0
Block 0 mined, nounce 1591, hash: 0007d778c421ef5b9e0787077e324007fb666cda31ec6fb41f9fd95fe83edfc9
Took 42 milliseconds to mine block
Starting mining of block 1
Block 1 mined, nounce 1741, hash: 00088682aea9f4b613aad17d49600a78359675667b125ff6f84a7f5ea81665cb
Took 45 milliseconds to mine block
Starting mining of block 2
Block 2 mined, nounce 6843, hash: 000c33bde216eb3b3df35b07eea2027b03280a0eca2282dd49bf705b9ad4ea5a
Took 177 milliseconds to mine block

{
    "blocks": [
        {
            "blockHash": "0007d778c421ef5b9e0787077e324007fb666cda31ec6fb41f9fd95fe83edfc9",
            "difficulty": 3,
            "index": 0,
            "maxTransactionCount": 2,
            "minedAt": "10/24/20 04:25:00",
            "nounce": 1591,
            "previousBlockHash": "81ddc8d248b2dccdd3fdd5e84f0cad62b08f2d10b57f9a831c13451e5c5c80a5",
            "transactions": [
                {
                    "amount": 0.0,
                    "receiverKey": "Genesis",
                    "senderKey": "Genesis",
                    "timestamp": "10/24/20 04:25:00"
                },
                {
                    "amount": 1.0,
                    "receiverKey": "Paul",
                    "senderKey": "Joe",
                    "timestamp": "10/24/20 04:25:00"
                }
            ]
        },
        {
            "blockHash": "00088682aea9f4b613aad17d49600a78359675667b125ff6f84a7f5ea81665cb",
            "difficulty": 3,
            "index": 1,
            "maxTransactionCount": 2,
            "minedAt": "10/24/20 04:25:00",
            "nounce": 1741,
            "previousBlockHash": "0007d778c421ef5b9e0787077e324007fb666cda31ec6fb41f9fd95fe83edfc9",
            "transactions": [
                {
                    "amount": 3.14,
                    "receiverKey": "Ruth",
                    "senderKey": "Paul",
                    "timestamp": "10/24/20 04:25:00"
                },
                {
                    "amount": 1.32,
                    "receiverKey": "Ruth",
                    "senderKey": "Joe",
                    "timestamp": "10/24/20 04:25:00"
                }
            ]
        },
        {
            "blockHash": "000c33bde216eb3b3df35b07eea2027b03280a0eca2282dd49bf705b9ad4ea5a",
            "difficulty": 3,
            "index": 2,
            "maxTransactionCount": 2,
            "minedAt": "10/24/20 04:25:00",
            "nounce": 6843,
            "previousBlockHash": "00088682aea9f4b613aad17d49600a78359675667b125ff6f84a7f5ea81665cb",
            "transactions": [
                {
                    "amount": 5.52,
                    "receiverKey": "Joe",
                    "senderKey": "Steve",
                    "timestamp": "10/24/20 04:25:00"
                },
                {
                    "amount": 6.7,
                    "receiverKey": "Steve",
                    "senderKey": "Ruth",
                    "timestamp": "10/24/20 04:25:00"
                }
            ]
        },
        {
            "blockHash": "",
            "difficulty": 3,
            "index": 3,
            "maxTransactionCount": 2,
            "minedAt": "not mined yet",
            "nounce": 0,
            "previousBlockHash": "000c33bde216eb3b3df35b07eea2027b03280a0eca2282dd49bf705b9ad4ea5a",
            "transactions": [
                {
                    "amount": 7.62,
                    "receiverKey": "Steve",
                    "senderKey": "Joe",
                    "timestamp": "10/24/20 04:25:00"
                }
            ]
        }
    ],
    "difficulty": 3,
    "maxTransactionsCount": 2
}
Is blockchain valid? 1
```
