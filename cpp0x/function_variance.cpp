#include <functional>
#include <iostream>
#include <memory>

template <class T>
//using Ptr = T *;
using Ptr = std::shared_ptr<T>;

template <class T>
using Sink = std::function<void (Ptr<T>)>;

template <class T>
using Source = std::function<void (Sink<T>)>;

struct Vehicle {};
struct Car : Vehicle {};

struct Metal {};
struct Iron : Metal {};
struct Copper : Metal {};
    
int main(void)
{
  Sink<Vehicle> vehicle_sink = [](Ptr<Vehicle>){ std::cout << "Got some vehicle\n"; };
  Sink<Car> car_sink = vehicle_sink; // Works!
  car_sink(Ptr<Car>(new Car()));
    
  std::function<Car * (Metal *)> car_factory = 
    [](Metal *){ std::cout << "Got some Metal\n"; return new Car(); };
  std::function<Vehicle * (Iron *)> vehicle_factory = car_factory;
  Vehicle * some_vehicle = vehicle_factory(new Iron()); // Works
  (void) some_vehicle;
  //vehicle_sink = car_sink; // Fails to compile
    
  Source<Car> source_car = [](Sink<Car> sink_car){ sink_car(Ptr<Car>(new Car())); };
  Source<Vehicle> source_vehicle = source_car;
  source_vehicle([](Ptr<Vehicle>){ std::cout << "Got a vehicle (hopefull a Car)!!\n"; });
  source_car(car_sink);
    
  std::function<Ptr<Vehicle> (Ptr<Iron>, Ptr<Copper>)> vehicle_ii; 
  std::function<Ptr<Car> (Ptr<Metal>, Ptr<Metal>)> car_mm = [](Ptr<Metal>, Ptr<Metal>) { return Ptr<Car>(new Car()); };
  vehicle_ii = car_mm;
  vehicle_ii(Ptr<Iron>(new Iron()), Ptr<Copper>(new Copper()));
    
  std::function<std::function<Ptr<Vehicle> (Ptr<Copper>)> (Ptr<Iron>)> curried_vehicle;
  std::function<std::function<Ptr<Car> (Ptr<Metal>)> (Ptr<Metal>)> curried_car;
  curried_car = [](Ptr<Metal> m) { return std::function<Ptr<Car> (Ptr<Metal>)>([m](Ptr<Metal>) { return Ptr<Car>(new Car()); }); };  
  curried_vehicle = curried_car;
  curried_vehicle(Ptr<Iron>(new Iron()))(Ptr<Copper>(new Copper()));
}