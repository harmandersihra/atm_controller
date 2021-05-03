#include <iostream>

#include "Atm.hpp"

int main(void)
{
    // Create an ATM object with $50k
    Atm testAtm(50'000);
    // Create a bank object and add it to the ATM
    Bank chase;
    testAtm.AddBank(chase);

    // Create user at bank
    std::string accountName = "savings";
    int pin = 1337;
    BankCard user1 = chase.CreateAccount(accountName, pin);

    std::cout << "Test logging in with the wrong PIN" << std::endl;
    if (testAtm.InsertCard(user1, pin + 1)) {
        return 1;
    }

    std::cout << "Test logging in with the correct PIN" << std::endl;
    if (!testAtm.InsertCard(user1, pin)) {
        return 1;
    }

    std::cout << "Test account name retrieval" << std::endl;
    std::vector<std::string> expected = {"savings"};
    if (testAtm.ListAccounts() != expected) {
        return 1;
    }

    std::cout << "Test depositing money" << std::endl;
    if (!testAtm.DepositMoney("savings", 5'000)) {
        return 1;
    }

    std::cout << "Test seeing balance" << std::endl;
    if (testAtm.GetBalance("savings") != 5'000) {
        return 1;
    }

    std::cout << "Test withdrawing balance" << std::endl;
    if (!testAtm.WithdrawMoney("savings", 1'500)) {
        return 1;
    }

    std::cout << "Test seeing balance" << std::endl;
    if (testAtm.GetBalance("savings") != 3'500) {
        return 1;
    }

    std::cout << "Test seeing balance after removing card" << std::endl;
    testAtm.RemoveCard();
    if (testAtm.GetBalance("savings") != -1) {
        return 1;
    }

    std::cout << "All tests passed" << std::endl;
    return 0;
}