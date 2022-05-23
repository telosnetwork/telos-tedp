#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <intx/base.hpp>
#include <eosio.tedp/util.hpp>
#include <eosio.tedp/eosio.token.hpp>
#include <cmath>

using namespace std;
using namespace eosio;

class [[eosio::contract("eosio.tedp")]] tedp : public contract
{
public:
    using contract::contract;
    tedp(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds), payouts(receiver, receiver.value),  configuration(get_self(), get_self().value) {}

    ACTION settf(uint64_t amount);
    ACTION setecondev(uint64_t amount);
    ACTION setcoredev(uint64_t amount);
    ACTION setrex(uint64_t amount);
    ACTION setevmstake(uint64_t amount);
    ACTION delpayout(name to);
    ACTION pay();

private:
    static constexpr name CORE_SYM_ACCOUNT = name("eosio.token");
    static constexpr symbol CORE_SYM = symbol("TLOS", 4);
    static constexpr name EVM_CONTRACT = name("eosio.evm");
    static constexpr name REX_CONTRACT = name("eosio.rex");
    void setpayout(name to, uint64_t amount, uint64_t interval);
    void evmpay(uint64_t total_due);
    double getbalanceratio();
    double getfixedratio();

    struct [[eosio::table, eosio::contract("eosio.evm")]] AccountState {
        uint64_t index;
        eosio::checksum256 key;
        bigint::checksum256 value;

        uint64_t primary_key() const { return index; };
        eosio::checksum256 by_key() const { return key; };

        EOSLIB_SERIALIZE(AccountState, (index)(key)(value));
    };

    TABLE config {
        double ratio;
        EOSLIB_SERIALIZE(config, (ratio))
    };

    TABLE payout {
        name to;
        uint64_t amount;
        uint64_t interval;
        uint64_t last_payout;
        uint64_t primary_key() const { return to.value; }
    };

    typedef eosio::multi_index<"accountstate"_n, AccountState,
            eosio::indexed_by<eosio::name("bykey"), eosio::const_mem_fun<AccountState, eosio::checksum256, &AccountState::by_key>>
    > account_state_table;

    typedef multi_index<name("payouts"), payout> payout_table;

    typedef eosio::singleton<"config"_n, config> config_table;

    using setpayout_action = action_wrapper<name("setpayout"), &tedp::setpayout>;
    using delpayout_action = action_wrapper<name("delpayout"), &tedp::delpayout>;
    using pay_action = action_wrapper<name("payout"), &tedp::pay>;
    payout_table payouts;
    config_table configuration;
};