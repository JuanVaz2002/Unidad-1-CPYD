//
// Created by root on 8/18/25.
//

#include <iostream>
#include <memory>
#include <print>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>


struct Fork
{
    bool isTaken = false;
    std::string_view philosopher;
    std::mutex forkMutex;
    std::condition_variable taken;

    void takeFork(std::string_view philosopherName, std::string_view hand) {
        {
            std::unique_lock lock(forkMutex);
            taken.wait(lock, [&]
            {
                std::println("{} is waiting for {} hand fork", philosopherName, hand);
                return !isTaken;
            });
            std::println("{} is taking {} hand fork", philosopherName, hand);

            isTaken = true;
            philosopher = philosopherName;
        }
    }

    void releaseFork(std::string_view hand)
    {
        std::println("{} is releasing {} hand fork", philosopher, hand);
        isTaken = false;
        this->philosopher = "";
        taken.notify_all();
    }
    void printSate(const std::string_view &philosopherName, const std::string &forkName)
    {
        std::println("Used by {}, with {} hand", philosopherName, forkName);
    }
};

class Philosopher
{
    std::string name;
    std::shared_ptr<Philosopher> rightPHilosopher = nullptr;
    std::shared_ptr<Fork> leftFork;
    std::shared_ptr<Fork> rightFork;
public:
    Philosopher(std::string name) : name(name) {
    }

    void eat()
    {
        while (true)
        {
            leftFork->takeFork(name, "left");
            if (rightFork->isTaken) {
                leftFork->releaseFork("left");
                continue;
            }
            rightFork->takeFork(name, "right");
            std::println("{} started eating", name);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::println("{} done eating", name);
            rightFork->releaseFork("right");
            leftFork->releaseFork("left");
        }
    }

    std::string &Name()
    {
        return name;
    }

    std::string Name() const
    {
        return name;
    }

    std::shared_ptr<Fork> LeftFork() const
    {
        return leftFork;
    }

    std::shared_ptr<Fork> &LeftFork()
    {
        return leftFork;
    }

    std::shared_ptr<Fork> RightFork() const
    {
        return rightFork;
    }

    std::shared_ptr<Fork> &RightFork()
    {
        return rightFork;
    }
};

int main()
{
    std::shared_ptr<Fork> fork2 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork1 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork3 = std::make_shared<Fork>();

    std::shared_ptr<Philosopher> socrates =
        std::make_shared<Philosopher>("Socrates");
    socrates->LeftFork() = fork1;
    socrates->RightFork() = fork2;

    std::shared_ptr<Philosopher> platon =
        std::make_shared<Philosopher>("Platon");
    platon->LeftFork() = fork2;
    platon->RightFork() = fork3;

    std::shared_ptr<Philosopher> aristotle =
        std::make_shared<Philosopher>("Aristotle");
    aristotle->LeftFork() = fork3;
    aristotle->RightFork() = fork1;

    std::thread threadSocrates([&]() {socrates->eat();});
    std::thread threadPlaton([&]() {platon->eat();});
    std::thread threadAristotle([&]() {aristotle->eat();});

    threadSocrates.join();
    threadPlaton.join();
    threadAristotle.join();
    return 0;
}