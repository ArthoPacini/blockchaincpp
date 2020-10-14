#ifndef BLOCKCHAIN_HPP_
#define BLOCKCHAIN_HPP_


#include "Transaction.hpp"
#include "Block.hpp"

class Blockchain
{
    private:
    std::uint32_t difficulty = 3;
    std::uint32_t maxTransactionsCount = 2;
    std::vector<Block> chain = {createGenesis()};
    Block createGenesis()
    {
        return Block({0, sha256("Genesis"), difficulty, maxTransactionsCount,{0.0, "Genesis", "Genesis"}});
    }
    public:

    Blockchain(std::uint32_t difficulty, std::uint32_t maxTransactionsCount)
    {
        this->difficulty = difficulty;
        this->maxTransactionsCount = maxTransactionsCount;
    }

    Blockchain()
    {

    }

    void push_transaction(Transaction transaction)
    {
        if(chain.back().push_transaction(transaction))    
            return;

        chain.back().mine(difficulty);
        chain.push_back({chain.size(),chain.back().calculateHash(),difficulty, maxTransactionsCount,transaction});
    }

    bool validate() 
    {
        std::vector<Block>::iterator it;
        for(it = chain.begin(); it != chain.end(); ++it)
        {
            Block & currentBlock = *it;
            if(it == (chain.end() - 1))
                continue;

            if(!currentBlock.validate())
                {
                    std::cout << "Block " << currentBlock << " is invalid\n";
                    return false;
                }

            if(chain.size() > 1 && it != chain.begin())
            {
                Block previousBlock = *(it-1);
                if(currentBlock.previousHash() != previousBlock.calculateHash())
                {
                    std::cout << "Block " << currentBlock << " is invalid\n";
                    return false;
                }
            }
            
        }

        return true;
    }

    void dump() const
    {
        std::for_each(chain.begin(), chain.end(), std::bind(std::mem_fn(&Block::dump),std::placeholders::_1));
    }

};

#endif