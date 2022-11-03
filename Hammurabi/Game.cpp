#include <random>
#include <iostream>
#include <algorithm>
#include <clocale>

class Resource {
public:
    int count = 0;

    int add(int value) {
        return count += value;
    }

    int remove(int value) {
        return count -= value;
    }

    bool hasEnough(int value) {
        return count >= value;
    }
};

class Citizens : public Resource {
public:
    const int maxLandSize = 10;
    const int neededFood = 20;

    int sumForNeededFood() {
        return neededFood * count;
    }

    int feed(int foodValue) {
        int sum = sumForNeededFood();
        int notFedCount = 0;
        if (sum <= foodValue) {
            notFedCount = 0;
        } else {
            int dif = sum - foodValue;
            notFedCount = dif / neededFood; // TODO: check rounding
        }
        return notFedCount;
    }
};

class Food : public Resource {

public:
    int forFeedCount = 0;

    int getForFeedCount() {
        int value;
        if (count < forFeedCount) { value = count; } else { value = forFeedCount; }
        remove(value);
        forFeedCount = 0;
        return value;
    }

    void setForFeedCount(int value) {
        forFeedCount = value;
    }
};

class Land : public Resource {

public:
    const float plantCost = 0.5f;
    int plantedCount = 0;
    int buyCost = 1;
    int harvestModifier = 1;

    int sumForBuy(int count) {
        return buyCost * count;
    }

    int sumForPlant(int count) {
        return (int) (plantCost * (float) count);
    }

    void plant(int count) {
        plantedCount += count;
    }

    int harvest() {
        int sum = plantedCount * harvestModifier;
        plantedCount = 0;
        return sum;
    }
};


class Town {
public:
    Citizens citizens = Citizens();
    Food food = Food();
    Land land = Land();
    bool hasPlague = false;

    bool tryBuyLand(int count) {
        if (count < 0) return false;
        int sum = land.sumForBuy(count);
        bool canBuy = food.hasEnough(sum);
        if (canBuy) {
            buyLand(count);
            return true;
        } else {
            return false;
        }
    }

    bool trySellLand(int count) {
        if (count < 0) return false;
        bool canSell = land.hasEnough(count);
        if (canSell) {
            sellLand(count);
            return true;
        } else {
            return false;
        }
    }

    bool tryPlantLand(int count) {
        if (count < 0) return false;
        int sum = land.sumForPlant(count);
        bool canPlant = food.hasEnough(sum);
        if (canPlant) {
            plantLand(count);
            return true;
        } else {
            return false;
        }
    }

    int harvestLand() {
        int sum = land.harvest();
        food.add(sum);
        return sum;
    }

    int feedCitizens() {
        int dieCount = citizens.feed(food.getForFeedCount());
        citizens.remove(dieCount);
        return dieCount;
    }

    bool trySetForFeedFood(int count) {
        if (count < 0) return false;
        bool canPlant = food.hasEnough(count);
        if (canPlant) {
            food.setForFeedCount(count);
            return true;
        } else {
            return false;
        }
    }

    void ratEat(int value) {
        food.remove(value);
    }

    void newCitizens(int value) {
        citizens.add(value);
    }

    void plague() {
        if (!hasPlague) return;
        int count = citizens.count / 2;
        citizens.remove(count);
    }

private:
    void buyLand(int count) {
        int sum = land.sumForBuy(count);
        food.remove(sum);
        land.add(count);
    }

    void sellLand(int count) {
        int sum = land.sumForBuy(count);
        food.add(sum);
        land.remove(count);
    }

    void plantLand(int count) {
        int sum = land.sumForPlant(count);
        food.remove(sum);
        land.plant(count);
    }
};

class Statistic {
public:
    int lastHarvest = 0;
    int lastDied = 0;
    int lastNewCitizens = 0;
    int lastRatsEat = 0;
    int totalDied = 0;

    void setLastDied(int value){
        lastDied = value;
        totalDied += value;
    }
};

class Game {
    int round = 1;
    Town town = Town();
    Statistic statistic = Statistic();


    void generateNewValues() {
        town.land.buyCost = randomInt(17, 26);
        town.land.harvestModifier = randomInt(1, 6);
        statistic.lastRatsEat = randomInt(0, (int) (0.07f * (float) town.food.count));
        statistic.lastNewCitizens = std::min(std::max<int>(
                                       ((statistic.lastDied / 2) + ((5 - town.land.harvestModifier) * town.food.count / 600) + 1), 0),
                               50);
        town.hasPlague = randomInt(0, 20) <= 3;
    }

    int randomInt(int min, int max) {
        std::random_device seed;
        std::mt19937 gen{seed()};
        std::uniform_int_distribution<int> dist{min, max};
        int guess = dist(gen);
        return guess;
    }

public:
    void init() {
        town.citizens.count = 100;
        town.food.count = 2800;
        town.land.count = 1000;
    }

    void start() {
        while (round <= 10) {
            lastRoundStatistic();
            newRound();
        }
    }

    void doActions(){
        statistic.lastHarvest = town.harvestLand();
        town.ratEat(statistic.lastRatsEat);
        statistic.setLastDied(town.feedCitizens());
        town.newCitizens(statistic.lastNewCitizens);
        town.plague();
    }

    void newRound(){
        generateNewValues();
        currentRoundInfo();
        userInput();
        doActions();
        round++;
    }

    void currentRoundInfo(){
        std::cout << "====== Current Round Info ======" << "\n";
        std::cout << "Land cost: " << town.land.buyCost << "\n";
        std::cout << "Food: " << town.food.count << "\n";
    }

    void lastRoundStatistic(){
        std::cout << "====== Last Round Info ======" << "\n";
        std::cout << "Round: " << round << "\n";
        std::cout << "Died: " << statistic.lastDied<< "\n";
        std::cout << "New Citizens: " << statistic.lastNewCitizens << "\n";
        std::cout << "Has Plague: " << town.hasPlague << "\n";
        std::cout << "Citizens: " << town.citizens.count << "\n";
        std::cout << "Harvest: " << statistic.lastHarvest << "\n";
        std::cout << "Harvest from one: " << town.land.harvestModifier << "\n";
        std::cout << "Plant cost: " << town.land.plantCost << "\n";
        std::cout << "Rats eat: " << statistic.lastRatsEat << "\n";
        std::cout << "Land size: " << town.land.count << "\n";
    }

    void userInput(){
        int buyLandCount = 0;
        int sellLandCount = 0;
        int addFoodCount = 0;
        int plantLandCount = 0;

        bool correctInput = false;
        while (!correctInput) {
            std::cout << "Сколько акров земли повелеваешь купить?";
            std::cin >> buyLandCount;
            correctInput = town.tryBuyLand(buyLandCount);
            if (!correctInput) { std::cout << "ERROR"; continue; }
            std::cout << "Сколько акров земли повелеваешь продать?";
            std::cin >> sellLandCount;
            correctInput = town.trySellLand(sellLandCount);
            if (!correctInput) { std::cout << "ERROR"; continue; }
            std::cout << "Сколько бушелей пшеницы повелеваешь съесть?";
            std::cin >> addFoodCount;
            correctInput = town.trySetForFeedFood(addFoodCount);
            if (!correctInput) { std::cout << "ERROR"; continue; }
            std::cout << "Сколько акров земли повелеваешь засеять?";
            std::cin >> plantLandCount;
            correctInput = town.tryPlantLand(plantLandCount);
            if (!correctInput) { std::cout << "ERROR"; }
        }
    }
};

int main() {
    setlocale(0, "RUS");
    Game game = Game();
    game.init();
    game.start();
}

