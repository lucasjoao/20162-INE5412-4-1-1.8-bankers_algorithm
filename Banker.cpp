#include <vector>
#include "Banker.h"
#include "OperatingSystem.h"

Banker::Banker() {
}

Banker::Banker(int numberOfResources, int numberOfProcesses) {
  this->_existenceResources = new std::vector<int>(numberOfResources, 0);
  this->_availableResources = new std::vector<int>(numberOfResources, 0);

  std::vector<int>* resourceLine = new std::vector<int>(numberOfResources, 0);
  this->_currentAllocation = new std::vector<std::vector<int>>(
    numberOfProcesses, *resourceLine);
  this->_processNeeds = new std::vector<std::vector<int>>(
    numberOfProcesses, *resourceLine);
}

Banker::~Banker() {
}

void Banker::addExistenceResources(int resource, int amount) {
  Debug::cout(Debug::Level::trace, "Banker::addExistenceResources(" +
    std::to_string(resource) + ", " + std::to_string(amount) + ")");

  this->_existenceResources->at(resource-1) = amount;
}

void Banker::addProcessNeeds(int process, int resource, int amount) {
  Debug::cout(Debug::Level::trace, "Banker::addProcessNeeds(" +
    std::to_string(process) + ", " + std::to_string(resource) + ", " +
    std::to_string(amount) + ")");

  this->_processNeeds->at(process-1).at(resource-1) = amount;
}

bool Banker::request(int process, int resource, int amount) {
// TO-DO
  return true;
}

void Banker::free(int process, int resource) {
// TO-DO
}
