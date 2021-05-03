#include <iostream>

#include "Atm.hpp"


bool Atm::InsertCard(BankCard card, int pin)
{
    // First query the bank
    if (!supportedBanks[card.bankId]->VerifyUser(card, pin))
    {
        std::cout << "There was an issue logging in. Please contact your bank" << std::endl;
        return false;
    }

    // If we do get a valid response, we hold on to the token and log the user in
    insertedCard = card;
    pin = pin;
    return true;
}

void Atm::RemoveCard()
{
    insertedCard = dummyCard;
    pin = -1;
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
    std::vector<std::pair<std::string, int>> accountEntries =
        supportedBanks[insertedCard.bankId]->GetAccounts(insertedCard, pin);
    std::vector<std::string> accounts;
    for (std::pair<std::string, int> entry : accountEntries)
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
    return supportedBanks[insertedCard.bankId]->GetAccountBalance(insertedCard, account, pin);
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
    if (supportedBanks[insertedCard.bankId]->WithdrawMoney(insertedCard, account, withdrawal, pin))
    {
        // If it does, then take money out of ATM
        cashInBin -= withdrawal;
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
    if (supportedBanks[insertedCard.bankId]->DepositMoney(insertedCard, account, deposit, pin))
    {
        // If it does, then add money to the ATM
        cashInBin += deposit;
        return true;
    }

    return false;
}

void Atm::AddBank(Bank& bank)
{
    supportedBanks[bank.GetBankId()] = &bank;
} 