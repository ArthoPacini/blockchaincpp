# blockchaincpp
Simple Blockchain written in C++

just download it and run make

```cpp
int main()
{
    //Create blockchain with difficulty level 3 and 2 transactions per block max
    Blockchain blockchain(3, 2); 

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
Block 0 mined, nounce 36992, hash: 0000ac755ef4f0e42302b1054f3fca1eb0d9ddb5534306ee355b48586a977fe8
Took 481 milliseconds to mine block
Starting mining of block 1
Block 1 mined, nounce 35306, hash: 000057668ad7967dead93cccb412a8870f83c057b6a525a21f08f2bfd6cc37b9
Took 460 milliseconds to mine block
Starting mining of block 2
Block 2 mined, nounce 3028, hash: 0000c3b68c6a1bc17a1c305521177e4734fa4d119e78c9dc6bf18497ee9e15c3
Took 39 milliseconds to mine block


### Start of Block 0:
Block Nounce: 36992
Difficulty: 3
Block Hash: 0000ac755ef4f0e42302b1054f3fca1eb0d9ddb5534306ee355b48586a977fe8
Previous Block Hash: 81ddc8d248b2dccdd3fdd5e84f0cad62b08f2d10b57f9a831c13451e5c5c80a5


### Transaction:
        Sender: Genesis
        Receiver: Genesis
        Amount: 0
        Timestamp: 1602662085108602970
        Transaction Hash: fe8859d61fd0f4b7e702c8248da2380f88f6f0585d1fe9358063049fdf0b3a93

### Transaction:
        Sender: Joe
        Receiver: Paul
        Amount: 1
        Timestamp: 1602662085108607068
        Transaction Hash: 4f3b0687c222c31be6959e0978f1a17c1ea56b7e1291db62021d545693fc6aaa
### End of Block 0



### Start of Block 1:
Block Nounce: 35306
Difficulty: 4
Block Hash: 000057668ad7967dead93cccb412a8870f83c057b6a525a21f08f2bfd6cc37b9
Previous Block Hash: 0000ac755ef4f0e42302b1054f3fca1eb0d9ddb5534306ee355b48586a977fe8


### Transaction:
        Sender: Paul
        Receiver: Ruth
        Amount: 1.3
        Timestamp: 1602662085108607519
        Transaction Hash: a33dc61704eee6d87a66417e603d4665cc098f5bfecc3ca7eef48dc1ad9001ec

### Transaction:
        Sender: Joe
        Receiver: Ruth
        Amount: 1.3
        Timestamp: 1602662085590044362
        Transaction Hash: 90087c509b32140803d553eb7a73c3ca22c6a3c1bd98a6d8028117dd72745355
### End of Block 1



### Start of Block 2:
Block Nounce: 3028
Difficulty: 4
Block Hash: 0000c3b68c6a1bc17a1c305521177e4734fa4d119e78c9dc6bf18497ee9e15c3
Previous Block Hash: 000057668ad7967dead93cccb412a8870f83c057b6a525a21f08f2bfd6cc37b9


### Transaction:
        Sender: Steve
        Receiver: Joe
        Amount: 1.3
        Timestamp: 1602662085590044763
        Transaction Hash: d00ee11fdc6d667882f9e6ec051250f7e973ee80ca72938d809b9d1a27246032

### Transaction:
        Sender: Ruth
        Receiver: Steve
        Amount: 1.3
        Timestamp: 1602662086050870946
        Transaction Hash: 2ed9711c8da7026cb07b9830428df68af30a118f57cd7c2e1640f57f4d0c2602
### End of Block 2



### Start of Block 3:
Block Nounce: 0
Difficulty: 4
Block Hash: 
Previous Block Hash: 0000c3b68c6a1bc17a1c305521177e4734fa4d119e78c9dc6bf18497ee9e15c3


### Transaction:
        Sender: Joe
        Receiver: Steve
        Amount: 1.3
        Timestamp: 1602662086050871217
        Transaction Hash: 3318957ab6c951911530e9b7ec085acf4032b3466185bfe2f722a38d45a64223
### End of Block 3

Is blockchain valid? 1
```
