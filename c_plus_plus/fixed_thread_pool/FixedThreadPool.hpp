#ifndef FIXED_THREAD_POOL_
#define FIXED_THREAD_POOL_

/*
*Environment：
*Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description：
*固定线程数量的线程池
*/

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

using std::future;
using std::result_of;
using std::vector;
using std::queue;
using std::thread;
using std::function;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::move;
using std::shared_ptr;
using std::make_shared;
using std::packaged_task;
using std::bind;
using std::forward;
using std::runtime_error;

//参数流向
#define IN
#define OUT
#define INOUT

namespace sinicheveen
{
    class FixedThreadPool
    {
    public:
        //构造函数
        inline FixedThreadPool(IN int32_t threadNum);

        //析构函数
        inline ~FixedThreadPool();

        //添加任务到队列中
        template<class F, class... Args>
        inline auto enqueue(IN F&& f, IN Args&&... args)->future<typename result_of<F(Args...)>::type>;

    private:
        //存储创建的线程，后续 join 使用
        vector<thread> m_vThreads;

        //任务队列
        queue<function<void()>>m_qTasks;

        //线程池停止标志
        bool bStop;

        //同步
        mutex m_mutexQueueAndStop;
        condition_variable cv;
    };    //FixedThreadPool

   /*
    *Name：FixedThreadPool
    *Description：构造函数
    *Input：IN int32_t threadNum，线程池中的线程数量
    *Return：
    *Caution：
    */
    inline FixedThreadPool::FixedThreadPool(IN int32_t threadNum)
    {
        bStop = false;
        m_vThreads.reserve(threadNum);
        for (int32_t iThread = 0; iThread < threadNum; iThread++)
        {
            m_vThreads.emplace_back(
                [this]()
                {
                    while (true)    //循环从队列中取任务执行任务
                    {
                        function<void()> task;

                        //如果队列中有任务则从队列中取出一个任务，否则线程阻塞或返回
                        {
                            unique_lock<mutex> lock(this->m_mutexQueueAndStop);
                            this->cv.wait(lock, [this]() {return (this->bStop || !this->m_qTasks.empty()); });    //当线程池没停止，且任务队列为空时阻塞等待任务
                            if (this->bStop && this->m_qTasks.empty())  return;    //当线程池停止，且任务队列为空时，此线程返回

                            //从队列中取出一个任务
                            task = move(this->m_qTasks.front());
                            this->m_qTasks.pop();
                        }    //unique_lock 作用域

                        task();    //执行取出的任务
                    }    //while()
                }    //end of lambda
            );    //emplace_back()
        }    //for()
    }    //FixedThreadPool()

   /*
    *Name：~FixedThreadPool
    *Description：析构函数
    *Input：
    *Return：
    *Caution：
    */
    inline FixedThreadPool::~FixedThreadPool()
    {
        //停止线程池
        {
            unique_lock<mutex> lock(m_mutexQueueAndStop);
            bStop = true;
        }    //unique_lock 作用域
        cv.notify_all();    //通知线程池的所有线程

        // join 操作
        for (thread& iThread : m_vThreads)
        {
            iThread.join();
        }
    }    //~FixedThreadPool()

   /*
    *Name：enqueue
    *Description：添加任务到队列中
    *Input：IN F&& f，函数指针、函数对象、lambda 表达式等
    *             IN Args&&... args，函数参数
    *Return：执行函数 f 获得的返回值
    *Caution：
    */
    template<class F, class... Args>
    inline auto FixedThreadPool::enqueue(IN F&& f, IN Args&&... args)->future<typename result_of<F(Args...)>::type>
    {
        using returnType = typename result_of<F(Args...)>::type;    //获取返回类型
        shared_ptr< packaged_task<returnType()> > task = make_shared< packaged_task<returnType()> >(bind(forward<F>(f), forward<Args>(args)...));    //封装成参数列表为空，且可以异步执行的函数
        future<returnType> returnValue = task->get_future();    //储存返回结果

        //任务加入队列
        {
            unique_lock<mutex>lock(m_mutexQueueAndStop);
            if (bStop)
            {
                throw runtime_error("enqueue on stopped thread pool");
            }
            m_qTasks.emplace([task]() {(*task)(); });    //再次封装成调用特征标为 void() 型函数，放入队列中
        }    //unique_lock 作用域

        //通知一个可能正处于等待状态的线程
        cv.notify_one();

        return returnValue;
    }    //enqueue

}    //sinicheveen

#endif    //FIXED_THREAD_POOL_
