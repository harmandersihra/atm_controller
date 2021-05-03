#include <iostream>

#include "Bank.hpp"

Bank::Bank()
{
    bankId = rand();
    userIdCount = 0;
}

int Bank::GetBankId()
{
    return bankId;
}

BankCard Bank::CreateAccount(std::string account, int pin)
{
    // Create new account with the hashed pin and empty account
    userInfo[userIdCount] = 
        std::pair<int, std::vector<std::pair<std::string, int>>>
            (pinHasher(pin), {std::pair<std::string, int>(account, 0)});

    return {userIdCount++, bankId};
}

bool Bank::VerifyUser(BankCard card, int pin)
{
    // Check to see if the user exists and then if their hashed PIN matches
    if (userInfo.find(card.userId) == userInfo.end())
    {
        return false;
    }
    if (userInfo[card.userId].first != pinHasher(pin))
    {
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, int>> Bank::GetAccounts(BankCard card, int pin)
{
    if (VerifyUser(card, pin))
    {
        return userInfo[card.userId].second;
    }
    return {};
}

int Bank::GetAccountBalance(BankCard card, std::string account, int pin)
{
    if (VerifyUser(card, pin))
    {
        // Go through user's accounts and return balance of selected account
        for (std::pair<std::string, int> accountEntry : userInfo[card.userId].second)
        {
            if (accountEntry.first.compare(account) == 0)
            {
                return accountEntry.second;
            }
        }
    }

    return 0;
}

bool Bank::WithdrawMoney(BankCard card, std::string account, int withdrawal, int pin)
{
    if (!VerifyUser(card, pin)) return false;

    // Get account balance and see if the withdrawal is valid (not trying to withdraw
    // more money than is in the account).
    for (std::pair<std::string, int> &accountEntry : userInfo[card.userId].second)
    {
        if (accountEntry.first.compare(account) == 0)
        {
            // Check to see if they have enough money
            if (withdrawal > accountEntry.second)
            {
                return false;
            }

            accountEntry.second -= withdrawal;
            return true;
        }
    }

    // If the account isn't found, return false
    return false;
}

bool Bank::DepositMoney(BankCard card, std::string account, int deposit, int pin)
{
    if (!VerifyUser(card, pin)) return false;

    // Make sure the account exists and if it does, deposit the money
    for (std::pair<std::string, int> &accountEntry : userInfo[card.userId].second)
    {
        if (accountEntry.first.compare(account) == 0)
        {
            accountEntry.second += deposit;
            return true;
        }
    }

    // If the account isn't present, we can't do the deposit
    return false;
}
