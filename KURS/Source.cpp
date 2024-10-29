#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <locale>
using namespace std;

class Stop {
public:
    std::string name;
    std::string arrivalTime;
    std::string departureTime;

    Stop(const std::string& name, const std::string& arrivalTime, const std::string& departureTime)
        : name(name), arrivalTime(arrivalTime), departureTime(departureTime) {}
};

enum class TypeVagon {
    PLAC, //Плацкартный
    KUPE, //Купейный
    SV, //СВ
    MAIN, //Общий
    SEAT //Сидячий
};

class Vagon {
public:
    int number;
    TypeVagon typeVagon;
    int NumberOfSeats;
    string description;

    void inputVagonInfo() {
        cout << "Введите номер вагона: ";
        cin >> number;
        cout << "Введите тип вагона: ";
        //...
        cout << "Введите описание вагона: ";
        cin.ignore(); // Очищаем буфер
        getline(cin, description);
    }
};

enum class RegionalTrainType {
    ELECTRIC, //Электричка
    DIESEL, //Дизель
    MAIN //Общий вагон
};

enum class RouteType {
    CityLines, //Городские линии
    RegionalLinesEconomy, //Региональные линии экономкласса
    RegionalLinesBiznes, //Региональные линии бизнес класса
    IntercityLinesEconomy, //Межрегиональные линии экономкласса
    InterCityLinesBiznes,  //Межрегиональные линии бизнес класса
    InterCountries //Международные линии
};

class Route {
protected:
    vector<Route> routes;
    RouteType routeType;
    string routeName;
    int routeNumber;
    string departureTime;
    string arrivalTime;
    vector<Stop> stops;
    string trainType;

public:
    // Конструктор по умолчанию
    Route()
        : routeType(routeType), routeName("Unnamed Route"), routeNumber(0),
        departureTime("00:00"), arrivalTime("00:00"),
        trainType("Unknown Train Type") {}

    // Конструктор с параметрами
    Route(const string& routeName, int routeNumber, const string& departureTime,
        const string& arrivalTime, const string& trainType)
        : routeName(routeName), routeNumber(routeNumber),
        departureTime(departureTime), arrivalTime(arrivalTime),
        trainType(trainType) {}

    RouteType getRouteType() const {
        return routeType;
    }

    void addStop(const Stop& stop) {
        stops.push_back(stop);
    }

    const string& getRouteName() const {
        return routeName;
    }

    int getRouteNumber() const {
        return routeNumber;
    }

    const vector<Stop>& getStops() const {
        return stops;
    }

    virtual void addRoute() {
        string routeName, departureTime, arrivalTime, trainType;
        int routeNumber;

        cout << "Enter route name: ";
        cin >> routeName;
        cout << "Enter route number: ";
        cin >> routeNumber;
        cout << "Enter departure time (HH:MM): ";
        cin >> departureTime;
        cout << "Enter arrival time (HH:MM): ";
        cin >> arrivalTime;
        cout << "Enter train type: ";
        cin >> trainType;

        Route newRoute(routeName, routeNumber, departureTime, arrivalTime, trainType);

        char addMoreStops;
        do {
            string stopName, arrivalTime, departureTime;
            cout << "Enter stop name: ";
            cin >> stopName;
            cout << "Enter arrival time (HH:MM): ";
            cin >> arrivalTime;
            cout << "Enter departure time (HH:MM): ";
            cin >> departureTime;

            newRoute.addStop(Stop(stopName, arrivalTime, departureTime));

            cout << "Add another stop? (y/n): ";
            cin >> addMoreStops;
        } while (addMoreStops == 'y' || addMoreStops == 'Y');

        routes.push_back(newRoute);
    }

    virtual void viewRoutesAtStation() {
        string stationToCheck;
        cout << "Enter station name to check routes passing through it: ";
        cin >> stationToCheck;

        bool found = false;
        cout << "Routes passing through " << stationToCheck << ":\n";
        for (const auto& route : routes) {
            if (route.passesThrough(stationToCheck)) {
                route.printRouteInfo();
                cout << "\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No routes found passing through " << stationToCheck << ".\n";
        }
    }

    bool passesThrough(const string& stationName) const {
        for (const auto& stop : stops) {
            if (stop.name == stationName) {
                return true;
            }
        }
        return false;
    }

    virtual void printRouteInfo() const {
        cout << "Тип поезда: " << static_cast<int>(routeType) << "\n"
            << "Route Name: " << routeName << "\n"
            << "Route Number: " << routeNumber << "\n"
            << "Departure Time: " << departureTime << "\n"
            << "Arrival Time: " << arrivalTime << "\n"
            << "Train Type: " << trainType << "\n"
            << "Stops:\n";
        for (const auto& stop : stops) {
            cout << "  - " << stop.name
                << " (Arrival: " << stop.arrivalTime
                << ", Departure: " << stop.departureTime << ")\n";
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile
                << static_cast<int>(routeType)
                << routeName << "\n"
                << routeNumber << "\n"
                << departureTime << "\n"
                << arrivalTime << "\n"
                << trainType << "\n"
                << stops.size() << "\n"; // Сохраняем количество остановок 
            for (const auto& stop : stops) {
                outFile << stop.name << "\n"
                    << stop.arrivalTime << "\n"
                    << stop.departureTime << "\n";
            }
            outFile.close();
            std::cout << "Маршрут сохранен в файл." << std::endl;
        }
        else {
            std::cerr << "Ошибка открытия файла для записи." << std::endl;
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile.is_open()) {
            int type; // Для загрузки типа маршрута
            inFile >> type;
            routeType = static_cast<RouteType>(type);
            getline(inFile, routeName);
            inFile >> routeNumber;
            inFile.ignore(); // Игнорируем символ новой строки после числа 
            getline(inFile, departureTime);
            getline(inFile, arrivalTime);
            getline(inFile, trainType);

            size_t numberOfStops;
            inFile >> numberOfStops;
            inFile.ignore(); // Игнорируем символ новой строки после числа 

            stops.clear(); // Очищаем предыдущие остановки 
            for (size_t i = 0; i < numberOfStops; ++i) {
                string stopName, arrival, departure;
                getline(inFile, stopName);
                getline(inFile, arrival);
                getline(inFile, departure);
                addStop(Stop(stopName, arrival, departure));
            }
            inFile.close();
            std::cout << "Маршрут загружен из файла." << std::endl;
        }
        else {
            std::cerr << "Ошибка открытия файла для чтения." << std::endl;
        }
    }
};

class RegionalRoute : public Route {
public:
    vector<Route> Regionalroutes;
    float AvarageSpeed;
    RegionalTrainType Type;
private:
    RegionalTrainType chooseTrainType() {
        int choice;

        std::cout << "Выберите тип поезда:\n";
        std::cout << "0 - Электричка\n";
        std::cout << "1 - Дизель\n";
        std::cout << "2 - Общий вагон\n";
        std::cout << "Введите номер вашего выбора: ";

        std::cin >> choice;
        while (choice < 0 || choice > 2) {
            std::cout << "Неверный выбор. Пожалуйста, выберите снова: ";
            std::cin >> choice;
        }
        Type = static_cast<RegionalTrainType>(choice);

        return static_cast<RegionalTrainType>(choice);
    }

    void printTrainType() const {
        switch (Type) {
        case RegionalTrainType::ELECTRIC:
            std::cout << "Тип поезда: Электричка\n";
            break;
        case RegionalTrainType::DIESEL:
            std::cout << "Тип поезда: Дизель\n";
            break;
        case RegionalTrainType::MAIN:
            std::cout << "Тип поезда: Общий вагон\n";
            break;
        default:
            std::cout << "Тип поезда не определен.\n";
            break;
        }
    }

    void addRoute() override {

        Route::addRoute();
        cout << "Введите среднюю скорость поезда";
        cin >> AvarageSpeed;
        cout << "Выберите тип поезда из предложенных";
        RegionalTrainType selectedTrainType = chooseTrainType();

    }

    void printRouteInfo() const override {
        Route::printRouteInfo();
        cout << AvarageSpeed;
        printTrainType();
    }
};

class CityRoute : public RegionalRoute {
public:
    string TrainDescription;

};

class LongDistanceRoute : public Route {
public:
    string TrainDescription;
    int NumOfVagons;
    vector<Vagon> Vagons;
    float AvarageSpeed;
    float Cost;
    float Time;
};

class InterCountriesRoute : public LongDistanceRoute {
public:
    string Country;
    float CostForCrossingBoard;
    string InformationAboutCrossingBoard;
};


int main() {
    setlocale(LC_ALL, "");

    vector<Route> routes;
    int choice;
    Route routs;
    do {
        cout << "Menu:\n"; //Русский язык в процессе
        cout << "1. Add Route\n";
        cout << "2. View Routes at Station\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            routs.addRoute();
            break;
        case 2:
            routs.viewRoutesAtStation();
            break;
        case 3:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
            break;
        }

        cout << "\n"; // Для лучшей читаемости
    } while (choice != 3);

    return 0;
}
