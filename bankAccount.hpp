#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct BankCard
{
    int userId;
    int bankId;
};

bool operator==(const BankCard& card1, const BankCard& card2) {
    return card1.bankId == card2.bankId &&
           card1.userId == card2.userId;
}

class AccessToken
{
public:
    bool valid;
private:
    int creationTime;
    friend class Bank;

    AccessToken(bool valid, int creationTime)
    {
        valid = valid;
        creationTime = creationTime;
    }
};

class Atm
{
private:
    int cashOnHand;
    BankCard insertedCard;
    AccessToken token;

    struct BankCard dummyCard = 
    {
        -1,
        -1
    };

    // I'm not sure how ATMs handle actual communication with banks.
    // I'd imagine they have some internal table full of numbers or URLs
    // to query different kinds of banks. I'm simulating this with a map
    // of supported bank objects
    std::unordered_map<int, Bank&> supportedBanks;

public:
    bool InsertCard(BankCard card, int pin);
    void RemoveCard();

    std::vector<std::string> ListAccounts();

    int GetBalance(std::string account);
    bool WithdrawMoney(std::string account, int withdrawal);
    bool DepositMoney(std::string account, int deposit);
};

class Bank
{
private:
    std::hash<int> pinHasher;

    // Standard TTL for a token will be 5 minutes
    double timeToLive = 300;

    // This generates a token every time a new user is verified
    AccessToken GenerateToken();

    // This does a quick verify of the access token passed in
    bool CheckToken(AccessToken token);

    // Empty access token to send to signify a failed response
    AccessToken invalidResponse = AccessToken(false, -1);

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
    bool CreateAccount(BankCard card, std::string account, int pin);
    AccessToken VerifyUser(BankCard card, int pin);

    std::vector<std::pair<std::string, int>> GetAccounts(BankCard card, AccessToken token);
    int GetAccountBalance(BankCard card, std::string account, AccessToken token);
    bool WithdrawMoney(BankCard card, std::string account, int withdrawal, AccessToken token);
    bool DepositMoney(BankCard card, std::string account, int deposit, AccessToken token);
};
