#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

class Window {
 public:
  std::string get_color() { return color; };
  Window(std::string color_ = "black") { color = color_; }

 private:
  std::string color;
};

class Seat {
 public:
  Seat(std::string material_ = "leather", std::string color_ = "brown") {
    material = material_;
    color = color_;
  }
  std::string get_material() { return material; }
  std::string get_color() { return color; };

 private:
  std::string material;
  std::string color;
};

class Wheel {
 public:
  int get_diameter() { return diameter; }
  Wheel(int diameter_ = 950) { diameter = diameter_; }

 private:
  int diameter;
};

class Roof {
 public:
  Roof(std::string material_ = "steel") { material = material_; }
  std::string get_mateiral() { return material; }

 private:
  std::string material;
};

class Carriage {
 public:
  Carriage() {
    id = count_of_carriages++;
  }
  virtual void print_information() {
    std::cout <<"Carriage #" << id 
              << ":\nwindows: " << windows.size() << "\nseats: " << seats.size()
              << "\nwheels: " << wheels.size() << "\nroofs: " << roofs.size()
              << '\n';
  }
  void add_window(Window* window_) { windows.push_back(window_); }
  void add_seat(Seat* seat_) { seats.push_back(seat_); }
  void add_wheel(Wheel* wheel_) { wheels.push_back(wheel_); }
  void add_roof(Roof* roof_) { roofs.push_back(roof_); }

  friend class Head_Carriage;

 protected:
  std::vector<Window*> windows;
  std::vector<Seat*> seats;
  std::vector<Wheel*> wheels;
  std::vector<Roof*>
      roofs;  // допускаем, что можно построить несколько крыш вагону
  int id;
  static int count_of_carriages;
};

class Head_Carriage : public Carriage {
 public:
 void print_information() override {
    std::cout << "Head Carriage #" << id 
              << ":\nwindows: " << windows.size() << "\nseats: " << seats.size()
              << "\nwheels: " << wheels.size() << "\nroofs: " << roofs.size()
              << '\n';
  }
  Head_Carriage(Carriage* other) {
    this->windows = other->windows;
    this->seats = other->seats;
    this->wheels = other->wheels;
    this->roofs = other->roofs;
    this->id = other->id;
    delete other;
  }
};

int Carriage::count_of_carriages = 0;

class Builder {
 public:
  virtual void add_windows() const = 0;
  virtual void add_seats() const = 0;
  virtual void add_wheels() const = 0;
  virtual void add_roof() const = 0;
  virtual void make_head() = 0;
  virtual Carriage* get_product() = 0;
};

class BuilderOfClassicalCarriage : public Builder {
 public:
  BuilderOfClassicalCarriage() { Reset(); }
  void add_windows() const override {
    for (int i = 0; i < 6; i++) {
      product->add_window(new Window);
    }
  }
  void add_seats() const override {
    for (int i = 0; i < 42; i++) {
      product->add_seat(new Seat);
    }
  }
  void add_wheels() const override {
    for (int i = 0; i < 12; i++) {
      product->add_wheel(new Wheel);
    }
  }
  void add_roof() const override { product->add_roof(new Roof); }
  void make_head() override {
    Carriage* tmp = new Head_Carriage(product);
    product = tmp;
  }
  Carriage* get_product() override {
    Carriage* result = product;
    Reset();
    return result;
  }

 private:
  Carriage* product;
  void Reset() { this->product = new Carriage; }
};

class BuilderOfModernCarriage : public Builder {
 public:
  BuilderOfModernCarriage() { Reset(); }
  void add_windows() const override {
    for (int i = 0; i < 6; i++) {
      product->add_window(new Window("grey"));
    }
  }
  void add_seats() const override {
    for (int i = 0; i < 42; i++) {
      product->add_seat(new Seat("plastic", "blue"));
    }
  }
  void add_wheels() const override {
    for (int i = 0; i < 12; i++) {
      product->add_wheel(new Wheel);
    }
  }
  void add_roof() const override { product->add_roof(new Roof("aluminium")); }
  void make_head() override {
    Carriage* tmp = new Head_Carriage(product);
    product = tmp;
  }
  Carriage* get_product() override {
    Carriage* result = product;
    Reset();
    return result;
  }

 private:
  Carriage* product;
  void Reset() { product = new Carriage; }
};

class Director {
 public:
  void set_builder(Builder* builder_) { builder = builder_; }
  void build_full_featured_carriage(bool is_head = 0) {
    builder->add_windows();
    builder->add_seats();
    builder->add_wheels();
    builder->add_roof();
    if (is_head) {
      builder->make_head();
    }
  }
  void build_minimal_viable_carriage(bool is_head = 0) {
    builder->add_roof();
    builder->add_wheels();
    if (is_head) {
      builder->make_head();
    }
  }

 private:
  Builder* builder;
};

class Train {
 public:
  Train(Director* director_to_build_head_carriage,
        Builder* builder_of_head_carriage,
        std::pair<int, int> where_to_place = std::make_pair(0, 0),
        bool head_is_minimal_viable = 0) {
    id = count_of_trains++;
    coordinates_of_head = where_to_place;
    director_to_build_head_carriage->set_builder(builder_of_head_carriage);
    if (head_is_minimal_viable) {
      director_to_build_head_carriage->build_minimal_viable_carriage(1);
    } else {
      director_to_build_head_carriage->build_full_featured_carriage(1);
    }
    carriages.push_back(builder_of_head_carriage->get_product());
  }
  void add_carriage(Carriage* carriage) { carriages.push_back(carriage); }
  void print_information() {
    std::cout << "----------\n";
    std::cout << "Train #" << id << ":\n";
    for (Carriage* i : carriages) {
      i->print_information();
    }
    std::cout << "----------\n";
  }

 private:
  std::vector<Carriage*> carriages;
  std::pair<int, int> coordinates_of_head;
  int id;
  static int count_of_trains;
};

int Train::count_of_trains = 0;

class TrainFleet {
 public:
  TrainFleet(){};
  void add_train(Train* train) { trains.push_back(train); }
  void print_information() {
    for (Train* i : trains) {
      i->print_information();
    }
  }

 private:
  std::vector<Train*> trains;
};

int main() {
  Builder* builder_of_classical_carriage = new BuilderOfClassicalCarriage;
  Builder* builder_of_modern_carriage = new BuilderOfModernCarriage;
  Director* director = new Director;
  TrainFleet* train_fleet = new TrainFleet;
  Train* train =
      new Train(director, builder_of_classical_carriage, std::make_pair(0, 0));
  director->set_builder(builder_of_classical_carriage);
  for (int i = 0; i < 7; i++) {
    director->build_full_featured_carriage();
    train->add_carriage(builder_of_classical_carriage->get_product());
  }
  train_fleet->add_train(train);
  Train* train2 =
      new Train(director, builder_of_modern_carriage, std::make_pair(0, 1));
  director->set_builder(builder_of_modern_carriage);
  for (int i = 0; i < 7; i++) {
    director->build_full_featured_carriage();
    train2->add_carriage(builder_of_modern_carriage->get_product());
  }
  train_fleet->add_train(train2);
  train_fleet->print_information();
}
