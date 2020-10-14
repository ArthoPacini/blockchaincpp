# blockchaincpp
Simple Blockchain written in C++

```cpp
int main()
{
    //Create blockchain with 3 level difficulty, max 2 transactions per block
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
