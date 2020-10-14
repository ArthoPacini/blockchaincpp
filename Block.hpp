#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "Transaction.hpp"
#include "sha256.hpp"

class Block
{
    private:
    std::uint64_t index;
    std::string blockHash;
    std::string previousBlockHash;
    std::uint64_t nounce = 0;
    std::uint32_t maxTransactionsCount = 3;
    std::uint32_t difficulty;
    std::vector<Transaction> transactions;
    std::string transactionsHashesSum = "";

    public:
    Block(std::uint64_t index, std::string previousBlockHash, std::uint32_t difficulty, std::uint32_t maxTransactionsCount, Transaction transaction)
    {
        this->index = index;
        this->previousBlockHash = previousBlockHash;
        this->difficulty = difficulty;
        this->maxTransactionsCount = maxTransactionsCount;
        transactions.push_back(transaction);
    }

    constexpr friend std::ostream& operator<<(std::ostream& os, const Block& block)
    {
        return os << block.index;
    }

    bool push_transaction(Transaction transaction)
    {
        if(transactions.size() + 1 > maxTransactionsCount)
            return false;

        transactions.push_back(transaction);
        return true;
    }

    std::string calculateHash() 
    {
        transactionsHashesSum.clear();

        std::for_each(transactions.begin(), transactions.end(), [&](Transaction & transaction){
            transactionsHashesSum = sha256(transactionsHashesSum + transaction.getHash());
        });

        return sha256(transactionsHashesSum + previousBlockHash + std::to_string(index) + std::to_string(nounce));
    }

    void mine(std::uint32_t difficulty = 1)
    {
        std::cout << "Starting mining of block " << index << '\n';
        do
        {
            ++nounce;
            blockHash = calculateHash();
        } while (!std::all_of(blockHash.begin(), blockHash.begin() + difficulty, [](char & a){
                return a == '0';
            }));

        std::cout << "Block "<< index << " mined, nounce " << nounce << ", hash: " << blockHash << '\n'; 
    }

    bool validate() 
    {
        return this->blockHash == calculateHash();
    }

    std::string previousHash() const 
    {
        return previousBlockHash;
    }

    void dump() const
    {
        std::cout << "\n\n### Start of Block " << index << ":\n";
        std::cout << "Block Nounce: " << nounce << "\n";
        std::cout << "Difficulty: " << difficulty << "\n";
        std::cout << "Block Hash: " << blockHash << "\n";
        std::cout << "Previous Block Hash: " << previousBlockHash << "\n\n";
        std::for_each(transactions.begin(), transactions.end(), std::bind(std::mem_fn(&Transaction::dump),std::placeholders::_1));
        std::cout << "### End of Block " << index << "\n\n";
        return;
    }

};

#endif