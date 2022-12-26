//
// Created by Takex on 21.11.2022.
//

#ifndef RIDERS_LAB2_ARRAY_H
#define RIDERS_LAB2_ARRAY_H

template<typename T>
class Array final {
private:
    void resize();
    void moveRight(int fromIndex);
    void moveLeft(int fromIndex);
public:
    class Iterator {
        bool isForward;
        T *ptr_;
        T *ptrEnd_;
    public:
        const T &get() const;
        void set(const T &value);
        void next();
        bool hasNext() const;
        explicit Iterator(Array<T> *a, bool isForward);
    };

    Array();
    explicit Array(int capacity);
    Array(Array<T> const &other);
    Array(Array<T>&& other) noexcept;
    ~Array();

    size_t capacity();
    int insert(const T& value);
    int insert(int index, const T& value);
    void remove(int index);
    const T& operator[](int index) const;
    T& operator[](int index);
    Array<T>& operator=(Array<T> other);
    Array<T>& operator=(Array&& other) noexcept;
    int size() const;
    void swap(Array<T>& other);
    Iterator iterator();
    Iterator reverseIterator();

private:
    size_t size_;
    size_t capacity_;
    T* data_;
};

template<typename T>
Array<T>::Array() {
    capacity_ = 16;
    size_ = 0;
    data_ = (T*) malloc(capacity_ * sizeof(T));
}

template<typename T>
Array<T>::Array(int capacity) {
    capacity_ = capacity;
    size_ = 0;
    data_ = (T*) malloc(capacity_ * sizeof(T));
}

template<typename T>
Array<T>::Array(Array<T> const& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = (T *) malloc(capacity_ * sizeof(T));
    for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(other.data_[i]);
    }
}

template<typename T>
Array<T>::Array(Array<T>&& other) noexcept{
    data_ = other.data_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
}

template<typename T>
Array<T>::~Array(){
    for (int i = 0; i < size_; i++) {
        data_[i].~T();
    }
    free(data_);
}

template<typename T>
int Array<T>::insert(const T &value) {
    if (size_ >= capacity_) resize();
    new (data_ + size_) T(value);
    return size_++;
}

template<typename T>
int Array<T>::insert(int index, const T &value) {
    if (index < 0) {
        return -1;
    }
    if (size_ >= capacity_) resize();

    size_++;
    moveRight(index);
    new(data_ + index) T(value);
    return index;
}

template<typename T>
void Array<T>::moveRight(int fromIndex) {
    for (size_t i = size_; i > fromIndex; --i) {
        new (data_ + i) T(std::move(data_[i - 1]));
        data_[i - 1].~T();
    }
}

template<typename T>
void Array<T>::resize() {
    capacity_ *= 2;

    T* tempData = (T*) malloc(capacity_ * sizeof(T));

    for(size_t i = 0; i < size_; ++i){
        new (tempData + i) T(std::move(data_[i]));
        data_[i].~T();
    }

    free(data_);
    data_ = tempData;
}

template<typename T>
void Array<T>::remove(int index) {
    moveLeft(index);
    size_--;
}

template<typename T>
void Array<T>::moveLeft(int fromIndex) {
    for (size_t i = fromIndex; i < size_; ++i) {
        data_[i].~T();
        new (data_ + i) T(std::move(data_[i + 1]));
    }
}

template<typename T>
int Array<T>::size() const {
    return size_;
}

template<typename T>
size_t Array<T>::capacity() {
    return capacity_;
}

template<typename T>
void Array<T>::swap(Array& other){
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
T &Array<T>::operator[](int index) {
    return *(data_ + index);
}

template<typename T>
const T &Array<T>::operator[](int index) const {
    return *(data_ + index);
}

template<typename T>
Array<T>& Array<T>::operator=(Array other) {
    swap(other);
    return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept{
    if(this == &other) return *this;

    for (size_t i = 0; i < size_; ++i) data_[i].~T();
    free(data_);

    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = other.data_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

template<typename T>
const T &Array<T>::Iterator::get() const {
    return *ptr_;
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator() {
    return Array::Iterator(this, true);
}

template<typename T>
typename Array<T>::Iterator Array<T>::reverseIterator() {
    return Array::Iterator(this, false);
}

template<typename T>
Array<T>::Iterator::Iterator(Array<T> *a, bool isForward) {
    this->isForward = isForward;
    if (isForward){
        ptrEnd_ = a->data_ + a->size_;
        ptr_ = a->data_;
    } else {
        ptrEnd_ = a->data_;
        ptr_ = a->data_ + a->size_ - 1;
    }

}

template<typename T>
void Array<T>::Iterator::next() {
    if (this->isForward) ptr_++; else ptr_--;
}

template<typename T>
void Array<T>::Iterator::set(const T &value) {
    *ptr_ = value;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    if (this->isForward){
        return ptr_ < ptrEnd_;
    } else {
        return ptr_ >= ptrEnd_;
    }
}

#endif //RIDERS_LAB2_ARRAY_H