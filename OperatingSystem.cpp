/*
 * File:   System.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 9:04 PM
 */
#include <string>

#include "OperatingSystem.h"
#include "Application.h"
#include "HW_CPU.h"
#include "HW_Machine.h"
#include "Simulator.h"
#include "Banker.h"


void OperatingSystem::LoadApplication(Application* app, MMU::PhysicalAddress address) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::LoadApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + "," + std::to_string(address) + ")");
    std::list<Application::Information> code = app->getCode();
    HW_MMU::Information info;
    for(std::list<HW_MMU::Information>::iterator it = code.begin(); it != code.end(); it++) {
        info = (*it);
        HW_Machine::RAM()->write(address, info);
        address++;
    }
}

void OperatingSystem::SetBootApplication(Application* app) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::SetBootApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + ")");
    HW_CPU::Register address = HW_Machine::CPU()->readRegister(HW_CPU::pc);
    LoadApplication(app, address);
}


void OperatingSystem::Init() {
    Debug::cout(Debug::Level::trace, "OperatingSystem::init()");
    HW_Machine::Init();

    OperatingSystem::CPU_Mediator();
    OperatingSystem::DMA_Mediator();
    OperatingSystem::HardDisk_Mediator();
    OperatingSystem::MMU_Mediator();
    OperatingSystem::Timer_Mediator();

    SetBootApplication(Application::DefaultBootApplication());  // load boot application into RAMs
}


void OperatingSystem::ExecuteTestCode() {
  Debug::cout(Debug::Level::trace, "OperatingSystem::ExecuteTestCode()");
  Simulator* simulator = Simulator::getInstance();
  Entity* entity = simulator->getEntity();
  Module* module = simulator->getModule();
  int executionStep = std::stoi(entity->getAttribute("ExecutionStep")->getValue());
  double timeNow = simulator->getTnow();

  /* maybe i can use this later...
  ****************************************************************************** STATICS OF SAFE
  auto areSafe = 0;
  bool safe;

  safe = memoryManager->banker(ids, hasMems, needMems, j, ask);

  if (safe)
    areSafe += 1;

  auto areSafePercent = (double(areSafe)/double(nro_process)) * 100;
  auto notSafePercent = 100 - areSafePercent;

  Debug::cout(Debug::Level::info, "De todos os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão aprovados e "
    + std::to_string(notSafePercent) + "\045 serão reprovados.");

  Debug::cout(Debug::Level::info, "De todas os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão estados seguros e "
    + std::to_string(notSafePercent) + "\045 serão estados inseguros.");
  */

  /* maybe i can use this later...
  ****************************************************************************** COUT OF TIME
  Debug::cout(Debug::Level::info,
    "Foi gasto " + std::to_string(time) + " segundos no algoritmo.");

  Debug::cout(Debug::Level::info,
    "O tempo médio de execução do algoritmo foi de " +
    std::to_string(averageTime) + " segundos.");
  */

  /* maybe i can use this later...
  ****************************************************************************** BANKER OLD ALGORITHM
  bool MemoryManager::banker(std::vector<unsigned int> *ids,
    std::vector<unsigned int> *hasMems, std::vector<unsigned int> *needMems,
    int i, unsigned int request) {

    Debug::cout(Debug::Level::trace, "MemoryManager::banker");

    auto used = 0;
    std::vector<bool>* finishedJob = new std::vector<bool>();
    for (int j = 0; j < ids->size(); j++) {
      used += hasMems->at(j);
      finishedJob->push_back(false);
    }

    auto memorySize = Traits<MemoryManager>::physicalMemorySize;
    auto available = memorySize - used;
    auto availableFixBug = available;
    auto hasMemsFixBug = hasMems->at(i);

    bool safe = false;
    if (request > available) {
      Debug::cout(Debug::Level::info,
        "Estado inseguro, porque foi solicitado pelo processo " +
        std::to_string(ids->at(i)) + " " + std::to_string(request) + " de " +
        "recurso, sendo que há " + std::to_string(available) + " disponível.");
      return safe;
    }

    Debug::cout(Debug::Level::info, "Há " + std::to_string(available) + " de"
      + " memória disponível.");

    bool dontNeedMore = request + hasMems->at(i) >= needMems->at(i);
    if (dontNeedMore) {
      available += hasMems->at(i);
      finishedJob->at(i) = true;
      Debug::cout(Debug::Level::info, "Após solicitar " + std::to_string(request)
        + " de memória, o processo " + std::to_string(ids->at(i)) + " terminará"
        + " o seu trabalho.");
    } else {
      available -= request;
      hasMems->at(i) += request;
      Debug::cout(Debug::Level::info, "Solicitar " + std::to_string(request)
        + " de memória não será suficiente para o processo " +
        std::to_string(ids->at(i)) + " terminar o seu trabalho. Ele ainda " +
        "precisa de " + std::to_string(needMems->at(i) - hasMems->at(i)) + ".");
    }

    bool infiniteLoop;
    while (available != memorySize) {
      infiniteLoop = true;
      for (int k = 0; k < ids->size(); k++) {
        if (k == i && finishedJob->at(i) || finishedJob->at(k)) continue;

        auto askMem = needMems->at(k) - hasMems->at(k);

        Debug::cout(Debug::Level::info, "Há " + std::to_string(available) + " de"
          + " memória disponível.");
        Debug::cout(Debug::Level::info, "Processo " + std::to_string(ids->at(k))
          + " com " + std::to_string(hasMems->at(k)) + " que precisa de "
          + std::to_string(needMems->at(k)) +
          + " quer utilizar " + std::to_string(askMem) + " da memória. Se isso "
          + "acontecer, ficará " + std::to_string(int(available-askMem)) +
          " do recurso disponível enquanto ele trabalha.");

        if (available >= askMem && !finishedJob->at(k)) {
          infiniteLoop = false;
          finishedJob->at(k) = true;
          available += hasMems->at(k);

          Debug::cout(Debug::Level::info,
            "Processo " + std::to_string(ids->at(k)) +
            + " com " + std::to_string(hasMems->at(k)) + " que precisa de "
            + std::to_string(needMems->at(k)) + " usará mais " +
            std::to_string(askMem) + " da memória, terminará seu trabalho e " +
            "deixará " + std::to_string(available) + " do recurso disponível.");
        }
      }

      if (infiniteLoop) break;
    }

    if (available == memorySize) {
      safe = true;
      Debug::cout(Debug::Level::info,
        "A requisição gera um estado seguro, logo pode ser atendida.");
    } else {
      Debug::cout(Debug::Level::info,
        "A requisição gera um estado inseguro, logo não pode ser atendida.");
    }

    available = availableFixBug;
    hasMems->at(i) = hasMemsFixBug;
    return safe;
  }
  */

  int numberOfResources = 4;
  int numberOfProcesses = 5;
  Banker* banker = new Banker(numberOfResources, numberOfProcesses);

  switch (executionStep) {
    case 0:  // executionStep is initialized with 0
      banker->addExistenceResources(1, 6);
      banker->addExistenceResources(2, 3);
      banker->addExistenceResources(3, 4);
      banker->addExistenceResources(4, 2);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }

  // advance execution step
  entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++));
  // future event when execution will advance
  simulator->insertEvent(timeNow + 10.0, module, entity);
  simulator->stop();
}

 /*
  Not used
  */
 HW_MMU::Information OperatingSystem::asmm(std::string mnemonic) {
     HW_MMU::Information bincode;
     /* @TODO
      */
     if (mnemonic == "Process:exec()") {
         bincode = 0xFFFFFFFF; // - (HW_CPU::addi<<24) +
     }
     //@TODO
     bincode = (HW_CPU::addi<<26) + (HW_CPU::s0<<21) + (HW_CPU::s1<<16) + 1; // for tests purpose only
     return bincode;
 }