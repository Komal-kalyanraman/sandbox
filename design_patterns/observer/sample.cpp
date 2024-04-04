#include <iostream>
#include <vector>
#include <algorithm>

// Forward declaration
class Observer;

class Subject {
public:
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notifyObservers();

private:
    std::vector<Observer*> observers;
};

class Observer {
public:
    Observer(Subject* subject) : subject(subject) {
        subject->addObserver(this);
    }
    virtual ~Observer() {
        subject->removeObserver(this);
    }
    virtual void update() = 0;

protected:
    Subject* subject;
};

void Subject::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Subject::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::notifyObservers() {
    for (Observer* observer : observers) {
        observer->update();
    }
}

class ConcreteObserver : public Observer {
public:
    ConcreteObserver(Subject* subject) : Observer(subject) {}
    void update() override {
        std::cout << "ConcreteObserver updated" << std::endl;
    }
};

int main() {
    Subject subject;

    // Create three observers
    ConcreteObserver observer1(&subject);
    ConcreteObserver observer2(&subject);
    ConcreteObserver observer3(&subject);

    // Notify all observers (outputs "ConcreteObserver updated" three times)
    subject.notifyObservers();

    // Remove observer2
    subject.removeObserver(&observer2);

    // Notify remaining observers (outputs "ConcreteObserver updated" two times)
    subject.notifyObservers();
    
    return 0;
}