#ifndef BLOCKCHAIN_HPP_
#define BLOCKCHAIN_HPP_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


#include "Transaction.hpp"
#include "Block.hpp"
#include "nlohmann/json.hpp"
#include "sha256.hpp"




class Blockchain
{
    private:
    std::uint32_t difficulty = 3;
    std::uint32_t maxTransactionsCount = 2;
    std::vector<Block> chain; //Here goes all the mined blocks
    Block currentBlock; //This is the block accepting incoming transactions, not mined and not pushed to the main blockchain yet

    Block createGenesis()
    {
        return Block(0, sha256("Genesis"), difficulty, maxTransactionsCount,{0.0, "Genesis", "Genesis"});
    }

    public:

    Blockchain(std::uint32_t difficulty, std::uint32_t maxTransactionsCount)
    {
        this->difficulty = difficulty;
        this->maxTransactionsCount = maxTransactionsCount;
        currentBlock = createGenesis();
    }

    Blockchain(const std::string filename = "blockchain.json")
    {
        load(filename);
    }

    void push_transaction(Transaction transaction)
    {
        if(currentBlock.push_transaction(transaction))
            return;

        chain.push_back(currentBlock);
        currentBlock = Block({chain.size(), chain.back().calculateHash(), difficulty, maxTransactionsCount, transaction});

        chain.back().mine(difficulty);
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

    std::string dump(std::int16_t indent = 4) const
    {
        nlohmann::json output_json = {{"difficulty", difficulty}, {"maxTransactionsCount",maxTransactionsCount}, {"blocks", nlohmann::json::array({})}};
        std::for_each(chain.begin(), chain.end(), std::bind(std::mem_fn(&Block::dump), std::placeholders::_1, std::ref(output_json)));
        return output_json.dump(indent);
    }

    void save(const std::string output_filename = "blockchain.json", const std::int16_t indent = 4)
    {
        std::filesystem::path output_path = std::filesystem::path(std::filesystem::current_path() / "blockchain" / output_filename);

        if(!std::filesystem::exists(output_path.parent_path()))
            std::filesystem::create_directories(output_path.parent_path());

        std::ofstream output_file(output_path);
        output_file << dump();
    }

    void save(const std::int16_t indent, const std::string output_filename = "blockchain.json")
    {
        save(output_filename, indent);
    }

    void load(const std::string input_filename = "blockchain.json")
    {
        chain.clear();

        std::filesystem::path input_path = std::filesystem::path(std::filesystem::current_path() / "blockchain" / input_filename);

        if(!std::filesystem::exists(input_path))
        {
            std::cerr << "Blockchain file does not exists!" << std::endl;
            return;
        }

        nlohmann::json input_json;
        
        std::ifstream input_file(input_path);
        input_file >> input_json;

        this->difficulty = input_json["difficulty"];
        this->maxTransactionsCount = input_json["maxTransactionsCount"];

        for(const auto & block_json : input_json["blocks"])
            chain.push_back(Block(block_json));
    }
};

#endif
