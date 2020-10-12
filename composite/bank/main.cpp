#include <iostream>
#include <random>
#include <string>
#include <vector>

std::mt19937 rnd;

class Person {
 public:
  //virtual ~Person(){};
  virtual void Work() = 0;

 protected:
  std::string name;
};

class Banking_Analyst : public Person {
 public:
  Banking_Analyst(std::string name_) { name = name_; }
  void Work() override {
    MakePresentation();
    GetCompanyIndicator();
  }

 protected:
  void MakePresentation() {
    std::cout << "Banking Analyst " << name << ": making presentation\n";
  }
  void GetCompanyIndicator() {
    std::cout << "Banking Analyst " << name << ": getting company indicator\n";
  }
};

class Banking_Associate : public Person {
 public:
  Banking_Associate(std::string name_, std::vector<Banking_Analyst*>& staff_) {
    name = name_;
    staff = staff_;
  }
  /*~Banking_Associate() override {
    for (Banking_Analyst* i : staff) {
      delete (i);
    }
  }*/
  void Work() override {
    GatherCompaniesIndicators();
    for (Banking_Analyst* i : staff) {
      i->Work();
    }
    PutCompanyIndicator();
  }

 protected:
  std::vector<Banking_Analyst*> staff;
  void GatherCompaniesIndicators() {
    std::cout << "Banking Associate " << name
              << ": gathering companies indicators\n";
  }
  void PutCompanyIndicator() {
    std::cout << "Banking Associate " << name
              << ": putting company indicator\n";
  }
};

class VP : public Person {
 public:
  VP(std::string name_, std::vector<Banking_Associate*>& staff_) {
    name = name_;
    staff = staff_;
  }
  /*~VP() override {
    for (Banking_Associate* i : staff) {
      delete (i);
    }
  }*/
  void Work() override {
    for (Banking_Associate* i : staff) {
      i->Work();
    }
    PreparePitchBook();
  }

 protected:
  std::vector<Banking_Associate*> staff;
  void PreparePitchBook() {
    std::cout << "VP " << name << ": preparing pitch book\n";
  }
};

class Senior_VP : public Person {
 public:
  Senior_VP(std::string name_, std::vector<VP*>& staff_) {
    name = name_;
    staff = staff_;
  }
  /*~Senior_VP() override {
    for (VP* i : staff) {
      delete (i);
    }
  }*/
  void Work() override {
    for (VP* i : staff) {
      i->Work();
    }
    PrepareMeeting();
  }

 protected:
  std::vector<VP*> staff;
  void PrepareMeeting() {
    std::cout << "Senior VP " << name << ": preparing meeting\n";
  }
};

class Managing_Director : public Person {
 public:
  Managing_Director(std::string name_, std::vector<Senior_VP*>& staff_) {
    name = name_;
    staff = staff_;
  }
  /*~Managing_Director() override {
    for (Senior_VP* i : staff) {
      delete (i);
    }
  }*/
  void Work() override {
    DrinkCoffee();
    for (Senior_VP* i : staff) {
      i->Work();
    }
  }

 protected:
  std::vector<Senior_VP*> staff;
  void DrinkCoffee() {
    std::cout << "Managing director " << name << ": drinking coffee\n";
  }
};

int main() {
  std::vector<std::string> names;
  names.push_back(std::string("Alex"));
  names.push_back(std::string("John"));
  names.push_back(std::string("Michael"));
  names.push_back(std::string("Caterine"));
  names.push_back(std::string("Nancy"));
  names.push_back(std::string("Nick"));
  names.push_back(std::string("Andrew"));
  std::vector<Senior_VP*> senior_vps;
  for (int i = 0; i < 4; i++) {
    std::vector<VP*> vps(0);
    for (int j = 0; j < 4; j++) {
      std::vector<Banking_Associate*> associates(0);
      for (int k = 0; k < 4; k++) {
        std::vector<Banking_Analyst*> analysts(0);
        for (int m = 0; m < 4; m++) {
          analysts.push_back(
              new Banking_Analyst(names[rnd() % names.size()]));
        }
        associates.push_back(new Banking_Associate(
            names[(int)rnd() % (int)names.size()], analysts));
      }
      vps.push_back(new VP(names[rnd() % names.size()], associates));
    }
    senior_vps.push_back(
        new Senior_VP(names[rnd() % names.size()], vps));
  }
  Managing_Director* director =
      new Managing_Director(names[rnd() % names.size()], senior_vps);
  director->Work();
}
