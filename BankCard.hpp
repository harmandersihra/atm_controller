#pragma once

struct BankCard
{
    int userId;
    int bankId;

    bool operator==(const BankCard& card) {
        return card.bankId == bankId &&
               card.userId == userId;
    }
};
