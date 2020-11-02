#ifndef BLOCKCHAIN_HPP_
#define BLOCKCHAIN_HPP_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <set>

#include "Transaction.hpp"
#include "Block.hpp"
#include "nlohmann/json.hpp"
#include "sha256.hpp"

#include "httplib.hpp"


class Blockchain
{
    private:
    std::uint32_t difficulty = 3;
    std::uint32_t maxTransactionsCount = 2;
    std::vector<Block> chain; //Here goes all the mined blocks
    Block currentBlock; //This is the block accepting incoming transactions, not mined and not pushed to the main blockchain yet

    //Networking node
    bool nodeNetworkStarted = false; //networking node
    std::thread nodeNetworkingThread;
    std::set<std::string> nodeIPs = {"127.0.0.1:6666","127.0.0.1:6667"};
    

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
        //DEBUG, SET NODE PORT
        std::cout << "[NODE CONFIGURATION]: enter port: ";
        std::uint16_t port;
        std::cin >> port;
        std::thread(&Blockchain::start_node, this, port).detach(); //start node
    }

    Blockchain(const std::string filename = "blockchain.json")
    {
        
        load(filename);
        synchronize();
        //currentBlock = createGenesis();
        std::cout << "[NODE CONFIGURATION]: enter port: ";
        std::uint16_t port;
        std::cin >> port;
        std::thread(&Blockchain::start_node, this, port).detach(); //start node
    }

    void push_transaction(Transaction transaction)
    {
        if(currentBlock.push_transaction(transaction))
            return;
        
        chain.push_back(currentBlock);
        chain.back().mine(difficulty);
        std::thread(&Blockchain::broadcast_last_block, this, chain.back()).detach(); //Broadcast last mined block
        currentBlock = Block({chain.size(), chain.back().calculateHash(), difficulty, maxTransactionsCount, transaction});
    }

    //Validates the entire Blockchain by recalculating all hashes and comparing with stored hashes into blocks
    bool validate(const bool reportInvalidBlock = true) 
    {
        std::vector<Block>::iterator it;
        for(it = chain.begin(); it != chain.end(); ++it)
        {
            Block & currentBlock = *it;
            if(it == (chain.end() - 1))
                continue;

            if(!currentBlock.validate())
                {
                    if(reportInvalidBlock)
                        std::cout << "Block " << currentBlock << " is invalid\n";

                    return false;
                }

            if(chain.size() > 1 && it != chain.begin())
            {
                Block previousBlock = *(it-1);
                if(currentBlock.previousHash() != previousBlock.calculateHash())
                {
                    if(reportInvalidBlock)
                        std::cout << "Block " << currentBlock << " is invalid\n";

                    return false;
                }
            }
            
        }

        return true;
    }

    //Dumps the entire Blockchain to a json format
    std::string dump(std::int16_t indent = 4) const
    {
        nlohmann::json output_json = {{"difficulty", difficulty}, {"maxTransactionsCount",maxTransactionsCount}, {"blocks", nlohmann::json::array({})}};
        std::for_each(chain.begin(), chain.end(), std::bind(std::mem_fn<void(nlohmann::json &) const>(&Block::dump), std::placeholders::_1, std::ref(output_json["blocks"])));
        return output_json.dump(indent);
    }

    //Saves the dump of the entire Blockchain to a json file on disk
    void save(const std::string output_filename = "blockchain.json", const std::int16_t indent = 4)
    {
        std::filesystem::path output_path = std::filesystem::path(std::filesystem::current_path() / "blockchain" / output_filename);

        if(!std::filesystem::exists(output_path.parent_path()))
            std::filesystem::create_directories(output_path.parent_path());

        std::ofstream output_file(output_path);
        output_file << dump();
    }

    //Saves the dump of the entire Blockchain to a json file on disk
    void save(const std::int16_t indent, const std::string output_filename = "blockchain.json")
    {
        save(output_filename, indent);
    }

    //Loads the entire Blockchain from a json object
    void load(nlohmann::json & input_json)
    {
        this->difficulty = input_json["difficulty"];
        this->maxTransactionsCount = input_json["maxTransactionsCount"];

        for(const auto & block_json : input_json["blocks"])
            chain.push_back(Block(block_json));

        currentBlock.updatePreviousBlockHash(chain.back());
    }

    //Loads the entire Blockchain from a json file on disk
    void load(const std::string input_filename = "blockchain.json")
    {
        chain.clear();

        std::filesystem::path input_path = std::filesystem::path(std::filesystem::current_path() / "blockchain" / input_filename);
        std::cout << "VENDO PATH: " << input_path << std::endl;
        if(!std::filesystem::exists(input_path))
        {
            std::cerr << "Blockchain file does not exists!" << std::endl;
            return;
        }

        nlohmann::json input_json;
        
        std::ifstream input_file(input_path);
        input_file >> input_json;

        load(input_json);
    }

    //Returns a block in json by it's index
    std::string getBlockJson(std::uint64_t index) 
    {
        nlohmann::json blockOutput;

        for(const auto & block : chain)
            if(block.getIndex() == index)
            {
                //find block
                nlohmann::json blockOutputBuffer;
                block.dump(blockOutputBuffer);
                blockOutput = blockOutputBuffer[0];
                blockOutput["found"] = "true";
                return blockOutput.dump();
            }

        blockOutput["found"] = "false";
        return blockOutput.dump();
    }
    
    //Networking code :NODE-SERVER: 
    //Traps a thread to work as a Server for incoming connections
    void start_node(std::uint16_t port = 8080)
    {
        /*if(!nodeNetworkStarted)
        {
            nodeNetworkStarted = true;
            nodeNetworkingThread = std::thread(&Blockchain::start_node, this);
            nodeNetworkingThread.detach();
            return;
        }*/
        
        httplib::Server server;

        //Get block \d+
        server.Get(R"(/block/(\d+))", [&](const httplib::Request & request, httplib::Response &response) 
        {
            std::string blockIndexString = request.matches[1];
            /*if(!std::all_of(blockIndexString.begin(), blockIndexString.end(), ::isdigit))
            {
                response.set_content("\"result\":\"false\",\"message\":\"not and number\"", "application/json");
                return;
            }*/

            std::uint64_t blockIndex = std::stoi(blockIndexString);
            const std::string result = getBlockJson(blockIndex); 
            response.set_content(result.c_str(), "application/json");
        });

        //Get entire blockchain
        server.Get("/blockchain", [&](const httplib::Request &, httplib::Response &response) 
        {
            const std::string result = this->dump(-1);
            response.set_content(result.c_str(), "application/json");
        });

        //Get blockchain height
        server.Get("/height", [&](const httplib::Request &, httplib::Response &response) 
        {
            const std::string result = "{\"height\":" + std::to_string(chain.size()) + "}"; //const std::string result = nlohmann::json({{"height", chain.size()}}).dump();
            response.set_content(result.c_str(), "application/json");
        });

        //Receive a block
        server.Post("/block", [&](const httplib::Request & request, httplib::Response &response)
        {
            std::cout << "[NODE-SERVER]: Receiving block from " << request.headers.find("REMOTE_ADDR")->second << ':' << request.headers.find("REMOTE_PORT")->second << std::endl;
            const std::string incomingMessage = request.body;
            nlohmann::json incomingBlockJson = nlohmann::json::parse(incomingMessage, nullptr, false);

            if (incomingBlockJson.is_discarded())
            {
                std::cerr << "[NODE-SERVER]: The incoming json could not be parsed" << std::endl;
                response.set_content("{\"result\":\"false\"}", "application/json");
                return;
            }

            Block incomingBlock = Block(incomingBlockJson);
            
            if(incomingBlock.getIndex() == (chain.back().getIndex() + 1) && incomingBlock.previousHash() == chain.back().calculateHash())
            {
                std::cout << "[NODE-SERVER]: The incoming block of index " << incomingBlock << " is eligible to be inserted into the chain, inserting it..." << std::endl;
                chain.push_back(incomingBlock);

                //Update the previousBlockHash of the currentBlock (buffer)
                currentBlock.updatePreviousBlockHash(chain.back());
                response.set_content("{\"result\":\"true\"}", "application/json");
                return;
            }

            std::cout << "[NODE-SERVER]: The incoming block of index " << incomingBlock << " is not eligible to be inserted into the chain..." << std::endl;
            response.set_content("{\"result\":\"false\"}", "application/json");
        });

        std::cout << "[NODE-SERVER]: Running node on localhost at port " << port << std::endl;
        server.listen("0.0.0.0", port);
    }

    //Networking code :NODE-CLIENT:
    //Broadcast the last mined block to all connected nodes
    void broadcast_last_block(Block lastMinedBlock)
    {
        //ok so never ever use a reference for 'lastMinedBlock', always use a copy, i've just wasted 1+ hour debugging this as 'Block &', thanks god i was listening to good house music so i could get though the pain
        //if you pass the block to this function as a reference, the reference can be lost if there is another block being mined and pushed to the vector, a memory allocation is occurred.
        std::string lastMinedBlockMessage = lastMinedBlock.dump();
        std::cout << "[NODE-CLIENT]: Starting broadcasting the last mined block" << std::endl;
        //std::ofstream broadcast_file("broadcast_file.txt", std::ios::out | std::ios::app);
        //broadcast_file << lastMinedBlockMessage << '\n';

        for(const auto & ip : nodeIPs)
        {
            std::cout << "[NODE-CLIENT]: Reaching node at " << ip << '\n';  
            httplib::Client cli(ip.c_str());

            if(auto res = cli.Post("/block", lastMinedBlockMessage.c_str(), "application/json"))
            {
                if(res->status >= 200 && res->status <= 299)
                    std::cout << "[NODE-CLIENT]: Successfully sent last mined block to " << ip << std::endl;
                else
                    std::cout << "[NODE-CLIENT]: Could not send the last mined block to " << ip << ", HTTP status code from response: " << res->status << std::endl;
            }
            else
                std::cout << "[NODE-CLIENT]: Could not reach node at " << ip << std::endl;
        }
    }

    void get_blockchain_from_specific_node(const std::string & node)
    {
        httplib::Client cli(node.c_str());

        if(auto res = cli.Get("/blockchain"))
        {
            if(res->status >= 200 && res->status <= 299)
            {
                std::cout << "[NODE-CLIENT]: Successfully reached node " << node << std::endl;
                const std::string incomingMessage = res->body;
                nlohmann::json incomingBlockchainMessageJson = nlohmann::json::parse(incomingMessage, nullptr, false);

                if (incomingBlockchainMessageJson.is_discarded())
                {
                    std::cerr << "[NODE-CLIENT]: The incoming blockchain json could not be parsed" << std::endl;
                    return;
                }

                std::cout << "[NODE-CLIENT]: Successfully downloaded blockchain from node " << node << ", loading it" << std::endl;
                load(incomingBlockchainMessageJson);
                std::cout << "[NODE-CLIENT]: Blockchain loaded, is it valid? response: " << this->validate() << std::endl;
                return;
            }
            else
                    std::cout << "[NODE-CLIENT]: Could not get blockchain from " << node << ", HTTP status code from response: " << res->status << std::endl;
        }
        else
            std::cout << "[NODE-CLIENT]: Could not reach node at " << node << std::endl;
    }

    //Get the entire blockchain from nodes
    void synchronize_entire_blockchain()
    {
        std::cout << "[NODE-CLIENT]: Starting synchronizing the entire blockchain with the network..." << std::endl;
        const std::size_t blockchainHeight = chain.size();
        for(const auto & node : nodeIPs)
        {
            std::cout << "[NODE-CLIENT]: Reaching node at " << node << '\n';  
            httplib::Client cli(node.c_str());

            if(auto res = cli.Get("/height"))
            {
                if(res->status >= 200 && res->status <= 299)
                {   
                    std::cout << "[NODE-CLIENT]: Successfully reached node " << node << std::endl;
                    const std::string incomingMessage = res->body;
                    nlohmann::json incomingHeightMessageJson = nlohmann::json::parse(incomingMessage, nullptr, false);

                    if (incomingHeightMessageJson.is_discarded())
                    {
                        std::cerr << "[NODE-CLIENT]: The incoming height json could not be parsed, skipping node" << std::endl;
                        continue;
                    }
                    std::size_t incomingBlockchainHeight = incomingHeightMessageJson["height"];
                    if(blockchainHeight == incomingBlockchainHeight)
                    {
                        std::cout << "[NODE-CLIENT]: The blockchain height is equal, skipping node " << node << std::endl;
                        continue;
                    }

                    if(blockchainHeight < incomingBlockchainHeight)
                    {
                        std::size_t heightDifference = incomingBlockchainHeight - blockchainHeight;
                        std::cout << "[NODE-CLIENT]: We are " << heightDifference << " blocks behind node " << node << ", downloading it's blockchain..." << std::endl;
                        get_blockchain_from_specific_node(node);
                        continue;
                    }

                }
                else
                    std::cout << "[NODE-CLIENT]: Could not get height from " << node << ", HTTP status code from response: " << res->status << std::endl;
            }
            else
                std::cout << "[NODE-CLIENT]: Could not reach node at " << node << std::endl;
        }
    }

    void get_multiple_blocks_from_node(std::size_t blockchainHeight, const std::size_t targetBlock, httplib::Client & cli, const std::string & node)
    {
        for(std::size_t currentBlockHeight = blockchainHeight + 1; currentBlockHeight < targetBlock; ++currentBlockHeight)
        {
            std::cout << "[NODE-CLIENT]: Getting block " << currentBlockHeight << " from node " << node << std::endl;
            const std::string requestUrl = "/block/"+std::to_string(currentBlockHeight);
            if(auto res = cli.Get(requestUrl.c_str()))
            {
                if(res->status >= 200 && res->status <= 299)
                {   
                    std::cout << "[NODE-CLIENT]: Successfully reached node " << node << std::endl;
                    const std::string incomingMessage = res->body;
                    nlohmann::json incomingBlockMessageJson = nlohmann::json::parse(incomingMessage, nullptr, false);

                    if (incomingBlockMessageJson.is_discarded())
                    {
                        std::cerr << "[NODE-CLIENT]: The incoming block json could not be parsed, skipping node" << std::endl;
                        return;
                    }
                    if(incomingBlockMessageJson["found"] == "false")
                    {
                        std::cerr << "[NODE-CLIENT]: The block could not be found..." << std::endl;
                        return;
                    }

                    std::cout << "[NODE-CLIENT]: Successfully got block of height " + std::to_string(currentBlockHeight) + " from node node " << node << ", inserting it" << std::endl;
                    chain.push_back(Block(incomingBlockMessageJson));
                    currentBlock.updatePreviousBlockHash(chain.back());
                    std::cout << "[NODE-CLIENT]: Successfully added to the chain, current blockchain size: " << chain.size() << std::endl;
                }
                else
                    std::cout << "[NODE-CLIENT]: Could not get block from " << node << ", HTTP status code from response: " << res->status << std::endl;
            }
            else
                std::cout << "[NODE-CLIENT]: Could not reach node at " << node << std::endl;
        }
    }

    void synchronize_just_needed_blocks()
    {
        std::cout << "[NODE-CLIENT]: Starting synchronizing the missing blocks from the network..." << std::endl;
        const std::size_t blockchainHeight = chain.size();
        for(const auto & node : nodeIPs)
        {
            std::cout << "[NODE-CLIENT]: Reaching node at " << node << '\n';  
            httplib::Client cli(node.c_str());

            if(auto res = cli.Get("/height"))
            {
                if(res->status >= 200 && res->status <= 299)
                {   
                    std::cout << "[NODE-CLIENT]: Successfully reached node " << node << std::endl;
                    const std::string incomingMessage = res->body;
                    nlohmann::json incomingHeightMessageJson = nlohmann::json::parse(incomingMessage, nullptr, false);

                    if (incomingHeightMessageJson.is_discarded())
                    {
                        std::cerr << "[NODE-CLIENT]: The incoming height json could not be parsed, skipping node" << std::endl;
                        continue;
                    }
                    std::size_t incomingBlockchainHeight = incomingHeightMessageJson["height"];
                    if(blockchainHeight == incomingBlockchainHeight)
                    {
                        std::cout << "[NODE-CLIENT]: The blockchain height is equal, skipping node " << node << std::endl;
                        continue;
                    }

                    if(blockchainHeight < incomingBlockchainHeight)
                    {
                        std::size_t heightDifference = incomingBlockchainHeight - blockchainHeight;
                        std::cout << "[NODE-CLIENT]: We are " << heightDifference << " blocks behind node " << node << ", downloading it's blocks..." << std::endl;
                        get_multiple_blocks_from_node(chain.size(), incomingBlockchainHeight, cli, node);
                        continue;
                    }

                }
                else
                    std::cout << "[NODE-CLIENT]: Could not get height from " << node << ", HTTP status code from response: " << res->status << std::endl;
            }
            else
                std::cout << "[NODE-CLIENT]: Could not reach node at " << node << std::endl;
        }
    }

    //This is going to synchronize our blockchain with the network
    void synchronize()
    {
        if(chain.size() == 1)
            return synchronize_entire_blockchain();
        else
            return synchronize_just_needed_blocks();
    }
};

#endif
