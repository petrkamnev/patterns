#include <iostream>
#include <string>
#include <map>

class SharedState
{
private:
    std::string mobile_operator, country;

public:
    SharedState()
    {
        this->mobile_operator = "unknown";
        this->country = "unknown";
    }
    SharedState(const std::string &country_, std::string mobile_operator_ = "unknown")
    {
        this->country = country_;
        this->mobile_operator = mobile_operator_;
    }
    friend class Flyweight;
    friend class FlyweightFactory;
};

class UniqueState
{
private:
    std::string mobile_number, owner;

public:
    UniqueState()
    {
        this->mobile_number = "unknown";
        this->owner = "unknown";
    }
    UniqueState(const std::string &mobile_number_, std::string owner_ = "unknown")
    {
        this->mobile_number = mobile_number_;
        this->owner = owner_;
    }
    friend class Flyweight;
    friend class FlyweightFactory;
};

class Flyweight
{
private:
    SharedState *shared_state;

public:
    Flyweight()
    {
    }
    Flyweight(const SharedState *shared_state_) : shared_state(new SharedState(*shared_state_))
    {
    }
    Flyweight(const Flyweight &other) : shared_state(new SharedState(*other.shared_state))
    {
    }
    void PrintInformation(const UniqueState &unique_state) const
    {
        std::cout << "Phone number: " + unique_state.mobile_number + '\n'
                  << "Owner: " + unique_state.owner + '\n' + "Country: "
                  << shared_state->country + '\n' + "Mobile operator: "
                  << shared_state->mobile_operator + '\n';
    }
    friend class FlyweightFactory;
};

class FlyweightFactory
{
private:
    std::string GetKey(const SharedState &shared_state)
    {
        return shared_state.country + shared_state.mobile_operator;
    }
    std::map<std::string, Flyweight> flyweights;

public:
    FlyweightFactory(){};
    Flyweight GetFlyweight(const SharedState &shared_state)
    {
        std::string key = GetKey(shared_state);
        if (this->flyweights.find(key) != this->flyweights.end())
        {
            return this->flyweights[key];
        }
        else
        {
            this->flyweights.insert(std::make_pair(key, Flyweight(&shared_state)));
            return this->flyweights[key];
        }
    }
};

class Database
{
private:
    std::map<std::string, std::pair<UniqueState, Flyweight> > database;
    FlyweightFactory flyweight_factory;

public:
    Database(){};
    void AddNumber(const std::string &number, const std::string &owner, const std::string &country,
                   const std::string &mobile_operator)
    {
        database[number] = std::make_pair(UniqueState(number, owner),
                                          flyweight_factory.GetFlyweight(SharedState(country, mobile_operator)));
    }
    void FindInformationByNumber(const std::string &number)
    {
        std::cout << number + ": ";
        if (database.find(number) == database.end())
        {
            std::cout << "NOT FOUND!\n";
        }
        else
        {
            std::cout << '\n';
            database[number].second.PrintInformation(database[number].first);
        }
    }
};

int main(int, char **)
{
    Database database;
    database.AddNumber("+70000000000", "Иванов Иван Иванович", "Russia", "MTS");
    database.AddNumber("+70000000001", "Иванов Иван Юрьевич", "Russia", "MTS");
    database.AddNumber("+70000000002", "Иванов Иван Петрович", "Russia", "MTS");
    database.AddNumber("+70000000003", "Терентьев Михаил Павлович", "Russia", "Megafon");
    database.AddNumber("+70000000004", "Петров Пётр Петрович", "Russia", "Megafon");
    database.AddNumber("+70000000005", "Ильин Илья Ильич", "Russia", "Megafon");
    database.AddNumber("+70000000006", "Дмитриев Дмитрий Дмитриевич", "Russia", "MTS");
    database.AddNumber("+70000000007", "Максименко Максим Максимович", "Russia", "Beeline");
    database.AddNumber("+70000000008", "Сергиенко Сергей Сергеевич", "Russia", "MTS");
    database.AddNumber("+70000000009", "Александрова Александра Александровна", "Russia", "Megafon");
    std::cout << "Введите номер, который нужно проверить в базе данных: \n";
    std::string number;
    std::cin >> number;
    database.FindInformationByNumber(number);
}
