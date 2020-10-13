#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

std::mt19937 rnd(time(0));

struct Restaraunt;
struct Courier;

struct Order {
  std::string name;
  bool is_completed = 0;
  Order(const std::string& name_) { name = name_; }
};

struct Restaraunt {
  std::unordered_map<Order*, Courier*> orders;
  std::vector<std::pair<Order*, Courier*>> completed_orders;
  virtual void Subscribe(Courier* courier, Order* order) = 0;
  virtual void Unsubscribe(Courier* courier, Order* order) = 0;
  virtual void FinishSomeOrders() = 0;
  virtual void Notify() = 0;
};

struct Courier {
  std::string name;
  Restaraunt* restaraunt;
  Courier(const std::string& name_, Restaraunt* restaraunt_) {
    name = name_;
    restaraunt = restaraunt_;
  }
  void Get(Order* order) {
    std::cout << "Order " << order->name << " is taken by courier " << name
              << '\n';
    restaraunt->Unsubscribe(this, order);
  }
};

struct KFC : Restaraunt {
  void Subscribe(Courier* courier, Order* order) override {
    orders.insert(std::make_pair(order, courier));
  }
  void Unsubscribe(Courier* courier, Order* order) override {
    orders.erase(order);
  }
  void FinishSomeOrders() override {
    for (auto& i : orders) {
      i.first->is_completed |= rnd() % 2;
    }
  }
  void Notify() override {
    for (auto& i : orders) {
      if (i.first->is_completed) {
        completed_orders.emplace_back(i.first, i.second);
      }
    }
    for (auto i : completed_orders) {
      i.second->Get(i.first);
    }
    completed_orders.clear();
  }
};

struct McDonalds : Restaraunt {
  void Subscribe(Courier* courier, Order* order) override {
    orders.insert(std::make_pair(order, courier));
  }
  void Unsubscribe(Courier* courier, Order* order) override {
    orders.erase(order);
  }
  void FinishSomeOrders() override {
    for (auto& i : orders) {
      i.first->is_completed |= rnd() % 2;
    }
  }
  void Notify() override {
    for (auto& i : orders) {
      if (i.first->is_completed) {
        completed_orders.emplace_back(i.first, i.second);
      }
    }
    for (auto i : completed_orders) {
      i.second->Get(i.first);
    }
    completed_orders.clear();
  }
};

int main() {
  Restaraunt* kfc = new KFC;
  Restaraunt* macdonalds = new McDonalds;
  Courier* courier1 = new Courier("Michael", kfc);
  Courier* courier2 = new Courier("Andrew", kfc);
  Courier* courier3 = new Courier("Alex", macdonalds);
  Courier* courier4 = new Courier("Dmitry", macdonalds);
  Order* order1 = new Order("Chicken");
  Order* order2 = new Order("Cheeseburger");
  Order* order3 = new Order("Coffee");
  Order* order4 = new Order("Salad");
  kfc->Subscribe(courier1, order1);
  kfc->Subscribe(courier3, order3);
  macdonalds->Subscribe(courier2, order2);
  macdonalds->Subscribe(courier4, order4);
  macdonalds->FinishSomeOrders();
  macdonalds->Notify();
  kfc->FinishSomeOrders();
  kfc->Notify();
}
