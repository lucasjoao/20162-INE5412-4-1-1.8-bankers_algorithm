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

  int numberOfResources = 4;
  int numberOfProcesses = 5;
  Banker* banker = new Banker(numberOfResources, numberOfProcesses);
  auto areSafe = 0;

  // here the ideal is do in several cases, but if i do that, the simulation
  // dont work, because in each case the 'banker' will be initialized again and
  // the older values of earlies case will be lost (e.g. what i do in case 0
  // dont persist to case 1)
  switch (executionStep) {
    case 0:  // executionStep is initialized with 0
      Debug::cout(Debug::Level::info,
        "Haverá " + std::to_string(numberOfResources) +
        " recurso(s) na simulação");
      Debug::cout(Debug::Level::info,
        "Haverá " + std::to_string(numberOfProcesses) +
        " processo(s) na simulação");

      // make the base of figure 6-12 of Tanenbaum book
      banker->addExistenceResources(1, 6);
      banker->addExistenceResources(2, 3);
      banker->addExistenceResources(3, 4);
      banker->addExistenceResources(4, 2);
      banker->addProcessNeeds(1, 1, 4);
      banker->addProcessNeeds(1, 2, 1);
      banker->addProcessNeeds(1, 3, 1);
      banker->addProcessNeeds(1, 4, 1);
      banker->addProcessNeeds(2, 1, 0);
      banker->addProcessNeeds(2, 2, 2);
      banker->addProcessNeeds(2, 3, 1);
      banker->addProcessNeeds(2, 4, 2);
      banker->addProcessNeeds(3, 1, 4);
      banker->addProcessNeeds(3, 2, 2);
      banker->addProcessNeeds(3, 3, 1);
      banker->addProcessNeeds(3, 4, 0);
      banker->addProcessNeeds(4, 1, 1);
      banker->addProcessNeeds(4, 2, 1);
      banker->addProcessNeeds(4, 3, 1);
      banker->addProcessNeeds(4, 4, 1);
      banker->addProcessNeeds(5, 1, 2);
      banker->addProcessNeeds(5, 2, 1);
      banker->addProcessNeeds(5, 3, 1);
      banker->addProcessNeeds(5, 4, 0);
      // make requests to arrive in state of figure 6-12 of Tanenbaum book
      areSafe += banker->request(1, 1, 3) ? 1 : 0;
      areSafe += banker->request(1, 3, 1) ? 1 : 0;
      areSafe += banker->request(1, 4, 1) ? 1 : 0;
      areSafe += banker->request(2, 2, 1) ? 1 : 0;
      areSafe += banker->request(3, 1, 1) ? 1 : 0;
      areSafe += banker->request(3, 2, 1) ? 1 : 0;
      areSafe += banker->request(3, 3, 1) ? 1 : 0;
      areSafe += banker->request(4, 1, 1) ? 1 : 0;
      areSafe += banker->request(4, 2, 1) ? 1 : 0;
      areSafe += banker->request(4, 4, 1) ? 1 : 0;
      // first example take from the Tanenbaum book, page 455
      areSafe += banker->request(2, 3, 1) ? 1 : 0;
      // second example take from the Tanenbaum book, page 455
      areSafe += banker->request(5, 3, 1) ? 1 : 0;
      // more examples created by me
      areSafe += banker->request(2, 4, 2) ? 1 : 0;
      banker->free(1, 4);
      areSafe += banker->request(5, 1, 1) ? 1 : 0;
      areSafe += banker->request(5, 3, 1) ? 1 : 0;
      break;
    default:
      break;
  }

  // statistics about time
  double averageTime = simulator->getTnow() / 15;

  Debug::cout(Debug::Level::info,
    "O tempo médio de execução após 15 solicitações foi de " +
    std::to_string(averageTime));

  // statistics about percent
  auto areSafePercent = (double(areSafe)/15.0) * 100;
  auto notSafePercent = 100 - areSafePercent;

  Debug::cout(Debug::Level::info, "De todos os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão aprovados e "
    + std::to_string(notSafePercent) + "\045 serão reprovados");

  Debug::cout(Debug::Level::info, "De todos os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão estados seguros e "
    + std::to_string(notSafePercent) + "\045 serão estados inseguros");

  // advance execution step
  entity->getAttribute("ExecutionStep")->setValue(std::to_string(++executionStep));
  // future event when execution will advance
  simulator->insertEvent(timeNow + 10.0, module, entity);

  // if i dont do this, the program will be a infinite loop
  if (executionStep == 1) simulator->stop();
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