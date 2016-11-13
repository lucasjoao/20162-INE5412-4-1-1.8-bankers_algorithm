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

  this->_numberOfResources = numberOfResources;
  this->_numberOfProcesses = numberOfProcesses;
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
// COLOCAR ISSO EM UM MÉTODO SEPARADO QUE SEJA DO ALGORITMO E AQUI SÓ FAÇA CHAMADA PARA ELE E ESTATÍSTICA
  Debug::cout(Debug::Level::trace, "Banker::request(" +
    std::to_string(process) + ", " + std::to_string(resource) + ", " +
    std::to_string(amount) + ")");

  std::vector<bool>* finishedJob = new std::vector<bool>(
    this->_numberOfProcesses, false);
  int resourceID = resource - 1;
  int processID = process - 1;
  bool safe = false;

  if (amount > this->_availableResources->at(resourceID)) return safe;

  // TMP: check if amount is enough to finish job
  if (amount >= this->_processNeeds->at(processID).at(resourceID)) {
    bool dontNeedMore = true;
    for (int i = 0; i < this->_numberOfResources; i++) {
      if (i != resourceID &&
          this->_processNeeds->at(processID).at(resourceID) != 0) {

        dontNeedMore = false;
        break;
      }
    }
    if (dontNeedMore) {
      this->_availableResources->at(resourceID) += this->_currentAllocation->
        at(processID).at(resourceID);
      this->_currentAllocation->at(processID).at(resourceID) = 0;
      this->_processNeeds->at(processID).at(resourceID) = 0;
      finishedJob->at(processID) = true;
    }
  } else {
    this->_availableResources->at(resourceID) -= amount;
    this->_currentAllocation->at(processID).at(resourceID) += amount;
    this->_processNeeds->at(processID).at(resourceID) -= amount;
  }



}

void Banker::free(int process, int resource) {
// TO-DO
}
