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

  // pedindo mais do que tem disponivel, inseguro ***TMP***
  if (amount > this->_availableResources->at(resourceID)) return false;

  // pediu mais do que ele precisa ***TMP***
  if (amount >= this->_processNeeds->at(processID).at(resourceID)) {
    bool dontNeedMore = true;
    for (int i = 0; i < this->_numberOfResources; i++) {
      if (i != resourceID &&
          this->_processNeeds->at(processID).at(resourceID) != 0) {

        dontNeedMore = false;
        break;
      }
    }
    // nao precisa de mais nada, entao terminou o seu trabalho ***TMP***
    if (dontNeedMore) {
      this->_availableResources->at(resourceID) += this->_currentAllocation->
        at(processID).at(resourceID);
      this->_currentAllocation->at(processID).at(resourceID) = 0;
      this->_processNeeds->at(processID).at(resourceID) = 0;
      finishedJob->at(processID) = true;
    }
  } else { // ainda precisa de recursos
    this->_availableResources->at(resourceID) -= amount;
    this->_currentAllocation->at(processID).at(resourceID) += amount;
    this->_processNeeds->at(processID).at(resourceID) -= amount;
  }

  bool infiniteLoop;
  bool haveWorkToDo = true;
  while (haveWorkToDo) {
    infiniteLoop = true;
    for (int j = 0; j < this->_numberOfProcesses; j++) {
      if (finishedJob->at(j)) continue;

      // passo 1 do algoritmo no livro
      bool lessThan = true;
      for (int k = 0; k < this->_numberOfResources; k++) {
        if (this->_processNeeds->at(j).at(k) > this->_availableResources->at(k))
          lessThan = false;
      }

      // passo 2 do algoritmo no livro
      if (lessThan) {
        for (int = l = 0; l < this->_numberOfResources; l++) {
          this->_availableResources->at(j).at(l) += this->_currentAllocation->
            at(j).at(l);
          this->_currentAllocation->at(j).at(l) = 0;
          this->_processNeeds->at(j).at(l) = 0;
        }
        finishedJob->at(j) = true;
        infiniteLoop = false;
      }
    }

    haveWorkToDo = false;
    for (unsigned int m = 0; m < finishedJob->size(); m++) {
      if (!finishedJob->at(m)) {
        haveWorkToDo = true;
        break;
      }
    }

    if (infiniteLoop) break;
  }

  return haveWorkToDo ? false : true;
}

void Banker::free(int process, int resource) {
  Debug::cout(Debug::Level::trace, "Banker::free(" + std::to_string(process) + ", " + std::to_string(resource) + ")");

  int processID = process - 1;
  int resourceID = resourceID - 1;

  if (this->_currentAllocation->at(processID).at(resourceID) != 0) {
    this->_availableResources->at(resourceID) += 1;
    this->_currentAllocation->at(processID).at(resourceID) -= 1;
    this->_processNeeds->at(processID).at(resourceID) += 1;
  } else {
    Debug::cout(Debug::Level::info, "Não é possível liberar 1 unidade do " +
      "recurso " + std::to_string(resource) + " do processo " +
      std::to_string(process) + " porque ele não possui nenhuma unidade desse "
      + "recurso.");
  }
}
