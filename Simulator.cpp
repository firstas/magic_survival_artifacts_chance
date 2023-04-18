#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>

class Simulator {
public:
  Simulator(int _tries = 1000) : tries(_tries), sum(allArtifacts * 100) {
    rng = std::mt19937((std::random_device())());
    artifacts = std::vector<int>(allArtifacts, 100);
    if(debug)
      MyFile = std::ofstream("log.txt");
  }
  int simulateRun(int wantedArtifacts, int chestsInRun, int buffer, bool keyChainInPool_ = true, bool prioritizeKeyChain_ = true, bool alwaysKeychain_ = false) {
    auto start = std::chrono::steady_clock::now(), end = start;
    keyChainInPool = keyChainInPool_;
    prioritizeKeyChain = prioritizeKeyChain_;
    alwaysKeychain = alwaysKeychain_;
    if(alwaysKeychain)
      keyChainInInventory = 1;
    int successfulTries = 0;
    for(int try_ = 0; try_ < tries; try_++) {
      if(debug)
        MyFile << "NEW RUN =======================\n\n\n\n\n";
      int successfulGets = 0;
      for(int chestIndex = 0; condition(wantedArtifacts, chestsInRun, buffer, successfulGets, chestIndex); chestIndex++) {
// version one line below was my first and however now it's worse, only if mutliple buffers were calculated at the same time (much faster), it would be only possible:
      // for(int chestIndex = 0; chestIndex < chestsInRun; chestIndex++) {
        if(sum < 300)
          goto lackOfArtifacts;
        chest(wantedArtifacts);
        successfulGets = 0;
        for(int artifactIndex = 0; artifactIndex < wantedArtifacts; artifactIndex++)
          if(artifacts[artifactIndex] == 0)
            successfulGets++;
      }
      successfulGets = 0;
      if(debug)
        MyFile << "END RUN chests =======================\n\n";
      for(int artifactIndex = 0; artifactIndex < wantedArtifacts; artifactIndex++)
        if(artifacts[artifactIndex] == 0)
          successfulGets++;
      if(successfulGets + buffer >= wantedArtifacts)
        successfulTries++;
      // restart:
      artifacts = std::vector<int>(allArtifacts, 100);
      sum = allArtifacts * 100;
      keyChainInInventory = 0;
      if(alwaysKeychain)
        keyChainInInventory = 1;
      keyChainUsedThisChest = false;
    }
    end = std::chrono::steady_clock::now();
    if(timeMeasure)
      std::cout << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms ";
    return successfulTries;
    lackOfArtifacts:
      std::cout << "lackOfArtifacts (from 3 to 15 left from 112) calculations stopped (this limitation allows faster calculations for possible situations)\n";
    return successfulTries;
  }
private:
  // std::uniform_int_distribution<> dist(1, 18);
  // dist(rng);
  const int allArtifacts = 112;
  const int tries;
  std::mt19937 rng;
  std::vector<int> artifacts;
  int sum;
  std::ofstream MyFile;
  const bool debug = false;
  const bool timeMeasure = false;
  bool keyChainInPool;
  const int keyChainIndex = 111;
  int keyChainInInventory = 0;
  bool prioritizeKeyChain;
  bool keyChainUsedThisChest = false;
  bool alwaysKeychain = true;
  bool condition(int wantedArtifacts, int chestsInRun, int buffer, int successfulGets, int chestIndex) {
    if(alwaysKeychain)
      return (wantedArtifacts <= buffer + successfulGets + chestsInRun - chestIndex + (chestsInRun - chestIndex));
    return (wantedArtifacts <= buffer + successfulGets + chestsInRun - chestIndex + std::min(chestsInRun - chestIndex, keyChainInInventory));
  }
  void printArtifactsState() {
    for(int i = 0; i < artifacts.size(); i++)
      MyFile << "i: " << i << " artifacts[i]: " << artifacts[i] << " ";
      MyFile << "\n";
  }
  int giveNum(int upperLimit) { //never actually reaching it
    int temp = rng();
    if(temp < 0)
      temp *= -1;
    return temp % upperLimit;
  }
  int findArtifact(int searchedPosition) {
    if(debug) {
      MyFile << "entering findArtifact ";
      printArtifactsState();
      std::cout << " searchedPosition: " << searchedPosition << " ";
    }
    int current_pos = 0;
    int i = 0;
    if(debug) {
      MyFile << "sum: " << sum << " searchedPosition: " << searchedPosition << " ";
      MyFile << "artifacts[111]: " << artifacts[111] << " ";
    }
    while(true) {
      // if(artifacts[i++] == 0)
      //   continue;
      if(debug)
        MyFile << "pretrue:current_pos: " << current_pos << " i: " << i << " ";
      current_pos += artifacts[i];
      if(searchedPosition < current_pos)
        break;
      i++;
      if(debug)
        MyFile << "posttrue:current_pos: " << current_pos << " i: " << i << " ";
    }
    if(debug)
      MyFile << "\n";
    return i;
  }
  void discard(int artifactIndex) {
    int temp = artifacts[artifactIndex];
    if(artifacts[artifactIndex] >= 50)
      artifacts[artifactIndex] /= 2;
    else
      artifacts[artifactIndex] = 20;
    if(debug)
      std::cout << " ==just before sum update== ";
    sum -= (temp - artifacts[artifactIndex]);
    if(debug)
      std::cout << "sum: " << sum << " artifactIndex: " << artifactIndex << " ";
  }
  void chest(int maxWanted) {  //wanted are all from 0 to (maxWanted - 1)
    if(debug)
      std::cout << "chest starts\n";
    int item1 = findArtifact(giveNum(sum));
    int item2 = findArtifact(giveNum(sum));
    while(item2 == item1)
      item2 = findArtifact(giveNum(sum));
    int item3 = findArtifact(giveNum(sum));
    while(item3 == item1 || item3 == item2)
      item3 = findArtifact(giveNum(sum));
    if(debug) {
      if(item1 < maxWanted || item2 < maxWanted || item3 < maxWanted)
        std::cout << "foundddd\n\n";
      std::cout << "item1: " << item1 << " ";
      std::cout << "item2: " << item2 << " ";
      std::cout << "item3: " << item3 << "\n";
    }
    int choosenArtifact;
    if(item1 < maxWanted && !(prioritizeKeyChain && (item2 == keyChainIndex || item3 == keyChainIndex)))
      choosenArtifact = item1;
    else if(item2 < maxWanted && !(prioritizeKeyChain && item3 == keyChainIndex))
      choosenArtifact = item2;
    else if((!(item3 < maxWanted)) && (keyChainInInventory > 0) && !keyChainUsedThisChest) {
      discard(item1);
      discard(item2);
      discard(item3);
      keyChainInInventory--;
      keyChainUsedThisChest = true;
      return chest(maxWanted);    //it was so important to return to don't continue function actions
    }
    else // yeah, without keyChain you're forced to take something
      choosenArtifact = item3;
    if(debug)
      std::cout << " (after discards) ";
    keyChainUsedThisChest = false;
    if(alwaysKeychain)
      keyChainInInventory = 1;
    sum -= artifacts[choosenArtifact];
    if(debug)
      std::cout << " (sum updates in the end of chest) ";
    artifacts[choosenArtifact] = 0;
    if(item1 != choosenArtifact)
      discard(item1);
    if(item2 != choosenArtifact)
      discard(item2);
    if(item3 != choosenArtifact)
      discard(item3);
    if(keyChainInPool && choosenArtifact == keyChainIndex)
      keyChainInInventory = 3;
  }
};