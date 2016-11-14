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

  int numberOfResources = 4;
  int numberOfProcesses = 5;
  Banker* banker = new Banker(numberOfResources, numberOfProcesses);

  Debug::cout(Debug::Level::info,
    "Haverá " + std::to_string(numberOfResources) + " recursos na simulação");
  Debug::cout(Debug::Level::info,
    "Haverá " + std::to_string(numberOfProcesses) + " processos na simulação");

  switch (executionStep) {
    case 0:  // executionStep is initialized with 0
      banker->addExistenceResources(1, 6);
      banker->addExistenceResources(2, 3);
      banker->addExistenceResources(3, 4);
      banker->addExistenceResources(4, 2);
      break;
    case 1:
      banker->addProcessNeeds(1, 1, 4);
      banker->addProcessNeeds(1, 2, 1);
      banker->addProcessNeeds(1, 3, 1);
      banker->addProcessNeeds(1, 4, 1);
      break;
    case 2:
      banker->addProcessNeeds(2, 1, 0);
      banker->addProcessNeeds(2, 2, 2);
      banker->addProcessNeeds(2, 3, 1);
      banker->addProcessNeeds(2, 4, 2);
      break;
    case 3:
      banker->addProcessNeeds(3, 1, 4);
      banker->addProcessNeeds(3, 2, 2);
      banker->addProcessNeeds(3, 3, 1);
      banker->addProcessNeeds(3, 4, 0);
      break;
    case 4:
      banker->addProcessNeeds(4, 1, 1);
      banker->addProcessNeeds(4, 2, 1);
      banker->addProcessNeeds(4, 3, 1);
      banker->addProcessNeeds(4, 4, 1);
    case 5:
      banker->addProcessNeeds(5, 1, 2);
      banker->addProcessNeeds(5, 2, 1);
      banker->addProcessNeeds(5, 3, 1);
      banker->addProcessNeeds(5, 4, 0);
    default:
      break;
  }

  // advance execution step
  entity->getAttribute("ExecutionStep")->setValue(std::to_string(++executionStep));
  // future event when execution will advance
  simulator->insertEvent(timeNow + 10.0, module, entity);

  if (executionStep == 100) simulator->stop();
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