#include "DataCenter.h"
#include "DataCenterLog.h"
#include <string.h>
#include <algorithm>

/**
  * @brief  Data center constructor
  * @param  name: The name of the data center
  * @retval None
  */
DataCenter::DataCenter(const char* name)
    : AccountMain(name, this)
{
    Name = name;
}

/**
  * @brief  Data center destructor
  * @param  None
  * @retval None
  */
DataCenter::~DataCenter()
{

}

/**
  * @brief  Search account
  * @param  id: Account ID
  * @retval If the search is successful, return the pointer of the account
  */
Account *DataCenter::SearchAccount(const char *id)
{
    return Find(&AccountPool, id);
}

/**
  * @brief  Search account in vector
  * @param  vec: Pointer to vector
  * @param  id:  Account ID
  * @retval If the search is successful, return the pointer of the account
  */
Account* DataCenter::Find(std::vector<Account*>* vec, const char* id)
{
    for (auto iter : *vec)
    {
        if (strcmp(id, iter->ID) == 0)
        {
            return iter;
        }
    }
    return nullptr;
}

/**
  * @brief  Add an account to the account pool
  * @param  account: Pointer to account
  * @retval If the addition is successful, return true
  */
bool DataCenter::AddAccount(Account *account)
{
    if (account == &AccountMain)
    {
        return false;
    }

    if (SearchAccount(account->ID) != nullptr)
    {
        DC_LOG_ERROR("Multi add Account[%s]", account->ID);
        return false;
    }

    AccountPool.push_back(account);
    AccountMain.Subscribe(account->ID);

    return true;
}

/**
  * @brief  Remove the account from the account pool
  * @param  account: Pointer to account
  * @retval Return true if the removal is successful
  */
bool DataCenter::RemoveAccount(Account *account)
{
    return Remove(&AccountPool, account);
}

/**
  * @brief  Remove account in vector
  * @param  vec: Pointer to vector
  * @param  id:  Account ID
  * @retval Return true if the removal is successful
  */
bool DataCenter::Remove(std::vector<Account*>* vec, Account* account)
{
    auto iter = std::find(vec->begin(), vec->end(), account);

    if (iter == vec->end())
    {
        DC_LOG_ERROR("Account[%s] was not found", account->ID);
        return false;
    }

    vec->erase(iter);

    return true;
}

/**
  * @brief  Get the number of accounts in the account pool
  * @param  None
  * @retval Number of accounts
  */
uint32_t DataCenter::GetAccountLen()
{
    return AccountPool.size();
}
