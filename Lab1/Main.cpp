//
// Created by Takex on 26.12.2022.
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <Windows.h>
#include <ctime>

constexpr int kLastRound = 10;

constexpr int kMinLandPrice = 17;
constexpr int kMaxLandPrice = 26;

constexpr int kMinWheatFormLand = 1;
constexpr int kMaxWheatFormLand = 6;

constexpr float kMinWheatEatenByRats = 0;
constexpr float kMaxWheatEatenByRats = 0.07;

constexpr int kWheatToFeadCitizen = 20;
constexpr int kNumberOfLandFarmedByCitizen = 10;

constexpr int kPercentProbobilityOfPlague = 15;

constexpr int kStartNumberOfCitisens = 100;
constexpr int kStartNumberOfWheat = 2800;
constexpr int kStartNumberOfLand = 1000;

const std::string kSaveFileName = "save.txt";

struct City {
    int citizens = kStartNumberOfCitisens;
    int wheat = kStartNumberOfWheat;
    int land = kStartNumberOfLand;
    int hungryCitizens = 0;
    int plantedLand = 0;
};

struct RoundInfo {
    int round = 0;
    int wheatFromOneLand = 0;
    int wheatEatedByRats = 0;
    int landPrice = 0;
    bool isPlague = 0;
    float sumPercentageOfDeath = 0;
};

void buyLand(City &city, int landPrice) {
    while (true) {
        int numberOfLandToBuy;
        std::cout << "������� ����� ����� ����������� ������? ";
        std::cin >> numberOfLandToBuy;
        if (numberOfLandToBuy < 0 || numberOfLandToBuy * landPrice > city.wheat) {
            std::cout << "�, ����������, ������ ���! � ��� ������ "<< city.wheat << " ������� �������!\n";
        } else {
            city.land += numberOfLandToBuy;
            city.wheat -= numberOfLandToBuy * landPrice;
            return;
        }
    }
}

void sellLand(City &city, int landPrice) {
    while (true) {
        int numberOfLandToSell;
        std::cout << "������� ����� ����� ����������� �������? ";
        std::cin >> numberOfLandToSell;
        if (numberOfLandToSell < 0 || numberOfLandToSell > city.land) {
            std::cout << "�, ����������, ������ ���! � ��� ������ " << city.land << " ����� �����!\n";
        } else {
            city.land -= numberOfLandToSell;
            city.wheat += numberOfLandToSell * landPrice;
            return;
        }
    }
}


void feedCity(City &city) {
    while (true) {
        int numberOfWheatToEat;
        std::cout << "������� ������� ������� ����������� ������? ";
        std::cin >> numberOfWheatToEat;
        if (numberOfWheatToEat < 0 || numberOfWheatToEat > city.wheat) {
            std::cout << "�, ����������, ������ ���! � ��� ������ " << city.wheat << " ������� �������!\n";
        } else {
            city.wheat -= numberOfWheatToEat;
            city.hungryCitizens = city.citizens - numberOfWheatToEat / kWheatToFeadCitizen;
            if (city.hungryCitizens < 0) {
                city.hungryCitizens = 0;
            }
            return;
        }
    }
}

void plantLand(City &city) {
    while (true) {
        int numberOfLandToPlant;
        std::cout << "������� ����� ����� ����������� �������? ";
        std::cin >> numberOfLandToPlant;
        if (numberOfLandToPlant < 0 || numberOfLandToPlant > city.land) {
            std::cout << "�, ����������, ������ ���! � ��� ������ " << city.land << " ����� ����� !\n";
        } else if (numberOfLandToPlant / kNumberOfLandFarmedByCitizen > city.citizens) {
            std::cout << "�, ����������, ������ ���! � ��� ������ " << city.citizens << " �������!\n";
        } else {
            city.plantedLand = numberOfLandToPlant;
            return;
        }
    }
}

void saveRound(City city, RoundInfo roundInfo){
    std::ofstream fout(kSaveFileName, std::ios_base::out | std::ios_base::trunc);

    fout << roundInfo.round << " " << roundInfo.isPlague << " " << roundInfo.wheatEatedByRats << " "
         << roundInfo.wheatFromOneLand << " " << roundInfo.landPrice << " " << roundInfo.sumPercentageOfDeath << " ";
    fout << city.citizens << " " << city.hungryCitizens << " " << city.land << " " << city.plantedLand << " " << city.wheat;

    fout.close();
}

bool loadRound(City& city, RoundInfo &roundInfo) {
    std::ifstream fin(kSaveFileName);
    if (!fin.is_open()) {
        return false;
    }

    fin >> roundInfo.round >> roundInfo.isPlague >> roundInfo.wheatEatedByRats >> roundInfo.wheatFromOneLand
        >> roundInfo.landPrice >> roundInfo.sumPercentageOfDeath;
    fin >> city.citizens >> city.hungryCitizens >> city.land >> city.plantedLand >> city.wheat;

    fin.close();
    return true;
}

void initialInfoPrint(City city, RoundInfo roundInfo) {
    std::cout << "��� ����������, �������� �������� ���� � ���� ������ ����� ������� ������ ����������� ���������\n";
    std::cout << "\t��������� ������ ������ ���������� " << city.citizens << " �������;\n";
    std::cout << "\t� ������� " << city.wheat << " ������ �������;\n";
    std::cout << "\t����� ������ �������� " << city.land << " �����;\n";
    std::cout << "\t1 ��� ����� ����� " << roundInfo.landPrice << " ������.\n";
}

void printInfo(City city, RoundInfo roundInfo, int newSitizens, int wheatGrown) {
    std::cout << "��� ����������, �������� �������� ����\n";
    std::cout << "\t� ���� " << roundInfo.round << " ������ ����������� ���������\n";
    std::cout << "\t";
    if (city.hungryCitizens != 0) {
        std::cout << city.hungryCitizens << " ������� ������ � ������, � ";
    }
    std::cout << newSitizens << " ������� ������� � ��� ������� �����;\n";

    if (roundInfo.isPlague) {
        std::cout << "\t���� ���������� �������� ���������;\n";
    }
    std::cout << "\t��������� ������ ������ ���������� " << city.citizens << " �������;\n";
    std::cout << "\t�� ������� " << wheatGrown << " ������ �������, �� " << roundInfo.wheatFromOneLand << " ������ � ����;\n";
    std::cout << "\t";
    if (roundInfo.wheatEatedByRats > 0) {
        std::cout << "����� ��������� " << roundInfo.wheatEatedByRats << " ������ �������, ������� ";
    }
    std::cout << city.wheat << " ������ ������� � �������;\n";
    std::cout << "\t����� ������ �������� " << city.land << " �����;\n";
    std::cout << "\t1 ��� ����� ����� " << roundInfo.landPrice << " ������.\n";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::srand(std::time(nullptr));

    City city;
    RoundInfo roundInfo;
    bool isSaveLoaded = false;
    bool isHungryGameOver = false;
    float sumPercentageOfDeath = 0;

    if (loadRound(city, roundInfo)) {
        while (true) {
            std::cout << "������� ���������� ���������� ����? [��/���] ";
            std::string answer;
            answer = "��";
            std::cin >> answer;
            if (answer == "���") {
                roundInfo.round = 0;
                roundInfo.sumPercentageOfDeath = 0;
                city.citizens = kStartNumberOfCitisens;
                city.wheat = kStartNumberOfWheat;
                city.land = kStartNumberOfLand;
                city.hungryCitizens = 0;
                city.plantedLand = 0;
                break;
            } else if (answer == "��") {
                isSaveLoaded = true;
                break;
            }
        }
    }

    while (roundInfo.round < kLastRound) {
        if (!isSaveLoaded) {
            roundInfo.landPrice = rand() % (kMaxLandPrice - kMinLandPrice + 1) + kMinLandPrice;
        }
        if (roundInfo.round == 0) {
            initialInfoPrint(city, roundInfo);
        } else {

            if (!isSaveLoaded) {
                roundInfo.wheatFromOneLand = rand() % (kMaxWheatFormLand - kMinWheatFormLand + 1) + kMinWheatFormLand;
            }

            int wheatGrown = roundInfo.wheatFromOneLand * city.plantedLand;

            if (!isSaveLoaded) {
                city.wheat += wheatGrown;
                roundInfo.wheatEatedByRats = rand() % (int)(city.wheat * kMaxWheatEatenByRats - city.wheat * kMinWheatEatenByRats + 1) + city.wheat * kMinWheatEatenByRats;
                city.wheat -= roundInfo.wheatEatedByRats;
            }

            int newSitizens = city.hungryCitizens / 2 + (5 - roundInfo.wheatFromOneLand) * city.wheat / 600 + 1;
            if (newSitizens < 0) {
                newSitizens = 0;
            } else if (newSitizens > 50) {
                newSitizens = 50;
            }

            if (!isSaveLoaded) {
                roundInfo.sumPercentageOfDeath += 1.0 * city.hungryCitizens / city.citizens;
                city.citizens += newSitizens - city.hungryCitizens;
                roundInfo.isPlague = rand() % 100 < kPercentProbobilityOfPlague;
                if (roundInfo.isPlague) {
                    city.citizens /= 2;
                }
            }

            isSaveLoaded = false;

            printInfo(city, roundInfo, newSitizens, wheatGrown);

            saveRound(city, roundInfo);
        }

        std::cout << "��� ���������, ����������?\n";

        buyLand(city, roundInfo.landPrice);

        sellLand(city, roundInfo.landPrice);

        feedCity(city);

        plantLand(city);

        if (city.hungryCitizens * 1.0 / city.citizens > 0.45) {
            std::cout << "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������.������ �� ��������� ������� ������ ������������� � ��������\n";
            isHungryGameOver = true;
            break;
        }

        roundInfo.round++;
    }
    if (!isHungryGameOver) {
        float p = roundInfo.sumPercentageOfDeath / kLastRound;
        float l = city.land * 1.0 / city.citizens;
        if (p > 0.33 && l < 7) {
            std::cout << "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������.������ �� ��������� ������� ������ ������������� � ��������\n";
        } else if (p > 10 && l < 9) {
            std::cout << "�� ������� �������� �����, ������� ������ � ����� ��������.����� �������� � �����������, � ����� ������ �� ������ ������ ��� ����������\n";
        } else if (p > 3 && l < 10) {
            std::cout << "�� ���������� ������ �������, � ���, �������, ���� ���������������, �� ������ ������ �� ������� ��� �� ����� ������ �����\n ";
        } else {
            std::cout << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����\n ";
        }
    }

    std::remove(kSaveFileName.c_str());

    return 0;
}