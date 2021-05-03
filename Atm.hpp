#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Bank.hpp"

class Atm
{
private:
    int cashInBin;
    int storedPin;
    BankCard insertedCard;
    BankCard dummyCard = 
    {
        -1,
        -1
    };

    // I'm not sure how ATMs handle actual communication with banks.
    // I'd imagine they have some internal table full of numbers or URLs
    // to query different kinds of banks. I'm simulating this with a map
    // of supported bank objects
    std::unordered_map<int, Bank*> supportedBanks;

public:
    Atm(int cashInBin) : cashInBin(cashInBin) {}

    bool InsertCard(BankCard card, int pin);
    void RemoveCard();

    std::vector<std::string> ListAccounts();

    int GetBalance(std::string account);
    bool WithdrawMoney(std::string account, int withdrawal);
    bool DepositMoney(std::string account, int deposit);

    // Again, not sure about this
    void AddBank(Bank& bank);
};
