#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

/* Создаем указатель, который поочередно указывает на одну или другую переменную
 * для каждого потока отдельно. Далее пользуемся указателем в качестве флага
 * По освобождению барьера, тумблер переключается. Таким образом, для потоков,
 * оставшихся в барьере указатель isNotWaiting ссылается на одну переменную,
 * а для потоков, дошедшие до следующего барьера - уже на другую.
 */

class Barrier
{
public:
    Barrier(unsigned int n);
    Barrier(const Barrier&) = delete;
    void operator=(const Barrier&) = delete;

    void wait();
private:
    const unsigned int threadCount;
    std::atomic<unsigned int> threadsWaiting;

    std::condition_variable waitVariable;
    std::mutex mut;

    /* Чтобы при входе в следующий барьер не переключить переменную waiting в true,
     * по которой еще не все потоки успели выйти из предыдущего барьера из цикле проверки
     * избежания spurious wakeup (ложных блокировок), необходимо чередовать переменные
     * waiting ПЕПЕЛ И СТАЛЬ
     */
    
    bool waiting1;
    bool waiting2;
    bool tumbler;
    bool switchTumbler(){ tumbler ? tumbler = false : tumbler = true; return !tumbler; }
};
