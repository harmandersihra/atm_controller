#include <iostream>

#include "Atm.hpp"

int main(void)
{
    // Create an ATM object
    Atm testAtm(50'000);
    // Create a bank object and add it to the ATM
    Bank chase;
    testAtm.AddBank(chase);

    // Create user at bank
    std::string accountName = "savings";
    int pin = 1337;
    BankCard user1 = chase.CreateAccount(accountName, pin);

    // Test logging in with the wrong pin
    if (testAtm.InsertCard(user1, pin + 1)) {
        return 1;
    }

    // Log in with the correct pin
    if (!testAtm.InsertCard(user1, pin)) {
        return 1;
    }

    std::cout << "All tests passed" << std::endl;
    return 0;
}