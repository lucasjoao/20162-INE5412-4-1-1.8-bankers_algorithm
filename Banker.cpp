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
  Debug::cout(Debug::Level::info, "Existe " + std::to_string(amount) +
    " unidade(s) do recurso " + std::to_string(resource));

  this->_existenceResources->at(resource-1) = amount;
  this->_availableResources->at(resource-1) = amount;
}

void Banker::addProcessNeeds(int process, int resource, int amount) {
  Debug::cout(Debug::Level::trace, "Banker::addProcessNeeds(" +
    std::to_string(process) + ", " + std::to_string(resource) + ", " +
    std::to_string(amount) + ")");
  Debug::cout(Debug::Level::info, "O processo " + std::to_string(process) +
    " precisa de " + std::to_string(amount) + " unidade(s) do recurso " +
    std::to_string(resource));

  this->_processNeeds->at(process-1).at(resource-1) = amount;
}

bool Banker::request(int process, int resource, int amount) {
  Debug::cout(Debug::Level::trace, "Banker::request(" +
    std::to_string(process) + ", " + std::to_string(resource) + ", " +
    std::to_string(amount) + ")");
  Debug::cout(Debug::Level::info, "O processo " + std::to_string(process) +
    " solicitou " + std::to_string(amount) + " unidade(s) do recurso " +
    std::to_string(resource));

  int resourceID = resource - 1;
  int processID = process - 1;

  this->printHelperDebug(process);

  if (amount > this->_processNeeds->at(processID).at(resourceID)) {
    Debug::cout(Debug::Level::info, "Ele só precisa de " +
      std::to_string(this->_processNeeds->at(processID).at(resourceID)) +
      " unidade(s) e solicitou um valor maior, então não é possível o atender");

    return false;
  }

  if (this->algorithm(process, resource, amount)) {
    Debug::cout(Debug::Level::info,
      "A solicitação gera um estado seguro, então é possível atendê-la");

    this->_availableResources->at(resourceID) -= amount;
    this->_currentAllocation->at(processID).at(resourceID) += amount;
    this->_processNeeds->at(processID).at(resourceID) -= amount;
    return true;
  }

  Debug::cout(Debug::Level::info,
    "A solicitação gera um estado inseguro, então não é possível atendê-la");

  return false;
}

bool Banker::algorithm(int process, int resource, int amount) {
  Debug::cout(Debug::Level::trace, "Banker::algorithm(" +
    std::to_string(process) + ", " + std::to_string(resource) + ", " +
    std::to_string(amount) + ")");
  Debug::cout(Debug::Level::info,
    "Vamos ver se a solicitação do processo " + std::to_string(process) +
    " de " + std::to_string(amount) + " unidade(s) do recurso " +
    std::to_string(resource) + " gera um estado inseguro ou não");

  std::vector<bool>* finishedJob = new std::vector<bool>(
    this->_numberOfProcesses, false);
  int resourceID = resource - 1;
  int processID = process - 1;

  std::vector<int> aRfixBug = *this->_availableResources;
  std::vector<std::vector<int>> cAfixBug = *this->_currentAllocation;
  std::vector<std::vector<int>> pNfixBug = *this->_processNeeds;

  if (amount > this->_availableResources->at(resourceID)) {
    Debug::cout(Debug::Level::info,
      "Foi solicitado mais recursos do que há disponível. Não pode!");

    return false;
  }

  if (amount == this->_processNeeds->at(processID).at(resourceID)) {
    bool dontNeedMore = true;
    for (int i = 0; i < this->_numberOfResources; i++) {
      if (i != resourceID &&
          this->_processNeeds->at(processID).at(resourceID) != 0) {

        dontNeedMore = false;
        break;
      }
    }

    if (dontNeedMore) {
      Debug::cout(Debug::Level::info,
        "A solicitação irá fazer com que o processo termine o seu trabalho");

      this->_availableResources->at(resourceID) += this->_currentAllocation->
        at(processID).at(resourceID);
      this->_currentAllocation->at(processID).at(resourceID) = 0;
      this->_processNeeds->at(processID).at(resourceID) = 0;
      finishedJob->at(processID) = true;
    }
  } else {
    Debug::cout(Debug::Level::info,
      "A solicitação não é suficiente para que o processo termine");

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

      bool lessThan = true;
      for (int k = 0; k < this->_numberOfResources; k++) {
        if (this->_processNeeds->at(j).at(k) > this->_availableResources->at(k))
          lessThan = false;
      }

      if (lessThan) {
        Debug::cout(Debug::Level::info,
          "Processo " + std::to_string(j) + " pode pegar todos os recursos " +
          "necessários, terminar o seu trabalho e devolver os recursos");

        for (int l = 0; l < this->_numberOfResources; l++) {
          this->_availableResources->at(l) += this->_currentAllocation->at(j).
            at(l);
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
        Debug::cout(Debug::Level::info, "Nem todos os processos terminaram!");

        haveWorkToDo = true;
        break;
      }
    }

    if (infiniteLoop) {
      Debug::cout(Debug::Level::info, "Não há mais nada que possa ser feito!");

      break;
    }
  }

  *this->_availableResources = aRfixBug;
  *this->_currentAllocation = cAfixBug;
  *this->_processNeeds = pNfixBug;
  return haveWorkToDo ? false : true;
}

void Banker::free(int process, int resource) {
  Debug::cout(Debug::Level::trace, "Banker::free(" + std::to_string(process) + ", " + std::to_string(resource) + ")");

  int processID = process - 1;
  int resourceID = resourceID - 1;

  if (this->_currentAllocation->at(processID).at(resourceID) != 0) {
    Debug::cout(Debug::Level::info, "O processo " + std::to_string(process) +
      " liberou 1 unidade do recurso " + std::to_string(resource));

    this->_availableResources->at(resourceID) += 1;
    this->_currentAllocation->at(processID).at(resourceID) -= 1;
    this->_processNeeds->at(processID).at(resourceID) += 1;
  } else {
    Debug::cout(Debug::Level::info,
      "Não é possível liberar 1 unidade do recurso " +
      std::to_string(resource) + " do processo " + std::to_string(process) +
      " porque ele não possui nenhuma unidade desse recurso");
  }
}

void Banker::printHelperDebug(int process) {
  Debug::cout(Debug::Level::trace,
    "Banker::printHelperDebug(" + std::to_string(process) + ")");

  auto processID = process - 1;

  Debug::cout(Debug::Level::info,
    "availableResources do processo " + std::to_string(process) + " é: " +
    std::to_string(_availableResources->at(0)) + " " +
    std::to_string(_availableResources->at(1)) + " " +
    std::to_string(_availableResources->at(2)) + " " +
    std::to_string(_availableResources->at(3)));

  Debug::cout(Debug::Level::info,
    "currentAllocation do processo " + std::to_string(process) + " é: " +
    std::to_string(_currentAllocation->at(processID).at(0)) + " " +
    std::to_string(_currentAllocation->at(processID).at(1)) + " " +
    std::to_string(_currentAllocation->at(processID).at(2)) + " " +
    std::to_string(_currentAllocation->at(processID).at(3)));

  Debug::cout(Debug::Level::info,
    "processNeeds  do processo " + std::to_string(process) + " é: " +
    std::to_string(_processNeeds->at(processID).at(0)) + " " +
    std::to_string(_processNeeds->at(processID).at(1)) + " " +
    std::to_string(_processNeeds->at(processID).at(2)) + " " +
    std::to_string(_processNeeds->at(processID).at(3)));
}
