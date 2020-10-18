# blockchaincpp
Simple Blockchain written in C++

just download it and run make

```cpp
int main()
{
    //Create blockchain with difficulty level 3 and 2 transactions per block max
    Blockchain blockchain(4, 2); 

    //add transactions
    //the block is mined when the transaction count reaches maximum value
    blockchain.push_transaction({1.0, "Joe", "Paul"});
    blockchain.push_transaction({1.3, "Paul", "Ruth"});
    blockchain.push_transaction({1.3, "Joe", "Ruth"});
    blockchain.push_transaction({1.3, "Steve", "Joe"});
    blockchain.push_transaction({1.3, "Ruth", "Steve"});
    blockchain.push_transaction({1.3, "Joe", "Steve"});

    //dump the chain
    blockchain.dump();
    
    //validates the chain
    std::cout << "Is blockchain valid? " << blockchain.validate() << std::endl;
}
```
Possible output:
```console
Starting mining of block 0
Block 0 mined, nounce 18788, hash: 000032f4b367df5c5a5b3285577f81f441e56377f9c054b19b6bb9af4f9f082e
Took 237 milliseconds to mine block
Starting mining of block 1
Block 1 mined, nounce 132887, hash: 0000c76972b7fac7d1f4087f9612e7984e05fd1466cf5263768839839c3a725b
Took 1668 milliseconds to mine block
Starting mining of block 2
Block 2 mined, nounce 196001, hash: 0000190ed0e1a3df898cfec39a319a3dc93ab21fde5ff75c743663bafd6d8c50
Took 2490 milliseconds to mine block


### Start of Block 0:
Block Nounce: 18788
Difficulty: 4
Block Hash: 000032f4b367df5c5a5b3285577f81f441e56377f9c054b19b6bb9af4f9f082e
Previous Block Hash: 81ddc8d248b2dccdd3fdd5e84f0cad62b08f2d10b57f9a831c13451e5c5c80a5


### Transaction:
        Sender: Genesis
        Receiver: Genesis
        Amount: 0
        Timestamp: 1602662401718233891
        Transaction Hash: fe843a0e8ae1b78473323b72a6f186b0927e8705409569cfbd4ff292d15f6de9

### Transaction:
        Sender: Joe
        Receiver: Paul
        Amount: 1
        Timestamp: 1602662401718238129
        Transaction Hash: 6e19ffab9c8925508484d7b5e95bae0418c6ed9b0314a70f68082ec0bf81c5c7
### End of Block 0



### Start of Block 1:
Block Nounce: 132887
Difficulty: 4
Block Hash: 0000c76972b7fac7d1f4087f9612e7984e05fd1466cf5263768839839c3a725b
Previous Block Hash: 000032f4b367df5c5a5b3285577f81f441e56377f9c054b19b6bb9af4f9f082e


### Transaction:
        Sender: Paul
        Receiver: Ruth
        Amount: 1.3
        Timestamp: 1602662401718238619
        Transaction Hash: 83de875b0cbda2adfb7fbcfe5216a48f3deda2eeeff7217bd0f9199f486fe05b

### Transaction:
        Sender: Joe
        Receiver: Ruth
        Amount: 1.3
        Timestamp: 1602662401956275661
        Transaction Hash: bb275cc64de37eab74c6bb06ffc70e69451918f7028b4b51f7acca18e153e7c6
### End of Block 1



### Start of Block 2:
Block Nounce: 196001
Difficulty: 4
Block Hash: 0000190ed0e1a3df898cfec39a319a3dc93ab21fde5ff75c743663bafd6d8c50
Previous Block Hash: 0000c76972b7fac7d1f4087f9612e7984e05fd1466cf5263768839839c3a725b


### Transaction:
        Sender: Steve
        Receiver: Joe
        Amount: 1.3
        Timestamp: 1602662401956276072
        Transaction Hash: 377109c6411ec8b8f6a1087ed8f791b2213f1f3a439f45426b2afb508fce7777

### Transaction:
        Sender: Ruth
        Receiver: Steve
        Amount: 1.3
        Timestamp: 1602662403624894848
        Transaction Hash: 86f01dc272b9092d6f8b1d1a24b193815a38df93d3644191b34de448e30173cc
### End of Block 2



### Start of Block 3:
Block Nounce: 0
Difficulty: 4
Block Hash: 
Previous Block Hash: 0000190ed0e1a3df898cfec39a319a3dc93ab21fde5ff75c743663bafd6d8c50


### Transaction:
        Sender: Joe
        Receiver: Steve
        Amount: 1.3
        Timestamp: 1602662403624895259
        Transaction Hash: 9f588a915d491b9c5337f3064dc8367f5485fcc3827e8a393a15234956b96f70
### End of Block 3

Is blockchain valid? 1
```
