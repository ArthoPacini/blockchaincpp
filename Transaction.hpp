#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <string>
#include <chrono>
#include <iostream>

#include "sha256.hpp"


class Transaction
{
    private:
    double amount;
    std::string senderKey;
    std::string receiverKey;
    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();

    public:
    Transaction(double amount, std::string senderKey, std::string receiverKey)
    {
        this->amount = amount;
        this->senderKey = senderKey;
        this->receiverKey = receiverKey;
    }

    std::string getHash() const
    {
        return sha256(std::to_string(amount) + senderKey + receiverKey + std::to_string(timestamp.time_since_epoch().count()));
    }

    void dump() const
    {
        std::cout << "\n### Transaction:";
        std::cout << "\n\tSender: " << senderKey;
        std::cout << "\n\tReceiver: " << receiverKey;
        std::cout << "\n\tAmount: " << amount;
        std::cout << "\n\tTimestamp: " << std::to_string(timestamp.time_since_epoch().count());
        std::cout << "\n\tTransaction Hash: " << getHash() << '\n';
    }
};

#endif