// finalexams.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

// Base class for currencies with polymorphism
//class Currency {
//public:
//
//    virtual std::string getName() const = 0;
//    virtual int getFoundedYear() const = 0;
//    virtual double getInCirculation() const = 0;
//    virtual double getExchangeRateToUSD() const = 0;
//};

// Base class for currencies with polymorphism
class Currency {
public:
    virtual std::string getName() const = 0;
    virtual int getFoundedYear() const = 0;
    virtual double getInCirculation() const = 0;
    virtual double getExchangeRateToUSD() const = 0;

    // overloaded operator< with const
    bool operator<(const Currency& other) const {
        return getExchangeRateToUSD() < other.getExchangeRateToUSD();
    }

    // Overloaded operator> with const
    bool operator>(const Currency& other) const {
        return getExchangeRateToUSD() > other.getExchangeRateToUSD();
    }
};



// class that is derived from main class currency
class USD : public Currency {
public:
    ////getting variables for name, founded year, how much is in circulation and the exchange rate to the usd to 0

    std::string getName() const override { return "US Dollar"; }
    int getFoundedYear() const override { return 1792; }
    ///using scientifcc notation because else the numbers can be too big
    double getInCirculation() const override { return 2.1e12; }
    double getExchangeRateToUSD() const override { return 1.0; }
};

// Class composition
class CurrencyInfo {
public:
    CurrencyInfo(std::shared_ptr<Currency> currency) : currency_(currency) {}
    //printing currencies 
    void printInfo() const {
        std::cout << "Currency: " << currency_->getName() << std::endl;
        std::cout << "Founded: " << currency_->getFoundedYear() << std::endl;
        std::cout << "In circulation: " << currency_->getInCirculation() << std::endl;
        std::cout << "Exchange rate to USD: " << currency_->getExchangeRateToUSD() << std::endl;
    }


    const Currency& getCurrency() const { return *currency_; }

private:
    //point to other objects
    std::shared_ptr<Currency> currency_;
};

class Euro : public Currency {
public:
    std::string getName() const override { return "Euro"; }
    int getFoundedYear() const override { return 1999; }
    double getInCirculation() const override { return 12.67e12; }
    double getExchangeRateToUSD() const override { return 1.1; }
};

class JapaneseYen : public Currency {
public:
    std::string getName() const override { return "Japanese Yen"; }
    int getFoundedYear() const override { return 1871; }
    double getInCirculation() const override { return 106.4e15; }
    double getExchangeRateToUSD() const override { return 0.0087; }
};

class BritishPound : public Currency {
public:
    std::string getName() const override { return "British Pound"; }
    int getFoundedYear() const override { return 1694; }
    double getInCirculation() const override { return 2.6e12; }
    double getExchangeRateToUSD() const override { return 1.3; }
};



// user defind template class
template <typename T>
class Data {
public:
    //taking objects and adding them to data vector and then returning them
    void add(const T& item) { data_.push_back(item); }
    T& get(size_t index) { return data_[index]; }
    const T& get(size_t index) const { return data_[index]; }
    size_t size() const { return data_.size(); }

    // Provide access to the underlying std::vector
    std::vector<T>& getVector() { return data_; }
    const std::vector<T>& getVector() const { return data_; }

private:
    std::vector<T> data_;
};

// Overloaded operators
bool operator==(const Currency& a, const Currency& b) {
    return a.getName() == b.getName();
}

std::ostream& operator<<(std::ostream& os, const Currency& currency) {
    os << currency.getName();
    return os;
}

// User-defined function that performs searching
//searching and returning currencies
template <typename T>
int findCurrency(const Data<T>& data, const T& currency) {
    for (size_t i = 0; i < data.size(); i++) {
        if (data.get(i) == currency) {
            return i;
        }
    }
    return -1;
}

// User-defined function that performs sorting
template <typename T>
//sorting currencies by value
void sortCurrencies(T& currencies) {
    std::sort(currencies.getVector().begin(), currencies.getVector().end(), [](const CurrencyInfo& a, const CurrencyInfo& b) {
        return a.getCurrency() < b.getCurrency();
        });
}



int main() {
    try {
        // Read currency data from file
        std::ifstream inputFile("currency_data.txt");
        if (!inputFile) {
            throw std::runtime_error("Could not open the input file.");
        }

        // Use of Standard Template Library container (vector)
        Data<CurrencyInfo> currencyInfos;
        std::string line;
        std::string name;
        int foundedYear;
        double inCirculation;
        double exchangeRateToUSD;
        //reading currency names from file and storing them
        while (std::getline(inputFile, name)) {
            if (!std::getline(inputFile, line)) {
                //throwing error if input format is wrong
                throw std::runtime_error("Invalid input format.");
            }
            //extracting data
            std::istringstream iss1(line);
            iss1 >> foundedYear;

            if (!std::getline(inputFile, line)) {
                //throwing error if input format is wrong
                throw std::runtime_error("Invalid input format.");
            }
            std::istringstream iss2(line);
            iss2 >> inCirculation;

            if (!std::getline(inputFile, line)) {
                //throwing error if input format is wrong
                throw std::runtime_error("Invalid input format.");
            }
            std::istringstream iss3(line);
            iss3 >> exchangeRateToUSD;

            // Dynamic memory allocation using shared_ptr
            std::shared_ptr<Currency> currency;
            if (name == "US Dollar") {
                currency = std::make_shared<USD>();
            }
            else if (name == "Euro") {
                currency = std::make_shared<Euro>();
            }
            else if (name == "Japanese Yen") {
                currency = std::make_shared<JapaneseYen>();
            }
            else if (name == "British Pound") {
                currency = std::make_shared<BritishPound>();
            }
            else {
                throw std::runtime_error("Unsupported currency.");
            }

            // Add currency info to the list
            currencyInfos.add(CurrencyInfo(currency));
        }






        // Sort currencies by exchange rate
        sortCurrencies(currencyInfos);

        // Print sorted currency data
        //I/O to console
        for (size_t i = 0; i < currencyInfos.size(); i++) {
            const CurrencyInfo& currencyInfo = currencyInfos.get(i);
            currencyInfo.printInfo();
            std::cout << std::endl;
        }

        // Write sorted currency data to a file
        std::ofstream outputFile("sorted_currency_data.txt");
        if (!outputFile) {
            throw std::runtime_error("Could not open the output file.");
        }
        //iterating, reading and writing to outputfile
        for (size_t i = 0; i < currencyInfos.size(); i++) {
            const CurrencyInfo& currencyInfo = currencyInfos.get(i);
            outputFile << "Currency: " << currencyInfo.getCurrency().getName() << std::endl;
            outputFile << "Founded: " << currencyInfo.getCurrency().getFoundedYear() << std::endl;
            outputFile << "In circulation: " << currencyInfo.getCurrency().getInCirculation() << std::endl;
            outputFile << "Exchange rate to USD: " << currencyInfo.getCurrency().getExchangeRateToUSD() << std::endl;
            outputFile << std::endl;
        }

    }
    catch (const std::exception& e) {
        // Exception handling
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}