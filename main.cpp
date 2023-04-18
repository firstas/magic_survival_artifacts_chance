#include <iostream>
#include <random>
#include <vector>
#include "Simulator.cpp"

int tries = 100000;
Simulator sim(tries);

void defaultTest(int buffer, int wantedArtifacts, int chestsInRun) {
  double withoutKeyChainInPool = (double)sim.simulateRun(wantedArtifacts, chestsInRun, buffer, false, false)/tries;
  if(buffer > 0) std::cout.width(7); else std::cout.width(10); std::cout << withoutKeyChainInPool*100 << "%, #: ";
  std::cout.width(7); std::cout << 1/withoutKeyChainInPool;
  double withKeyChainInPool = (double)sim.simulateRun(wantedArtifacts, chestsInRun, buffer, true, false)/tries;
  std::cout.width(9); std::cout << withKeyChainInPool*100 << "%, #: ";
  std::cout.width(8); std::cout << 1/withKeyChainInPool;
  double withPrioritizeKeyChainInPool = (double)sim.simulateRun(wantedArtifacts, chestsInRun, buffer)/tries;
  std::cout.width(11); std::cout << withPrioritizeKeyChainInPool*100 << "%, #: ";
  std::cout.width(8); std::cout << 1/withPrioritizeKeyChainInPool;
  double keyChainEveryChest = (double)sim.simulateRun(wantedArtifacts, chestsInRun, buffer, true, true, true)/tries;
  std::cout.width(25); std::cout << keyChainEveryChest*100 << "%, #: ";
  std::cout.width(6); std::cout << 1/keyChainEveryChest << "\n";
}
void bufferIterator(int wantedArtifacts, int chestsInRun) {
  std::cout << "Chance (in %) and avg # of runs to get " << wantedArtifacts << " artifacts in " << chestsInRun << " chests:\n";
  std::cout << "                without keyChainInPool | with keyChainInPool | prioritizeKeyChainOverAllOtherArtifacts | keyChainEveryChestButOncePerNormalChestGet\n";
  std::cout << "All artifacts:";
  defaultTest(0, wantedArtifacts, chestsInRun);
  std::cout << "All or without 1:";
  defaultTest(1, wantedArtifacts, chestsInRun);
  std::cout << "All or without 2:";
  defaultTest(2, wantedArtifacts, chestsInRun);
}
void tankBishop() {
  bufferIterator(26, 33);
}
void tankArcheologist() {
  bufferIterator(26, 47);
}

int main(){
  int wantedArtifacts = 26;
  int chestsInRun = 33;

  std::cout.precision(2);
  std::cout.setf(std::ios::fixed);

  while(true) {
    std::cout << "How many artifacts do you want to get?\n";
    std::cin >> wantedArtifacts;
    std::cout << "How many chests in run do you expect to get?\n";
    std::cin >> chestsInRun;
    bufferIterator(wantedArtifacts, chestsInRun);
    std::cout << std::endl;
  }
  // tankBishop();
  // tankArcheologist();
}