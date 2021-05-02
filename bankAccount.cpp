#include <iostream>

#include "bankAccount.hpp"

// ATM
bool Atm::InsertCard(BankCard card, int pin)
{
    // We first query the bank to see if this user can be logged in
    AccessToken response = supportedBanks[card.bankId].VerifyUser(card, pin);

    // If we don't get a valid response, then we notify the user and return false
    if (!response.valid)
    {
        std::cout << "Issue logging in, please contact your bank" << std::endl;
        return false;
    }

    // If we do get a valid response, we hold on to the token and log the user in
    token = response;
    insertedCard = card;
    return true;
}

void Atm::RemoveCard()
{
    insertedCard = dummyCard;
}

std::vector<std::string> Atm::ListAccounts()
{
    // If user isn't logged in, don't return anything
    if (insertedCard == dummyCard)
    {
        std::cout << "User not logged in. Please insert card" << std::endl;
        return {};
    }

    // Query bank for accounts and pull account names
    std::vector<std::string> accounts;
    for (std::pair<std::string, int> entry : supportedBanks[insertedCard.bankId].GetAccounts(insertedCard, token))
    {
        accounts.push_back(entry.first);
    }

    return accounts;
}

int Atm::GetBalance(std::string account)
{
    // If user isn't logged in, don't return anything
    if (insertedCard == dummyCard)
    {
        std::cout << "User not logged in. Please insert card" << std::endl;
        return {};
    }

    // Query bank for balance
    return supportedBanks[insertedCard.bankId].GetAccountBalance(insertedCard, account, token);
}

bool Atm::WithdrawMoney(std::string account, int withdrawal)
{
    // If user isn't logged in, don't return anything
    if (insertedCard == dummyCard)
    {
        std::cout << "User not logged in. Please insert card" << std::endl;
        return {};
    }

    // Check to see if bank allows withdrawal
    if (supportedBanks[insertedCard.bankId].WithdrawMoney(insertedCard, account, withdrawal, token))
    {
        // If it does, then take money out of ATM
        cashOnHand -= withdrawal;
        return true;
    }

    return false;
}

bool Atm::DepositMoney(std::string account, int deposit)
{
    // If user isn't logged in, don't return anything
    if (insertedCard == dummyCard)
    {
        std::cout << "User not logged in. Please insert card" << std::endl;
        return {};
    }

    // Check to see if bank allows depositing the money
    if (supportedBanks[insertedCard.bankId].DepositMoney(insertedCard, account, deposit, token))
    {
        // If it does, then add money to the ATM
        cashOnHand += deposit;
        return true;
    }

    return false;
}


// Bank
AccessToken Bank::GenerateToken()
{
    return AccessToken(true, time(0));
}

bool Bank::CheckToken(AccessToken token)
{
    return token.valid && (difftime(time(0), token.creationTime) < timeToLive);
}

bool Bank::CreateAccount(BankCard card, std::string account, int pin)
{
    // This function should only be called for new users. If an account already
    // exists with this user, this will return false
    if (userInfo.find(card.userId) != userInfo.end())
    {
        return false;
    }

    // Create new account with the hashed pin and empty account
    userInfo[card.userId] = 
        std::pair<int, std::vector<std::pair<std::string, int>>>
            (pinHasher(pin), {std::pair<std::string, int>(account, 0)});
    
    return true;
}

AccessToken Bank::VerifyUser(BankCard card, int pin)
{
    // Check to see if the user exists and then if their hashed PIN matches
    if (userInfo.find(card.userId) == userInfo.end())
    {
        std::cout << "User not found. Please create an account first." << std::endl;
        return invalidResponse;
    }
    if (userInfo[card.userId].first != pinHasher(pin))
    {
        std::cout << "Incorrect PIN. Please try again." << std::endl;
        return invalidResponse;
    }

    return GenerateToken();
}

std::vector<std::pair<std::string, int>> Bank::GetAccounts(BankCard card, AccessToken token)
{
    // Check the token
    if (!CheckToken(token))
    {
        std::cout << "Invalid access token." << std::endl;
        return {};
    }

    return userInfo[card.userId].second;
}

int Bank::GetAccountBalance(BankCard card, std::string account, AccessToken token)
{
    // Check the token
    if (!CheckToken(token))
    {
        std::cout << "Invalid access token." << std::endl;
        return -1;
    }

    // Go through user's accounts and return balance of selected account
    for (std::pair<std::string, int> accountEntry : userInfo[card.userId].second)
    {
        if (accountEntry.first.compare(account) == 0)
        {
            return accountEntry.second;
        }
    }

    // If not found, then return error
    std::cout << "This account does not exist" << std::endl;
    return 0;
}

bool Bank::WithdrawMoney(BankCard card, std::string account, int withdrawal, AccessToken token)
{
    // Check the token
    if (!CheckToken(token))
    {
        std::cout << "Invalid access token." << std::endl;
        return false;
    }

    // Get account balance and see if the withdrawal is valid (not trying to withdraw
    // more money than is in the account).
    for (std::pair<std::string, int> accountEntry : userInfo[card.userId].second)
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

bool Bank::DepositMoney(BankCard card, std::string account, int deposit, AccessToken token)
{
    // Check the token
    if (!CheckToken(token))
    {
        std::cout << "Invalid access token." << std::endl;
        return -1;
    }

    // Make sure the account exists and if it does, deposit the money
    for (std::pair<std::string, int> accountEntry : userInfo[card.userId].second)
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