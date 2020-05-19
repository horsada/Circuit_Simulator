#ifndef "example_data_structure_hpp"
#define "example_data_structure_hpp"

class Component
{
  public:
  string name;
  vector<component>* components;
  node positive; // positive terminal
	node negative; // negative terminal
}

class Resistor : public Component
{
  private:
  float resistance;
  float voltage;
  float current;
  public:
  float conductance()
  {
    return 1/Resistor.resistance;
  }
}

class Capacitor : public Component
{
  private:
  float impedance;
}

#endif