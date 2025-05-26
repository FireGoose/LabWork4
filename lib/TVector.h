#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template <class T>
class TVectorIterator;

template <class T>
class TVector
{
protected:
    T* vector;
    int len;
    bool isNew;
public:
    TVector();
    TVector(int len_);
    TVector(const TVector& p);
    TVector(TVector&& p);
    ~TVector();

    int GetLen() const;
    void GetVector(T** vector_);

    void SetLen(int len_);
    void SetVector(T* vector_, int len_);

    TVector operator+(const TVector<T>& p);
    TVector operator-(const TVector<T>& p);
    T operator*(const TVector<T>& p);
    TVector operator*(const T& p);

    TVector<T> operator/(const TVector<T>& p) const;

    TVector& operator=(const TVector<T>& p);
    TVector& operator=(TVector<T>&& p);
    bool operator==(const TVector<T>& p) const;

    T& operator[](int index);
    const T& operator[](int index) const;


    template <class T1>
    friend ostream& operator<<(ostream& o, TVector<T1>& p);
    template <class T1>
    friend istream& operator>>(istream& i, TVector<T1>& p);

    void BubbleSort(); // Доп задание 1
    void QuickSort();
    void InsertionSort();
    int Partition(int low, int high);
    void QuickSortHelper(int low, int high);
    T Normalization();
    T FirstNorm();
    T SecondNorm();
    T InfinityNorm();
    T HolderNorm(double p);

    TVectorIterator<T> begin();
    TVectorIterator<T> end();

    void Rand();
};


template<class T>
class TVectorIterator
{
protected:
    TVector<T>& p;
    int index;
public:
    TVectorIterator(TVector<T>& vector, int index_ = 0);
    T& operator*();
    TVectorIterator<T>& operator++();
    TVectorIterator<T>& operator++(int);

    bool operator != (const TVectorIterator<T>& p);
};

template<class T>
inline TVector<T>::TVector()
{
    vector = nullptr;
    isNew = true;
    len = 0;
}

template<class T>
inline TVector<T>::TVector(int len_)
{
    if (len_ < 0) throw(0);
    else if (len_ == 0)
    {
        len = 0;
        vector = nullptr;
        return;
    }
    else len = len_;
    vector = new T[len]{ 0 };
    isNew = true;
}

template<class T>
inline TVector<T>::TVector(const TVector& p):TVector<T>::TVector(p.len)
{
    for (int i = 0; i < len; i++) vector[i] = p.vector[i];
    isNew = true;
}

template<class T>
inline TVector<T>::TVector(TVector&& p)
{
    vector = p.vector;
    len = p.len;
    isNew = p.isNew;
    p.len = 0;
    p.vector = nullptr;
    p.isNew = false;
}

template<class T>
inline TVector<T>::~TVector()
{
    if (vector != nullptr)
        if (isNew == true)
            delete[] vector;
    vector = nullptr;
    len = 0;
}

template<class T>
inline int TVector<T>::GetLen() const
{
    return len;
}

template<class T>
inline void TVector<T>::GetVector(T** vector_)
{
    *vector_ = vector;
}


template<class T>
inline void TVector<T>::SetLen(int len_)
{

    if (len == len_) return;
    if (len_ < 0) throw -1;
    else if (len_ == 0)
    {
        if (vector != nullptr) delete[] vector;
        vector = nullptr;
        len = 0;
        return;
    }

    T* newVector = new T[len_];
    for (int i = 0; i < min(len, len_); i++) newVector[i] = vector[i];
    if (vector != nullptr) delete[] vector;

    vector = newVector;
    len = len_;
}

template<class T>
inline void TVector<T>::SetVector(T* vector_, int len_)
{
    if (len_ < 0) throw -1;
    if (vector != nullptr) delete[] vector;
    isNew = false;
    vector = vector_;
    len = len_;
}

template<class T>
inline TVector<T> TVector<T>::operator+(const TVector<T>& p)
{
    if (len != p.len) throw -1;

    TVector<T> res = len;
    for (int i = 0; i < len; i++)
    {
        res[i] = (*this)[i] + p[i];
    }
    return res;
}

template<class T>
inline TVector<T> TVector<T>::operator-(const TVector<T>& p)
{
    if (len != p.len) throw -1;

    TVector<T> res = len;
    for (int i = 0; i < len; i++)
    {
        res[i] = (*this)[i] - p[i];
    }
    return res;
}

template<class T>
inline T TVector<T>::operator*(const TVector<T>& p) {
    if (len != p.len) throw -1;
    T res = 0;
    for (int i = 0; i < len; i++)
    {
        res = res + (*this)[i] * p[i];
    }
    return res;
}

template<class T>
inline TVector<T> TVector<T>::operator*(const T& mul)
{
    TVector<T> res(len);
    for (size_t i = 0; i < len; ++i)
    {
        res[i] = (*this)[i] * mul;
    }
    return res;
}

template <class T>
inline TVector<T> TVector<T>::operator/(const TVector<T>& div) const {
    if (len != div.len)
    {
        throw invalid_argument("Length error");
    }
    TVector<T> res(len);
    for (int i = 0; i < len; ++i) res.vector[i] = this->vector[i] / div.vector[i];
    return res;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(const TVector<T>& p)
{
    if (this == &p)
        return *this;

    if (len != p.len)
    {
        delete[] vector;
        vector = new T[p.len];
    }
    len = p.len;
    for (int i = 0; i < len; i++)
        vector[i] = p.vector[i];

    return *this;
}


template<class T>
inline TVector<T>& TVector<T>::operator=(TVector<T>&& p)
{
    if (this == &p)
        return *this;

    if (vector != nullptr)
        delete[] vector;

    vector = p.vector;
    len = p.len;
    p.vector = nullptr;
    p.len = 0;
    return *this;
}

template<class T>
inline bool TVector<T>::operator==(const TVector<T>& p) const
{
    if (len != p.len) return false;
    for (int i = 0; i < len; ++i)
    {
        if (vector[i] != p.vector[i])
        {
            return false;
        }
    }
    return true;
}

template<class T>
inline T& TVector<T>::operator[](int index) {
    if (vector == nullptr || index < 0 || index >= len)
        throw out_of_range("Index error");
    return vector[index];
}

template<class T>
inline const T& TVector<T>::operator[](int index) const {
    if (vector == nullptr || index < 0 || index >= len)
        throw out_of_range("Index error");
    return vector[index];
}




template<class T>
inline void TVector<T>::BubbleSort()
{
    T temp;
    for (int s = 0; s < len; ++s)
    {
        for (int j = 0; j < len - 1; ++j)
        {
            if ((vector[j]) > (vector[j + 1]))
            {
                temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
        }
    }
}

template<class T>
inline void TVector<T>::QuickSort()
{
    if (len <= 1) return;
    QuickSortHelper(0, len - 1);
}


template<class T>
inline void TVector<T>::InsertionSort() {
    for (int i = 1; i < len; ++i) {
        T key = vector[i];
        int j = i - 1;

        while (j >= 0 && vector[j] > key) {
            vector[j + 1] = vector[j];
            j--;
        }
        vector[j + 1] = key;

    }
}

template<class T>
inline void TVector<T>::QuickSortHelper(int low, int high)
{
    if (low < high) {
        int pi = Partition(low, high);
        QuickSortHelper(low, pi - 1);
        QuickSortHelper(pi + 1, high);
    }
}

template<class T>
inline int TVector<T>::Partition(int low, int high) {
    T pivot = vector[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (vector[j] <= pivot) {
            i++;
            swap(vector[i], vector[j]);
        }
    }
    swap(vector[i + 1], vector[high]);
    return i + 1;
}

// tnx Roman
template<class T>
inline T TVector<T>::Normalization()
{
    T l = SecondNorm();
    for (int i = 0; i < len; ++i)
        (*this)[i] = l;
    return l;
}

template<class T>
inline T TVector<T>::FirstNorm()
{
    int norm = 0;
    for (int i = 0; i < len; ++i) {
        norm += abs(vector[i]);
    }
    return norm;
}

template<class T>
inline T TVector<T>::SecondNorm()
{
    double norm = 0.0;

    for (int i = 0; i < len; ++i) {
        norm += pow(vector[i], 2);
    }
    return sqrt(norm);
}

template<class T>
inline T TVector<T>::InfinityNorm()
{
    T ans;
    for (int i = 0; i < len; ++i) {
        if (abs(vector[i]) > ans) {
            ans = abs(vector[i]);
        }
    }
    return ans;
}

template<class T>
inline T TVector<T>::HolderNorm(double p)
{
    double res = 0;
    double power = 1 / (double) p;
    for (int i = 0; i < len; ++i)
        res += pow((vector[i]), p);
    res = pow(res, power);
    return res;
}

template<class T>
inline TVectorIterator<T> TVector<T>::begin()
{
    return TVectorIterator<T>(*this, 0);
}

template<class T>
inline TVectorIterator<T> TVector<T>::end()
{
    return TVectorIterator<T>(*this, len);
}

template<class T>
inline void TVector<T>::Rand()
{
    for (int i = 0; i < len; i++) vector[i] = rand();
}
template<class T1>
inline ostream& operator<<(ostream& o, TVector<T1>& p)
{
    for (int i = 0; i < p.GetLen(); i++)
        o << p[i] << "\t";
    return o;
}

template<class T1>
inline istream& operator>>(istream& is, TVector<T1>& p)
{
    for (int j = 0; j < p.len; ++j)
    {
        cout << "a[" << j << "] = ";
        is >> p[j];
    }
    return is;
}

template<class T>
inline TVectorIterator<T>::TVectorIterator(TVector<T>& vector, int index_):
        p(vector), index(index_)
{
}

template<class T>
inline T& TVectorIterator<T>::operator*()
{
    return p[index];
}

template<class T>
inline TVectorIterator<T>& TVectorIterator<T>::operator++()
{
    index++;
    return *this;
}

template<class T>
inline TVectorIterator<T>& TVectorIterator<T>::operator++(int)
{
    index++;
    return* this;
}

template<class T>
inline bool TVectorIterator<T>::operator!=(const TVectorIterator<T>& p)
{
    return index != p.index;
}