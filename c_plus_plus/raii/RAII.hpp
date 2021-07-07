#ifndef _RAII_
#dfine _RAII_

namespace sinicheveen
{
    template<class T>
    class NewGuard
    {
    public:
        explicit NewGuard(const T* ptr = nullptr) :ptr(ptr) {}
        ~NewGuard()
        {
            if (nullptr != ptr)
            {
                delete ptr;
                ptr = nullptr;
            }
        }

    private:
        const T* ptr;
    };    //class NewGuard

    template<class T>
    class NewGuards
    {
    public:
        explicit NewGuards(const T* ptr = nullptr) :ptr(ptr) {}
        ~NewGuards()
        {
            if (nullptr != ptr)
            {
                delete[] ptr;
                ptr = nullptr;
            }
        }

    private:
        const T* ptr;
    };    //class NewGuards

}    //sinicheveen

#endif    //_RAII_