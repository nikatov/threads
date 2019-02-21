#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Barrier.hpp"


/* Во избежании ситуации, когда один из потоков, вышедших из барьера, может вновь
 * войти в барьер в цикле быстрее, чем все оставшиеся потоки успеют выйти из него,
 * из-за чего происходит закрытие барьера - необходимо сохранять значение переменной
 * waiting = false, что означает открытый барьер, для потоков, которые еще не успели выйти,
 * при этом закрывая этот же барьер путем изменения переменной waiting = true для вновь
 * пришедних потоков. 
 */

Barrier::Barrier(unsigned int n) : threadCount(n),
                                   threadsWaiting(0),
                                   waiting1(true),
                                   waiting2(true),
                                   tumbler (true)
{
}

void Barrier::wait()
{
    std::unique_lock<std::mutex> lock(mut);
    bool* waiting = tumbler ? &waiting1 : &waiting2;
    
    if (threadsWaiting.fetch_add(1) < threadCount - 1) // RMW (read-modify-write)
    {
        *waiting = true;
        waitVariable.wait(lock, [&]{ return !(*waiting); });
    }
    else
    {
        switchTumbler();
        *waiting = false;
        waitVariable.notify_all();
        threadsWaiting.store(0);
    }
}

//bool wait() 
//{
//    boost::mutex::scoped_lock lock(m_mutex);
//    unsigned int gen = m_generation;
//
//    if (--m_count == 0)
//    {
//        m_generation++;
//        m_count = m_threshold;
//        m_cond.notify_all();
//        return true;
//    }
//
//    while (gen == m_generation)
//        m_cond.wait(lock);
//    return false;
//}
