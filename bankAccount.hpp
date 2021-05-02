#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class BankCard {
public:
    int userId;
};

class AccessToken {
private:
    int timeToLive;
    int creationTime;
    friend class Bank;
};

class Atm {
private:
    int cashOnHand;
    BankCard insertedCard;

public:
    bool InsertCard(BankCard card, int pin);
    void RemoveCard();

    std::vector<std::string> ListAccounts();

    int GetBalance(std::string account);
    bool WithdrawMoney(std::string account, int withdrawal);
    bool DepositMoney(std::string account, int deposit);
};

class Bank {
private:
    /*
     * This data structure holds all relevant user info and is stored as such:
     *          unordered map
     *          /           \
     *       (key)          (value)
     * userId (int)   pair <int, vector>
     *                   /         \
     *       hashed PIN (int)    vector of pairs <string, int>
     *                              /                  \
     *                  account name (string)       account balance (int)
     */
    std::unordered_map<int, std::pair<int, std::vector<std::pair<std::string, int>>>> userInfo;

public:
    AccessToken VerifyUser(BankCard card, int pin);

    bool CreateAccount(BankCard card, std::string account, AccessToken token);
    std::string GetAccounts(BankCard card, AccessToken token);
    bool WithdrawMoney(BankCard card, std::string account, int withdrawal, AccessToken token);
    bool DepositMoney(BankCard card, std::string account, int deposit, AccessToken token);
};