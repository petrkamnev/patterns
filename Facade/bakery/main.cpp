#include <iostream>
#include <string>

class Baker
{ // пекарь
private:
    std::string name; // имя пекаря
    std::string current_kind_of_bread; // тип хлеба, который пекарь готовит

public:
    Baker() // по умолчанию имя пекаря Unknown и он готовит неопределенный тип хлеба
    {
        name = "Unknown";
        current_kind_of_bread = "";
    }
    Baker(std::string &name_) : name(name_){};
    void choose_kind_of_bread(std::string &kind) // изменить выбранный тип хлеба
    {
        current_kind_of_bread = kind;
        std::cout << "Baker " << this->name << " choises " << kind
                  << " bread.\n";
    }
    void make_dough() // сделать тесто
    {
        std::cout << "Baker " << this->name << " makes dough.\n";
    }
    void turn_on_the_oven(int &temperature) // включить печь
    {
        std::cout << "Baker " << this->name << " turns on the oven and "
                  << "selects temperature " << temperature << " degrees.\n";
    }
    void put_the_bread(int &minutes) // поставить хлеб в печь
    {
        std::cout << "Baker " << this->name << " puts the bread in the oven for " << minutes << (minutes == 1 ? " minute&\n" : " minutes&\n");
    }
    void wait() // ждать
    {
        std::cout << "Baker " << this->name << " waits.\n";
    }
    void get_the_bread() // достать хлеб из печи
    {
        std::cout << "Baker " << this->name << " gets the bread out of the oven.\n";
    }
    void turn_off_the_oven() // выключить печь
    {
        std::cout << "Baker " << this->name << " turns off the oven.\n";
    }
    std::string get_name() // возвращает имя пекаря
    {
        return this->name;
    }
};

class Bread
{ // хлеб в процессе приготовления
private:
    std::string kind_of_bread; // тип хлеба
    int cooking_time; // время приготовления
    int cooking_temperature; // температура, при которой хлеб приготовлен

public:
    Bread() // по умолчанию хлеб не имеет типа и время приготовления равно 0
    {
        kind_of_bread = "";
        cooking_time = 0;
    }
    void choose_kind_of_bread(std::string &kind) // храним тип хлеба
    {
        kind_of_bread = kind;
        std::cout << "KIND OF BREAD: " << kind << ".\n";
    }
    void make_dough() // сделать тесто
    {
        std::cout << "DOUGH: is ready.\n";
    }
    void remember_the_temperature(int &temperature)// храним температуру,
        //при которой был приготовлен хлеб
    {
        cooking_temperature = temperature;
    }
    void put_the_bread(int &minutes) // поставить хлеб в печь
    {
        cooking_time += minutes;
        std::cout << "BREAD: is in the baking process.\n";
    }
    void get_the_bread() // достать хлеб из печи
    {
        std::cout << "BREAD: is out of the oven.\n";
    }
    std::pair<int, int> get_cooking_time_and_temperature() // возвращает температуру и время приготовления
    {
        return std::make_pair(this->cooking_time, this->cooking_temperature);
    }
};

class Oven
{ // печь
private:
    int temperature; // температура в печи сейчас
public:
    Oven() // по умолчанию в выключенной печи поддерживается температура 20 градусов
    {
        temperature = 20;
    }
    void turn_on_the_oven(int &temperature_) // включить печь и выставить температуру
    {
        std::cout << "OVEN: the temperature is " << temperature_ << " degrees.\n";
        temperature = temperature_;
    }
    void turn_off_the_oven() // выключить печь - температура возвращается к исходной
    {
        temperature = 20;
        std::cout << "OVEN: the temperature is " << temperature << " degrees.\n";
    }
};

class Bakery // пекарня - фасад
{
private:
    Baker baker; // пекарь - владелец пекарни
    Oven oven; // печь в пекарне

public:
    Bakery() {};
    Bakery(std::string &name) // конструктор пекарни с заданным именем владельца
    {
        baker = Baker(name);
    }
    Bread cook_bread(std::string &kind, int &time, int &temperature) // последовательность действий чтобы приготовить хлеб
    {
        Bread bread;
        std::cout << baker.get_name() << "\'s bakery begins to bake the " << kind << " bread...\n";
        baker.choose_kind_of_bread(kind);
        bread.choose_kind_of_bread(kind);
        baker.make_dough();
        bread.make_dough();
        baker.turn_on_the_oven(temperature);
        oven.turn_on_the_oven(temperature);
        bread.remember_the_temperature(temperature);
        baker.put_the_bread(time);
        bread.put_the_bread(time);
        baker.wait();
        baker.get_the_bread();
        bread.get_the_bread();
        baker.turn_off_the_oven();
        oven.turn_off_the_oven();
        std::cout << "The bread is ready!\n\n";
        return bread;
    }
    void taste_the_bread(Bread &bread) // пекарня проверяет, правильно ли приготовлен хлеб
    { 
        std::pair<int, int> temp = bread.get_cooking_time_and_temperature();
        int time = temp.first;
        int temperature = temp.second;
        if (time >= 20 && time <= 25 && temperature >= 190 && temperature <= 210)
        {
            std::cout << "The bread is excellent!\n";
        }
        else
        {
            std::cout << "The bread is bad:(\n";
        }
    }
};

int main(int, char **)
{
    std::string name;
    std::cout << "Select baker's name: ";
    std::cin >> name;
    Bakery bakery(name);
    std::string kind;
    int time, temperature;
    std::cout << "Select the kind of bread: ";
    std::cin >> kind;
    std::cout << "Select the cooking time: ";
    std::cin >> time;
    std::cout << "Select the cooking temperature: ";
    std::cin >> temperature;
    std::cout << '\n';
    Bread bread = bakery.cook_bread(kind, time, temperature);
    bakery.taste_the_bread(bread);
}