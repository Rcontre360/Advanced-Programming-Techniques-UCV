#ifndef HEAP_DEF
#define HEAP_DEF

template <class T> class IHeap {
    public:
        virtual T getMinimum() = 0;

        virtual T removeMinimum() = 0;

        virtual void insert(const T &val) = 0;

        virtual ~IHeap() {}
};

#endif
