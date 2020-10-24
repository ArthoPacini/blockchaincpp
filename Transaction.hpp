#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <string>
#include <chrono>
#include <iostream>

#include "sha256.hpp"
#include "nlohmann/json.hpp"
#include "timestamp_as_string.hpp"

class Transaction
{
    private:
    double amount;
    std::string senderKey;
    std::string receiverKey;
    std::string timestamp = getTimestampAsString(std::chrono::system_clock::now());

    public:
    Transaction(double amount, std::string senderKey, std::string receiverKey)
    {
        this->amount = amount;
        this->senderKey = senderKey;
        this->receiverKey = receiverKey;
    }

    Transaction(nlohmann::json transaction_json)
    {
        this->amount = transaction_json["amount"];
        this->senderKey = transaction_json["senderKey"];
        this->receiverKey = transaction_json["receiverKey"];
        this->timestamp = transaction_json["timestamp"];
    }

    std::string getHash() const
    {
        return sha256(std::to_string(amount) + senderKey + receiverKey + timestamp);
    }

    void dump(nlohmann::json & output_json) const
    {
        output_json["transactions"].push_back( {{"amount", amount}, {"senderKey", senderKey}, {"receiverKey", receiverKey}, {"timestamp", timestamp}});
        return;
        std::cout << "\n### Transaction:";
        std::cout << "\n\tSender: " << senderKey;
        std::cout << "\n\tReceiver: " << receiverKey;
        std::cout << "\n\tAmount: " << amount;
        std::cout << "\n\tTimestamp: " << timestamp;
        std::cout << "\n\tTransaction Hash: " << getHash() << '\n';
    }
};

#endif