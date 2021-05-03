#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "BankCard.hpp"

class Bank
{
private:
    std::hash<int> pinHasher;

    /*
     * This data structure holds all relevant user info and is stored as such:
     *          unordered map
     *          /           \
     *       (key)          (value)
     * userId (int)   pair<int, vector<pair<string, int>>>
     *                   /         \
     *       hashed PIN (int)    vector of pairs pair<string, int>
     *                              /                  \
     *                  account name (string)       account balance (int)
     */
    std::unordered_map<int, std::pair<int, std::vector<std::pair<std::string, int>>>> userInfo;
    int userIdCount;
    int bankId;

public:
    Bank();
    int GetBankId();

    BankCard CreateAccount(std::string account, int pin);
    bool VerifyUser(BankCard card, int pin);
    std::vector<std::pair<std::string, int>> GetAccounts(BankCard card, int pin);
    int GetAccountBalance(BankCard card, std::string account, int pin);
    bool WithdrawMoney(BankCard card, std::string account, int withdrawal, int pin);
    bool DepositMoney(BankCard card, std::string account, int deposit, int pin);
};
